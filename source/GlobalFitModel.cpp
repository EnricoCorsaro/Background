#include "GlobalFitModel.h"


// GlobalFitModel::GlobalFitModel()
//
// PURPOSE: 
//      Constructor. Initializes model computation.
//
// INPUT:
//      covariates:             one-dimensional array containing the values
//                              of the independent variable.
//      NparametersPerType:     configuring numbers for the Peak Bagging model
//      nuMax:                  the frequency of maximum power excess
//

GlobalFitModel::GlobalFitModel(const RefArrayXd covariates)
: Model(covariates)
{
}










// GlobalFitModel::GlobalFitModel()
//
// PURPOSE: 
//      Destructor.
//

GlobalFitModel::~GlobalFitModel()
{

}










// GlobalFitModel::predict()
//
// PURPOSE:
//      Builds the predictions from a GlobalFit model based on a simple
//      inputting of the central frequencies for all the desired modes to be fitted.
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
//      (2) White noise background (flat noise level)
//      (3) Mode central frequency
//      (5) Mode profile ln(height)
//      (6) Mode profile linewidth

void GlobalFitModel::predict(RefArrayXd predictions, RefArrayXd const modelParameters)
{
    Nparameters = modelParameters.size();


    // Initialize global parameters

    double flatNoiseLevel = modelParameters(0);
    double amplitude1 = modelParameters(1);
    double amplitude2 = modelParameters(2);
    double amplitude3 = modelParameters(3);
    double noiseFrequency1 = modelParameters(4);
    double noiseFrequency2 = modelParameters(5);
    double noiseFrequency3 = modelParameters(6);
    double oscillationHeight = modelParameters(7);
    double nuMax = modelParameters(8);
    double sigma = modelParameters(9);


    // Compute granulation noise components and add them to the predictions

    predictions = 2*Functions::PI*amplitude1*amplitude1/(noiseFrequency1*(1.0 + pow(covariates/noiseFrequency1,4)));
    predictions += 2*Functions::PI*amplitude2*amplitude2/(noiseFrequency2*(1.0 + pow(covariates/noiseFrequency2,4)));
    predictions += 2*Functions::PI*amplitude3*amplitude3/(noiseFrequency3*(1.0 + pow(covariates/noiseFrequency3,4)));


    // Compute Gaussian envelope for RGB Oscillations and add it to the predictions

    predictions += oscillationHeight * exp(-1.0*(nuMax - covariates)*(nuMax - covariates)/(2.0 * sigma * sigma));


    // Add flat noise level component

    predictions += flatNoiseLevel;           
}











