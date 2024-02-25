#include <cmath>
#include <omp.h>

#include "MAE.hpp"
#include "TrainingElement.hpp"

/********************/
/*                  */
/*    MAE CLASS     */
/*                  */
/********************/

//
MAE::MAE() {}

//
MAE::~MAE() {}

//
void MAE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

//
std::string MAE::get_Name() { return "Mean Absolute Value"; }

//
double MAE::compute(size_t position, const DataSet &data,
                    const std::vector<size_t> &index,
                    const double split_Criteria) const {


  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  size_t base_Population = index.size();

  double left_MAE = 0.0;
  TreeNode left_Child{};
  double left_Prediction = data.labels_Mean(left_index.value());
  size_t left_Population = left_index.value().size();

  double right_MAE = 0.0;
  TreeNode right_Child{};
  double right_Prediction = data.labels_Mean(right_index.value());
  size_t right_Population = right_index.value().size();
  
  for (size_t idx : left_index.value()) {
    left_MAE += std::abs(data.get_Labels()[idx] - left_Prediction);
  }
  left_MAE *= (1.0 / left_Population);

  for (size_t idx : right_index.value()) {
    right_MAE += std::abs(data.get_Labels()[idx] - right_Prediction);
  }
  right_MAE *= (1.0 / right_Population);

  // Compute the result of MAE for the split at position
  double res = ((left_MAE * left_Population) + (right_MAE * right_Population)) *
               (1.0 / base_Population);

  return res;
}

//
double MAE::apply(const std::vector<double> &exact,
                  const std::vector<double> &prediction) {

  double res = 0.0;
  size_t size = prediction.size();

  for (size_t i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]);
  }

  // Compute the MAE
  res *= (1.0 / size);

  return res;
}
