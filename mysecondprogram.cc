#include <memory>
#include "fast.h"
#include "random.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

int main(){
  const int size = 4, order = 3, word_dimension = 10;
  const int input_dimension = order * word_dimension;
  const int output_dimension = 20;
  const int max_sequence_length = 9;
  const int max_batch_size = 4;

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

  const int GetOffset = output_dimension * max_batch_size;
  const int GetOffsetSlice = max_batch_size * word_dimension;


  // Fill x
  for (size_t i = 0; i < order * size; i++) {
    const int word_position = i * word_dimension;
    cout << i << endl;
    // Just fill x here using new_word_position
    FastAddConstant(&x[word_position], word_dimension, i + 1, &x[word_position]);
  }

  // Initialize random numbers
  const Real mu = 0.;
  const Real sigma = 0.1;


  random.ComputeGaussianRandomNumbers(output_dimension * word_dimension,
                                      mu,
                                      sigma,
                                      weights_);
  // Display weights_ matrix

  for (size_t i = 0; i < output_dimension; i++) {
    for (size_t j = 0; j < word_dimension; j++) {
      cout << weights_[j + i * word_dimension] << ' ';
    }
    cout << '\n';
  }
  // beginning of new_x
  new_x_t = new_x;

  // Fill new_x to obtain slice 1
  for (size_t j = 0; j < 2; j++) {
    cout << j << ' ' << "Next slice" << endl;
    for (size_t i = 0; i < size; i++) {
      const int word_position = word_dimension * j + order * i * word_dimension;
      const int new_word_position = j * size * word_dimension + i * word_dimension;
      cout << i << endl;
      FastCopy(&x[word_position], word_dimension, &new_x[new_word_position]);
    }
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
    for (size_t k = 0; k < size * 2; k++) {
      for (size_t l = 0; l < output_dimension; l++) {
        cout << b_t_[l + k * output_dimension - j * GetOffset] << ' ';
      }
      cout << '\n';
    }
    new_x_t += GetOffsetSlice;
    b_t_ += GetOffset;
  }






  // TO DO: finish the loop by adding the output after order computation



  return 0;
}
