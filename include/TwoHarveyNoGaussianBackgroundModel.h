// Derived class for global background fit to photometric power spectra with no colored-noise,
// no long-trend component, and no Gaussian envelope
// Created by Enrico Corsaro @ OACT - January 2019
// e-mail: emncorsaro@gmail.com
// Header file "TwoHarveyNoGaussianBackgroundModel.h"
// Implementations contained in "TwoHarveyNoGaussianBackgroundModel.cpp"


#ifndef TWOHARVEYNOGAUSSIANBACKGROUNDMODEL_H
#define TWOHARVEYNOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class TwoHarveyNoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        TwoHarveyNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~TwoHarveyNoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
