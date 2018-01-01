// Derived class for global background fit to a simple background model for photometric observations.
// Created by Enrico Corsaro @ CEA - January 2015
// e-mail: emncorsaro@gmail.com
// Header file "SimpleBackgroundModel.h"
// Implementations contained in "SimpleBackgroundModel.cpp"


#ifndef SIMPLEBACKGROUNDMODEL_H
#define SIMPLEBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class SimpleBackgroundModel : public BackgroundModel
{
    public:
    
        SimpleBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~SimpleBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
