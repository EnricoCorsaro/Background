// Derived class for global background fit to photometric power spectra with a single Harvey component and no colored-noise
// Created by Enrico Corsaro @ OACT - January 2019
// e-mail: emncorsaro@gmail.com
// Header file "OneHarveyBackgroundModel.h"
// Implementations contained in "OneHarveyBackgroundModel.cpp"


#ifndef ONEHARVEYBACKGROUNDMODEL_H
#define ONEHARVEYBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class OneHarveyBackgroundModel : public BackgroundModel
{
    public:
    
        OneHarveyBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~OneHarveyBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
