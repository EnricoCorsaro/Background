// Derived class for global background fit to photometric power spectra with colored-noise 
// included (typical for low frequency stars, nuMax < 50 muHz).
// Created by Enrico Corsaro @ CEA - January 2016
// e-mail: emncorsaro@gmail.com
// Header file "ThreeHarveyColorBackgroundModel.h"
// Implementations contained in "ThreeHarveyColorBackgroundModel.cpp"


#ifndef THREEHARVEYCOLORBACKGROUNDMODEL_H
#define THREEHARVEYCOLORBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class ThreeHarveyColorBackgroundModel : public BackgroundModel
{
    public:
    
        ThreeHarveyColorBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~ThreeHarveyColorBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
