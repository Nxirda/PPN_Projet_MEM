#include "MPIWrapper.hpp"
#include "Answers.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"
#include "Serializer.hpp"
#include "Timer.hpp"

namespace MPI_Wrapper {

//
uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                  int process_Rank) {
  uint16_t res = total_Elements / num_Processes;
  uint16_t remainder = total_Elements % num_Processes;

  return res + (process_Rank < remainder ? 1 : 0);
}

// Need to do a reduction in an other way because if to much processes
// then the comm will become a bottleneck
void MPI_Cross_Val(const BaggingModel &model, const DataSet &data, int K) {

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  BaggingModel model_To_Validate{};
  model_To_Validate = model;  
  /* (model.get_Operator()->get_Name(),
    model.get_Criteria()->get_Name(),
    model.get_Depth(), model.get_Trees_Number()); */


  Answers cross_Val_Res = CrossValidation::K_Folds(model_To_Validate, data, K);

  //  Send && receive values from MPI Process to print in proc 0
  //  Only print once it has ended ppbly
  //  Serialize the values to display

  if (rank == 0) {
    int counter = 0;

    while (counter < size - 1) {
      int flag = 0;
      // Checks for incoming message
      MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);

      if (flag) {
        int recv_Size;
        MPI_Status status;

        // Get infos
        MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &recv_Size);

        std::vector<char> recv_Buffer(recv_Size);

        // Get message and handle infos
        MPI_Recv(recv_Buffer.data(), recv_Size, MPI_CHAR, status.MPI_SOURCE, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::string recv_String(recv_Buffer.begin(), recv_Buffer.end());

        Answers recv_Answer = Serializer::deserialize_Answers(recv_String);
        cross_Val_Res.add_And_Mean_Values(recv_Answer);

        ++counter;
      }
    }
    cross_Val_Res.print();
  } else {
    std::string local_Answer = Serializer::serialize_Answers(cross_Val_Res);

    MPI_Send(local_Answer.c_str(), local_Answer.size(), MPI_CHAR, 0, 0,
             MPI_COMM_WORLD);
  }
}

//
void MPI_Train(BaggingModel &model, const DataSet &data) {
  int rank, size;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  uint16_t trees_For_Process = balancer(model.get_Trees_Number(), size, rank);

  model.set_Trees_Number(trees_For_Process);
  model.train(data);
}

} // namespace MPI_Wrapper
