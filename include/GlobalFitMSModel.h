// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ IvS - 18 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "GlobalFitMSModel.h"
// Implementations contained in "GlobalFitMSModel.cpp"


#ifndef GLOBALFITMSMODEL_H
#define GLOBALFITMSMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class GlobalFitMSModel : public Model
{
    public:
    
        GlobalFitMSModel(const RefArrayXd covariates);
        ~GlobalFitMSModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:

        ArrayXd responseFunction;


    private:

}; 


#endif
