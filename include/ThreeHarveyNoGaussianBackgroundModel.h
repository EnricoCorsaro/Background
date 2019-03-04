// Derived class for global background fit for photometric observations.
// The model includes three Harvey components and no Gaussian envelope.
// Created by Enrico Corsaro @ INAF-OACT - November 2017
// e-mail: emncorsaro@gmail.com
// Header file "ThreeHarveyNoGaussianBackgroundModel.h"
// Implementations contained in "ThreeHarveyNoGaussianBackgroundModel.cpp"


#ifndef THREEHARVEYNOGAUSSIANBACKGROUNDMODEL_H
#define THREEHARVEYNOGAUSSIANBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class ThreeHarveyNoGaussianBackgroundModel : public BackgroundModel
{
    public:
    
        ThreeHarveyNoGaussianBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~ThreeHarveyNoGaussianBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
