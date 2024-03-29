#ifndef RANDOM_VALUES_H_
#define RANDOM_VALUES_H_

#include "ICriteria.hpp"
#include <string>

/** @class RandomValues
 * @brief Class computing some random values in a given distribution of values.
 *
 * This class implements the ICriteria interface.
 */
class RandomValues : public ICriteria {
private:
  // Parameters
  const std::string name =
      "Random_Values"; /**< A string variable to display the name. */
  int number_Of_Elements =
      10; /**< Integer representing the number of values to compute */

public:
  /**
   * @brief Default constructor for the RandomValues class.
   *
   * Initializes the class with default parameters.
   */
  RandomValues();

  /**
   * @brief Parameterized constructor for the RandomValues class.
   *
   * @param int x: The number of values to compute.
   */
  RandomValues(int x);

  /**
   * @brief Destructor for the RandomValues class.
   */
  ~RandomValues() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Static method to get the name of the RandomValues criteria.
   *
   * @return The name of the RandomValues criteria.
   */
  static std::string get_Name();

  /**
   * @brief Computes random values in given data distribution.
   *
   * This method implements the computation of number_Of_Elements of random
   * values in the provided list of data.
   *
   * @param vector<float> list: The distribution of data
   * @return Type: vector<float>, A vector of the random values
   */
  std::vector<float> compute(const std::vector<float> list) const override;

  /**
   * @brief Computes a random float in given boundaries
   * 
   * @param float min: minimal value we can return
   * @param float max: maximal value we can return
   * 
   * @return Type: float, A random flaot
  */
  float get_Random_Float(float min, float max) const;
};

#endif