
#include <iostream>
#include <string>
#include <vector>

#ifndef DATASET_H_
#define DATASET_H_

using namespace std;

class Dataset {
protected:
  // Parameters
  vector<string> Labels;
  vector<vector<float>> Values;

public:
  // Constructor
  Dataset();
  Dataset(string FilePath);
  Dataset(vector<string> L, vector<vector<float>> V);

  // Destructor
  ~Dataset();

  // Getters
  vector<string> get_Labels() const;
  vector<vector<float>> get_Values() const;

  // Methods
  void print() const;
  bool empty() const;

  int Label_length() const;
  int Entries_size() const;
};

#endif