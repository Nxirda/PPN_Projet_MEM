
#include <iostream>
#include <string>
#include <vector>

#ifndef DATASET_H_
#define DATASET_H_

class DataSet {
protected:
  // Parameters
  
  std::vector<std::string> features;
  std::vector<std::vector<float>> samples;

public:
  // Constructor

  DataSet();
  DataSet(std::string file_Path);
  DataSet(std::vector<std::string> features, std::vector<std::vector<float>> values);

  // Destructor

  ~DataSet();

  // Getters

  std::vector<std::string> get_Features() const;
  std::vector<float> get_Column(int position, const std::vector<int> &idx) const;
  std::vector<std::vector<float>> get_Samples() const;
 

  // Methods
  
  void print() const;
  bool empty() const;
  void print_With_Index(std::vector<int> idx) const;
  
  int features_Length() const;
  int samples_Number() const;

  float column_Mean(int position, const std::vector<int> &idx) const;
  float column_Variance(const std::vector<int> &idx) const;
  
  std::vector<std::vector<int>> split(int position, float criteria, const std::vector<int> &idx) const;
};

#endif