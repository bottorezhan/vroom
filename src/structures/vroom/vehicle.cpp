/*

This file is part of VROOM.

Copyright (c) 2015-2022, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include <algorithm>
#include <numeric>

#include "structures/vroom/vehicle.h"
#include "utils/exception.h"

namespace vroom {

Vehicle::Vehicle(Id id,
                 const std::optional<Location>& start,
                 const std::optional<Location>& end,
                 std::string profile,
                 const Amount& capacity,
                 Skills skills,
                 const TimeWindow& tw,
                 const std::vector<Break>& breaks,
                 std::string description,
                 const VehicleCosts& costs,
                 double speed_factor,
                 const size_t max_tasks,
                 const std::optional<UserDuration>& max_travel_time,
                 const std::vector<VehicleStep>& input_steps)
  : id(id),
    start(start),
    end(end),
    profile(std::move(profile)),
    capacity(capacity),
    skills(std::move(skills)),
    tw(tw),
    breaks(breaks),
    description(std::move(description)),
    costs(costs),
    cost_wrapper(speed_factor, costs.per_hour),
    max_tasks(max_tasks),
    max_travel_time(max_travel_time.has_value()
                      ? utils::scale_from_user_duration(max_travel_time.value())
                      : std::numeric_limits<Duration>::max()),
    has_break_max_load(
      std::any_of(breaks.cbegin(), breaks.cend(), [](const auto& b) {
        return b.max_load.has_value();
      })) {
  if (!static_cast<bool>(start) and !static_cast<bool>(end)) {
    throw InputException("No start or end specified for vehicle " +
                         std::to_string(id) + '.');
  }

  for (unsigned i = 0; i < breaks.size(); ++i) {
    const auto& b = breaks[i];

    if (break_id_to_rank.find(b.id) != break_id_to_rank.end()) {
      throw InputException("Duplicate break id: " + std::to_string(b.id) + ".");
    }
    break_id_to_rank[b.id] = i;

    if (b.max_load.has_value() and
        b.max_load.value().size() != capacity.size()) {
      throw InputException("Inconsistent break max_load size for break: " +
                           std::to_string(b.id) + ".");
    }
  }

  if (!input_steps.empty()) {
    // Populating steps. We rely on always having start and end steps
    // in input, so just add them if they're missing.
    unsigned rank_after_start = 0;
    if (input_steps.front().type == STEP_TYPE::START) {
      steps.push_back(input_steps.front());
      rank_after_start = 1;
    } else {
      steps.emplace_back(STEP_TYPE::START);
    }

    for (unsigned i = rank_after_start; i < input_steps.size(); ++i) {
      if (input_steps[i].type == STEP_TYPE::START) {
        throw InputException("Unexpected start in input steps for vehicle " +
                             std::to_string(id) + ".");
      }
      if (input_steps[i].type == STEP_TYPE::END and
          (i != input_steps.size() - 1)) {
        throw InputException("Unexpected end in input steps for vehicle " +
                             std::to_string(id) + ".");
      }

      steps.push_back(input_steps[i]);
    }

    if (steps.back().type != STEP_TYPE::END) {
      steps.emplace_back(STEP_TYPE::END);
    }
  }
}

bool Vehicle::has_start() const {
  return static_cast<bool>(start);
}

bool Vehicle::has_end() const {
  return static_cast<bool>(end);
}

bool Vehicle::has_same_locations(const Vehicle& other) const {
  bool same = (this->has_start() == other.has_start()) and
              (this->has_end() == other.has_end());

  if (same and this->has_start()) {
    same = this->start.value() == other.start.value();
  }

  if (same and this->has_end()) {
    same = this->end.value() == other.end.value();
  }

  return same;
}

bool Vehicle::has_same_profile(const Vehicle& other) const {
  return (this->profile == other.profile) and
         (this->cost_wrapper.get_discrete_duration_factor() ==
          other.cost_wrapper.get_discrete_duration_factor());
}

bool Vehicle::cost_based_on_duration() const {
  return cost_wrapper.cost_based_on_duration();
}

Duration Vehicle::available_duration() const {
  Duration available = tw.end - tw.start;

  Duration breaks_duration =
    std::accumulate(breaks.begin(),
                    breaks.end(),
                    0,
                    [](auto sum, const auto& b) { return sum + b.service; });

  assert(breaks_duration <= available);

  return available - breaks_duration;
}

Index Vehicle::break_rank(Id id) const {
  auto search = break_id_to_rank.find(id);
  assert(search != break_id_to_rank.end());
  return search->second;
}

} // namespace vroom
