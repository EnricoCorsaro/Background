#include "ThreeHarveyColorBackgroundModel.h"


// ThreeHarveyColorBackgroundModel::ThreeHarveyColorBackgroundModel()
//
// PURPOSE: 
//      Constructor. Initializes model computation.
//
// INPUT:
//      covariates:             one-dimensional array containing the values
//                              of the independent variable.
//      inputNyquestFrequencyFileName:      the string containing the file name of the input ASCII file with the
//                                          value of the Nyquist frequency to be adopted in the response function.
//

ThreeHarveyColorBackgroundModel::ThreeHarveyColorBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    // NyquistFrequency = 8496.355743094671     muHz     // Kepler SC
    // NyquistFrequency = 283.2116656017908     muHz     // Kepler LC

    readNyquistFrequencyFromFile(inputNyquistFrequencyFileName);

    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square();
}










// ThreeHarveyColorBackgroundModel::ThreeHarveyColorBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

ThreeHarveyColorBackgroundModel::~ThreeHarveyColorBackgroundModel()
{

}










// ThreeHarveyColorBackgroundModel::predict()
//
// PURPOSE:
//      Builds the predictions from a GlobalFit model for red giant stars.
//      The model consists of one constant component, three Harvey-like profiles
//      and a Gaussian for modeling the oscillation envelope.
//      A component for colored noise is included, more indicated for low-numax stars
//
// INPUT:
//      predictions:        one-dimensional array to contain the predictions
//                          from the model
//      modelParameters:    one-dimensional array where each element
//                          contains the value of a free parameter of the model
//
// OUTPUT:
//      void
//
// NOTE:
//      The free parameters are to be given in the order
//      (1) White noise background (flat noise level, ppm^2 / muHz)
//      (2) Colored noise amplitude (ppm)
//      (3) Colored noise frequency (muHz)
//      (4) Amplitude of the background component (ppm)
//      (5) Frequency of the background component (muHz)
//      (6) Height of the oscillation envelope (ppm^2 / muHz)
//      (7) nuMax (muHz)
//      (8) sigma (muHz)
//

void ThreeHarveyColorBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double amplitudeNoise = modelParameters(1);
    double frequencyNoise = modelParameters(2);
    double amplitudeHarvey1 = modelParameters(3);
    double frequencyHarvey1 = modelParameters(4);
    double amplitudeHarvey2 = modelParameters(5);
    double frequencyHarvey2 = modelParameters(6);
    double amplitudeHarvey3 = modelParameters(7);
    double frequencyHarvey3 = modelParameters(8);
    double heightOscillation = modelParameters(9);
    double nuMax = modelParameters(10);
    double sigma = modelParameters(11);


    // Compute Harvey components and add them to the predictions

    double zeta = 2.0*sqrt(2.0)/Functions::PI;
    predictions = zeta*amplitudeHarvey1*amplitudeHarvey1/(frequencyHarvey1*(1.0 + (covariates/frequencyHarvey1).pow(4)));
    predictions += zeta*amplitudeHarvey2*amplitudeHarvey2/(frequencyHarvey2*(1.0 + (covariates/frequencyHarvey2).pow(4)));
    predictions += zeta*amplitudeHarvey3*amplitudeHarvey3/(frequencyHarvey3*(1.0 + (covariates/frequencyHarvey3).pow(4)));


    // Compute Gaussian envelope for RGB Oscillations and add it to the predictions

    predictions += heightOscillation * exp(-1.0*(nuMax - covariates)*(nuMax - covariates)/(2.0 * sigma * sigma));

    
    // Modulate the model by the response function (apodization)
    
    predictions *= responseFunction;           


    // Add flat noise and colored noise components

    predictions += flatNoiseLevel;
    predictions += 2.0*Functions::PI*amplitudeNoise*amplitudeNoise/(frequencyNoise*(1.0 + (covariates/frequencyNoise).pow(2)));
}











