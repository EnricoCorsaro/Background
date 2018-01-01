// Derived class for global background fit to standard background model for photometric observations.
// Created by Enrico Corsaro @ OACT - November 2016
// e-mail: emncorsaro@gmail.com
// Header file "StandardSlopeBackgroundModel.h"
// Implementations contained in "StandardSlopeBackgroundModel.cpp"


#ifndef STANDARDSLOPEBACKGROUNDMODEL_H
#define STANDARDSLOPEBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class StandardSlopeBackgroundModel : public BackgroundModel
{
    public:
    
        StandardSlopeBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~StandardSlopeBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
