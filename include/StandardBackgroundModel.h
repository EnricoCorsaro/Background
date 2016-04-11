// Derived class for global background fit to standard background model for photometric observations.
// Created by Enrico Corsaro @ CEA - January 2015
// e-mail: emncorsaro@gmail.com
// Header file "StandardBackgroundModel.h"
// Implementations contained in "StandardBackgroundModel.cpp"


#ifndef STANDARDBACKGROUNDMODEL_H
#define STANDARDBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class StandardBackgroundModel : public BackgroundModel
{
    public:
    
        StandardBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~StandardBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
