/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/generic/matrix.h"

namespace vroom {

template <class T> Matrix<T>::Matrix(std::size_t n) : n(n) {
  data.resize(n);
}

template <class T> Matrix<T>::Matrix() : Matrix(0) {
}

template class Matrix<UserCost>;

} // namespace vroom
