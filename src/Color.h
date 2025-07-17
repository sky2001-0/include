#ifndef CXXCOLOR_H
#define CXXCOLOR_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>
#include <string>
#include <tuple>

#include "Vector3.h"



namespace csp::utils
{
  class Color
  {
    using RGB = std::tuple<unsigned char, unsigned char, unsigned char>;
    using HSV = std::tuple<float, float, float>;


    unsigned char r_;
    unsigned char g_;
    unsigned char b_;


  public:
    Color() = default;

    explicit Color(const std::uint32_t hex)
    : r_((hex >> 16) & 0xFF), g_((hex >> 8) & 0xFF), b_(hex & 0xFF)
    {
    }

    Color(const unsigned char r, const unsigned char g, const unsigned char b)
    : r_(r), g_(g), b_(b)
    {
    }

    Color(const float h, const float s, const float v)
    {
      const unsigned char max_val = std::round(255.f * v);
      const unsigned char min_val = std::round(255.f * v * (1.f - s));
      const unsigned char x_val = std::round(
        255.f * v * (1.f - s * std::abs(std::fmod(h / 60.f, 2.f) - 1.f))
      );

      if (h < 60.f) {
        r_ = max_val;
        g_ = x_val;
        b_ = min_val;
      } else if (h < 120.f) {
        r_ = x_val;
        g_ = max_val;
        b_ = min_val;
      } else if (h < 180.f) {
        r_ = min_val;
        g_ = max_val;
        b_ = x_val;
      } else if (h < 240.f) {
        r_ = min_val;
        g_ = x_val;
        b_ = max_val;
      } else if (h < 300.f) {
        r_ = x_val;
        g_ = min_val;
        b_ = max_val;
      } else {
        r_ = max_val;
        g_ = min_val;
        b_ = x_val;
      }
    }

    explicit Color(const RGB& rgb)
    : Color(std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb))
    {
    }

    explicit Color(const HSV& hsv)
    : Color(std::get<0>(hsv), std::get<1>(hsv), std::get<2>(hsv))
    {
    }

    ~Color() = default;

    Color(const Color& rh) = default;

    Color(Color&& rh) = default;

    Color& operator=(const Color& rh) = default;

    Color& operator=(Color&& rh) = default;


    std::uint32_t to_hex() const
    {
      return (
        (static_cast<std::uint32_t>(r_) << 16)
        | (static_cast<std::uint32_t>(g_) << 8)
        | static_cast<std::uint32_t>(b_)
      );
    }

    RGB to_rgb() const
    {
      return std::make_tuple(r_, g_, b_);
    }

    HSV to_hsv() const
    {
      const float rf = r_ / 255.f;
      const float gf = g_ / 255.f;
      const float bf = b_ / 255.f;

      const float max_val = std::max({rf, gf, bf});
      const float min_val = std::min({rf, gf, bf});
      const float delta = max_val - min_val;

      float h;
      if (delta == 0.f) {
        h = 0.f;
      } else {
        if (max_val == rf) {
          h = 60.f * ((gf - bf) / delta);
        } else if (max_val == gf) {
          h = 60.f * ((bf - rf) / delta + 2.f);
        } else {
          h = 60.f * ((rf - gf) / delta + 4.f);
        }

        if (h < 0.f)
          h += 360.f;
      }

      const float s = (max_val == 0.f) ? 0.f : delta / max_val;
      const float v = max_val;

      return {h, s, v};
    }


    bool operator==(const Color& rh) const noexcept
    {
      return r_ == rh.r_ && g_ == rh.g_ && b_ == rh.b_;
    }

    bool operator!=(const Color& rh) const noexcept
    {
      return !(*this == rh);
    }


    Color GenerateColor(const int number) const
    {
      static constexpr float pi = std::numbers::pi_v<float>;

      auto [h, s, v] = to_hsv();
      const float theta = pi / 2.f * v;
      const float phi = 2.f * pi * h;

      using Vec3F = math::Vector3<float>;

      Vec3F vec = {
        s * std::sin(theta) * std::cos(phi),
        s * std::sin(theta) * std::sin(phi),
        -std::cos(theta)
      };
      Vec3F vec_normal = vec.normalize();
      Vec3F up = {0.f, 0.f, 1.f};
      Vec3F axis1 = vec_normal.cross(up).normalize();
      Vec3F axis2 = vec_normal.cross(axis1).normalize();

      const float step = pi / 36.f * number;

      Vec3F new_vec = (
        std::sin(step) * (std::cos(step) * axis1 + std::sin(step) * axis2)
        + std::cos(step) * vec
      );

      const float new_v = 1.f - std::acos(-new_vec.z_) / pi;
      const float r = std::sqrt(
        new_vec.x_ * new_vec.x_ + new_vec.y_ * new_vec.y_
      );
      const float new_s = r / std::sin(pi / 2.f * new_v);
      float new_h = std::atan2(new_vec.y_, new_vec.x_) / (2.f * pi);
      if (new_h < 0.f) {
        new_h += 1.f;
      }

      return {new_h, new_s, new_v};
    }
  };
}



#endif // CXXCOLOR_H
