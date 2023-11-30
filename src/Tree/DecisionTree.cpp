#include <fstream>
#include <iostream>
#include <iterator>
#include <limits.h> //For Max_Int value
#include <string>
#include <vector>

#include "DecisionTree.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* 
Default Constructor                     
Inputs  :                               
Outputs : Object of Decision Tree Class 
*/
DecisionTree::DecisionTree() {}

/* 
Base Constructor with a DataSet parameter 
Shall be used only for the first Node     
Inputs  : Object of DataSet Class         
Outputs : Object of Decision Tree Class   
*/
DecisionTree::DecisionTree(const DataSet &data) {
  std::vector<int> idx(data.samples_Number());
  for (long unsigned int i = 0; i < idx.size(); ++i) {
    idx[i] = i;
  }
  this->parent = nullptr;
  this->curr_Node = std::make_shared<TreeNode>(
      TreeNode{std::make_shared<DataSet>(data), idx});
  this->left = nullptr;
  this->right = nullptr;
}

/* 
Constructor for child nodes                
Inputs  : shared_ptr<DataSet>, vector<int> 
Outputs : Object of Decision Tree Class    
*/
DecisionTree::DecisionTree(std::shared_ptr<TreeNode> tree_Node,
                           std::vector<int> idx) {
  this->parent = nullptr;
  this->curr_Node = std::make_shared<TreeNode>(*tree_Node);
  this->curr_Node.get()->set_Index(idx);
  this->left = nullptr;
  this->right = nullptr;
}

/* 
Default Destructor 
Inputs  :          
Outputs :          
*/
DecisionTree::~DecisionTree(){};

/* 
Returns the Current Node of the Tree      
Inputs  :                                 
Outputs : pointer of Decision Tree Object 
*/
std::shared_ptr<TreeNode> DecisionTree::get_Current_Node() {
  return this->curr_Node;
}

/* 
Returns the Parent Tree                   
Inputs  :                                 
Outputs : pointer of Decision Tree Object 
*/
DecisionTree &DecisionTree::get_Parent_Tree() { return *this->parent; }

/* 
Returns the Left Sub Tree                 
Inputs  :                                 
Outputs : pointer of Decision Tree Object 
*/
DecisionTree *DecisionTree::get_Left_Tree() { return this->left.get(); }

/* 
Returns the Right Sub Tree                
Inputs  :                                 
Outputs : pointer of Decision Tree Object 
*/
DecisionTree *DecisionTree::get_Right_Tree() { return this->right.get(); }

/* 
Sets a new Parent for the given tree      
Inputs  : pointer of Decision Tree Object 
Outputs :                                 
*/
void DecisionTree::add_Parent(DecisionTree *d) { this->parent = d; }

/* 
Sets a new left Subtree          
Inputs : Object of DataSet class 
Output :                         
*/
void DecisionTree::add_Left(std::unique_ptr<DecisionTree> dt) {
  this->left = std::move(dt);
  this->left->add_Parent(this);
  this->left->add_Operator(this->split_Operator);
}

/* 
Sets a new right Subtree         
Inputs : Object of DataSet class 
Output :                         
*/
void DecisionTree::add_Right(std::unique_ptr<DecisionTree> dt) {
  this->right = std::move(dt);
  this->right->add_Parent(this);
  this->right->add_Operator(this->split_Operator);
}

void DecisionTree::add_Operator(IOperator *wanted_Operator) {
  this->split_Operator = wanted_Operator;
}

/* 
Print function for Decision Trees 
Inputs  :                         
Outputs :                         
*/
void DecisionTree::print_Tree() {
  this->curr_Node->node_Print();
  if (this->left) {
    this->get_Left_Tree()->print_Tree();
  }
  if (this->right) {
    this->get_Right_Tree()->print_Tree();
  }
}

/* 
Builds a Decision Tree recursively following a splitting criteria 
Inputs  :                                                         
Outputs :                                                         
*/
void DecisionTree::build_Splitted_Tree(int depth) {
  if (depth > 0) {
    this->split_Operator->set_Tree(this);
    int split_Feature = this->split_Operator->find_Best_Split_Feature();
    
    float split_Criteria = this->split_Operator->get_Best_Split_Criteria();

    // Computing the Indexes of the Dataset for each childs
    std::vector<std::vector<int>> child_Indexes =
        this->curr_Node->node_Split(split_Feature, split_Criteria);

    // Creating a left child unique ptr
    auto left_Child =
        std::make_unique<DecisionTree>(curr_Node, child_Indexes[0]);
    this->add_Left(std::move(left_Child));

    // Creating a right child unique ptr
    auto right_Child =
        std::make_unique<DecisionTree>(curr_Node, child_Indexes[1]);
    this->add_Right(std::move(right_Child));

    // Recursive part
    this->left->build_Splitted_Tree(depth - 1);
    this->right->build_Splitted_Tree(depth - 1);
  }
}
