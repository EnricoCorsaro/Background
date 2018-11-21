// Derived class for global background fit to a simple background model for photometric observations.
// Created by Enrico Corsaro @ CEA - January 2015
// e-mail: emncorsaro@gmail.com
// Header file "FlatBackgroundModel.h"
// Implementations contained in "FlatBackgroundModel.cpp"

#ifndef FLATBACKGROUNDMODEL_H
#define FLATBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class FlatBackgroundModel : public BackgroundModel
{
    public:
    
        FlatBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~FlatBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
