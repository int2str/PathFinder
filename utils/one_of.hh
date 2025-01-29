#ifndef ONE_OF_HH
#define ONE_OF_HH

#include <tuple>

//
// one_of<...> defines a convenient std::tuple wrapper type that allows
// comparisons to a given number of values.
//
// For example:
//     if (chr == 'A' or chr == 'B' or chr == 'C') ...
//
// becomes:
//     if (chr == one_of('A', 'B', 'C')) ...
//
template <typename... Ts>
struct one_of : private std::tuple<Ts...> {
  using std::tuple<Ts...>::tuple;

  template <typename T>
  [[nodiscard]] constexpr auto operator==(const T& val) const noexcept -> bool {
    const auto& tup = static_cast<const std::tuple<Ts...>&>(*this);
    return std::apply(
        [&val](const auto&... args) { return ((args == val) or ...); }, tup);
  }
};

//
// one_of<> template deduction guide
//
template <typename... Ts>
one_of(Ts...) -> one_of<Ts...>;

#endif  // ONE_OF_HH


