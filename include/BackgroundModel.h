// Derived class for global background fit to RG stars.
// Created by Enrico Corsaro @ IvS - 19 November 2013
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "BackgroundModel.h"
// Implementations contained in "BackgroundModel.cpp"


#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class BackgroundModel : public Model
{
    public:
    
        BackgroundModel(const RefArrayXd covariates);
        ~BackgroundModel();
        
        ArrayXd getResponseFunction();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters) = 0;


    protected:

        ArrayXd responseFunction;

    private:

}; 


#endif
