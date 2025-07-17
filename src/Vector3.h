#ifndef CXXVECTOR3_H
#define CXXVECTOR3_H

#include <cmath>
#include <concepts>
#include <ostream>



namespace csp::math
{
  template <std::floating_point T>
  class Vector3
  {
  public:
    T x_{};
    T y_{};
    T z_{};


    constexpr Vector3() = default;

    constexpr Vector3(const T x, const T y, const T z)
    : x_(x), y_(y), z_(z)
    {
    }

    ~Vector3() = default;

    Vector3(const Vector3& rh) = default;

    Vector3(Vector3&& rh) = default;

    Vector3& operator=(const Vector3& rh) = default;

    Vector3& operator=(Vector3&& rh) = default;


    constexpr Vector3& operator+=(const Vector3& rh) noexcept
    {
      x_ += rh.x_;
      y_ += rh.y_;
      z_ += rh.z_;
      return *this;
    }

    constexpr Vector3& operator-=(const Vector3& rh) noexcept
    {
      x_ -= rh.x_;
      y_ -= rh.y_;
      z_ -= rh.z_;
      return *this;
    }

    constexpr Vector3& operator*=(const T scalar) noexcept
    {
      x_ *= scalar;
      y_ *= scalar;
      z_ *= scalar;
      return *this;
    }

    constexpr Vector3& operator/=(const T scalar) noexcept
    {
      x_ /= scalar;
      y_ /= scalar;
      z_ /= scalar;
      return *this;
    }


    constexpr bool operator==(const Vector3& rh) const noexcept
    {
      return x_ == rh.x_ && y_ == rh.y_ && z_ == rh.z_;
    }

    constexpr bool operator!=(const Vector3& rh) const noexcept
    {
      return !(*this == rh);
    }

    constexpr Vector3 operator+(const Vector3& rh) const
    {
      return {x_ + rh.x_, y_ + rh.y_, z_ + rh.z_};
    }

    constexpr Vector3 operator-(const Vector3& rh) const
    {
      return {x_ - rh.x_, y_ - rh.y_, z_ - rh.z_};
    }

    constexpr Vector3 operator*(const T scalar) const
    {
      return {x_ * scalar, y_ * scalar, z_ * scalar};
    }

    friend constexpr Vector3 operator*(const T scalar, const Vector3& vec)
    {
      return vec * scalar;
    }

    constexpr Vector3 operator/(const T scalar) const
    {
      return {x_ / scalar, y_ / scalar, z_ / scalar};
    }

    constexpr Vector3 operator-() const
    {
      return {-x_, -y_, -z_};
    }


    constexpr T dot(const Vector3& rh) const
    {
      return x_ * rh.x_ + y_ * rh.y_ + z_ * rh.z_;
    }

    constexpr Vector3 cross(const Vector3& rh) const
    {
      return {
        y_ * rh.z_ - z_ * rh.y_,
        z_ * rh.x_ - x_ * rh.z_,
        x_ * rh.y_ - y_ * rh.x_
      };
    }

    T norm() const
    {
      return std::sqrt(norm2());
    }

    constexpr T norm2() const
    {
      return x_ * x_ + y_ * y_ + z_ * z_;
    }

    Vector3 normalize() const
    {
      T n = norm();
      return n != T(0) ? *this / n : *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& vec)
    {
      return os << '(' << vec.x_ << ", " << vec.y_ << ", " << vec.z_ << ')';
    }
  };


  template <std::size_t I, std::floating_point T>
  constexpr T& get(Vector3<T>& vec) noexcept
  {
    static_assert(I < 3, "Index out of range for Vector3");
    if constexpr (I == 0) {
      return vec.x_;
    } else if constexpr (I == 1) {
      return vec.y_;
    } else {
      return vec.z_;
    }
  }

  template <std::size_t I, std::floating_point T>
  constexpr const T& get(const Vector3<T>& vec) noexcept
  {
    static_assert(I < 3, "Index out of range for Vector3");
    if constexpr (I == 0) {
      return vec.x_;
    } else if constexpr (I == 1) {
      return vec.y_;
    } else {
      return vec.z_;
    }
  }
}


namespace std
{
  template <std::floating_point T>
  struct tuple_size<csp::math::Vector3<T>>
  {
    static constexpr std::size_t value = 3;
  };

  template <std::size_t I, std::floating_point T>
  struct tuple_element<I, csp::math::Vector3<T>>
  {
    static_assert(I < 3, "Index out of range for Vector3");
    using type = T;
  };
}



#endif // CXXVECTOR3_H
