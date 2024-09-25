// Derived class for global background fit to photometric power spectra with a classic one Harvey profile.
// Created by Enrico Corsaro @ OACT - February 2019
// e-mail: emncorsaro@gmail.com
// Header file "OriginalBackgroundModel.h"
// Implementations contained in "OriginalBackgroundModel.cpp"


#ifndef ORIGINALBACKGROUNDMODEL_H
#define ORIGINALBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class OriginalBackgroundModel : public BackgroundModel
{
    public:
    
        OriginalBackgroundModel(const RefArrayXd covariates, const string inputNyquistFrequencyFileName);
        ~OriginalBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
