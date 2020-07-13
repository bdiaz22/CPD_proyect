#include <iostream>
#include "tnt_array1d.h"
#include "tnt_array1d_utils.h"
#include "tnt_array2d.h"
#include "tnt_array2d_utils.h"
#include "tnt_math_utils.h"
#include "funclass.h"
#include "RngStream.h"
#include <chrono>
using namespace std;

int main(int argc,char *argv[]){

  auto start = std::chrono::high_resolution_clock::now();

  TNT::Array2D<int> patn(32, 5, 0);
  funClass::genPats(0, 31, 0, patn);
  
  double* pAlpha = new double[5];
  pAlpha[0] = -1.868265; 
  pAlpha[1] = -0.7910062; 
  pAlpha[2] = -1.460980;
  pAlpha[3] = -0.521506; 
  pAlpha[4] = -1.992975;
  
  double beta = 1.011268;
  int nRep = 1e5;

  unsigned long seed[6] = { 1806547166, 3311292359,  
			                      643431772, 1162448557, 
			                      3335719306, 4161054083 };
  
  RngStream::SetPackageSeed(seed);
  RngStream myRngStream;

  TNT::Array1D<double> Z(nRep, 0.0);
  funClass f(pAlpha, beta, patn.dim2());

  TNT::Array1D<double> prob(patn.dim1(), 0.0);

  for(int i = 0;i < nRep; i++)
    Z[i] =  2.*f.qnorm(myRngStream.RandU01());
  prob = f.probFunc(Z, patn); 
  
  for(int i = 0; i < prob.dim(); i++){
    cout <<prob[i] << endl;
  }
  
  auto end = std::chrono::high_resolution_clock::now();
  chrono::duration<double> diff = end-start;
  
  cout << "Serial time was: "<< diff.count() << endl;
      
  delete[] pAlpha;
  return 0;
}

