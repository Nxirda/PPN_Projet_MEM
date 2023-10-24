#include <iostream>
#include <string>
#include <vector>

#ifndef DecisionTree_H_
#define DecisionTree_H_

#include "data_loading.h"

struct DecisionTree { // Classe arbre de désion 

	struct TreeNode{ // Classe d'un noeud

 		std::string label;
 		std::string attribute;
 		TreeNode* Right; //pointeur du sous arbre droit
 		TreeNode* Left; //pointeur du sous arbre gauche

	};
 		
 		//construction d'un arbre, fonction build dnas le fichier.cpp
		DecisionTree();
    	TreeNode* buildTree(const Dataset& data, const std::vector<std::string>& labels);
};

#endif