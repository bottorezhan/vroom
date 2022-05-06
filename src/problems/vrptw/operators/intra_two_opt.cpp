/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "problems/vrptw/operators/intra_two_opt.h"

namespace vroom {
namespace vrptw {

IntraTwoOpt::IntraTwoOpt(const Input& input,
                         const utils::SolutionState& sol_state,
                         TWRoute& tw_s_route,
                         Index s_vehicle,
                         Index s_rank,
                         Index t_rank)
  : cvrp::IntraTwoOpt(input,
                      sol_state,
                      static_cast<RawRoute&>(tw_s_route),
                      s_vehicle,
                      s_rank,
                      t_rank),
    _tw_s_route(tw_s_route) {
}

bool IntraTwoOpt::is_valid() {
  auto rev_t = s_route.rbegin() + (s_route.size() - t_rank - 1);
  auto rev_s_next = s_route.rbegin() + (s_route.size() - s_rank);

  return cvrp::IntraTwoOpt::is_valid() and
         _tw_s_route.is_valid_addition_for_tw(_input,
                                              rev_t,
                                              rev_s_next,
                                              s_rank,
                                              t_rank + 1);
}

void IntraTwoOpt::apply() {
  // TODO
}

} // namespace vrptw
} // namespace vroom
