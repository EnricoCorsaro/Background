// Derived class for global background fit to standard background model for photometric observations.
// Created by Enrico Corsaro @ CEA - January 2015
// e-mail: emncorsaro@gmail.com
// Header file "ThreeHarveyBackgroundModel.h"
// Implementations contained in "ThreeHarveyBackgroundModel.cpp"


#ifndef THREEHARVEYBACKGROUNDMODEL_H
#define THREEHARVEYBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class ThreeHarveyBackgroundModel : public BackgroundModel
{
    public:
    
        ThreeHarveyBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~ThreeHarveyBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
