// Derived class for global background fit to RG stars.
// Created by Enrico Corsaro @ IvS - 19 November 2013
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "GlobalFitModel.h"
// Implementations contained in "GlobalFitModel.cpp"


#ifndef GLOBALFITMODEL_H
#define GLOBALFITMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class GlobalFitModel : public Model
{
    public:
    
        GlobalFitModel(const RefArrayXd covariates);
        ~GlobalFitModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
