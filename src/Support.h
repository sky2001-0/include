#ifndef CXXSUPPORT_H
#define CXXSUPPORT_H

#include <cmath>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <functional>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>



namespace csp
{
  namespace math
  {
    template <typename FloatType = double>
    FloatType Rand(
      const FloatType min = FloatType(0),
      const FloatType max = FloatType(1)
    )
    {
      static_assert(
        std::is_floating_point_v<FloatType>,
        "Template parameter 'FloatType' must be a floating point type."
      );
      if (min >= max) {
        throw std::invalid_argument(
          "Minimum value must be less than maximum value."
        );
      }

      std::random_device rd;
      std::mt19937 mt(rd());
      std::uniform_real_distribution<FloatType> urd(min, max);

      return urd(mt);
    }


    template <typename FloatType = double>
    constexpr FloatType Mod(
      const FloatType numerator,
      const FloatType denominator
    )
    {
      static_assert(
        std::is_floating_point_v<FloatType>,
        "Template parameter 'FloatType' must be a floating point type."
      );
      if (denominator == FloatType(0)) {
        throw std::domain_error("Zero division error.");
      }

      const FloatType remainder = std::fmod(numerator, denominator);
      if (remainder < 0.) {
        return remainder + std::abs(denominator);
      } else {
        return remainder;
      }
    }


    constexpr int Sign(const double x) noexcept
    {
      return static_cast<int>(x > 0.) - static_cast<int>(x < 0.);
    }


    inline constexpr double Sinc(const double x) noexcept
    {
      if (std::abs(x) < 1e-3) {
        return 1. - std::pow(x, 2) / 6. + std::pow(x, 4) / 120.;
      } else {
        return std::sin(x) / x;
      }
    }


    inline int Factorial(unsigned int n)
    {
      if (n > 12) {
        throw std::range_error("int overflow");
      }
      std::vector<unsigned int> ran(n);
      std::iota(ran.begin(), ran.end(), 1);
      return std::accumulate(ran.begin(), ran.end(), 1, std::multiplies<>());
    }
  }


  namespace file
  {
    inline std::string Open(const std::filesystem::path& filepath)
    {
      std::ifstream ifs(filepath, std::ios::binary | std::ios::ate);
      if (!ifs) {
        throw std::runtime_error("Failed to open file: " + filepath.string());
      }

      const auto fileSize = ifs.tellg();
      if (fileSize == -1) {
        throw std::runtime_error(
          "Failed to determine file size: " + filepath.string()
        );
      }

      std::string content;
      content.resize(static_cast<std::size_t>(fileSize));
      ifs.seekg(0);
      ifs.read(content.data(), content.size());

      if (!ifs) {
        throw std::runtime_error("Error reading file: " + filepath.string());
      }

      return content;
    }


    inline void Write(
      const std::filesystem::path& filepath,
      const std::string content,
      const bool append = false
    )
    {
      std::ios_base::openmode mode = std::ios::binary;
      if (append) {
        mode |= std::ios::app;
      } else {
        mode |= std::ios::out;
      }
      std::ofstream ofs(filepath, mode);
      if (!ofs) {
        throw std::runtime_error("Failed to open file: " + filepath.string());
      }

      ofs.write(content.data(), content.size());
      if (!ofs) {
        throw std::runtime_error("Error writing file: " + filepath.string());
      }
    }
  }


  namespace utils
  {
    [[deprecated]]
    inline const std::string TimeStamp()
    {
      std::time_t now = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
      );
      std::stringstream ss;
      ss << std::put_time(std::localtime(&now), "%Y-%m-%d_%H:%M:%S");
      return ss.str();
    }


    inline std::filesystem::path PathNormalize(
      const std::filesystem::path& raw_path
    )
    {
      if (raw_path.filename() == std::filesystem::path("")) {
        return raw_path.parent_path();
      } else {
        return raw_path;
      }
    }


    inline std::string ToString(const double value, const int precision)
    {
      std::ostringstream out;
      out << std::fixed << std::setprecision(precision) << value;
      return out.str();
    }
  }
}



#endif // CXXSUPPORT_H
