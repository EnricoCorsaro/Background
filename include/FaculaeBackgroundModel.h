// Derived class for global background fit to MS stars.
// Created by Enrico Corsaro @ IvS - 18 January 2014
// e-mail: enrico.corsaro@ster.kuleuven.be
// Header file "FaculaeBackgroundModel.h"
// Implementations contained in "FaculaeBackgroundModel.cpp"


#ifndef FACULAEBACKGROUNDMODEL_H
#define FACULAEBACKGROUNDMODEL_H

#include <iostream>
#include "BackgroundModel.h"
#include "Functions.h"

using namespace std;
using Eigen::ArrayXd;
typedef Eigen::Ref<Eigen::ArrayXd> RefArrayXd;


class FaculaeBackgroundModel : public BackgroundModel
{
    public:
    
        FaculaeBackgroundModel(const RefArrayXd covariates);
        ~FaculaeBackgroundModel();

        virtual void predict(RefArrayXd predictions, RefArrayXd const modelParameters);


    protected:


    private:

}; 


#endif
