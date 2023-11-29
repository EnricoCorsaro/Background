#include "TwoHarveyTwoGaussianBackgroundModel.h"


// TwoHarveyTwoGaussianBackgroundModel::TwoHarveyTwoGaussianBackgroundModel()
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

TwoHarveyTwoGaussianBackgroundModel::TwoHarveyTwoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    // NyquistFrequency = 8496.355743094671     muHz     // Kepler SC
    // NyquistFrequency = 283.2116656017908     muHz     // Kepler LC

    readNyquistFrequencyFromFile(inputNyquistFrequencyFileName);

    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square();
}










// TwoHarveyTwoGaussianBackgroundModel::TwoHarveyTwoGaussianBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

TwoHarveyTwoGaussianBackgroundModel::~TwoHarveyTwoGaussianBackgroundModel()
{

}










// TwoHarveyTwoGaussianBackgroundModel::predict()
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
//      (2) Colored noise amplitude (ppm)
//      (3) Colored noise frequency (muHz)
//      (4) Amplitude of the background component (ppm)
//      (5) Frequency of the background component (muHz)
//      (6) Height 1 of the oscillation envelope (ppm^2 / muHz)
//      (7) nuMax 1 (muHz)
//      (8) sigma 1 (muHz)
//      (9) Height 2 of the oscillation envelope (ppm^2 / muHz)
//      (10) nuMax 2 (muHz)
//      (11) sigma 2 (muHz)
//

void TwoHarveyTwoGaussianBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double amplitudeHarvey1 = modelParameters(1);
    double frequencyHarvey1 = modelParameters(2);
    double amplitudeHarvey2 = modelParameters(3);
    double frequencyHarvey2 = modelParameters(4);
    double heightOscillation1 = modelParameters(5);
    double nuMax1 = modelParameters(6);
    double sigma1 = modelParameters(7);
    double heightOscillation2 = modelParameters(8);
    double nuMax2 = modelParameters(9);
    double sigma2 = modelParameters(10);


    // Compute Harvey components and add them to the predictions

    double zeta = 2.0*sqrt(2.0)/Functions::PI;
    predictions = zeta*amplitudeHarvey1*amplitudeHarvey1/(frequencyHarvey1*(1.0 + (covariates/frequencyHarvey1).pow(4)));
    predictions += zeta*amplitudeHarvey2*amplitudeHarvey2/(frequencyHarvey2*(1.0 + (covariates/frequencyHarvey2).pow(4)));


    // Compute Gaussian envelopes for RGB Oscillations and add it to the predictions

    predictions += heightOscillation1 * exp(-1.0*(nuMax1 - covariates)*(nuMax1 - covariates)/(2.0 * sigma1 * sigma1));
    predictions += heightOscillation2 * exp(-1.0*(nuMax2 - covariates)*(nuMax2 - covariates)/(2.0 * sigma2 * sigma1));

    
    // Modulate the model by the response function (apodization)
    
    predictions *= responseFunction;           


    // Add flat noise and colored noise components

    predictions += flatNoiseLevel;
}











