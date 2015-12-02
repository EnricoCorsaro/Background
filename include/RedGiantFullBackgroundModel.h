// Derived class for global background fit to RG stars with colored-noise included (low frequency stars).
// Created by Enrico Corsaro @ IvS - 14 February 2015
// e-mail: emncorsaro@gmail.com
// Header file "RedGiantFullBackgroundModel.h"
// Implementations contained in "RedGiantFullBackgroundModel.cpp"


#ifndef REDGIANTFULLBACKGROUNDMODEL_H
#define REDGIANTFULLBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class RedGiantFullBackgroundModel : public BackgroundModel
{
    public:
    
        RedGiantFullBackgroundModel(const RefArrayXd covariates);
        ~RedGiantFullBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
