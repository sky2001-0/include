#ifndef CXXMATHUTILS_H
#define CXXMATHUTILS_H

#include <cmath>
#include <concepts>
#include <random>
#include <type_traits>



namespace csp::math
{
  template <std::floating_point T = double>
  T Rand(const T min = T(0), const T max = T(1))
  {
    static thread_local std::mt19937 mt{std::random_device{}()};
    std::uniform_real_distribution<T> urd(min, max);
    return urd(mt);
  }


  template <typename T>
    requires std::is_arithmetic_v<T>
  constexpr int Sign(const T x) noexcept
  {
    return static_cast<int>(x > T(0)) - static_cast<int>(x < T(0));
  }


  template <std::floating_point T>
  T Sinc(const T x) noexcept
  {
    if (std::abs(x) < 1e-3) {
      return 1. - std::pow(x, 2) / 6. + std::pow(x, 4) / 120.;
    } else {
      return std::sin(x) / x;
    }
  }


  template <std::integral T>
  constexpr T Pmod(T a, T m) noexcept
  {
    return (a % m + m) % m;
  }


  template <std::floating_point T>
  T Pmod(T a, T m) noexcept
  {
    T r = std::fmod(a, m);
    return (r < 0) ? r + m : r;
  }
}



#endif // CXXMATHUTILS_H