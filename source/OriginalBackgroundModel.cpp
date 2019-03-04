#include "OriginalBackgroundModel.h"


// OriginalBackgroundModel::OriginalBackgroundModel()
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

OriginalBackgroundModel::OriginalBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    // NyquistFrequency = 8496.355743094671     muHz     // Kepler SC
    // NyquistFrequency = 283.2116656017908     muHz     // Kepler LC

    readNyquistFrequencyFromFile(inputNyquistFrequencyFileName);

    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square();
}










// OriginalBackgroundModel::OriginalBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

OriginalBackgroundModel::~OriginalBackgroundModel()
{

}










// OriginalBackgroundModel::predict()
//
// PURPOSE:
//      Builds the predictions from a background model for red giant stars.
//      The model consists of one constant component, two Harvey-like profiles
//      and a Gaussian for modeling the oscillation envelope.
//      The Harvey-like profile for long-trend variations is not considered in this model.
//      This model is more suited for very low-numax stars.
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
//      (2) Amplitude of the background component (ppm)
//      (3) Frequency of the background component (muHz)
//      (4) Height of the oscillation envelope (ppm^2 / muHz)
//      (5) nuMax (muHz)
//      (6) sigma (muHz)
//

void OriginalBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double amplitudeHarvey1 = modelParameters(1);
    double frequencyHarvey1 = modelParameters(2);
    double heightOscillation = modelParameters(3);
    double nuMax = modelParameters(4);
    double sigma = modelParameters(5);


    // Compute Harvey components and add them to the predictions

    predictions = 4.0*amplitudeHarvey1*amplitudeHarvey1/(frequencyHarvey1*(1.0 + (2*Functions::PI*covariates/frequencyHarvey1).pow(2)));


    // Compute Gaussian envelope for RGB Oscillations and add it to the predictions

    predictions += heightOscillation * exp(-1.0*(nuMax - covariates)*(nuMax - covariates)/(2.0 * sigma * sigma));

    
    // Modulate the model by the response function (apodization)
    
    predictions *= responseFunction;           


    // Add flat noise and colored noise components

    predictions += flatNoiseLevel;
}











