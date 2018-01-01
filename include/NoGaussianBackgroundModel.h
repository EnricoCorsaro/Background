// Derived class for global background fit to standard background model for photometric observations.
// Created by Enrico Corsaro @ INAF-OACT - November 2017
// e-mail: emncorsaro@gmail.com
// Header file "NoGaussianBackgroundModel.h"
// Implementations contained in "NoGaussianBackgroundModel.cpp"


#ifndef NOGAUSSIANBACKGROUNDMODEL_H
#define NOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class NoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        NoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~NoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
