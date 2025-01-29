#ifndef UTILS_DEFAULT_MAP_HH
#define UTILS_DEFAULT_MAP_HH

#include <limits>
#include <unordered_map>

namespace Utils {

//
// default_map<KEY, VALUE> defines a simple wrapper around std::unordered_map,
// offering s single convenience member functino 'at_or_max()', which returns
// the numerically maximal value if a given key does not exist.
//
template <typename KEY, typename VALUE>
  requires std::is_integral_v<VALUE> or std::is_floating_point_v<VALUE>
class default_map : public std::unordered_map<KEY, VALUE> {
  static inline VALUE max_ = std::numeric_limits<VALUE>::max();

 public:
  [[nodiscard]] constexpr auto at_or_max(const KEY& key) const -> const VALUE& {
    if (this->contains(key)) return this->at(key);
    return max_;
  }
};

}  // namespace Utils

#endif  // UTILS_DEFAULT_MAP_HH
