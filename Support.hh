#ifndef CXXSUPPORT_HH
#define CXXSUPPORT_HH

#include <chrono>
#include <cmath>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <functional>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>



namespace csp
{
  /*
    functions for mathematics
  */
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


  /*
    functions for utility
  */
  namespace utils
  {
    [[deprecated]] inline const std::string TimeStamp()
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


  /*
    Range is a class desined to represent a linear sequence of values.
  */
  template <typename NumberLikeType = std::size_t>
  class Range
  {
    static_assert(
      std::is_arithmetic_v<NumberLikeType>,
      "Template parameter 'NumberLikeType' must be arithmeric."
    );

  private:
    const NumberLikeType front_;
    const NumberLikeType step_;
    const std::size_t size_;

  private:
    std::size_t ConvertSizePositive(
      const NumberLikeType front,
      const NumberLikeType end
    ) noexcept
    {
      if (front <= end) {
        return (end - front) / NumberLikeType(1);
      } else {
        return 0;
      }
    }

  public:
    Range() = delete;

    explicit Range(const std::size_t size) noexcept
    : front_(0), step_(1), size_(size)
    {
    }

    Range(const NumberLikeType front, const NumberLikeType end) noexcept
    : front_(front), step_(1), size_(ConvertSizePositive(front, end))
    {
    }

    Range(
      const NumberLikeType&& front,
      const NumberLikeType&& step,
      const std::size_t&& size
    ) noexcept
    : front_(front), step_(step), size_(size)
    {
    }

    Range(
      const NumberLikeType front,
      const NumberLikeType back_end,
      const std::size_t size,
      const bool with_end
    )
    : front_(front),
      step_(
        with_end ? (back_end - front) / (size - 1) : (back_end - front) / size
      ),
      size_(size)
    {
      if (size_ == 0) {
        throw std::invalid_argument("Argument 'size' must be positive.");
      }
      if (front_ >= back_end) {
        throw std::invalid_argument(
          "Argument 'front' must be less than back_end."
        );
      }
      if (with_end && size_ == 1) {
        throw std::invalid_argument(
          "With 'with_end' set to true, Argument 'size' must be greater than 1."
        );
      }
    }

    ~Range() noexcept = default;

    Range(const Range<NumberLikeType>& rh) = delete;

    Range(Range<NumberLikeType>&& rh) = default;

    Range& operator=(const Range<NumberLikeType>& rh) = delete;

    Range& operator=(Range<NumberLikeType>&& rh) = default;

    virtual NumberLikeType operator[](const std::size_t index) const noexcept
    {
      return front_ + index * step_;
    }

    NumberLikeType At(const std::size_t index) const
    {
      if (index >= size_) {
        throw std::range_error("Range range error.");
      }
      return (*this)[index];
    }

    NumberLikeType front() const noexcept { return front_; }

    NumberLikeType back() const noexcept { return (*this)[size_ - 1]; }

    std::size_t size() const noexcept { return size_; }

    NumberLikeType step() const noexcept { return step_; }

    std::vector<NumberLikeType> ToVector() const noexcept
    {
      std::vector<NumberLikeType> vec(size_);
      for (std::size_t i = 0; i < size_; ++i) {
        vec[i] = (*this)[i];
      }
      return vec;
    }

    class IteratorLike
    {
    private:
      const Range<NumberLikeType>& obj_;
      std::size_t index_;

    public:
      IteratorLike() = delete;

      IteratorLike(
        const Range<NumberLikeType>& obj, const std::size_t index
      ) noexcept
      : obj_(obj), index_(index)
      {}

      ~IteratorLike() noexcept = default;

      std::size_t operator()() const noexcept { return index_; }

      NumberLikeType operator*() const noexcept { return obj_[index_]; }

      IteratorLike& operator++() noexcept
      {
        ++index_;
        return *this;
      }

      IteratorLike operator++(int) noexcept
      {
        IteratorLike old(*this);
        ++*this;
        return old;
      }

      bool operator!=(const IteratorLike& rh) const noexcept
      {
        return index_ != rh.index_;
      }
    };

    IteratorLike begin() const noexcept
    {
      return IteratorLike(*this, 0);
    }

    IteratorLike end() const noexcept
    {
      return IteratorLike(*this, size_);
    }
  };


