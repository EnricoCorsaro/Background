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
#include "GlobalFitMSModel.h"
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

    int Ndimensions = 9;
   

    // -------------------------------------------------------------------
    // ----- First step. Set up the models for the inference problem ----- 
    // -------------------------------------------------------------------
    
    GlobalFitMSModel model(covariates);


    // -------------------------------------------------------
    // ----- Second step. Set up all prior distributions -----
    // -------------------------------------------------------
    
    vector<Prior*> ptrPriors(1);


    // Uniform Prior

    ArrayXd parametersMinima(Ndimensions);                      // Minima
    ArrayXd parametersMaxima(Ndimensions);                      // Maxima
   
   
    // Configuration for Punto (KIC 9139163)
    
    parametersMinima(0) = 0.3;                   // Flat noise level
    parametersMaxima(0) = 0.5;
    parametersMinima(1) = 9.30;                  // Height power law 
    parametersMaxima(1) = 9.36;
    parametersMinima(2) = -2.1;                   // Exponent power law
    parametersMaxima(2) = -1.85;
    parametersMinima(3) = 41.;                   // Amplitude Harvey 1    
    parametersMaxima(3) = 53.;
    parametersMinima(4) = 280.0;                 // Timescale Harvey 1
    parametersMaxima(4) = 312.0;    
    parametersMinima(5) = 1.8;                  // Exponent Harvey 1
    parametersMaxima(5) = 2.2;    
    parametersMinima(6) = 0.3;                   // Height Oscillation    
    parametersMaxima(6) = 0.7;    
    parametersMinima(7) = 1650.;                 // nuMax
    parametersMaxima(7) = 1675.;
    parametersMinima(8) = 290.;                  // sigma
    parametersMaxima(8) = 315.;

    UniformPrior uniformPrior(parametersMinima, parametersMaxima);
    ptrPriors[0] = &uniformPrior;


    // -----------------------------------------------------------------
    // ----- Third step. Set up the likelihood function to be used -----
    // -----------------------------------------------------------------
    
    ExponentialLikelihood likelihood(observations, model);
    

    // -------------------------------------------------------------------------------
    // ----- Fourth step. Set up the K-means clusterer using an Euclidean metric -----
    // -------------------------------------------------------------------------------

    int minNclusters = 1;
    int maxNclusters = 4;
    int Ntrials = 10;
    double relTolerance = 0.01;

    EuclideanMetric myMetric;
    KmeansClusterer kmeans(myMetric, minNclusters, maxNclusters, Ntrials, relTolerance); 


    // ---------------------------------------------------------------------
    // ----- Sixth step. Configure and start nested sampling inference -----
    // ---------------------------------------------------------------------
    
    bool printOnTheScreen = true;                   // Print results on the screen
    int initialNobjects = 800;                     // Maximum (and initial) number of live points evolving within the nested sampling process. 
    int minNobjects = 800;                         // Minimum number of live points allowed in the computation
    int maxNdrawAttempts = 10000;                   // Maximum number of attempts when trying to draw a new sampling point
    int NinitialIterationsWithoutClustering = static_cast<int>(initialNobjects*0.5);    // The first N iterations, we assume that there is only 1 cluster
    int NiterationsWithSameClustering = static_cast<int>(initialNobjects*0.05);        // Clustering is only happening every N iterations.
    double initialEnlargementFraction = 2.50;       // Fraction by which each axis in an ellipsoid has to be enlarged.
                                                    // It can be a number >= 0, where 0 means no enlargement.
    double shrinkingRate = 0.6;                     // Exponent for remaining prior mass in ellipsoid enlargement fraction.
                                                    // It is a number between 0 and 1. The smaller the slower the shrinkage
                                                    // of the ellipsoids.
    double terminationFactor = 1.0;                // Termination factor for nested sampling process.

    
    MultiEllipsoidSampler nestedSampler(printOnTheScreen, ptrPriors, likelihood, myMetric, kmeans, 
                                        initialNobjects, minNobjects, initialEnlargementFraction, shrinkingRate);
    
    double tolerance = 1.e2;
    double exponent = 0.4;
    PowerlawReducer livePointsReducer(nestedSampler, tolerance, exponent, terminationFactor);

    string outputPathPrefix = outputDirName + "background_";
    nestedSampler.run(livePointsReducer, NinitialIterationsWithoutClustering, NiterationsWithSameClustering, 
                      maxNdrawAttempts, terminationFactor, outputPathPrefix);


    // -------------------------------------------------------
    // ----- Last step. Save the results in output files -----
    // -------------------------------------------------------
   
    Results results(nestedSampler);
    results.writeParametersToFile("parameter");
    results.writeLogLikelihoodToFile("logLikelihood.txt");
    results.writeEvidenceInformationToFile("evidenceInformation.txt");
    results.writePosteriorProbabilityToFile("posteriorDistribution.txt");

    double credibleLevel = 68.3;
    bool writeMarginalDistributionToFile = true;
    results.writeParametersSummaryToFile("parameterSummary.txt", credibleLevel, writeMarginalDistributionToFile);


    return EXIT_SUCCESS;
}
