// Main code for background fitting to MS stars by means of nested sampling analysis
// Created by Enrico Corsaro @ IvS - 21 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Source code file "Background.cpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Eigen/Dense>
#include "Functions.h"
#include "File.h"
#include "MultiEllipsoidSampler.h"
#include "KmeansClusterer.h"
#include "EuclideanMetric.h"
#include "Prior.h"
#include "UniformPrior.h"
#include "NormalPrior.h"
#include "ExponentialLikelihood.h"
#include "RedGiantBackgroundModel.h"
#include "FerozReducer.h"
#include "PowerlawReducer.h"
#include "Results.h"
#include "Ellipsoid.h"

int main(int argc, char *argv[])
{

    // Check number of arguments for main function
    
    if (argc != 3)
    {
        cerr << "Usage: peakbagging <input file> <output directory>" << endl;
        exit(EXIT_FAILURE);
    }


    // ---------------------------
    // ----- Read input data -----
    // ---------------------------

    unsigned long Nrows;
    int Ncols;
    ArrayXXd data;
    string inputFileName(argv[1]);
    string outputDirName(argv[2]);
    string outputPathPrefix = outputDirName + "background_";

    ifstream inputFile;
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Nrows, Ncols);
    data = File::arrayXXdFromFile(inputFile, Nrows, Ncols);
    inputFile.close();

   
    // Creating arrays for each data type
    
    ArrayXd covariates = data.col(0);
    ArrayXd observations = data.col(1);
   
    
    // --------------------------------------------------
    // ----- Zeroth step. Set up problem dimensions -----
    // --------------------------------------------------

    int Ndimensions = 10;
   

    // -------------------------------------------------------------------
    // ----- First step. Set up the models for the inference problem ----- 
    // -------------------------------------------------------------------
    
    RedGiantBackgroundModel model(covariates);


    // -------------------------------------------------------
    // ----- Second step. Set up all prior distributions -----
    // -------------------------------------------------------
    
    vector<Prior*> ptrPriors(1);


    // Uniform Prior

    ArrayXd parametersMinima(Ndimensions);                      // Minima
    ArrayXd parametersMaxima(Ndimensions);                      // Maxima
   
   
    // Configuration for Punto (KIC 9139163)
    
    parametersMinima(0) = 14.0;                  // Flat noise level
    parametersMaxima(0) = 30.0;
    
    parametersMinima(1) = 120.;                  // Noise Amplitude 1  
    parametersMaxima(1) = 250.;
    parametersMinima(2) = 0.4;                   // Noise Frequency 1
    parametersMaxima(2) = 4.0;    
    
    parametersMinima(3) = 140.;                  // Noise Amplitude 2
    parametersMaxima(3) = 190.;
    parametersMinima(4) = 20.;                   // Noise Frequency 2
    parametersMaxima(4) = 60.;    
    
    parametersMinima(5) = 130.;                  // Noise Amplitude 3    
    parametersMaxima(5) = 180.;
    parametersMinima(6) = 130.;                   // Noise Frequency 3    
    parametersMaxima(6) = 180.;    
   
    parametersMinima(7) = 220.;                   // Oscillation Height
    parametersMaxima(7) = 300.;
    parametersMinima(8) = 153.;                  // Oscillation nuMax
    parametersMaxima(8) = 167.;
    parametersMinima(9) = 12.;                   // Oscillation sigma
    parametersMaxima(9) = 20.;


    UniformPrior uniformPrior(parametersMinima, parametersMaxima);
    ptrPriors[0] = &uniformPrior;

    string fullPathHyperParameters = outputPathPrefix + "hyperParametersUniform.txt";
    uniformPrior.writeHyperParametersToFile(fullPathHyperParameters);


    // -----------------------------------------------------------------
    // ----- Third step. Set up the likelihood function to be used -----
    // -----------------------------------------------------------------
    
    ExponentialLikelihood likelihood(observations, model);
    

    // -------------------------------------------------------------------------------
    // ----- Fourth step. Set up the K-means clusterer using an Euclidean metric -----
    // -------------------------------------------------------------------------------

    int minNclusters = 1;
    int maxNclusters = 10;
    int Ntrials = 10;
    double relTolerance = 0.01;

    EuclideanMetric myMetric;
    KmeansClusterer kmeans(myMetric, minNclusters, maxNclusters, Ntrials, relTolerance); 


    // ---------------------------------------------------------------------
    // ----- Sixth step. Configure and start nested sampling inference -----
    // ---------------------------------------------------------------------
    
    bool printOnTheScreen = false;                   // Print results on the screen
    int initialNobjects = 500;                      // Maximum (and initial) number of live points evolving within the nested sampling process. 
    int minNobjects = 500;                          // Minimum number of live points allowed in the computation
    int maxNdrawAttempts = 10000;                   // Maximum number of attempts when trying to draw a new sampling point
    int NinitialIterationsWithoutClustering = 1500;    // The first N iterations, we assume that there is only 1 cluster
    int NiterationsWithSameClustering = 50;         // Clustering is only happening every N iterations.
    double initialEnlargementFraction = 1.60;        // Fraction by which each axis in an ellipsoid has to be enlarged.
                                                    // It can be a number >= 0, where 0 means no enlargement.
    double shrinkingRate = 0.02;                     // Exponent for remaining prior mass in ellipsoid enlargement fraction.
                                                    // It is a number between 0 and 1. The smaller the slower the shrinkage
                                                    // of the ellipsoids.
    double terminationFactor = 0.01;                // Termination factor for nested sampling process.

    
    MultiEllipsoidSampler nestedSampler(printOnTheScreen, ptrPriors, likelihood, myMetric, kmeans, 
                                        initialNobjects, minNobjects, initialEnlargementFraction, shrinkingRate);
    
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


    return EXIT_SUCCESS;
}
