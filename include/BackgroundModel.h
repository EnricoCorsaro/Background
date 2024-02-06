// Derived class for global background fit to RG stars.
// Created by Enrico Corsaro @ IvS - 19 November 2013
// e-mail: emncorsaro@gmail.com
// Header file "BackgroundModel.h"
// Implementations contained in "BackgroundModel.cpp"


#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H

#include <iostream>
#include "Model.h"
#include "Functions.h"
#include "File.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class BackgroundModel : public Model
{
    public:
    
        BackgroundModel(const RefArrayXd covariates);
        ~BackgroundModel();
        
        ArrayXd getResponseFunction();
        double getNyquistFrequency();

        void readNyquistFrequencyFromFile(const string inputFileName);
        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters) = 0;
        virtual void computeVariance(RefArrayXd modelVariance, const RefArrayXd modelParameters){};

    protected:

        double NyquistFrequency;
        ArrayXd responseFunction;

    private:

}; 


#endif
