// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ CEA - October 2015
// e-mail: emncorsaro@gmail.com
// Header file "MainSequenceBackgroundModel.h"
// Implementations contained in "MainSequenceBackgroundModel.cpp"


#ifndef MAINSEQUENCEBACKGROUNDMODEL_H
#define MAINSEQUENCEBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class MainSequenceBackgroundModel : public BackgroundModel
{
    public:
    
        MainSequenceBackgroundModel(const RefArrayXd covariates);
        ~MainSequenceBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
