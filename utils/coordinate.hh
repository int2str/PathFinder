#ifndef COORDINATE_HH
#define COORDINATE_HH

#include <array>
#include <cmath>
#include <compare>  // IWYU pragma: keep
#include <type_traits>
#include <vector>  // IWYU pragma: keep - Included for std::hash

namespace Utils {

//
// CoordinateBase defines an arbitrary integer size, two-dimensional coordinate
// representation.
//
template <typename INTEGER_TYPE>
  requires std::is_integral_v<INTEGER_TYPE>
struct CoordinateBase {
  INTEGER_TYPE x{};
  INTEGER_TYPE y{};

  [[nodiscard]] constexpr auto operator<=>(const CoordinateBase&) const =
      default;

  // Math

  constexpr auto operator+=(const CoordinateBase& other) -> CoordinateBase& {
    x += other.x;
    y += other.y;
    return *this;
  }

  constexpr auto operator-=(const CoordinateBase& other) -> CoordinateBase& {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  // Transform

  [[nodiscard]] constexpr auto rotatedClockwise() const -> CoordinateBase {
    return CoordinateBase{-y, x};
  }

  constexpr auto rotateClockwise() -> CoordinateBase& {
    *this = this->rotatedClockwise();
    return *this;
  }

  [[nodiscard]] constexpr auto rotatedCounterClockwise() const
      -> CoordinateBase {
    return CoordinateBase{y, -x};
  }

  constexpr auto rotateCounterClockwise() -> CoordinateBase& {
    *this = this->rotatedCounterClockwise();
    return *this;
  }

  [[nodiscard]] constexpr auto flipped() const -> CoordinateBase {
    return CoordinateBase{-x, -y};
  }

  constexpr auto flip() -> CoordinateBase& {
    *this = this->flipped();
    return *this;
  }

  // Info

  [[nodiscard]] constexpr auto isZero() const -> bool {
    return x == 0 and y == 0;
  }

  [[nodiscard]] constexpr auto normalized() const -> CoordinateBase {
    if (x == 0 and y == 0) return {};
    return *this / std::max(std::abs(x), std::abs(y));
  }

  [[nodiscard]] constexpr auto neighbors() const
      -> std::array<CoordinateBase, 8> {
    return {CoordinateBase{x - 1, y - 1},
            {x, y - 1},
            {x + 1, y - 1},
            {x - 1, y},
            {x + 1, y},
            {x - 1, y + 1},
            {x, y + 1},
            {x + 1, y + 1}};
  }

  [[nodiscard]] constexpr auto neighborsUpDownLeftRight() const
      -> std::array<CoordinateBase, 4> {
    return {
        CoordinateBase{x, y - 1},  // Up
        {x, y + 1},                // Down
        {x - 1, y},                // Left
        {x + 1, y}                 // Right
    };
  }

  [[nodiscard]] constexpr auto neighorsDiagonal() const
      -> std::array<CoordinateBase, 4> {
    return {CoordinateBase{x - 1, y - 1},
            {x + 1, y + 1},
            {x + 1, y - 1},
            {x - 1, y + 1}};
  }

  [[nodiscard]] constexpr auto distanceFrom(const CoordinateBase& other) const
      -> double {
    return std::sqrt(((x - other.x) * (x - other.x)) +
                     ((y - other.y) * (y - other.y)));
  }

  [[nodiscard]] constexpr auto manhattanDistanceFrom(
      const CoordinateBase& other) const -> INTEGER_TYPE {
    return std::abs(x - other.x) + std::abs(y - other.y);
  }
};

//
// Default coordinate type
//
using Coordinate = CoordinateBase<int>;

}  // namespace Utils

namespace std {

//
// std::hash<> customization for Utils::Coordinate allows usage of
// Utils::Coordinate with std::unordered_map<>, std::unordered_set<> and similar
// types.
//
template <>
struct hash<Utils::Coordinate> {
  [[nodiscard]] auto operator()(
      const Utils::Coordinate& coordinate) const noexcept -> size_t {
    return std::hash<int>()(coordinate.x) ^ std::hash<int>()(coordinate.y);
  }
};

}  // namespace std

// Free-standing math operators

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator+(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator-(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator*(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x * rhs.x, .y = lhs.y * rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator/(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs,
    const Utils::CoordinateBase<INTEGER_TYPE>& rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x / rhs.x, .y = lhs.y / rhs.y};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator+(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs, INTEGER_TYPE rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x + rhs, .y = lhs.y + rhs};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator-(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs, INTEGER_TYPE rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x - rhs, .y = lhs.y - rhs};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator*(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs, INTEGER_TYPE rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x * rhs, .y = lhs.y * rhs};
}

template <typename INTEGER_TYPE>
[[nodiscard]] constexpr auto operator/(
    const Utils::CoordinateBase<INTEGER_TYPE>& lhs, INTEGER_TYPE rhs)
    -> Utils::CoordinateBase<INTEGER_TYPE> {
  return {.x = lhs.x / rhs, .y = lhs.y / rhs};
}

#endif  // COORDINATE_HH
