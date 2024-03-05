#include "TrainingElement.hpp"

#include <algorithm>
#include <functional>
#include <omp.h>
#include <random>
#include <stack>
#include <thread>
#include <tuple>

/**************************/
/*                        */
/* TRAINING ELEMENT CLASS */
/*                        */
/**************************/

//
TrainingElement::TrainingElement() {
  this->depth = 0;
  this->node = nullptr;
  this->index = std::vector<size_t>();
}

//
TrainingElement::TrainingElement(TreeNode *node,
                                 const std::vector<size_t> &index,
                                 uint16_t depth) {
  this->node = node;
  this->depth = depth;
  this->index = std::move(index);
}

//
TrainingElement::TrainingElement(const TrainingElement &TE) {
  this->depth = TE.depth;
  this->index = TE.index;
  this->node = TE.node;
}

//
TrainingElement &TrainingElement::operator=(const TrainingElement &TE) {
  this->depth = TE.depth;
  this->index = TE.index;
  this->node = TE.node;
  return *this;
}

//
TrainingElement::~TrainingElement(){};

//
const std::vector<size_t> &TrainingElement::get_Index() { return this->index; }

//
void TrainingElement::set_Node(TreeNode *node) { this->node = node; }

//
void TrainingElement::set_Index(const std::vector<size_t> &index) {
  this->index = index;
}

//
void TrainingElement::set_depth(uint16_t depth) { this->depth = depth; }

//
void TrainingElement::set_Root(size_t dataset_Size, TreeNode *node) {
  this->depth = 0;

  bootstrap_Index(dataset_Size);
  this->node = node;
}

//
void TrainingElement::bootstrap_Index(size_t dataset_Size) {

  // Generate a unique seed using hardware entropy
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, dataset_Size - 1);
  std::vector<size_t> idx(dataset_Size);

  for (size_t i = 0; i < dataset_Size; ++i) {
    idx[i] = dist(gen);
  }
  this->index = std::move(idx);
}

//
//double compute_Split_Value(const std::vector<size_t> &index,
//                          const DataSet &data, size_t feature, double criteria
//                           /*IOperator *op*/) {

 /* // const std::vector<double> &labels;
  auto [left_index, right_index] =
      std::move(data.split(feature, criteria, index));

  size_t base_Population = index.size();

  double left_MAE = 0.0;
  std::vector<double> labels_Left =
      std::move(data.get_Labels(left_index.value()));
  auto left_Prediction =
      std::min_element(labels_Left.begin(), labels_Left.end());

  size_t left_Population = left_index.value().size();

  double right_MAE = 0.0;
  std::vector<double> labels_Right =
      std::move(data.get_Labels(right_index.value()));
  auto right_Prediction =
      std::min_element(labels_Right.begin(), labels_Right.end());

  size_t right_Population = right_index.value().size();

  for (auto const &label_Value : labels_Left) {
    left_MAE += std::abs(label_Value - *left_Prediction);
  }
  left_MAE *= (1.0 / left_Population);

  for (auto const &label_Value : labels_Right) {
    right_MAE += std::abs(label_Value - *right_Prediction);
  }
  right_MAE *= (1.0 / right_Population);

  // Compute the result of MAE for the split at position
  double res = ((left_MAE * left_Population) + (right_MAE * right_Population)) *
               (1.0 / base_Population);

  return res;
} */

//
std::tuple<size_t, double> TrainingElement::find_Best_Split(
    const DataSet &data, [[maybe_unused]] const IOperator *splitting_Operator,
    const ICriteria *splitting_Criteria) {

  size_t best_Feature = 0;
  double best_Criterion = 0.0;
  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;

  size_t feature, j;
  double tmp_var;
  // Minimize the error by trying multiple splits on features
  for (feature = 0; feature < data.get_Features().size(); ++feature) {

    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(feature), this->index);

    // Test multiple split criteria
    for (j = 0; j < criterias.size(); ++j) {

      tmp_var =
          splitting_Operator->compute(feature, data, this->index, criterias[j]);
       
      //tmp_var = compute_Split_Value(this->index, data, feature, criterias[j]);

      if (tmp_var < min) {
        min = tmp_var;
        best_Feature = feature;
        best_Criterion = criterias[j];
      }
    }
  }
  return {best_Feature, best_Criterion};
}

