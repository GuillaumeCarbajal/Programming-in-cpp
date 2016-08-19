#include <memory>
#include "fast.h"
#include "random.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;
blablabla

int main(){
  const int size = 4, order = 3, word_dimension = 10;
  const int input_dimension = order * word_dimension;
  const int output_dimension = 20;
  const int max_sequence_length = 9;

  Real x[size * input_dimension];

  Real *new_x;
  new_x = FastMalloc(size * input_dimension);

  Real *new_x_t;

  Real *current_slice;
  current_slice = FastMalloc(word_dimension * size);

  Real *b_t_;
  b_t_ = FastMalloc(output_dimension * size * max_sequence_length);

  Real *weights_;
  weights_ = FastMalloc(output_dimension * word_dimension);

  const uint32_t seed = 1;
  Random random(seed);


  // Fill x and new_x
  for (size_t i = 0; i < order; i++) {
    const int word_position = word_dimension * (size * (i % order) + i / order);
    const int new_word_position = i * word_dimension;
    cout << i << endl;
    // Just fill x here using new_word_position
    FastAddConstant(&x[new_word_position], word_dimension, i + 1, &x[new_word_position]);
    FastCopy(&x[word_position], word_dimension, &new_x[new_word_position]);
  }

  new_x_t = new_x;

  // Display x and new_x values
  for (size_t i = 0; i < size * input_dimension; i++) {
    cout << x[i]<< ' ' << typeid(x[i]).name() << ' ' << new_x[i] << ' ' << typeid(new_x[i]).name() << '\n';
  }

  // Initialize random numbers
  const Real mu = 0.;
  const Real sigma = 0.1;


  random.ComputeGaussianRandomNumbers(output_dimension * word_dimension,
                                      mu,
                                      sigma,
                                      weights_);
  // Display weights_ matrix
  /*
     for (size_t i = 0; i < output_dimension; i++) {
     for (size_t j = 0; j < word_dimension; j++) {
     cout << weights_[j + i * word_dimension] << ' ';
     }
     cout << '\n';
     }*/

  FastMatrixMatrixMultiply(1.0,
                           weights_,
                           false,
                           output_dimension,
                           word_dimension, // NB: input_dimension = word_dimension
                           new_x_t,
                           false,
                           size,
                           b_t_);
  // Display output b_t_
  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < output_dimension; j++) {
      cout << b_t_[j + i * output_dimension] << ' ';
    }
    cout << '\n';
  }


  // TO DO: finish the loop by adding the output after order computation



  return 0;
}
