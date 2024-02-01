#ifndef QUARTILES_H_
#define QUARTILES_H_

#include "ICriteria.hpp"
#include <string>

class Quartiles : public ICriteria {
private:
  // Parameters
  const std::string name = "Quartiles";
  const std::vector<float> quartiles = {25.0, 50.0, 75.0};

public:
  // Constructor
  Quartiles();

  // Destructor
  ~Quartiles() override;

  // Methods
  static std::string get_Name();

  void print() override;

  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif