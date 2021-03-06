#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>


template<class T>
class matrix{
/*  Cosa vuol dire sta cosa?
 *  Tutte le istanze di questi overload sono amiche della classe matrice,
 *  indipendentemente dal tipo U che matrix contiene.
 */
template <class U>
  friend std::istream& operator>>(std::istream& is, matrix<U>& m);
template <class U>
  friend std::ostream& operator<<(std::ostream& os, const matrix<U>& m);

private:
    int h;
    int l;
    T* raw_matrix;//array che tiene conto el

    //Fa una deep copy di un array (raw_matrix) da una matrice m, e ne torna
    //il puntatore
    static T* copyArr(const matrix<T>& m);

    //Esegue il prodotto vettoriale fra due vettori estrapolati da matrici
    //sulla riga row e colonna col
    static T VectProd(const matrix<T>&m1,int row, const matrix<T>&m2, int col);

public:
    int getH()const;
    int getL()const;

    //Ridefinizione dei big 3 ££$$$€€€€
    matrix(const int& =1,const int& =1);//inizializzato con l=1,h=1
    matrix(const matrix<T>&);//costruttore di copia
    virtual ~matrix();
    virtual matrix<T>* clone() const;

    //Operatori Matematici fra matrici
    matrix<T>& operator=(const matrix<T>&);
    matrix<T> operator+(const matrix<T>&)const;
    matrix<T> operator-(const matrix<T>&)const;
    virtual matrix<T> operator*(const matrix<T>&)const;
    bool sameDim(const matrix<T>&)const;
    bool isMultBy(const matrix<T>&)const;

    //Operazioni generali su matrici
    virtual void Trasposta();
    virtual void Gauss(int col_num =-1);
    virtual void GaussJordan(int col_num =-1);
    virtual bool isInvertible()const;

    //Metodi di taglia-cuci per matrici
    void Fill(const T& t);
    void Cut(int row_start, int row_num, int col_start, int col_num);
    void Append(const matrix<T>& m1);
    void AppendDown(const matrix<T>& m1);

    //Metodi utili per l'el. di Gauss
    void swap(int r1, int r2);
    void divRow(int r, T coeff);
    int maxCoeff(int now_row, int now_col)const;
    void subRow(int r1,int r2, T coeff =1);
    void approxZero();

    //Funzioni scalari che si applicano elemento per elemento
    matrix<T> operator*(const T&)const;//prodotto con scalare
    void mathOp(double (*function)(double));
    void mathOp(double (*function)(double,double),const double&);

    //Operatore di selezione di un elemento nell'array
    T& operator[](const int&)const;

    //Operatore di ugualianza
    bool operator==(const matrix<T>&)const;

    //Operatore di conversione fra vari tipi di matrice
    //Il template class U è necessario perchè voglio rendere
    //la conversione disponibile con ogni tipo di matrice
    template <class U>
    operator matrix<U>();

};

template<class T>
std::istream& operator>>(std::istream& is, matrix<T>& m);

template<class T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& m);

template<class T>
int matrix<T>::getH()const{ return h;}

template<class T>
int matrix<T>::getL()const{ return l;}

//////////////////////////////
//     S T A T I C H E      //
//////////////////////////////

template <class T>
T* matrix<T>::copyArr(const matrix<T>& m){
  T* t_arr = new T[m.h*m.l];
  for(int i = 0; i < m.h * m.l; i++)
    t_arr[i] = (T)m.raw_matrix[i];
  return t_arr;
}

template <class T>
T matrix<T>::VectProd(const matrix<T>&m1,int row, const matrix<T>&m2, int col){
  T sum = 0;
  for(int i = 0; i < m1.l; i++)
      sum += m1[i+m1.l*row] * m2[col+m2.l*i];
  return sum;
}




//////////////////////////////
//  C O S T R U T T O R I   //
//////////////////////////////

template <class T>
matrix<T>::matrix(const int& height,const int& length):
    h(height),l(length),raw_matrix(new T[length*height]){}


template <class T>
matrix<T>::matrix(const matrix<T>& m):h(m.h),l(m.l),raw_matrix(copyArr(m)){}

template<class T>
matrix<T>::~matrix(){
  delete[] raw_matrix;
}

//////////////////////////////
//   OP  O V E R L O A D    //
//////////////////////////////

template<class T>
std::istream& operator>>(std::istream& is, matrix<T>& m){
  for(int i=0; i < m.h * m.l; i++)
    is >> m[i];
  return is;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const matrix<T>& m){
  for(int i = 0; i < m.h; i++){
    for(int j = 0; j < m.l; j++){
      os << m[i*m.l+j] << "\t\t";
    }
    os << std::endl;
  }
  return os;
}


template <class T>
matrix<T> matrix<T>::operator*(const T& n)const{
  //T* t_arr = new T[h*l];
  matrix<T> m(h,l);
  for(int i=0; i < h*l; i++)
    m[i] = (*this)[i]*n;
  return m;
}

template <class T>
T& matrix<T>::operator[](const int& i)const{
  return raw_matrix[i];
}

#include "basxMath.h"
template <class T>
matrix<T> matrix<T>::operator+(const matrix<T>& m)const{
  if((*this).sameDim(m)){
    matrix<T> temp(h,l);
    for(int i = 0; i < l*h; i++)
      temp[i] = (*this)[i] + m[i];
    return temp;
  }
}

template <class T>
matrix<T> matrix<T>::operator-(const matrix<T>& m)const{
  if((*this).sameDim(m)){
    matrix<T> temp(h,l);
    for(int i = 0; i < l*h; i++)
      temp[i] = (*this)[i] - m[i];
    return temp;
  }
}

template <class T>
matrix<T> matrix<T>::operator*(const matrix<T>& m)const{
  if(l == m.h){
    matrix<T> temp(h,m.l);
    for(int i = 0; i < temp.h; i++)
      for(int j = 0; j < temp.l; j++)
        temp[i*temp.l+j] = VectProd((*this),i,m,j);
    return temp;
  }
}

template <class T>
bool matrix<T>::operator==(const matrix<T>& m)const{
  if(!(*this).sameDim(m))
    return false;
  int i = 0;
  while(i < l*h){
    if((*this)[i] != m[i])
      return false;
    i++;
  }
  return true;
}

template <class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& m){
    if (this == &m)
        return *this;
    T* p = raw_matrix;
    delete[] p;
    l = m.l;
    h = m.h;
    raw_matrix = copyArr(m);
    return (*this);
}

template <class T>
template <class U>
matrix<T>::operator matrix<U>(){
  matrix<U> temp(h,l);
  for(int i = 0; i < l*h; i++)
    temp[i] = (*this)[i];
  return temp;
}



//////////////////////////////
//   O P E R A Z I O N I    //
//////////////////////////////

template <class T>
void matrix<T>::Trasposta(){
  matrix<T> trans(l,h);
  for(int i = 0; i < h; ++i)
        for(int j = 0; j < l; ++j)
            trans[j*h+i] = (*this)[i*l+j];
  *this = trans;
}

template <class T>
void matrix<T>::mathOp(double (*function)(double)){
    for(int i = 0; i < l*h; i++)
        (*this)[i] = (*function)((*this)[i]);
}

template <class T>
void matrix<T>::mathOp(double (*function)(double,double),const double& d){
    for(int i = 0; i < l*h; i++)
        (*this)[i] = (*function)((*this)[i],d);
}

template <class T>
void matrix<T>::swap(int r1, int r2){
  if (r1 != r2){
    T temp;
    for(int c = 0; c < l; c++){
      temp = (*this)[r1*l+c];
      (*this)[r1*l+c] = (*this)[r2*l+c];
      (*this)[r2*l+c] = temp;
    }
  }
}

template <class T>
void matrix<T>::divRow(int r, T coeff){
  for(int c = 0; c < l; c++)
    if((*this)[r*l+c] != 0)
      (*this)[r*l+c] = (*this)[r*l+c]/coeff;
}

template <class T>
int matrix<T>::maxCoeff(int now_row, int now_col)const{
  int max = now_row;
  for(int r = now_row + 1; r < h; r++)
    if( (*this)[r*l+now_col] > (*this)[max*l+now_col])
      max = r;
  return max;
}

