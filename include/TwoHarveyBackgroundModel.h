// Derived class for global background fit to photometric power spectra with no colored-noise and no long-trend component
// Created by Enrico Corsaro @ OACT - January 2019
// e-mail: emncorsaro@gmail.com
// Header file "TwoHarveyBackgroundModel.h"
// Implementations contained in "TwoHarveyBackgroundModel.cpp"


#ifndef TWOHARVEYBACKGROUNDMODEL_H
#define TWOHARVEYBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class TwoHarveyBackgroundModel : public BackgroundModel
{
    public:
    
        TwoHarveyBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~TwoHarveyBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