//
std::tuple<std::optional<std::vector<size_t>>,
           std::optional<std::vector<size_t>>>
TrainingElement::split_Index(const DataSet &data, double criterion,
                             size_t position) {

  return data.split(position, criterion, this->get_Index());
}

//
std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
TrainingElement::split_Node(const DataSet &data,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria) {

  // Left node
  TreeNode left{};
  std::optional<TrainingElement> train_Left = std::nullopt;

  // Right node
  TreeNode right{};
  std::optional<TrainingElement> train_Right = std::nullopt;

  // Compute split attributes
  auto [column, criterion] = this->find_Best_Split_Parallel_2(
      data, splitting_Operator, splitting_Criteria);
  // this->find_Best_Split_Parallel(data, splitting_Operator,
  // splitting_Criteria);
  //  this->find_Best_Split(data, splitting_Operator, splitting_Criteria);

  // Compute new indexes
  auto [left_index, right_index] = split_Index(data, criterion, column);

  if (!left_index && !right_index) {
    return {train_Left, train_Right};
  }

  uint16_t next_Depth = this->depth + 1;

  // Set the datas for the current node
  this->node->set_Split_Column(column);
  this->node->set_Split_Criterion(criterion);

  // Case 1 : Build Right Node (if information gained)
  if (right_index.has_value()) {
    double predic_Right = data.labels_Mean(*right_index);
    this->node->add_Right(std::make_unique<TreeNode>(std::move(right)));
    this->node->get_Right_Node()->set_Predicted_Value(predic_Right);
    train_Right = std::move(TrainingElement(
        this->node->get_Right_Node(), std::move(*right_index), next_Depth));
  }

  // Case 2 : Build Left Node (if information gained)
  if (left_index.has_value()) {
    double predic_Left = data.labels_Mean(*left_index);
    this->node->add_Left(std::make_unique<TreeNode>(std::move(left)));
    this->node->get_Left_Node()->set_Predicted_Value(predic_Left);
    train_Left = std::move(TrainingElement(this->node->get_Left_Node(),
                                           std::move(*left_index), next_Depth));
  }

  return {train_Left, train_Right};
}

//
void TrainingElement::train(const DataSet &data, TreeNode *Node,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria,
                            uint16_t max_Depth, size_t threshold) {

  // Initialize the stack of Node that will be splitted
  std::stack<TrainingElement> remaining;

  TrainingElement base_Node{};

  // Initialize the current Node
  base_Node.set_Root(data.labels_Number(), Node);
  base_Node.node->set_Predicted_Value(data.labels_Mean(base_Node.index));

  remaining.push(base_Node);

  // Build iteratively the tree frame
  while (not remaining.empty()) {
    auto elem = remaining.top();
    remaining.pop();

    if (elem.depth >= max_Depth) {
      continue;
    }

    auto [left, right] =
        elem.split_Node(data, splitting_Operator, splitting_Criteria);

    if (left) {
      // Verify we gained information
      if (left.value().index.size() != elem.index.size() &&
          left.value().index.size() > threshold) {

        remaining.push(std::move(*left));
      }
    }

    if (right) {
      // Verify we gained information
      if (right.value().index.size() != elem.index.size() &&
          right.value().index.size() > threshold) {

        remaining.push(std::move(*right));
      }
    }
  }
}
/******************************* HAND WRITTEN THREADS **********************/
// Outter loop parallelism
auto thread_Task = [](const IOperator *splitting_Operator,
                      const ICriteria *splitting_Criteria,
                      const std::vector<size_t> &index, const DataSet &data,
                      size_t start, size_t end, size_t tid,
                      std::vector<std::tuple<size_t, double, double>> &res) {
  // We try to minimize the result for a split
  double local_min = INT_MAX;
  size_t best_Feature = 0;
  double best_Criterion = 0.0;

  size_t i, j;
  double tmp_var;

  for (i = start; i < end; ++i) {
    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(i), index);

    // Test multiple split criteria
    for (j = 0; j < splitting_Criteria->get_Criteria_Number(); ++j) {

      tmp_var = splitting_Operator->compute(i, data, index, criterias[j]);

      if (tmp_var < local_min) {
        local_min = tmp_var;
        best_Feature = i;
        best_Criterion = criterias[j];
      }
    }
  }
  res[tid] = {best_Feature, best_Criterion, local_min};
};

