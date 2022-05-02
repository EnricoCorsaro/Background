// Main code for background fitting to red giant stars by means of nested sampling analysis
// Created by Enrico Corsaro @ IvS - July 2014
// Edited by Enrico Corsaro @ OACT - January 2019
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
#include "ThreeHarveyColorBackgroundModel.h"
#include "ThreeHarveyColorNoGaussianBackgroundModel.h"
#include "ThreeHarveyBackgroundModel.h"
#include "ThreeHarveyNoGaussianBackgroundModel.h"
#include "TwoHarveyColorBackgroundModel.h"
#include "TwoHarveyBackgroundModel.h"
#include "TwoHarveyNoGaussianBackgroundModel.h"
#include "OneHarveyColorBackgroundModel.h"
#include "OneHarveyBackgroundModel.h"
#include "OneHarveyNoGaussianBackgroundModel.h"
#include "OriginalBackgroundModel.h"
#include "FlatBackgroundModel.h"
#include "FlatNoGaussianBackgroundModel.h"
#include "FerozReducer.h"
#include "PowerlawReducer.h"
#include "Results.h"
#include "PrincipalComponentProjector.h"


int main(int argc, char *argv[])
{

    // Check number of arguments for main function
    
    if (argc != 8)
    {
        cerr << "Usage: ./background <Catalog ID> <Star ID> <run number> <background model> <input prior base filename> <input threshold frequency (muHz)> <PCA flag> " << endl;
        exit(EXIT_FAILURE);
    }
    

    // ---------------------------
    // ----- Read input data -----
    // ---------------------------

    unsigned long Nrows;
    int Ncols;
    ArrayXXd data;
    string CatalogID(argv[1]);
    string StarID(argv[2]);
    string runNumber(argv[3]);
    string backgroundModelName(argv[4]);
    string inputPriorBaseName(argv[5]); 
    string inputThresholdFrequency(argv[6]);
    string inputPCAflag(argv[7]);
    double thresholdFrequency = stod(inputThresholdFrequency);
    int PCAflag = stoi(inputPCAflag);


    // Read the local path for the working session from an input ASCII file

    ifstream inputFile;
    File::openInputFile(inputFile, "localPath.txt");
    File::sniffFile(inputFile, Nrows, Ncols);
    vector<string> myLocalPath;
    myLocalPath = File::vectorStringFromFile(inputFile, Nrows);
    inputFile.close();

    
    // Set up some string paths used in the computation

    string baseInputDirName = myLocalPath[0] + "data/";
    string inputFileName = baseInputDirName + CatalogID + StarID + ".txt";
    string outputDirName = myLocalPath[0] + "results/" + CatalogID + StarID + "/";
    string outputPathPrefix = outputDirName + runNumber + "/background_";
    
    cout << "------------------------------------------------ " << endl;
    cout << " Background analysis of " + CatalogID + StarID << endl;
    cout << "------------------------------------------------ " << endl;
    cout << endl; 


    // Read the input dataset

    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Nrows, Ncols);
    data = File::arrayXXdFromFile(inputFile, Nrows, Ncols);
    inputFile.close();

   
    // Create arrays for each data type

    ArrayXd covariates = data.col(0);
    ArrayXd observations = data.col(1); 
 

    // Trim input dataset in the given frequency range

    if (thresholdFrequency > covariates.minCoeff())
    {
        // Activate the trimming only if a meaningful lower frequency threshold is supplied

        double lowerFrequency = thresholdFrequency;
        double upperFrequency = covariates.maxCoeff();
    
        vector<int> trimIndices = Functions::findArrayIndicesWithinBoundaries(covariates, lowerFrequency, upperFrequency);
        int Nbins = trimIndices.size();
        ArrayXd trimmedArray(Nbins);
            
        trimmedArray = covariates.segment(trimIndices[0],Nbins);
        covariates.resize(Nbins);
        covariates = trimmedArray;
        trimmedArray = observations.segment(trimIndices[0],Nbins);
        observations.resize(Nbins);
        observations = trimmedArray;
    }
    else
    {
        // Set input threshold frequency to 0 in case it is not larger than the minimum frequency of the dataset.
        // This implies that it is not used within the computation.

        thresholdFrequency = 0.0;
    }

    cout << "------------------------------------------------------- " << endl;
    cout << " Frequency range: [" << setprecision(4) << covariates.minCoeff() << ", " 
        << covariates.maxCoeff() << "] muHz" << endl;
    cout << "------------------------------------------------------- " << endl;
    cout << endl; 

    // -------------------------------------------------------
    // ----- First step. Set up all prior distributions -----
    // -------------------------------------------------------
    
    // Uniform Prior
    unsigned long Ndimensions;              // Number of parameters for which prior distributions are defined
    
    // Read prior hyper parameters for resolved modes
    inputFileName = outputDirName + inputPriorBaseName + "_" + runNumber + ".txt";
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Ndimensions, Ncols);
    
    if (Ncols != 2)
    {
        cerr << " Wrong number of input prior boundaries." << endl;
        cerr << " Two boundaries are required for uniform priors." << endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout << "------------------------------------------------------- " << endl;
        cout << " Reading prior file " + inputPriorBaseName + "_" + runNumber + ".txt" << endl;
        cout << "------------------------------------------------------- " << endl;
        cout << endl;
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
    BackgroundModel *model = nullptr;

    // Long-trend, meso-granulation, and granulation component included, with colored noise
    if (backgroundModelName == "ThreeHarveyColor")
    {
        model = new ThreeHarveyColorBackgroundModel(covariates, inputFileName); 
    }
    
    // Long-trend, meso-granulation, and granulation component included, with colored noise but no Gaussian envelope
    if (backgroundModelName == "ThreeHarveyColorNoGaussian")
    {
        model = new ThreeHarveyColorNoGaussianBackgroundModel(covariates, inputFileName); 
    }
    
    // Long-trend, meso-granulation, and granulation component included, but no colored noise
    if (backgroundModelName == "ThreeHarvey")
    {
        model = new ThreeHarveyBackgroundModel(covariates, inputFileName);
    }
    
    // Long-trend, meso-granulation, and granulation component included, but no colored noise and no Gaussian envelope
    if (backgroundModelName == "ThreeHarveyNoGaussian")
    {
        model = new ThreeHarveyNoGaussianBackgroundModel(covariates, inputFileName); 
    }

    // Meso-granulation and granulation components included, with colored noise
    if (backgroundModelName == "TwoHarveyColor")
    {
        model = new TwoHarveyColorBackgroundModel(covariates, inputFileName);   
    }
    
    // Meso-granulation and granulation components included, but no colored noise
    if (backgroundModelName == "TwoHarvey")
    {
        model = new TwoHarveyBackgroundModel(covariates, inputFileName);   
    }

    // Meso-granulation and granulation components included, but no colored noise and no Gaussian envelope
    if (backgroundModelName == "TwoHarveyNoGaussian")
    {
        model = new TwoHarveyNoGaussianBackgroundModel(covariates, inputFileName);   
    }

    // Only meso-granulation component included, with colored noise
    if (backgroundModelName == "OneHarveyColor")
    {    
        model = new OneHarveyColorBackgroundModel(covariates, inputFileName); 
    }
    
    // Only meso-granulation component included, but no colored noise
    if (backgroundModelName == "OneHarvey")
    {    
        model = new OneHarveyBackgroundModel(covariates, inputFileName); 
    }
    
        // Only meso-granulation component included, but no colored noise and no Gaussian envelope
    if (backgroundModelName == "OneHarveyNoGaussian")
    {    
        model = new OneHarveyNoGaussianBackgroundModel(covariates, inputFileName); 
    }

    // Only meso-granulation component included, but no colored noise and using the original Harvey law (exponent = 2)
    if (backgroundModelName == "Original")
    {    
        model = new OriginalBackgroundModel(covariates, inputFileName); 
    }

    // Only Gaussian envelope and white noise
    if (backgroundModelName == "Flat")
    {    
        model = new FlatBackgroundModel(covariates, inputFileName); 
    }
    
    // Only white noise, without Gaussian envelope
    if (backgroundModelName == "FlatNoGaussian")
    {    
        model = new FlatNoGaussianBackgroundModel(covariates, inputFileName); 
    }
    

    // -----------------------------------------------------------------
    // ----- Third step. Set up the likelihood function to be used -----
    // -----------------------------------------------------------------
    
    ExponentialLikelihood likelihood(observations, *model);
    

    // -------------------------------------------------------------------------------
    // ----- Fourth step. Set up the K-means clusterer using an Euclidean metric -----
    // -------------------------------------------------------------------------------

    unsigned long Nparameters;
    inputFileName = outputDirName + "Xmeans_configuringParameters.txt";
    File::openInputFile(inputFile, inputFileName);
    File::sniffFile(inputFile, Nparameters, Ncols);

    if (Nparameters != 2)
    {
        cerr << "Wrong number of input parameters for clustering algorithm." << endl;
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
    double relTolerance = 0.01;      // k-means

    bool printNdimensions = false;
    PrincipalComponentProjector projector(printNdimensions);
    bool featureProjectionActivated = false;
    
    if (PCAflag == 1)
    {
        featureProjectionActivated = true;
    }

    EuclideanMetric myMetric;
    KmeansClusterer clusterer(myMetric, projector, featureProjectionActivated, 
                           minNclusters, maxNclusters, Ntrials, relTolerance); 
    

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

    // Print results on the screen
    
    bool printOnTheScreen = true;                        

    
    // Initial number of live points
    
    int initialNlivePoints = configuringParameters(0);   
    
    
    // Minimum number of live points
    
    int minNlivePoints = configuringParameters(1);       

    
    // Maximum number of attempts when trying to draw a new sampling point
    
    int maxNdrawAttempts = configuringParameters(2);    
    
    
    // The first N iterations, we assume that there is only 1 cluster
    
    int NinitialIterationsWithoutClustering = configuringParameters(3);

    
    // Clustering is only happening every N iterations
    
    int NiterationsWithSameClustering = configuringParameters(4);
    
    
    // Fraction by which each axis in an ellipsoid has to be enlarged
    // It can be a number >= 0, where 0 means no enlargement. configuringParameters(5)
    // Calibration from Corsaro et al. (2018)
    
    double initialEnlargementFraction = 0.369*pow(Ndimensions,0.574);    

    
    // Exponent for remaining prior mass in ellipsoid enlargement fraction
    // It is a number between 0 and 1. The smaller the slower the shrinkage // of the ellipsoids.
    
    double shrinkingRate = configuringParameters(6);
                                                                                                                        
    
    // Termination factor for nested sampling process
    
    double terminationFactor = configuringParameters(7);    

    MultiEllipsoidSampler nestedSampler(printOnTheScreen, ptrPriors, likelihood, myMetric, clusterer, 
                                        initialNlivePoints, minNlivePoints, initialEnlargementFraction, shrinkingRate);
    
    double tolerance = 1.e2;
    double exponent = 0.4;
    PowerlawReducer livePointsReducer(nestedSampler, tolerance, exponent, terminationFactor);
 
    nestedSampler.run(livePointsReducer, NinitialIterationsWithoutClustering, NiterationsWithSameClustering, 
                      maxNdrawAttempts, terminationFactor, 0, outputPathPrefix);

    nestedSampler.outputFile << "# List of configuring parameters used for the ellipsoidal sampler and cluster algorithm" << endl;
    nestedSampler.outputFile << "# Row #1: Minimum Nclusters" << endl;
    nestedSampler.outputFile << "# Row #2: Maximum Nclusters" << endl;
    nestedSampler.outputFile << "# Row #3: Initial Enlargement Fraction" << endl;
    nestedSampler.outputFile << "# Row #4: Shrinking Rate" << endl;
    nestedSampler.outputFile << minNclusters << endl;
    nestedSampler.outputFile << maxNclusters << endl;
    nestedSampler.outputFile << initialEnlargementFraction << endl;
    nestedSampler.outputFile << shrinkingRate << endl;
    nestedSampler.outputFile << "# Other information on the run" << endl;
    nestedSampler.outputFile << "# Row #1: Low-Frequency threshold (0 if not used)" << endl;
    nestedSampler.outputFile << "# Row #2: Local working path used" << endl;
    nestedSampler.outputFile << "# Row #3: Catalog and Star ID" << endl;
    nestedSampler.outputFile << "# Row #4: Run Number" << endl;
    nestedSampler.outputFile << "# Row #5: Background model adopted" << endl;
    nestedSampler.outputFile << "# Row #6: PCA activated (1 = yes / 0 = no)" << endl;
    nestedSampler.outputFile << thresholdFrequency << endl;
    nestedSampler.outputFile << myLocalPath[0] << endl;
    nestedSampler.outputFile << CatalogID + StarID << endl;
    nestedSampler.outputFile << runNumber << endl;
    nestedSampler.outputFile << backgroundModelName << endl;
    nestedSampler.outputFile << featureProjectionActivated << endl;
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

    cout << "Process # " << runNumber << " has been completed." << endl;

    return EXIT_SUCCESS;
}
