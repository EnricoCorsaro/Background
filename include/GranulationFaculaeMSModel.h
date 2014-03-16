// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ IvS - 18 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "GranulationFaculaeMSModel.h"
// Implementations contained in "GranulationFaculaeMSModel.cpp"


#ifndef GRANULATIONFACULAEMSMODEL_H
#define GRANULATIONFACULAEMSMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class GranulationFaculaeMSModel : public Model
{
    public:
    
        GranulationFaculaeMSModel(const RefArrayXd covariates);
        ~GranulationFaculaeMSModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:

        ArrayXd responseFunction;


    private:

}; 


#endif
