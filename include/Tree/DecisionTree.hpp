#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "IModel.hpp"
#include "TreeNode.hpp"


// Binary Search Tree Class
class DecisionTree : IModel {
private:
  // Parameters

  int max_Depth;
  std::unique_ptr<TreeNode> root;
  IOperator *splitting_Operator;
  ICriteria *splitting_Criteria;

  // Boost part to serialize the trees so we can send them to other MPIs Process
  // Might need to build some sort of verif for MPI/Boost install at some point
  // so we can use it with or without them
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &root;
  }

public:
  // Constructor

  DecisionTree();
  DecisionTree(int max_Depth, ICriteria *crit, IOperator *op);
  DecisionTree(const DecisionTree &dt);
  DecisionTree &operator=(const DecisionTree &tree);

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode *get_Root() const;

  // Setters
  void set_Root(std::unique_ptr<TreeNode> node);

  // Methods

  void train(const DataSet &data) override;
  std::vector<double> predict(const DataSet &data) const override;

  void print_Tree();
};

#endif