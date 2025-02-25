/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/vroom/solution/solution.h"

namespace vroom {

Solution::Solution(unsigned code, std::string error)
  : code(code), error(std::move(error)) {
}

Solution::Solution(unsigned code,
                   const Amount& zero_amount,
                   std::vector<Route>&& routes,
                   std::vector<Job>&& unassigned)
  : code(code),
    summary(routes.size(), unassigned.size(), zero_amount),
    routes(std::move(routes)),
    unassigned(std::move(unassigned)) {

  for (const auto& route : this->routes) {
    summary.cost += route.cost;
    summary.delivery += route.delivery;
    summary.pickup += route.pickup;
    summary.setup += route.setup;
    summary.service += route.service;
    summary.priority += route.priority;
    summary.duration += route.duration;
    summary.waiting_time += route.waiting_time;
    summary.violations += route.violations;
  }
}

} // namespace vroom
