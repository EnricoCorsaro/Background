#include "FaculaeBackgroundModel.h"


// FaculaeBackgroundModel::FaculaeBackgroundModel()
//
// PURPOSE: 
//      Constructor. Initializes model computation.
//
// INPUT:
//      covariates:             one-dimensional array containing the values
//                              of the independent variable.
//

FaculaeBackgroundModel::FaculaeBackgroundModel(const RefArrayXd covariates)
: BackgroundModel(covariates)
{
    // Create response function modulating the sampling rate of input Kepler SC data

    double NyquistFrequency = 8496.355743094671;    // muHz
    ArrayXd sincFunctionArgument = (Functions::PI / 2.0) * covariates / NyquistFrequency;
    responseFunction = (sincFunctionArgument.sin() / sincFunctionArgument).square(); 
}










// FaculaeBackgroundModel::FaculaeBackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

FaculaeBackgroundModel::~FaculaeBackgroundModel()
{

}










// FaculaeBackgroundModel::predict()
//
// PURPOSE:
//      Builds the predictions from a GlobalFit model for main-sequence stars.
//      The model consists of one constant component, one power law and
//      one (or two) Harvey-like profiles, plus an additional Gaussian for
//      modeling the oscillation envelope.
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
//      (1) White noise background (flat noise level)
//      (2) Height power law (ppm^2 / muHz)
//      (3) Exponent power law
//      (4) Amplitude of the Harvey profile (ppm)
//      (5) Timescale of the Harvey profile (seconds)
//      (6) Exponent of the Harvey profile
//      (6) Height of the oscillation envelope (ppm^2 / muHz)
//      (7) nuMax (muHz)
//      (8) sigma (muHz)
//

void FaculaeBackgroundModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    Nparameters = modelParameters.size();


    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double heightPowerLaw = modelParameters(1);
    double exponentPowerLaw = modelParameters(2);
    double amplitudeHarvey1 = modelParameters(3);
    double timescaleHarvey1 = modelParameters(4);
    double exponentHarvey1 = modelParameters(5);
    double amplitudeHarvey2 = modelParameters(6);
    double timescaleHarvey2 = modelParameters(7);
    double exponentHarvey2 = modelParameters(8);
    double heightOscillation = modelParameters(9);
    double nuMax = modelParameters(10);
    double sigma = modelParameters(11);


    // Compute decaying power law component
    
    predictions = exp(heightPowerLaw) * pow(covariates, exponentPowerLaw);


    // Compute Harvey-like components and add them to the predictions

    predictions += 4*amplitudeHarvey1*amplitudeHarvey1 * (timescaleHarvey1/1.e6) / 
                   (1.0 + pow(2*Functions::PI*covariates*timescaleHarvey1/1.e6, exponentHarvey1));
    predictions += 4*amplitudeHarvey2*amplitudeHarvey2 * (timescaleHarvey2/1.e6) / 
                   (1.0 + pow(2*Functions::PI*covariates*timescaleHarvey2/1.e6, exponentHarvey2));


    // Compute Gaussian envelope for MS Oscillations and add it to the predictions

    predictions += heightOscillation * exp(-1.0*(nuMax - covariates)*(nuMax - covariates)/(2.0 * sigma * sigma));


    // Modulate the model by the response function (apodization)

    predictions *= responseFunction;


    // Add flat noise level component

    predictions += flatNoiseLevel;

}











