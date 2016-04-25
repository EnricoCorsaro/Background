// Main code for background fitting to red giant stars by means of nested sampling analysis
// Created by Enrico Corsaro @ IvS - July 2014
// Last update: April 2016 @ CEA
// e-mail: emncorsaro@gmail.com
// Source code file "Background.cpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Eigen/Dense>
#include "Functions.h"
#include "File.h"
#include "MultiEllipsoidSampler.h"
#include "Ellipsoid.h"
#include "KmeansClusterer.h"
#include "EuclideanMetric.h"
#include "Prior.h"
#include "UniformPrior.h"
#include "NormalPrior.h"
#include "ExponentialLikelihood.h"
#include "StandardBackgroundModel.h"
#include "FullBackgroundModel.h"
#include "FerozReducer.h"
#include "PowerlawReducer.h"
#include "Results.h"

int main(int argc, char *argv[])
{

    // Check number of arguments for main function
    
    if (argc != 3)
    {
        cerr << "Usage: background <KIC ID> <run number>" << endl;
        exit(EXIT_FAILURE);
    }


    // ---------------------------
    // ----- Read input data -----
    // ---------------------------

    unsigned long Nrows;
    int Ncols;
    ArrayXXd data;
    string KICID(argv[1]);
    string runNumber(argv[2]);


    // Read the local path for the working session from an input ASCII file
    ifstream inputFile;
    File::openInputFile(inputFile, "localPath.txt");
    File::sniffFile(inputFile, Nrows, Ncols);
    vector<string> myLocalPath;
    myLocalPath = File::vectorStringFromFile(inputFile, Nrows);
    inputFile.close();

    
    // Set up some string paths used in the computation
    string baseInputDirName = myLocalPath[0] + "data/";
    string inputFileName = baseInputDirName + "KIC" + KICID + ".txt";
    string outputDirName = myLocalPath[0] + "results/KIC" + KICID + "/";
    string outputPathPrefix = outputDirName + runNumber + "/background_";


    // Read the input dataset
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Nrows, Ncols);
    data = File::arrayXXdFromFile(inputFile, Nrows, Ncols);
    inputFile.close();

   
    // Create arrays for each data type
    ArrayXd covariates = data.col(0);
    ArrayXd observations = data.col(1); 
   

    // -------------------------------------------------------
    // ----- First step. Set up all prior distributions -----
    // -------------------------------------------------------
    
    // Uniform Prior
    unsigned long Ndimensions;              // Number of parameters for which prior distributions are defined
    
    // Read prior hyper parameters for resolved modes
    inputFileName = outputDirName + "background_hyperParameters.txt";
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Ndimensions, Ncols);
    
    if ((Ndimensions != 12.0) && (Ndimensions != 10.0))            // Colored-noise component included or excluded
    {
        cerr << "Wrong number of input prior hyper-parameters for background model." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (Ncols != 2)
    {
        cerr << "Wrong number of input prior boundaries." << endl;
        cerr << "Two boundaries are required for uniform priors." << endl;
    }
    
    ArrayXXd hyperParameters;
    hyperParameters = File::arrayXXdFromFile(inputFile, Ndimensions, Ncols);
    inputFile.close(); 
    ArrayXd hyperParametersMinima = hyperParameters.col(0);
    ArrayXd hyperParametersMaxima = hyperParameters.col(1);

    // Uniform Prior
    int NpriorTypes = 1;                                    // Total number of prior types included in the computation
    vector<Prior*> ptrPriors(NpriorTypes);
    
    UniformPrior uniformPrior(hyperParametersMinima, hyperParametersMaxima);
    ptrPriors[0] = &uniformPrior;

    string fullPathHyperParameters = outputPathPrefix + "hyperParametersUniform.txt";
    uniformPrior.writeHyperParametersToFile(fullPathHyperParameters);


    // -------------------------------------------------------------------
    // ---- Second step. Set up the models for the inference problem ----- 
    // -------------------------------------------------------------------
    
    inputFileName = outputDirName + "NyquistFrequency.txt";
    FullBackgroundModel model(covariates, inputFileName);      // Colored-noise component
    // StandardBackgroundModel model(covariates, inputFileName);       // No colored-noise component



    // -----------------------------------------------------------------
    // ----- Third step. Set up the likelihood function to be used -----
    // -----------------------------------------------------------------
    
    ExponentialLikelihood likelihood(observations, model);
    

    // -------------------------------------------------------------------------------
    // ----- Fourth step. Set up the K-means clusterer using an Euclidean metric -----
    // -------------------------------------------------------------------------------

    unsigned long Nparameters;
    inputFileName = outputDirName + "Xmeans_configuringParameters.txt";
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Nparameters, Ncols);

    if (Nparameters != 2)
    {
        cerr << "Wrong number of input parameters for X-means algorithm." << endl;
        exit(EXIT_FAILURE);
    }

    ArrayXd configuringParameters;
    configuringParameters = File::arrayXXdFromFile(inputFile, Nparameters, Ncols);
    inputFile.close();
    
    int minNclusters = configuringParameters(0);
    int maxNclusters = configuringParameters(1);
    
    if ((minNclusters <= 0) || (maxNclusters <= 0) || (maxNclusters < minNclusters))
    {
        cerr << "Minimum or maximum number of clusters cannot be <= 0, and " << endl;
        cerr << "minimum number of clusters cannot be larger than maximum number of clusters." << endl;
        exit(EXIT_FAILURE);
    }

    int Ntrials = 10;
    double relTolerance = 0.01;

    EuclideanMetric myMetric;
    KmeansClusterer kmeans(myMetric, minNclusters, maxNclusters, Ntrials, relTolerance); 


    // ---------------------------------------------------------------------
    // ----- Sixth step. Configure and start nested sampling inference -----
    // ---------------------------------------------------------------------
    
    inputFileName = outputDirName + "NSMC_configuringParameters.txt";
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Nparameters, Ncols);
    configuringParameters.setZero();
    configuringParameters = File::arrayXXdFromFile(inputFile, Nparameters, Ncols);
    inputFile.close();

    if (Nparameters != 8)
    {
        cerr << "Wrong number of input parameters for NSMC algorithm." << endl;
        exit(EXIT_FAILURE);
    }

    bool printOnTheScreen = true;                       // Print results on the screen
    int initialNlivePoints = configuringParameters(0);     // Initial number of live points 
    int minNlivePoints = configuringParameters(1);         // Minimum number of live points 
    int maxNdrawAttempts = configuringParameters(2);    // Maximum number of attempts when trying to draw a new sampling point
    int NinitialIterationsWithoutClustering = configuringParameters(3); // The first N iterations, we assume that there is only 1 cluster
    int NiterationsWithSameClustering = configuringParameters(4);       // Clustering is only happening every N iterations.
    double initialEnlargementFraction = 0.267*pow(Ndimensions,0.643);   //configuringParameters(5);   // Fraction by which each axis in an ellipsoid has to be enlarged.
                                                                    // It can be a number >= 0, where 0 means no enlargement.
    double shrinkingRate = configuringParameters(6);        // Exponent for remaining prior mass in ellipsoid enlargement fraction.
                                                            // It is a number between 0 and 1. The smaller the slower the shrinkage
                                                            // of the ellipsoids.
                                                            
    if ((shrinkingRate > 1) || (shrinkingRate) < 0)
    {
        cerr << "Shrinking Rate for ellipsoids must be in the range [0, 1]. " << endl;
        exit(EXIT_FAILURE);
    }

    double terminationFactor = configuringParameters(7);    // Termination factor for nested sampling process.

    
    MultiEllipsoidSampler nestedSampler(printOnTheScreen, ptrPriors, likelihood, myMetric, kmeans, 
                                        initialNlivePoints, minNlivePoints, initialEnlargementFraction, shrinkingRate);
    
    double tolerance = 1.e2;
    double exponent = 0.4;
    PowerlawReducer livePointsReducer(nestedSampler, tolerance, exponent, terminationFactor);
 
    nestedSampler.run(livePointsReducer, NinitialIterationsWithoutClustering, NiterationsWithSameClustering, 
                      maxNdrawAttempts, terminationFactor, outputPathPrefix);

    nestedSampler.outputFile << "# List of configuring parameters used for the ellipsoidal sampler and X-means" << endl;
    nestedSampler.outputFile << "# Row #1: Minimum Nclusters" << endl;
    nestedSampler.outputFile << "# Row #2: Maximum Nclusters" << endl;
    nestedSampler.outputFile << "# Row #3: Initial Enlargement Fraction" << endl;
    nestedSampler.outputFile << "# Row #4: Shrinking Rate" << endl;
    nestedSampler.outputFile << minNclusters << endl;
    nestedSampler.outputFile << maxNclusters << endl;
    nestedSampler.outputFile << initialEnlargementFraction << endl;
    nestedSampler.outputFile << shrinkingRate << endl;
    nestedSampler.outputFile << "# Local working path used: " + myLocalPath[0] << endl;
    nestedSampler.outputFile << "# KIC ID: " + KICID << endl;
    nestedSampler.outputFile << "# Run Number: " + runNumber << endl;
    nestedSampler.outputFile.close();


    // -------------------------------------------------------
    // ----- Last step. Save the results in output files -----
    // -------------------------------------------------------
   
    Results results(nestedSampler);
    results.writeParametersToFile("parameter");
    results.writeLogLikelihoodToFile("logLikelihood.txt");
    results.writeLogWeightsToFile("logWeights.txt");
    results.writeEvidenceInformationToFile("evidenceInformation.txt");
    results.writePosteriorProbabilityToFile("posteriorDistribution.txt");

    double credibleLevel = 68.3;
    bool writeMarginalDistributionToFile = true;
    results.writeParametersSummaryToFile("parameterSummary.txt", credibleLevel, writeMarginalDistributionToFile);

    cerr << "Process #" << runNumber << " has been completed." << endl;

    return EXIT_SUCCESS;
}
