#ifndef SCALARMATRIX_H
#define SCALARMATRIX_H

#include "diagonal_matrix.h" 

template<class T>
class scalar_matrix: public diagonal_matrix<T>{
private:

public:
    scalar_matrix<T>* clone()const;
    scalar_matrix(int, const T& =0);
    scalar_matrix(const matrix<T>& t);
    virtual T Det ()const;
};



//////////////////////////////
//  C O S T R U T T O R I   //
//////////////////////////////

template <class T>
scalar_matrix<T>::scalar_matrix(int dim, const T& t): diagonal_matrix<T>(dim,t){}

template <class T>
scalar_matrix<T>::scalar_matrix(const matrix<T>& t): diagonal_matrix<T>(t){}

//////////////////////////////
//   O P E R A Z I O N I    //
//////////////////////////////

#include <math.h>
template<class T>
T scalar_matrix<T>::Det()const{
  return pow((*this)[0],this->getL());
}

template <class T>
scalar_matrix<T>* scalar_matrix<T>::clone()const{
  return (new scalar_matrix<T>(*this));
}




#endif
