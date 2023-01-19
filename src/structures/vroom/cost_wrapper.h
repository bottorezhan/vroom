#ifndef COST_WRAPPER_H
#define COST_WRAPPER_H

/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/generic/matrix.h"
#include "structures/typedefs.h"

namespace vroom {

class CostWrapper {
private:
  const Duration discrete_duration_factor;
  const Matrix<UserDuration>* duration_matrix;

  Cost discrete_cost_factor;
  const Matrix<UserCost>* cost_matrix;

  Cost _per_hour;
  bool _cost_based_on_duration;

public:
  CostWrapper(double speed_factor, Cost per_hour);

  void set_durations_matrix(const Matrix<UserDuration>* matrix);

  void set_costs_matrix(const Matrix<UserCost>* matrix,
                        bool reset_cost_factor = false);

  Duration get_discrete_duration_factor() const {
    return discrete_duration_factor;
  }

  bool cost_based_on_duration() const {
    return _cost_based_on_duration;
  }

  Duration duration(Index i, Index j) const {
    return discrete_duration_factor *
           static_cast<Duration>((*duration_matrix).get(i, j));
  }

  Cost cost(Index i, Index j) const {
    return discrete_cost_factor *
           static_cast<Cost>((*cost_matrix).get(i, j));
  }

  UserCost user_cost_from_user_duration(UserDuration d) const;
};

} // namespace vroom

#endif
