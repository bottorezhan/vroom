#ifndef MATRIX_H
#define MATRIX_H

/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <initializer_list>
#include <unordered_map>

#include "structures/typedefs.h"

namespace vroom {

template <class T> class Matrix {

  std::size_t n;
  std::vector<std::unordered_map<std::size_t, T>> data;

public:
  Matrix();

  Matrix(std::size_t n);

  T get(std::size_t i, std::size_t j) const {
    return data[i].at(j);
  }
  
  void set(std::size_t i, std::size_t j, T value) {
    data[i][j] = value;
  }

  std::size_t size() const {
    return n;
  }
};

} // namespace vroom

#endif