template <class T>
void matrix<T>::subRow(int r1, int r2, T coeff){
  for(int c = 0; c < l; c++)
    (*this)[r1*l+c] = (*this)[r1*l+c] - (*this)[r2*l+c] * coeff;
}





template <class T>
void matrix<T>::Gauss(int col_num){
  if (col_num == -1)
    col_num = l;
  int c = 0;
  for(int r = 0; r < h && c < col_num; r++){
    if(raw_matrix[r*l+c] == 0){
      swap(r,maxCoeff(r,c));
      while (raw_matrix[maxCoeff(r,c)*l+c] == 0 && c < col_num)
        c++;
    }

    for(int i = r; i < h && c < col_num; i++)
      if(raw_matrix[i*l+c] != 0){
        divRow(i,raw_matrix[i*l+c]);
      }


    for(int i = r + 1; i < h && c < col_num; i++)
      if(raw_matrix[i*l+c] != 0)
        subRow(i,r,1);

    approxZero();

    c++;
  }

  for(int r = 0; r < h; r++){
      int minzeroInd = r;
      int minzero = col_num+1;
      for(int i = r; i < h; i++){
          int zeros = 0;
          for(int j = 0; j < c && raw_matrix[i*l+j] == 0; j++){
              zeros++;
          }
          if (zeros < minzero){
              minzero = zeros;
              minzeroInd = i;
          }
      }
      if (minzeroInd != r)
          swap(r,minzeroInd);
  }

}

template <class T>
void matrix<T>::approxZero(){
  for(int i = 0; i < l*h; i++)
    if(bxm::abs((*this)[i]) < 0.00000001)
      (*this)[i] = 0;
}

template <class T>
void matrix<T>::GaussJordan(int col_num){
  if (col_num == -1)
    col_num = l;
  Gauss(col_num);

  int r = h - 1;
  int rd;
  for(int c = col_num - 1; c >= 0 && r > 0; c--){
    rd = r;
    while( raw_matrix[rd*l+c] != 1 && rd > 0 )
      rd--;
    if (rd > 0){
      for(int i = rd - 1; i >= 0; i--){
        subRow(i,rd,raw_matrix[i*l+c]);
      }
      r--;
    }
  }
}

template <class T>
void matrix<T>::Cut(int row_start, int row_num, int col_start, int col_num){
  matrix<T> temp(row_num - row_start ,col_num - col_start);
  int pos = 0;
  for(int r = row_start; r < row_num; r++)
    for(int c = col_start; c < col_num; c++){
      temp[pos] = (*this)[r*l+c];
      pos++;
    }
  *this = temp;
}

template <class T>
void matrix<T>::Append(const matrix<T>& m1){
  if(m1.h == h){
    int pos = 0;
    matrix<T> temp(m1.h,l+m1.l);
    for(int r = 0; r < m1.h; r++){
      for(int c = 0; c < l; c++){
        temp[pos] = (*this)[r*l+c];
        pos++;
      }
      for(int c = 0; c < m1.l; c++){
        temp[pos] = m1[r*m1.l+c];
        pos++;
      }
    }
    *this = temp;
  }
}


template <class T>
void matrix<T>::AppendDown(const matrix<T>& m1){
  if(m1.l == l){
    int pos = 0;
    matrix<T> temp(h + m1.h,m1.l);
    int i = 0;
    for(; i < l*h; i++)
        temp[i] = (*this)[i];
    for(; i < temp.h*temp.l; i++)
        temp[i] = m1[i - l*h];

    *this = temp;
  }
}

template <class T>
bool matrix<T>::sameDim(const matrix<T>& m)const{
  return (h==m.h && l==m.l);
}

template <class T>
bool matrix<T>::isMultBy(const matrix<T>& m)const{
    return (h==m.l);
}

template <class T>
void matrix<T>::Fill(const T& t){
  for(int i = 0; i < l*h; i++)
    (*this)[i] = t;
}

template <class T>
bool matrix<T>::isInvertible()const{
  return false;
}

template <class T>
matrix<T>* matrix<T>::clone()const{
  return (new matrix<T>(*this));
}




#endif
