#include "DataSet.hpp"
#include "BaggingModel.hpp"

#include <stdio.h>

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}

int main() {

  cout << "=== DataSet Loading ===\n";
  double cpu_frequency = 1.4e9;
  uint64_t t0 = rdtsc();
  DataSet DS{"../data/datasets/d1.csv"};
  uint64_t t1 = rdtsc();
  std::cout << "Loading the dataset took                    : " << t1 - t0
            << " CPU cycles\n";
  std::cout << "Loading the dataset took                    : "
            << (t1 - t0) / cpu_frequency << " seconds\n";

  /* uint64_t t_Shared_DS = rdtsc();
  std::shared_ptr<DataSet> DS2 = std::make_shared<DataSet>(DS);
  uint64_t t_Shared_DS_F = rdtsc();
  std::cout << "Making shared ptr of dataset took          : "
            << t_Shared_DS_F - t_Shared_DS << " CPU cycles\n";
  std::cout << "Making shared ptr of dataset took          : "
            << (t_Shared_DS_F - t_Shared_DS) / cpu_frequency << " seconds\n";

  uint64_t t4 = rdtsc();
  TreeNode TN{DS2};
  uint64_t t5 = rdtsc();
  std::cout << "Making a Node of the dataset took          : " << t5 - t4
            << " CPU cycles\n";
  std::cout << "Making a Node of the dataset took          : "
            << (t5 - t4) / cpu_frequency << " seconds\n";

  uint64_t t2 = rdtsc();
  DecisionTree DT{DS};
  uint64_t t3 = rdtsc();
  std::cout << "Buidling one node Tree of the dataset took : " << t3 - t2
            << " CPU cycles\n";
  std::cout << "Buidling one node Tree of the dataset took : "
            << (t3 - t2) / cpu_frequency << " seconds\n";

  // Setting up the operator (fixed atm)
  IOperator *RIV = new ReductionInVar(&DT);
  DT.add_Operator(RIV);

  uint64_t t6 = rdtsc();
  DT.build_Splitted_Tree(5);
  uint64_t t7 = rdtsc();
  std::cout << "Splitting at depth 5 took                  : " << t7 - t6
            << " CPU cycles\n";
  std::cout << "Splitting at depth 5 took                  : "
            << (t7 - t6) / cpu_frequency << " seconds\n"; */

  int n = 10;

  uint64_t t2 = rdtsc();
  BaggingModel model{"USELESS_ATM", n};
  model.fit(DS);
  uint64_t t3 = rdtsc();
  std::cout << "Splitting at depth "<< n <<" took                  : " << t3 - t2
            << " CPU cycles\n";
  std::cout << "Splitting at depth "<< n <<" took                  : "
            << (t3 - t2) / cpu_frequency << " seconds\n";

  uint64_t t4 = rdtsc();
  model.predict(DS);
  uint64_t t5 = rdtsc();
  std::cout << "Predicting at depth "<< n <<" took                 : " << t5 - t4
            << " CPU cycles\n";
  std::cout << "Predicting at depth "<< n <<" took                 : "
            << (t5 - t4) / cpu_frequency << " seconds\n"; 
 
  return 0;
}