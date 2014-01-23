#include "GlobalFitRGModel.h"


// GlobalFitRGModel::GlobalFitRGModel()
//
// PURPOSE: 
//      Constructor. Initializes model computation.
//
// INPUT:
//      covariates:             one-dimensional array containing the values
//                              of the independent variable.
//

GlobalFitRGModel::GlobalFitRGModel(const RefArrayXd covariates)
: Model(covariates)
{
    // Create response function modulating the sampling rate of input Kepler LC data

    double NyquistFrequency = 283.2116656017908;    // muHz
    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square(); 
}










// GlobalFitRGModel::GlobalFitRGModel()
//
// PURPOSE: 
//      Destructor.
//

GlobalFitRGModel::~GlobalFitRGModel()
{

}










// GlobalFitRGModel::predict()
//
// PURPOSE:
//      Builds the predictions from a GlobalFit model for red giant stars.
//      The model consists of one constant component, three Harvey-like profiles
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
//      (2) Amplitude of the background component (ppm)
//      (3) Frequency of the background component (muHz)
//      (4) Height of the oscillation envelope (ppm^2 / muHz)
//      (5) nuMax (muHz)
//      (6) sigma (muHz)
//

void GlobalFitRGModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    Nparameters = modelParameters.size();


    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double amplitudeHarvey1 = modelParameters(1);
    double amplitudeHarvey2 = modelParameters(2);
    double amplitudeHarvey3 = modelParameters(3);
    double frequencyHarvey1 = modelParameters(4);
    double frequencyHarvey2 = modelParameters(5);
    double frequencyHarvey3 = modelParameters(6);
    double heightOscillation = modelParameters(7);
    double nuMax = modelParameters(8);
    double sigma = modelParameters(9);


    // Compute Harvey components and add them to the predictions

    predictions = 2*Functions::PI*amplitudeHarvey1*amplitudeHarvey1/(frequencyHarvey1*(1.0 + (covariates/frequencyHarvey1).pow(4)));
    predictions += 2*Functions::PI*amplitudeHarvey2*amplitudeHarvey2/(frequencyHarvey2*(1.0 + (covariates/frequencyHarvey2).pow(4)));
    predictions += 2*Functions::PI*amplitudeHarvey3*amplitudeHarvey3/(frequencyHarvey3*(1.0 + (covariates/frequencyHarvey3).pow(4)));


    // Compute Gaussian envelope for RGB Oscillations and add it to the predictions

    predictions += heightOscillation * exp(-1.0*(nuMax - covariates)*(nuMax - covariates)/(2.0 * sigma * sigma));

    
    // Modulate the model by the response function (apodization)
    
    predictions *= responseFunction;           


    // Add flat noise level component

    predictions += flatNoiseLevel;
}











