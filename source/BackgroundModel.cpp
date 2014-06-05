#include "BackgroundModel.h"


// BackgroundModel::BackgroundModel()
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

BackgroundModel::BackgroundModel(const RefArrayXd covariates)
: Model(covariates)
{
}










// BackgroundModel::BackgroundModel()
//
// PURPOSE: 
//      Destructor.
//

BackgroundModel::~BackgroundModel()
{

}











// BackgroundModel::getResponseFunction()
//
// PURPOSE:
//      Gets the protected data member responseFunction.
//
// OUTPUT:
//      An eigen array containing the apodization response function for the signal of the input data
//

ArrayXd BackgroundModel::getResponseFunction()
{
    return responseFunction;
}
