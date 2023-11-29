// Derived class for global background fit to photometric power spectra with no colored-noise and no long-trend component
// Created by Enrico Corsaro @ OACT - January 2019
// e-mail: emncorsaro@gmail.com
// Header file "TwoHarveyTwoGaussianBackgroundModel.h"
// Implementations contained in "TwoHarveyTwoGaussianBackgroundModel.cpp"


#ifndef TWOHARVEYTWOGAUSSIANBACKGROUNDMODEL_H
#define TWOHARVEYTWOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class TwoHarveyTwoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        TwoHarveyTwoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~TwoHarveyTwoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
