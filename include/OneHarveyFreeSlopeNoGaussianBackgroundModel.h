// Derived class for global background fit to photometric power spectra with a single Harvey component
// having a varying slope, and no colored-noise and no Gaussian envelope.
// Created by Enrico Corsaro @ OACT - May 2023
// e-mail: enrico.corsaro@inaf.it
// Header file "OneHarveyFreeSlopeNoGaussianBackgroundModel.h"
// Implementations contained in "OneHarveyFreeSlopeNoGaussianBackgroundModel.cpp"


#ifndef ONEHARVEYFREESLOPENOGAUSSIANBACKGROUNDMODEL_H
#define ONEHARVEYFREESLOPENOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class OneHarveyFreeSlopeNoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        OneHarveyFreeSlopeNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~OneHarveyFreeSlopeNoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
