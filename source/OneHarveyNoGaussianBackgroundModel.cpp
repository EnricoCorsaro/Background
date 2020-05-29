#include "OneHarveyNoGaussianBackgroundModel.h"


// OneHarveyNoGaussianBackgroundModel::OneHarveyNoGaussianBackgroundModel()
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

OneHarveyNoGaussianBackgroundModel::OneHarveyNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    // NyquistFrequency = 8496.355743094671     muHz     // Kepler SC
    // NyquistFrequency = 283.2116656017908     muHz     // Kepler LC

    readNyquistFrequencyFromFile(inputNyquistFrequencyFileName);

    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square();
}










// OneHarveyNoGaussianBackgroundModel::OneHarveyNoGaussianBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

OneHarveyNoGaussianBackgroundModel::~OneHarveyNoGaussianBackgroundModel()
{

}










// OneHarveyNoGaussianBackgroundModel::predict()
//
// PURPOSE:
//      Builds the predictions from a background model.
//      The model consists of one constant component and one Harvey-like profile.
//      The Harvey-like profile for long-trend variations and granulation are not considered in this model.
//      This model is more suited for low numax and low frequency resolution spectra.
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
//

void OneHarveyNoGaussianBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double amplitudeHarvey1 = modelParameters(1);
    double frequencyHarvey1 = modelParameters(2);


    // Compute Harvey components and add them to the predictions

    double zeta = 2.0*sqrt(2.0)/Functions::PI;
    predictions = zeta*amplitudeHarvey1*amplitudeHarvey1/(frequencyHarvey1*(1.0 + (covariates/frequencyHarvey1).pow(4)));


    // Modulate the model by the response function (apodization)
    
    predictions *= responseFunction;           


    // Add flat noise and colored noise components

    predictions += flatNoiseLevel;
}











