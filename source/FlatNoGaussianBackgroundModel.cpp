#include "FlatNoGaussianBackgroundModel.h"


// FlatNoGaussianBackgroundModel::FlatNoGaussianBackgroundModel()
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

FlatNoGaussianBackgroundModel::FlatNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    // NyquistFrequency = 8496.355743094671     muHz     // Kepler SC
    // NyquistFrequency = 283.2116656017908     muHz     // Kepler LC

    readNyquistFrequencyFromFile(inputNyquistFrequencyFileName);

    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square();
}










// FlatNoGaussianBackgroundModel::FlatNoGaussianBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

FlatNoGaussianBackgroundModel::~FlatNoGaussianBackgroundModel()
{

}










// FlatNoGaussianBackgroundModel::predict()
//
// PURPOSE:
//      Builds the predictions from a background model for solar-like and giant stars.
//      The model consists of one constant component without the Gaussian envelope.
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
//

void FlatNoGaussianBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);


    // Add flat noise level component

    predictions += flatNoiseLevel;
}

