// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ IvS - 18 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "GranulationMSModel.h"
// Implementations contained in "GranulationMSModel.cpp"


#ifndef GRANULATIONMSMODEL_H
#define GRANULATIONMSMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class GranulationMSModel : public Model
{
    public:
    
        GranulationMSModel(const RefArrayXd covariates);
        ~GranulationMSModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:

        ArrayXd responseFunction;


    private:

}; 


#endif
