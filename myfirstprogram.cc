#include <memory>
#include "fast.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

int main(){
  const int size = 3, order = 4;
  const size_t size_, order_;
  size_(size), order_(order);

  Real x[size * order];
  Real *new_x;
  new_x = FastMalloc(size * order);
  /*for (size_t i = 0; i < size * order_; i++) {
    x[i] = i + 1;
  }
  FastCopy(x, sizeof(x), new_x);
  for (int i = 0; i < sizeof(x); i++) {
    cout << x[i]<<'\n';
  }
  for (int i = 0; i < sizeof(x); i++) {
    count << new_x[i];
  }*/
  cout << size << '\n'
       << order_ << '\n'
       << typeid(size).name() << '\n'
       << typeid(x).name() << '\n'
       //<< typeid(x_new).name() << '\n'
       << endl;
       
  cout << sizeof(x) <<endl;
  return 0;
}
