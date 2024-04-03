#ifndef MPI_WRAPPER_H_
#define MPI_WRAPPER_H_

#include <stdio.h>
#include <string>

#include "boost/mpi.hpp"
#include <omp.h>

#include "BaggingModel.hpp"

namespace MPI_Wrapper {

//
void helper_Print();

//
uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                  int process_Rank);

//
int MPI_Main(int argc, char **argv);

//
BaggingModel MPI_Model_Init(std::string &metric, const std::string &criteria,
                            uint16_t depth, uint16_t number_Of_Trees);

//
void MPI_Train(BaggingModel &model, const DataSet &data);

//
void MPI_Cross_Val(const BaggingModel &model, const DataSet &data, int K);

} // namespace MPI_Wrapper

#endif