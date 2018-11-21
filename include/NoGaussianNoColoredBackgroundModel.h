// Derived class for global background fit to standard background model for photometric observations.
// This model only fits the three Harvey-like components due to stellar signal and a white noise.
// Created by Enrico Corsaro @ INAF-OACT - November 2017
// e-mail: emncorsaro@gmail.com
// Header file "NoGaussianNoColoredBackgroundModel.h"
// Implementations contained in "NoGaussianNoColoredBackgroundModel.cpp"


#ifndef NOGAUSSIANNOCOLOREDBACKGROUNDMODEL_H
#define NOGAUSSIANNOCOLOREDBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class NoGaussianNoColoredBackgroundModel : public BackgroundModel
{
    public:
    
        NoGaussianNoColoredBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~NoGaussianNoColoredBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