  template <typename FloatType>
  class LinRange : public Range<FloatType>
  {
    static_assert(
      std::is_floating_point_v<FloatType>,
      "Template parameter 'FloatType' must be floating point type."
    );

  public:
    LinRange() = delete;

    LinRange(
      const FloatType front,
      const FloatType step,
      const std::size_t size
    ) noexcept
    : Range<FloatType>(front, step, size)
    {
    }

    FloatType operator[](const FloatType index_like) const noexcept override
    {
      return this->get_front() + index_like * this->get_step();
    }

    FloatType IndexLikeOf(const FloatType value) const noexcept
    {
      return (value - this->get_front()) / this->get_step();
    }
  };


  /*
    ProgressBar is a class desinged to draw progress-bar.
  */
  template<typename IntegralType>
  class ProgressBar final
  {
    static_assert(
      std::is_integral_v<IntegralType>,
      "Template parameter IntegralType must be integral type."
    );

    using steady_clock = std::chrono::steady_clock;

    static std::string DurationPrint(
      const std::chrono::duration<double>& duration
    ) noexcept
    {
      const int days = static_cast<int>(
        std::chrono::duration_cast<std::chrono::hours>(duration).count() / 24
      );
      const int hours = static_cast<int>(
        std::chrono::duration_cast<std::chrono::hours>(duration).count() % 24
      );
      const int minutes = static_cast<int>(
        std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60
      );
      const double seconds = std::fmod(duration.count(), 60.);

      std::stringstream ss;
      {
        bool is_display = false;
        if (days > 0) {
          ss << days << "d " << std::setfill('0');
          is_display = true;
        }
        if (hours > 0 || is_display) {
          ss << std::setw(2) << hours << ":" << std::setfill('0');
          is_display = true;
        }
        if (minutes > 0 || is_display) {
          ss << std::setw(2) << minutes << ":" << std::setfill('0');
          is_display = true;
        }
        ss << std::setw(6) << std::fixed << std::setprecision(3) << seconds;
        if (!is_display) {
          ss << "s";
        }
      }
      return ss.str();
    }

    static std::string DurationPrint(
      const std::chrono::time_point<steady_clock>& time
    ) noexcept
    {
      return DurationPrint(steady_clock::now() - time);
    }

  private:
    const IntegralType total_;
    const int bar_width_;

    IntegralType progress_;

    const std::chrono::time_point<steady_clock> start_time_;

    std::mutex mtx_;

    void Draw() const noexcept
    {
      std::stringstream ss;
      {
        const int progress_width = bar_width_ * progress_ / total_;

        ss << "[";
        for (int i = 0; i < progress_width; ++i) {
          ss << "#";
        }
        for (int i = progress_width; i < bar_width_; ++i) {
          ss << " ";
        }
        ss << "]";
      }

      ss << std::setfill(' ')
         << std::setw(3)
         << 100 * progress_ / total_
         << "% ";

      ss << DurationPrint(start_time_);

      if (progress_ == total_) {
        std::cout << ss.str() << std::endl;
      } else {
        std::cout << ss.str() << "\r";
        std::cout.flush();
      }
    }

  public:
    ProgressBar(const IntegralType total, const int bar_width = 70) noexcept
    : total_(total),
      bar_width_(bar_width),
      progress_(0),
      start_time_(steady_clock::now())
    {
    }

    ~ProgressBar() noexcept
    {
      if (progress_ < total_) {
        progress_ = total_;
        Draw();
      }
    }

    ProgressBar(const ProgressBar& rh) = delete;

    ProgressBar(ProgressBar&& rh) = default;

    ProgressBar& operator=(const ProgressBar& rh) = delete;

    ProgressBar& operator=(ProgressBar&& rh) = default;

    void operator++() noexcept
    {
      std::lock_guard<std::mutex> lock(mtx_);

      ++progress_;

      if (progress_ == 1) {
        std::cout
          << "Estimated time : "
          << DurationPrint((steady_clock::now() - start_time_) * total_)
          << std::endl;
        Draw();

      } else if (100 * (progress_ - 1) / total_ < 100 * progress_ / total_) {
        Draw();
      }
    }

    IntegralType get_total() const noexcept
    {
      return total_;
    }

    IntegralType get_progress() const noexcept
    {
      return progress_;
    }
  };
}



#endif // CXXSUPPORT_HH
