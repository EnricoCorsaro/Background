// Derived class for global background fit to photometric power spectra with a single Harvey component
// having a varying slope, and no colored-noise
// Created by Enrico Corsaro @ OACT - May 2023
// e-mail: enrico.corsaro@inaf.it
// Header file "OneHarveyFreeSlopeBackgroundModel.h"
// Implementations contained in "OneHarveyFreeSlopeBackgroundModel.cpp"


#ifndef ONEHARVEYFREESLOPEBACKGROUNDMODEL_H
#define ONEHARVEYFREESLOPEBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class OneHarveyFreeSlopeBackgroundModel : public BackgroundModel
{
    public:
    
        OneHarveyFreeSlopeBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~OneHarveyFreeSlopeBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
