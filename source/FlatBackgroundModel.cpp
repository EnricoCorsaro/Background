#include "FlatBackgroundModel.h"


// FlatBackgroundModel::FlatBackgroundModel()
//
// PURPOSE: 
//      Constructor. Initializes model computation.
//
// INPUT:
//      covariates:             one-dimensional array containing the values
//                              of the independent variable
//      inputNyquestFrequencyFileName:      the string containing the file name of the input ASCII file with the
//                                          value of the Nyquist frequency to be adopted in the response function.
//

FlatBackgroundModel::FlatBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    // NyquistFrequency = 8496.355743094671     muHz     // Kepler SC
    // NyquistFrequency = 283.2116656017908     muHz     // Kepler LC

    readNyquistFrequencyFromFile(inputNyquistFrequencyFileName);

    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square();
}










// FlatBackgroundModel::FlatBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

FlatBackgroundModel::~FlatBackgroundModel()
{

}










// FlatBackgroundModel::predict()
//
// PURPOSE:
//      Builds the predictions from a background model for solar-like and giant stars.
//      The model consists of one constant component, two Harvey-like profiles
//      and a Gaussian for modeling the oscillation envelope.
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
//      (2) Height of the oscillation envelope (ppm^2 / muHz)
//      (3) nuMax (muHz)
//      (4) sigma (muHz)
//

void FlatBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double heightOscillation = modelParameters(1);
    double nuMax = modelParameters(2);
    double sigma = modelParameters(3);


    // Compute Gaussian envelope for RGB Oscillations and add it to the predictions

    predictions += heightOscillation * exp(-1.0*(nuMax - covariates)*(nuMax - covariates)/(2.0 * sigma * sigma));

    
    // Modulate the model by the response function (apodization)
    
    predictions *= responseFunction;           


    // Add flat noise level component

    predictions += flatNoiseLevel;
}

