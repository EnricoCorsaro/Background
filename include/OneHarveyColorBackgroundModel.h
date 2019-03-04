// Derived class for global background fit to photometric power spectra with a single Harvey component and colored-noise
// Created by Enrico Corsaro @ OACT - January 2019
// e-mail: emncorsaro@gmail.com
// Header file "OneHarveyColorBackgroundModel.h"
// Implementations contained in "OneHarveyColorBackgroundModel.cpp"


#ifndef ONEHARVEYCOLORBACKGROUNDMODEL_H
#define ONEHARVEYCOLORBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class OneHarveyColorBackgroundModel : public BackgroundModel
{
    public:
    
        OneHarveyColorBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~OneHarveyColorBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
