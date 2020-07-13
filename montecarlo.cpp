#include "RngStream/RngStream.h"
#include "RngStream/RngStreamSupp.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "mpi.h"
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
  double start_time, end_time;

  int rank;
  int numThreads;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &numThreads);
  RngStream myRngStream;

  if(rank == 0){
    unsigned long seed[6] = { 170220009, 800813030,
                              221120006, 357035871,
                              953570445, 434738743};

    unsigned long messageSeed[6];

    RngStream::SetPackageSeed(seed);

    for(int i=1; i < numThreads;i++){
      RngStreamSupp::AdvanceSeed(seed, messageSeed);
      MPI_Send(messageSeed, 6, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD);
      for(int j = 0; j < 6; j++)
        seed[j] = messageSeed[j];
    }
  }

  if(rank != 0){
    unsigned long mySeed[6];
    MPI_Recv(mySeed, 6, MPI_UNSIGNED_LONG, 0, 0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    RngStream::SetPackageSeed(mySeed);
  }

  MPI_Barrier(MPI_COMM_WORLD);

  start_time = MPI_Wtime();
  for(int iteration = 0; iteration < ITER; iteration++){

    double localTotal = 0.0;
    double globalTotal = 0.0;

    for(int i = (N/numThreads)*rank; i < (N/numThreads)*(rank+1); i++){
      randomValues[i] = fmod(myRngStream.RandU01()*100,upperLimit);
    }

    for(int i = (N/numThreads)*rank; i < (N/numThreads)*(rank+1); i++){
      localTotal += func(randomValues[i]);
    }

    MPI_Reduce(&localTotal,&globalTotal,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank == 0){;
      answer += (upperLimit - lowerLimit)/(double)N * globalTotal;
    }
  }

  if(rank == 0){
    end_time = MPI_Wtime();
    cout << end_time - start_time<<" segundos\n";
    cout << answer/ITER<<"\n";
  }

  MPI_Finalize();
  return 0;
}
