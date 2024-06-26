#include <algorithm>

#include "Percentiles.hpp"
#include "TrainingElement.hpp"

/************************/
/*                      */
/*  Percentiles CLASS   */
/*                      */
/************************/

//
Percentiles::Percentiles() noexcept { this->size = this->percentiles.size(); }

//
Percentiles::~Percentiles() {}

//
void Percentiles::print() const {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

//
size_t Percentiles::get_Criteria_Number() const { return this->size; }

//
std::string Percentiles::get_Name() const { return "P"; }

//
std::string Percentiles::get_Name_Static() { return "Percentiles"; }

//
std::vector<double> Percentiles::compute(const std::vector<double> &list,
                                         const std::vector<size_t> &idx) const {

  std::vector<double> percentiles_Values(this->percentiles.size(), 0);

  std::vector<double> sorted_Data;
  sorted_Data.reserve(idx.size());

  for (const auto &i : idx) {
    sorted_Data.push_back(list[i]);
  }

  // Sort the data
  const size_t length = sorted_Data.size();
  std::sort(sorted_Data.begin(), sorted_Data.end());

  for (size_t i = 0; i < this->percentiles.size(); ++i) {
    percentiles_Values[i] = sorted_Data[length * (this->percentiles[i] / 100)];
  }  
  return percentiles_Values;
}
