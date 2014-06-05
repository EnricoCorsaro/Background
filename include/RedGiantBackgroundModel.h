// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ IvS - 18 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "RedGiantBackgroundModel.h"
// Implementations contained in "RedGiantBackgroundModel.cpp"


#ifndef REDGIANTBACKGROUNDMODEL_H
#define REDGIANTBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class RedGiantBackgroundModel : public BackgroundModel
{
    public:
    
        RedGiantBackgroundModel(const RefArrayXd covariates);
        ~RedGiantBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
