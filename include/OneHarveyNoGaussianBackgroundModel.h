// Derived class for global background fit to photometric power spectra with no colored-noise,
// no long-trend component, and no Gaussian envelope
// Created by Enrico Corsaro @ OACT - January 2019
// e-mail: emncorsaro@gmail.com
// Header file "OneHarveyNoGaussianBackgroundModel.h"
// Implementations contained in "OneHarveyNoGaussianBackgroundModel.cpp"


#ifndef ONEHARVEYNOGAUSSIANBACKGROUNDMODEL_H
#define ONEHARVEYNOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class OneHarveyNoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        OneHarveyNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~OneHarveyNoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
