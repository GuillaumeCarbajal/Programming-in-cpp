#include <memory>
#include "fast.h"
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

  Real *current_slice;
  current_slice = FastMalloc(word_dimension * size);

  Real *b_t_;
  b_t_ = FastMalloc(output_dimension * size * max_sequence_length);

  Real *weights_;
  weights_ = FastMalloc(output_dimension * input_dimension);

  // Fill x and new_x
  for (size_t i = 0; i < size * order; i++) {
    const int word_position = i * word_dimension;
    const int new_word_position = word_dimension * (size * (i % order) + i / order);
    cout << i << endl;
    FastAddConstant(&x[word_position], word_dimension, i + 1, &x[word_position]);
    FastCopy(&x[word_position], word_dimension, &new_x[new_word_position]);
  }
  // Display x and new_x values
  for (size_t i = 0; i < size * input_dimension; i++) {
    cout << x[i]<< ' ' << typeid(x[i]).name() << ' ' << new_x[i] << ' ' << typeid(new_x[i]).name() << '\n';
  }
  // Fill weights_ with constant 1.0
  
  for (size_t i = 0; i < order; i++) {
    // output of a reccurent cell
    const int slice_position = i * word_dimension * size;
    FastCopy(&new_x[slice_position], word_dimension * size, current_slice);

    // Display current_slice
    for (size_t j = 0; j < word_dimension * size; j++){
      cout << current_slice[j] << '\n';
    }
    /*FastMatrixMatrixMultiply(1.0,
                             weights_,
                             false,
                             output_dimension(),
                             word_dimension, // NB: input_dimension = word_dimension
                             current_slice,
                             false,
                             size,
                             b_t_);*/
    // TO DO: finish the loop by adding the output after order computation
  }


  return 0;
}
