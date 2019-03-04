// Derived class for global background fit to a simple background model for photometric observations.
// Created by Enrico Corsaro @ INAF - February 2019
// e-mail: emncorsaro@gmail.com
// Header file "FlatNoGaussianBackgroundModel.h"
// Implementations contained in "FlatNoGaussianBackgroundModel.cpp"

#ifndef FLATNOGAUSSIANBACKGROUNDMODEL_H
#define FLATNOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class FlatNoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        FlatNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~FlatNoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
