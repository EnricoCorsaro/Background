// Derived class for global background fit to RG stars.
// Created by Enrico Corsaro @ IvS - 19 November 2013
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "GlobalFitRGModel.h"
// Implementations contained in "GlobalFitRGModel.cpp"


#ifndef GLOBALFITRGMODEL_H
#define GLOBALFITRGMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class GlobalFitRGModel : public Model
{
    public:
    
        GlobalFitRGModel(const RefArrayXd covariates);
        ~GlobalFitRGModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:

        ArrayXd responseFunction;

    private:

}; 


#endif
