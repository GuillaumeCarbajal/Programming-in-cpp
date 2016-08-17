#include <memory>
#include "fast.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

int main(){
  const int size = 3, order = 4;
  //const size_t order_;
  //order_(order);

  Real x[size * order];
  Real *new_x;
  new_x = FastMalloc(size * order);
  for (size_t i = 0; i < size * order; i++) {
    x[i] = i + 1;
    new_x[size * (i % order) + i / order] = x[i];
  }
  for (size_t i = 0; i < size * order; i++) {
    cout << x[i]<<'\n';
  }
  for (size_t i = 0; i < size * order; i++) {
    cout << new_x[i]<<'\n';
  }
  cout << size << '\n'
       << order << '\n'
       << typeid(size).name() << '\n'
       << typeid(x).name() << '\n'
       //<< typeid(x_new).name() << '\n'
       << endl;
  cout << sizeof(x) / sizeof(x[0]) << '\n' // nb of elements of x
       << sizeof(Real) << '\n'             // size of a Real type
       << sizeof(new_x)<< '\n' 
       << sizeof(x) <<endl;                // nb of bits of x
  return 0;
}
