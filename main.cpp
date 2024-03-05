#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"
#include "MPIWrapper.hpp"

#include "boost/mpi.hpp"

#include <cstdio>
#include <stdio.h>

/*********************/
/*                   */
/*      MAIN         */
/*                   */
/*********************/

//
int main(int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (((argc != 8 && argc != 6) || std::atoi(argv[5]) < size) && rank == 0) {
    std::cout << "Usage is : mpiexec -np [Process] " << argv[0]
              << " [Path to DataSet] [Split Metric] [Split Criteria] [Depth] "
                 "[Number of Trees] [Cross_Val] [Folds]\n";

    std::cout << "\n- Where process should be <= Number of Trees\n";

    std::cout << "\n== Split Metrics are ==\n";
    operators::print();

    std::cout << "\n== Split Criterias are ==\n";
    criterias::print();

    std::cout << "\nCross Val shall be 'CV' followed by the number of folds for activation\n";

    if(size > 1)
      MPI_Abort(MPI_COMM_WORLD, 1);

    return 1;
  }

  MPI_Wrapper::MPI_Main(argc, argv);

  MPI_Finalize();

  return 0;
}