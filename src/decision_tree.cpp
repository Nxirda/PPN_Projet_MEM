#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "../include/decision_tree.h"

/********************/
/*                  */
/*    NODE PART     */
/*                  */
/********************/

/* Default Constructor */
TreeNode::TreeNode() {}

/* Default Destructor */
TreeNode::~TreeNode() {}

/* Constructor with Dataset :            */
/* Build a Node with Dataset infos in it */
TreeNode::TreeNode(const Dataset &d) { this->data = d; }

/* Override "=" operator */
TreeNode &TreeNode::operator=(TreeNode const &TN) {
  data = get_Dataset();
  return *this;
}

/* Returns the Node's Dataset */
Dataset TreeNode::get_Dataset() { return this->data; }

/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* Base Constructor with a Dataset parameter :          */
/* Builds a Tree with One node containing all the Datas */
DecisionTree::DecisionTree(const Dataset &data) {
  this->Parent = nullptr;
  this->Curr_Node = std::move(new TreeNode{data});
  this->Left = nullptr;
  this->Right = nullptr;
}

/* Override "=" operator */
DecisionTree &DecisionTree::operator=(const DecisionTree &DT) {
  Parent = &get_ParentTree();
  Curr_Node = &get_Current_Node();
  Left = &get_LeftTree();
  Right = &get_RightTree();

  return *this;
}

/* Default Destructor */
DecisionTree::~DecisionTree() {
  delete this->Curr_Node;
  delete this->Left;
  delete this->Right;
}

/* Returns the Current Node of the Tree */
TreeNode &DecisionTree::get_Current_Node() { return *this->Curr_Node; }

/* Returns the Parent Tree */
DecisionTree &DecisionTree::get_ParentTree() { return *this->Parent; }

/* Returns the Left Sub Tree*/
DecisionTree &DecisionTree::get_LeftTree() { return *this->Left; }

/* Returns the Right Sub Tree */
DecisionTree &DecisionTree::get_RightTree() { return *this->Right; }

/* Sets a new Parent for the given tree*/
void DecisionTree::add_Parent(DecisionTree *d) { this->Parent = std::move(d); }

/* Sets a new left Subtree */
void DecisionTree::add_left(Dataset data) {
  this->Left = std::move(new DecisionTree{data});
  this->Left->add_Parent(this);
}

/* Sets a new right Subtree */
void DecisionTree::add_right(Dataset data) {
  this->Right = std::move(new DecisionTree{data});
  this->Right->add_Parent(this);
}

/* Print function for Decision Trees */
void DecisionTree::print_Tree() {
  this->get_Current_Node().get_Dataset().print();
  if (&this->get_LeftTree() != nullptr) {
    this->get_LeftTree().print_Tree();
  }
  if (&this->get_RightTree() != nullptr) {
    this->get_RightTree().print_Tree();
  }
}

/**********************/
/*                    */
/*    CLOBBERS        */
/*                    */
/**********************/

// Fonction qui construit un arbre
/*
DecisionTree *buildTree(const Dataset& data)
{

    if(data.empty())
    { // Si le dernier noeud est nulle

        return nullptr;
    }

    //std::string splitAttribute = Algo_Splitting(data);

    TreeNode* node = new TreeNode{}; // Création d'un noeud
    //node->attribute = splitAttribute;

    // Division des données en deux sous-ensembles en fonction de l'attribut de
division
    //std::vector<Dataset> subsets = Data_Splitting(data, splitAttribute);

    // Division des labels en deux sous-ensembles correspondants
    //std::vector<std::vector<std::string>> subsetsLabels =
Labels_Splitting(labels, splitAttribute);

    std::vector<Dataset> split = Data_Splitting_in_two(data);

    // Construction des sous-arbres récursivement
    node->Left = buildTree(split[0]);
    node->Right = buildTree(split[1]);

    return node;

}*/

// Fonction de l'ago de splitting
/*
std::string Algo_Splitting(const Dataset& data)
{
    return "";
}*/

/*
// Fonction qui divise les labels
std::vector<std::vector<std::string>> Labels_Splitting(const
std::vector<std::string>& labels, const std::string& attribute)
{

}*/
