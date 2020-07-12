#include "RngStream/RngStream.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <omp.h>
#define N 100000
#define ITER 100
#define PI 3.1459265358979323846
using namespace std;

double func(double value){
  return sin(value);
}

int main(){

  double upperLimit = PI;
  double lowerLimit = 0.0;
  double answer;
  double randomValues[N];

  unsigned long seed[6] = { 170220009, 800813030,
			                      221120006, 357035871,
			                      953570445, 434738743};
  RngStream::SetPackageSeed(seed);
  RngStream myRngStream;

  omp_set_num_threads(4);

  #pragma omp parallel shared(answer) private(randomValues)
  {
  #pragma omp for
  for(int iteration = 0; iteration < ITER; iteration++){

    double total = 0.0;

    for(int i = 0; i < N; i++){
      randomValues[i] = fmod(myRngStream.RandU01()*100,upperLimit);
    }

    for(int i = 0; i < N; i++){
      total += func(randomValues[i]);
    }

    #pragma omp atomic
    answer += (upperLimit - lowerLimit)/(double)N * total;

  }
  #pragma omp single
    cout << answer/ITER<<endl;
  }


  return 0;
}
