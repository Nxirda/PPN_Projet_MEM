#include <cmath>
#include <omp.h>

#include "MAE.hpp"
#include "TrainingElement.hpp"

/********************/
/*                  */
/*    MAE CLASS     */
/*                  */
/********************/

/*
Constructor
Parameters :
Inputs     :
Outputs    :
*/
MAE::MAE() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
MAE::~MAE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void MAE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Return the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string MAE::get_Name() { return "Mean Absolute Value"; }

/*
Computes the Mean Absolute Error of a split on a given column
Index is used to get the column of the dataset that can be accessed
Parameters : position, DataSet, index
Inputs     : int, DataSet, vector<int>
Outputs    : float
*/
float MAE::compute(int position, const DataSet &data, std::vector<int> index,
                   const float split_Criteria) const {

  float left_MAE = 0;
  float right_MAE = 0;

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  float base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  std::vector<float> labels = data.get_Labels();

  // Computes the Mean Absolute Error for left child
  float left_Prediction = data.labels_Mean(left_index.value());
  float left_Population = left_index.value().size();

#pragma omp parallel for reduction(+ : left_MAE)
  for (int idx : left_index.value()) {
    left_MAE += std::abs(labels[idx] - left_Prediction);
  }
  left_MAE /= left_Population;

  // Computes the Mean Absolute Error for right child
  float right_Prediction = data.labels_Mean(right_index.value());
  float right_Population = right_index.value().size();

#pragma omp parallel for reduction(+ : right_MAE)
  for (int idx : right_index.value()) {
    right_MAE += std::abs(labels[idx] - right_Prediction);
  }
  right_MAE /= right_Population;

  // Compute the result of MAE for the split at position
  float res = ((left_MAE * left_Population) + (right_MAE * right_Population)) /
              base_Population;

  return res;
}

/*
Computes the MAE of two vectors
Parameters : exact results, prediction results
Inputs     : const vector<float>, const vector<float>
Outputs    : double
*/
float MAE::apply(const std::vector<float> &exact,
                 const std::vector<float> &prediction) {

  float res = 0;
  int size = prediction.size();
#pragma omp parallel for reduction(+ : res)
  for (int i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]);
  }

  // Compute the MAE
  res = res / size;

  return res;
}
