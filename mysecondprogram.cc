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
  const Real learning_rate = 0.01;

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
       *momentum_weights_,
       *momentum_recurrent_weights_,
       *momentum_bias_;

  b_ = FastMalloc(output_dimension * max_batch_size * max_sequence_length * order_);
  delta_ = FastMalloc(output_dimension * max_batch_size * max_sequence_length * order_);
  new_delta_t_ = FastMalloc(output_dimension * max_batch_size * order_);
  weights_ = FastMalloc(output_dimension * word_dimension_);
  recurrent_weights_ = FastMalloc(output_dimension * output_dimension);
  bias_ = FastMalloc(output_dimension);
  momentum_weights_ = FastMalloc(output_dimension * word_dimension_);
  momentum_recurrent_weights_ = FastMalloc(output_dimension * output_dimension);
  momentum_bias_ = FastMalloc(output_dimension);
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

  // convert char to int
  const char a = '7';
  int b = a - '0';
  cout << typeid(b).name() << '\n'
       << b << endl;

  random.ComputeGaussianRandomNumbers(output_dimension * word_dimension_,
                                      mu,
                                      sigma,
                                      weights_);
  random.ComputeGaussianRandomNumbers(output_dimension * output_dimension,
                                      mu,
                                      sigma,
                                      recurrent_weights_);
  random.ComputeGaussianRandomNumbers(output_dimension,
                                      mu,
                                      sigma,
                                      bias_);
  random.ComputeGaussianRandomNumbers(output_dimension,
                                      10.,
                                      3.,
                                      delta_t_);
  // Display delta_t_
  for (size_t j = 0; j < output_dimension; j++) {
    cout << delta_t_[j] << ' ';
  }
  cout << '\n' << endl;
  // Display weights_
  for (size_t j = 0; j < output_dimension; j++) {
    cout << weights_[j] << ' ';
  }
  cout << '\n' << endl;
  FastCopy(weights_, output_dimension, delta_t_);
  // Display delta_t_
  for (size_t j = 0; j < output_dimension; j++) {
    cout << delta_t_[j] << ' ';
  }
  cout << '\n' << endl;

  try {
    int num1;
    cout << "Enter first number:" << endl;
    cin >> num1;
    
    int num2;
    cout << "Enter second number:" << endl;
    cin >> num2;

    if (num2 == 0) {
      throw 0;
    }

    cout << num1 / num2 << endl;


  } catch(...) { // pass 0 into x !
    cout << "you can't divide by" << endl;
  }
/*  // Display weights_ matrix
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

  // Evaluate

  // TO DO: LOOP to make 2 times imput
  for (size_t a = 0; a < 2; a++) {
    cout << "Input" << ' ' << a << endl;
    // Fill x
    random.ComputeGaussianRandomNumbers(input_dimension * size,
                                        10.,
                                        3.,
                                        x);

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
          FastCopy(bias_, output_dimension, b_t_ + i * output_dimension); 
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


      // Activation of reccurency layer
      f->Evaluate(output_dimension, size, b_t_);
      //// TO DO : declarer le destructeur


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

  // Display output b_
  cout << "output after 2 inputs" << endl;
  for (size_t k = 0; k < 2 * size * order_; k++) {
    for (size_t l = 0; l < output_dimension; l++) {
      cout << b_[l + k * output_dimension] << ' ';
    }
    cout << '\n';
  }

  // ComputeDelta
  // TO DO: LOOP to make 2 times imput
  for (size_t a = 0; a < 2; a++) {
    for(size_t i = 0; i < order_; ++i) {
      // let b_t_ point to current order_ of current time step
      b_t_ -= GetOffset;


      if (i == 0) {
        // delta_t_ points to current order_ of current time step
        // here fill delta_t_ with random numbers
        random.ComputeGaussianRandomNumbers(output_dimension * size,
                                            mu,
                                            sigma,
                                            delta_t_);
      }
      else {
        // let delta_t_ point to current order_ of current time step
        delta_t_ += GetOffset; 
        if (delta_t_ != delta_) {
          // batch_size_t >= batch_size_{t+1}, so delta_{t+1}_ must be filled with 0
          FastMatrixMatrixMultiply(1.0,
                                   recurrent_weights_,
                                   true,
                                   output_dimension,
                                   output_dimension,
                                   delta_t_ - GetOffset,
                                   false,
                                   size,  // smaller batch_size suffices
                                   delta_t_);
        }
      }
      f->MultiplyDerivative(output_dimension, size,
                            b_t_, delta_t_);
    }


    // let delta_t_ point to previous time step
    delta_t_ += GetOffset;

    // let new_x_t_ point to previous input x
    new_x_t_ -= GetOffsetSlice * order_;
  }

  // Display delta_
  cout << "delta_" << endl;
  for (size_t k = 0; k < 2 * size * order_; k++) {
    for (size_t l = 0; l < output_dimension; l++) {
      cout << delta_[l + k * output_dimension] << ' ';
    }
    cout << '\n';
  }
  // Display momentum_weights_
  for (size_t i = 0; i < output_dimension; i++) {
    for (size_t j = 0; j < word_dimension_; j++) {
      cout << momentum_weights_[j + i * word_dimension_] << ' ';
    }
    cout << '\n';
  }
  cout << '\n' << endl;
  // Display new_x_t_
  cout << "new_x_t_" << endl;
  for (size_t k = 0; k < 2 * size * order_; k++) {
    for (size_t l = 0; l < word_dimension_; l++) {
      cout << new_x_t_[l + k * word_dimension_] << ' ';
    }
    cout << '\n';
  }

  // UpdateWeights
  for (size_t a = 0; a < 2; a++) {
    for (size_t j = 0; j < order_; ++j) {
      delta_t_ -= GetOffset;
      if (bias_) {
        for (size_t i = 0; i < size; ++i) {
          FastMultiplyByConstantAdd(-learning_rate,
                                    delta_t_ + i * output_dimension,
                                    output_dimension,
                                    momentum_bias_);
        }
      }
      if (j == 0) {
        FastMatrixMatrixMultiply(-learning_rate,
                                 delta_t_, 
                                 false,
                                 output_dimension,
                                 size,
                                 new_x_t_, 
                                 true,
                                 word_dimension_,
                                 momentum_weights_);
      }
      else {
        FastMatrixMatrixMultiply(-learning_rate,
                                 delta_t_,
                                 false,
                                 output_dimension,
                                 size,
                                 new_x_t_,
                                 true,
                                 word_dimension_,
                                 momentum_weights_);

        b_t_ += GetOffset;
        FastMatrixMatrixMultiply(-learning_rate,
                                 delta_t_,
                                 false,
                                 output_dimension,
                                 size,
                                 b_t_ - GetOffset,
                                 true,
                                 output_dimension,
                                 momentum_recurrent_weights_);
      } 
      new_x_t_ += GetOffsetSlice;
      // Display momentum_weights_
      for (size_t i = 0; i < output_dimension; i++) {
        for (size_t j = 0; j < word_dimension_; j++) {
          cout << momentum_weights_[j + i * word_dimension_] << ' ';
        }
        cout << '\n';
      }
      cout << '\n' << endl;
    }

    b_t_ -= GetOffset * (order_ - 1);
    const Real *result = b_t_;

    // let b_t_ point to next time step
    b_t_ += GetOffset * order_;

    // new_x_t_ already points to next time step
  }*/
  return 0;
}
