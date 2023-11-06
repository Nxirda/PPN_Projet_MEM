#include <fstream>
#include <iostream>
#include <sstream>   // std::stringstream
#include <stdexcept> // std::runtime_error
#include <string>
#include <vector>

#include "../include/data_loading.h"

using namespace std;

// https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/

/* Take the path of the file to read
Instanciates an object of type Dataset from the given CSV */
Dataset::Dataset(string FilePath) {
  // input filestream
  ifstream file;
  file.open(FilePath);

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
      // Initialize Dataset Labels
      this->Labels.push_back(colname);
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
    this->Values.push_back(tmp);
  }

  // Close file
  file.close();
}

/* Explicit Constructor */
/* Use : First Argument : Labels | second : Values*/
Dataset::Dataset(vector<string> L, vector<vector<float>> V) {
  this->Labels = L;
  this->Values = V;
}

/* Default Constructor */
Dataset::Dataset() {}

/* Default Destructor */
Dataset::~Dataset() {}

/* Naive print function of the Dataset */
void Dataset::print() const {
  // Logical but Prints the Labels
  for (int i = 0; i < this->Labels.size(); ++i) {
    cout << Labels[i] << "\t";
  }
  cout << "\n";
  // Logical but Prints the Values
  for (int i = 0; i < int(this->Values.size()); ++i) {
    for (int j = 0; j < this->Values[0].size(); ++j) {
      cout << Values[i][j] << "|\t";
    }
    cout << "\n";
  }
  cout << "\n";
}

/* Returns the Labels of the Dataset */
vector<string> Dataset::get_Labels() const { return this->Labels; }

/* Returns the Values of the Dataset */
vector<vector<float>> Dataset::get_Values() const { return this->Values; }

/* Return True if there are no values in the Dataset */
bool Dataset::empty() const { return this->Values.empty(); }

/* Return the length (= number of Labels) of the Dataset */
int Dataset::Label_length() const { return this->get_Labels().size(); }

/* Return the height (= number of entries)  of the Dataset*/
int Dataset::Entries_size() const { return this->get_Values().size(); }
