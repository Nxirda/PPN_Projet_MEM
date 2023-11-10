#include <fstream>
#include <iostream>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

#include "../include/data_loading.h"

using namespace std;

// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/

/* Take the path of the file to read                         */
/* Instanciates an object of type DataSet from the given CSV */
/* Inputs : String                                           */
/* Ouputs : Object of DataSet Cass                           */
DataSet::DataSet(string file_Path) {
  // input filestream
  ifstream file;
  file.open(file_Path);

  // Make sure the file is open
  if (!file.is_open()) {
    throw runtime_error("Could not open given file");
  }

  string line;
  string colname;

  // If no error state flag
  if (file.good()) {
    // Extract the first row in the file
    getline(file, line);

    // Create a stringstream from line
    stringstream ss(line);

    // Extract each column name
    while (getline(ss, colname, ',')) {
      // Initialize DataSet Labels
      this->features.push_back(colname);
    }
  }

  float val;
  // Read the other rows
  while (getline(file, line)) {
    // string stream of current line
    stringstream ss(line);

    // tmp vector : represent a row in the matrix
    vector<float> tmp;

    while (ss >> val) {
      tmp.push_back(val);

      if (ss.peek() == ',') {
        ss.ignore();
      }
    }
    // Place the row in the 2D Matrix
    this->samples.push_back(tmp);
  }

  // Close file
  file.close();
}

/* Explicit Constructor, takes two vectors and builds a DataSet Object */
/* Inputs : vector<string>, vector<vector<float>>                      */
/* Ouputs : Object of DataSet Class                                    */
DataSet::DataSet(vector<string> features, vector<vector<float>> samples) {
  this->features = features;
  this->samples = samples;
}

/* Default Constructor              */
/* Inputs :                         */
/* Ouputs : Object of DataSet Class */
DataSet::DataSet() {}

/* Default Destructor */
/* Inputs :           */
/* Ouputs :           */
DataSet::~DataSet() {}

/* Naive print function of the DataSet */
/* Inputs :                            */
/* Ouputs :                            */
void DataSet::print() const {
  // Logical but prints the features
  for (int i = 0; i < this->features.size(); ++i) {
    cout << features[i] << "\t";
  }
  cout << "\n";
  // Logical but Prints the samples
  for (int i = 0; i < int(this->samples.size()); ++i) {
    for (int j = 0; j < this->samples[0].size(); ++j) {
      cout << samples[i][j] << "|\t";
    }
    cout << "\n";
  }
  cout << "\n";
}

/* Returns the features of the DataSet */
/* Inputs :                            */
/* Ouputs : vector<string>             */
vector<string> DataSet::get_Features() const { return this->features; }

/* Returns the Values of the DataSet */
/* Inputs :                          */
/* Ouputs : vector<vector<float>>    */
vector<vector<float>> DataSet::get_Samples() const { return this->samples; }

/* Return True if there are no values in the DataSet */
/* Inputs :                                          */
/* Ouputs : boolean                                  */
bool DataSet::empty() const { return this->get_Samples().empty(); }

/* Return the length (= number of features) of the DataSet */
/* Inputs :                                                */
/* Ouputs : int                                            */
int DataSet::features_Length() const { return this->get_Features().size(); }

/* Return the height (= number of samples) of the DataSet*/
/* Inputs :                                              */
/* Ouputs : int                                          */
int DataSet::samples_Number() const { return this->get_Samples().size(); }

/* Returns the specified column of the dataset */
/* Inputs : int                                */
/* Ouputs : vector<float>                      */
vector<float> DataSet::get_Column(int position) const {
  vector<float> Col;
  if (position > this->features_Length()) {
    return Col;
  }
  for (int j = 0; j < this->samples_Number(); ++j) {
    Col.push_back(this->get_Samples()[j][position]);
  }
  return Col;
}

/* Return 2 DataSets representing the splitting of the given DataSet on the */
/* given column feature position via a given limit (criteria)               */
/* Inputs : int, float                                                      */
/* Ouputs : vector<DataSet>                                                 */
vector<DataSet> DataSet::split(int position, float criteria) const {

  vector<vector<float>> sub_Values_Right;
  vector<vector<float>> sub_Values_Left;
  for (int row = 0; row < this->samples_Number(); ++row) {
    if (this->get_Samples()[row][position] < criteria) {
      sub_Values_Left.push_back(this->get_Samples()[row]);
    } else {
      sub_Values_Right.push_back(this->get_Samples()[row]);
    }
  }
  DataSet d_Right{this->get_Features(), sub_Values_Right};
  DataSet d_Left{this->get_Features(), sub_Values_Left};
  vector<DataSet> res{d_Left, d_Right};
  return res;
}

/* Computes the Mean of a given Column of the DataSet */
/* Inputs  : int                                      */
/* Outputs : float                                    */
float DataSet::column_Mean(int position) const {
  vector<float> current_Column = this->get_Column(position);
  int len = current_Column.size();
  //  check if there are values in the current Column
  if (len <= 0) {
    return 0.0;
  }
  float mean = 0.0;
  for (float elem : current_Column) {
    mean += elem;
  }
  mean /= len;
  return mean;
}

/* Computes the Variance of a given Column of the  DataSet */
/* Inputs  : int                                           */
/* Outputs : float                                         */
float DataSet::column_Variance(int position) const {
  vector<float> current_Column = this->get_Column(position);
  int len = current_Column.size();
  //  check if there are values in the current Column
  if (len <= 0) {
    return 0.0;
  }
  float mean = column_Mean(position);
  float variance = 0.0;
  for (float elem : current_Column) {
    float difference = elem - mean;
    variance += difference * difference;
  }
  variance /= len;
  return variance;
}

/* Computes the Global Variance of the  DataSet */
/* Inputs  :                                    */
/* Outputs : float                              */
float DataSet::global_Variance() const {
  int len = this->features_Length();
  float var_res = 0;
  for (int i = 0; i < len; ++i) {
    var_res += this->column_Variance(i);
  }
  return var_res;
}