//
std::tuple<size_t, double>
TrainingElement::find_Best_Split_Parallel(const DataSet &data,
                                          const IOperator *splitting_Operator,
                                          const ICriteria *splitting_Criteria) {

  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;
  double criterion = 0.0;
  size_t best_Feature = 0;
  size_t size = data.get_Features().size();

  uint16_t num_Threads =
      std::min<uint16_t>(size, (size_t)std::thread::hardware_concurrency());

  // Thread pool
  std::vector<std::thread> threads(num_Threads);
  std::vector<std::tuple<size_t, double, double>> res(num_Threads);

  // For the threads
  size_t end;
  size_t start;
  size_t chunk_Size = size / num_Threads;

  uint16_t tid;
  // Thread creation && task set
  for (tid = 0; tid < num_Threads; ++tid) {

    // Chunks computing
    start = tid * chunk_Size;
    end = std::min(start + chunk_Size, size);

    // Maybe ensure thread creation here
    threads[tid] = std::thread(thread_Task, splitting_Operator,
                               splitting_Criteria, std::cref(this->index),
                               std::cref(data), start, end, tid, std::ref(res));
  }

  // Thread join && result aggregation
  for (tid = 0; tid < num_Threads; ++tid) {
    threads[tid].join();

    auto &result = res[tid];
    if (std::get<2>(result) < min) {
      best_Feature = std::get<0>(result);
      criterion = std::get<1>(result);
      min = std::get<2>(result);
    }
  }

  return {best_Feature, criterion};
}

/******************************* OPEN MP THREADS*******************************/

std::tuple<size_t, double> TrainingElement::find_Best_Split_Parallel_2(
    const DataSet &data, const IOperator *splitting_Operator,
    const ICriteria *splitting_Criteria) {

  // std::vector<std::tuple<double, double, size_t>> candidates;
  std::vector<std::tuple<double, double, size_t>> candidates;

  std::vector<std::vector<double>> splitting_Thresholds;
  splitting_Thresholds.resize(data.features_Number());

  size_t column;
#pragma omp parallel
  {
#pragma omp for
    for (column = 0; column < data.features_Number(); ++column) {
      splitting_Thresholds[column] =
          splitting_Criteria->compute(data.get_Column(column), index);
    }
#pragma omp single
    {
      candidates.resize(omp_get_num_threads(),
                        {std::numeric_limits<double>::max(), 0.0, 0});
    }
    int thread_Id = omp_get_thread_num();
#pragma omp for
    for (column = 0; column < data.features_Number(); ++column) {
      for (const auto spliting_Threshold : splitting_Thresholds[column]) {
        double split_Score = splitting_Operator->compute(column, data, index,
                                                         spliting_Threshold);

        if (split_Score < std::get<0>(candidates[thread_Id])) {
          candidates[thread_Id] = {split_Score, spliting_Threshold, column};
        }
      }
    }
  }

  std::tuple<double, double, size_t> best_Split = {-1.0, -1.0, 0};
  for (const auto &candidate : candidates) {
    const auto &best = std::get<0>(best_Split);
    if (best > std::get<0>(candidate) || best == -1.0) {
      best_Split = candidate;
    }
  }

  return {std::get<2>(best_Split), std::get<1>(best_Split)};
}