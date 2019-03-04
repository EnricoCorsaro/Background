// Derived class for global background fit to photometric power spectra with colored-noise 
// included (typical for low frequency stars, nuMax < 50 muHz).
// Created by Enrico Corsaro @ OACT - November 2018
// e-mail: emncorsaro@gmail.com
// Header file "TwoHarveyColorBackgroundModel.h"
// Implementations contained in "TwoHarveyColorBackgroundModel.cpp"


#ifndef TWOHARVEYCOLORBACKGROUNDMODEL_H
#define TWOHARVEYCOLORBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class TwoHarveyColorBackgroundModel : public BackgroundModel
{
    public:
    
        TwoHarveyColorBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~TwoHarveyColorBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
