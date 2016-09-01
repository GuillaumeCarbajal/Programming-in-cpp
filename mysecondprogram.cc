#include <memory>
#include "fast.h"
#include "random.h"
#include "recurrency.h"
#include "sigmoid.h"
#include "function.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

int main(){
  const uint32_t seed = 1;
  Random random(seed);

  const int size = 4, order_ = 3, word_dimension_ = 10;
  const int input_dimension = order_ * word_dimension_;
  const int output_dimension = 20;
  const int max_sequence_length = 9;
  const int max_batch_size = 4;

  Real x[size * input_dimension];

  Real *new_x_,
       *new_x_t_,
       *b_, *b_t_,
       *weights_,
       *bias_,
       *delta_,
       *delta_t_,
       *new_delta_t_,
       *recurrent_weights_,
       *momentum_weights_;

  b_ = FastMalloc(output_dimension * max_batch_size * max_sequence_length * order_);
  delta_ = FastMalloc(output_dimension * max_batch_size * max_sequence_length * order_);
  new_delta_t_ = FastMalloc(output_dimension * max_batch_size * order_);
  weights_ = FastMalloc(output_dimension * word_dimension_);
  recurrent_weights_ = FastMalloc(output_dimension * output_dimension);
  bias_ = FastMalloc(output_dimension);
  momentum_weights_ = FastMalloc(output_dimension * word_dimension_);
  /*momentum_bias_ = use_bias ? FastMalloc(output_dimension) : nullptr;*/
  new_x_ = FastMalloc(input_dimension * max_batch_size * max_sequence_length); 

  const int GetOffset = output_dimension * max_batch_size;
  const int GetOffsetSlice = max_batch_size * word_dimension_;

  ActivationFunctionPointer f; 
  f = ActivationFunctionPointer(new Sigmoid());

  // beginning of each array 
  b_t_ = b_;
  new_x_t_ = new_x_;
  delta_t_ = delta_;

  // Initialize random numbers
  const Real mu = 0.;
  const Real sigma = 0.1;


  random.ComputeGaussianRandomNumbers(output_dimension * word_dimension_,
                                      mu,
                                      sigma,
                                      weights_);
  random.ComputeGaussianRandomNumbers(output_dimension * output_dimension,
                                      mu,
                                      sigma,
                                      recurrent_weights_);


  // TO DO: LOOP to make 2 times imput
  for (size_t a = 0; a < 2; a++) {
    cout << "Input" << ' ' << a << endl;
    // Fill x
    for (size_t i = 0; i < order_ * size; i++) {
      const int word_position = i * word_dimension_;
      cout << i << endl;
      // Just fill x here using new_word_position
      FastAddConstant(&x[word_position], word_dimension_, i + 1, &x[word_position]);
    }

    // Display weights_ matrix
    for (size_t i = 0; i < output_dimension; i++) {
      for (size_t j = 0; j < word_dimension_; j++) {
        cout << weights_[j + i * word_dimension_] << ' ';
      }
      cout << '\n';
    }
    cout << '\n' << endl;

    // Display recurrent_weights_ matrix
    for (size_t i = 0; i < output_dimension; i++) {
      for (size_t j = 0; j < output_dimension; j++) {
        cout << recurrent_weights_[j + i * output_dimension] << ' ';
      }
      cout << '\n';
    }
    cout << '\n' << endl;


    ///////////////////////////////////////////////////////


    // Fill new_x_ to obtain slice 1
    for (size_t j = 0; j < order_; j++) {
      cout << "order_" << ' ' << j << endl;
      for (size_t i = 0; i < size; i++) {

        cout << "slice" << ' ' << i << endl;
        const int word_position = word_dimension_ * (j + order_ * i);
        const int new_word_position = word_dimension_ * (j * size + i);
        FastCopy(&x[word_position], word_dimension_, &new_x_[new_word_position]);
      }

      if (bias_) {
        for (size_t i = 0; i < size; ++i)
          FastCopy(bias_, output_dimension, b_t_ + i * output_dimension + j * GetOffset);
      }

      FastMatrixMatrixMultiply(1.0,
                               weights_,
                               false,
                               output_dimension,
                               word_dimension_, // NB: input_dimension = word_dimension_
                               new_x_t_,
                               false,
                               size,
                               b_t_);

      // Display output b_t_
      cout << "Ouput after FFNN" << endl;
      for (size_t k = 0; k < 2 * size * order_; k++) {
        for (size_t l = 0; l < output_dimension; l++) {
          cout << b_t_[l + k * output_dimension - j * GetOffset - a * GetOffset * order_] << ' ';
        }
        cout << '\n';
      }
      cout << '\n' << endl;

      // Recurrency layer
      if (j != 0) {
        FastMatrixMatrixMultiply(1.0,
                                 recurrent_weights_,
                                 false,
                                 output_dimension,
                                 output_dimension,
                                 b_t_ - GetOffset,
                                 false,
                                 size,
                                 b_t_);
      }

      // Display output b_t_
      cout << "Output before activation" << endl;
      for (size_t k = 0; k < 2 * size * order_; k++) {
        for (size_t l = 0; l < output_dimension; l++) {
          cout << b_t_[l + k * output_dimension - j * GetOffset - a * GetOffset * order_] << ' ';
        }
        cout << '\n';
      }

      // Activation of reccurency layer
      f->Evaluate(output_dimension, size, b_t_);
      //// TO DO : declarer le destructeur

      // Display output b_t_
      cout << "Output after activation" << endl;
      for (size_t k = 0; k < 2 * size * order_; k++) {
        for (size_t l = 0; l < output_dimension; l++) {
          cout << b_t_[l + k * output_dimension - j * GetOffset - a * GetOffset * order_] << ' ';
        }
        cout << '\n';
      }

      // let new_x_t_ point to next slice
      new_x_t_ += GetOffsetSlice;
      // let b_t_ point to next time step
      b_t_ += GetOffset;
    }

    // Let b_t_ points to the last result
    b_t_ -= GetOffset;
    const Real *result = b_t_;

    // Let b_t_ points to next time step
    b_t_ += GetOffset;

    // let new_x_ point to next step for input x
    new_x_ += GetOffsetSlice * order_;
  }
  return 0;
}
