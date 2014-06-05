// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ IvS - 18 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "GranulationBackgroundModel.h"
// Implementations contained in "GranulationBackgroundModel.cpp"


#ifndef GRANULATIONBACKGROUNDMODEL_H
#define GRANULATIONBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class GranulationBackgroundModel : public BackgroundModel
{
    public:
    
        GranulationBackgroundModel(const RefArrayXd covariates);
        ~GranulationBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
