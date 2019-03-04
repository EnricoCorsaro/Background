// Derived class for global background fit for photometric observations.
// The model includes three Harvey components, a colored noise component, and no Gaussian envelope.
// Created by Enrico Corsaro @ INAF-OACT - November 2017
// e-mail: emncorsaro@gmail.com
// Header file "ThreeHarveyColorNoGaussianBackgroundModel.h"
// Implementations contained in "ThreeHarveyColorNoGaussianBackgroundModel.cpp"


#ifndef THREEHARVEYCOLORNOGAUSSIANBACKGROUNDMODEL_H
#define THREEHARVEYCOLORNOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class ThreeHarveyColorNoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        ThreeHarveyColorNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~ThreeHarveyColorNoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
