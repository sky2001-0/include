#ifndef CXXWAVEOBJ_H
#define CXXWAVEOBJ_H

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <functional>
#include <numeric>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "Range.hh"



namespace csp
{
  template <typename Elem, std::size_t size_>
  class WaveObj
  {
    static_assert(
      std::is_floating_v<Elem>,
      "Template parameter 'Elem' must be an floating type."
    );
    static_assert(
      size_ > 1,
      "Template parameter 'size_' must be greater than 1."
    );


    using Wave = WaveObj<Elem, size_>;


  protected:
    const Range<double>* xs_;
    std::array<Elem, size_> y_arr_;


  public:
    WaveObj() = delete;

    explicit WaveObj(const Range<double>& xs)
    : xs_(&xs)
    {
      if (xs->size() != size_) {
        std::invalid_argument("size error of x_range");
      }
      y_arr_.fill(Elem(0.));
    }

    WaveObj(
      const Range<double>& xs,
      const std::array<Elem, size_>& y_arr
    )
    : xs_(&xs), y_arr_(y_arr)
    {
      if (xs->size() != size_) {
        std::invalid_argument("size error of x_range");
      }
    }

    WaveObj(
      const Range<double>& xs,
      std::array<Elem, size_>&& y_arr
    )
    : xs_(&xs), y_arr_(std::move(y_arr))
    {
      if (xs->size() != size_) {
        std::invalid_argument("size error of x_range");
      }
    }

    ~WaveObj() = default;

    WaveObj(const Wave& rh) = default;

    WaveObj& operator=(const Wave& rh) = default;

    WaveObj(Wave&& rh) = default;

    WaveObj& operator=(Wave&& rh) = default;


    virtual Elem& operator[](const std::size_t index) noexcept final
    {
      return y_arr_[index];
    }

    virtual Elem& at(const std::size_t index) final
    {
      return y_arr_.at(index);
    }

    virtual Elem* data() noexcept final
    {
      return y_arr_.data();
    }

    virtual Elem x_to_y(const double x) noexcept final
    {
      const double i = xs_->x_to_i(x);

      double ratio;
      std::size_t left;
      if (i <= 0.) {
        ratio = 1 - i;
        left = 0;
      } else if (size_ - 1 <= i) {
        ratio = size_ - 1 - i;
        left = size_ - 2;
      } else {
        ratio = std::ceil(i) - i;
        left = static_cast<std::size_t>(std::floor(i));
        if (ratio == 0.) {
          return y_arr_[left];
        }
      }
      return ratio * y_arr_[left] + (1. - ratio) * y_arr_[left + 1];
    }


    virtual void operator+=(const Elem rh) noexcept final
    {
      std::for_each(y_arr_.begin(), y_arr_.end(), [&rh](Elem& y) { y += rh; });
    }

    virtual void operator*=(const Elem rh) noexcept
    {
      std::for_each(y_arr_.begin(), y_arr_.end(), [&rh](Elem& y) { y *= rh; });
    }

    virtual void Add(const Wave& rh, const Elem factor = 1.) final
    {
      const double gap = (rh.xs_->front() - xs_->front()) / xs_->step();
      if (std::abs(gap) > 1.) {
        throw std::range_error(
          "The x-ranges of two WaveObj are too far apart. Difference : "
          + std::to_string(rh.xs_->front() - xs_->front())
        );
      }

      for (std::size_t index = 0; index < size_; ++index) {
        y_arr_[index] += factor * rh.x_to_y((*xs_)[index]);
      }
    }


    virtual Elem Max(
      const Elem x_start, const Elem x_end, const bool is_max = true
    ) const final
    {
      const Elem i_start = xs_->x_to_i(x_start);
      const Elem i_end = xs_->x_to_i(x_end);
      if (i_end < i_start) {
        throw std::invalid_argument("x_start must be less than x_end.");
      }

      const auto i_start_ceil = static_cast<std::size_t>(std::ceil(i_start));
      const auto i_end_ceil = static_cast<std::size_t>(std::ceil(i_end));

      if (i_start_ceil == i_end_ceil) {
        if (is_max) {
          return std::max(i_to_y(i_start), i_to_y(i_end));

        } else {
          return std::min(i_to_y(i_start), i_to_y(i_end));

        }
      } else {
        if (is_max) {
          const Elem tmp = *std::max_element(
            y_arr_.begin() + i_start_ceil, y_arr_.begin() + i_end_ceil
          );
          return std::max({i_to_y(i_start), i_to_y(i_end), tmp});

        } else {
          const Elem tmp = *std::min_element(
            y_arr_.begin() + i_start_ceil, y_arr_.begin() + i_end_ceil
          );
          return std::min({i_to_y(i_start), i_to_y(i_end), tmp});

        }
      }
    }

    virtual Elem Min(const Elem x_start, const Elem x_end) const final
    {
      return Max(x_start, x_end, false);
    }

    virtual Elem Integral(const Elem x_start, const Elem x_end) const final
    {
      const Elem i_start = xs_->x_to_i(x_start);
      const Elem i_end = xs_->x_to_i(x_end);
      if (i_end < i_start) {
        return 0.;
      }

      const auto i_start_ceil = static_cast<std::size_t>(std::ceil(i_start));
      const auto i_end_floor = static_cast<std::size_t>(std::floor(i_end));

      Elem result = (
        (
          (i_to_y(i_start) + y_arr_[i_start_ceil])
          * (std::ceil(i_start) - i_start) / 2.
        )
        - y_arr_[i_start_ceil] / 2.
        + (
          (y_arr_[i_end_floor] + i_to_y(i_end))
          * (i_end - std::floor(i_end)) / 2.
        )
        - y_arr_[i_end_floor] / 2.
      );
      for (std::size_t index = i_start_ceil; index <= i_end_floor; ++index) {
        result += y_arr_[index];
      }

      return result * xs_->step();
    }

    virtual std::vector<PeakObj<Elem>> SearchPeak(
      const Elem threshold, double x_skip
    ) const noexcept final
    {
      if (x_skip < 0.) {
        x_skip = 0.;
      }

      std::vector<PeakObj<Elem>> pobj_vec;

      bool was_peak = false;
      double x_start = -x_skip;
      std::size_t i_peak = 0;
      Elem peak_value = 0.;

      for (std::size_t index = 0; index < size_; ++index) {
        const Elem y = y_arr_[index];

        if (threshold < y) {
          if (!was_peak) {
            if (index == 0) {
              x_start = xs_->front();
            } else {
              x_start = (
                (*xs_)[index]
                - (y - threshold) / (y - y_arr_[index - 1]) * xs->step()
              );
            }
            was_peak = true;
          }

          if (peak_value < y) {
            i_peak = index;
            peak_value = y;
          }

        } else {
          if ((*xs_)[index] < x_start + x_skip) {
            continue;
          }

          if (was_peak) {
            const double x_end = (
              (*xs_)[index]
              - (threshold - y) / (y_arr_[index - 1] - y) * xs->step()
            );
            pobj_vec.emplace_back(x_start, (*xs_)[i_peak], x_end, peak_value);
            peak_value = 0.;
            was_peak = false;
          }
        }
      }
      return std::move(pobj_vec);
    }


    struct PeakObj
    {
    public:
      const double x_start_;
      const double x_peak_;
      const double x_end_;
      const Elem peak_value_;


      PeakObj() = delete;

      PeakObj(
        const double x_start,
        const double x_peak,
        const double x_end,
        const Elem peak_value
      )
      : x_start_(x_start),
        x_peak_(x_peak),
        x_end_(x_end),
        peak_value_(peak_value)
      {
      }
    };
  };



  template <typename Elem, std::size_t size_>
  class WaveObjErrors final : public WaveObj<Elem, size_>
  {
    using Wave = WaveObj<Elem, size_>;
    using WaveE = WaveObjErrors<Elem, size_>;


  private:
    std::array<Elem, size_> y_err_arr_;


  public:
    WaveObjErrors() = delete;

    explicit WaveObj(const Range<double>& xs)
    : Wave(xs)
    {
      y_err_array_.fill(Elem(0));
    }

    explicit WaveObjErrors(
      const Range<double>& xs,
      const std::array<Elem, size_>& y_arr
    )
    : Wave(xs, y_arr)
    {
      y_err_array_.fill(Elem(0));
    }

    explicit WaveObjErrors(
      const Range<double>& xs,
      std::array<Elem, size_>&& y_arr
    )
    : Wave(xs, std::move(y_arr))
    {
      y_err_array_.fill(Elem(0));
    }

    explicit WaveObjErrors(
      const Range<double>& xs,
      const std::array<Elem, size_>& y_arr,
      const std::array<Elem, size_>& y_err_arr
    )
    : Wave(xs, y_arr), y_err_arr_(y_err_arr)
    {
    }

    explicit WaveObjErrors(
      const Range<double>& xs,
      const std::array<Elem, size_>& y_arr,
      const std::array<Elem, size_>& y_err_arr
    )
    : Wave(xs, std::move(y_arr)), y_err_arr_(std::move(y_err_arr))
    {
    }

    WaveObjErrors(const WaveE& rh) = default;

    WaveObjErrors& operator=(const WaveE& rh) = default;

    WaveObjErrors(WaveE&& rh) = default;

    WaveObjErrors& operator=(WaveE&& rh) = default;

    ~WaveObjErrors() = default;


    Elem& get_err(const std::size_t index) const noexcept
    {
      return y_err_array_[index];
    }

    Elem& get_err_at(const std::size_t index) const
    {
      return y_err_array_.at(index);
    }

    Elem* data_err() noexcept
    {
      return y_err_array_.data();
    }

    Elem x_to_y_err(const double x) noexcept
    {
      const double i = xs_->x_to_i(x);

      double ratio;
      std::size_t left;
      if (i <= 0.) {
        ratio = 1 - i;
        left = 0;
      } else if (size_ - 1 <= i) {
        ratio = size_ - 1 - i;
        left = size_ - 2;
      } else {
        ratio = std::ceil(i) - i;
        left = static_cast<std::size_t>(std::floor(i));
        if (ratio == 0.) {
          return y_err_arr_[left];
        }
      }
      return ratio * y_err_arr_[left] + (1. - ratio) * y_err_arr_[left + 1];
    }


    void operator*=(const Elem rh) noexcept override
    {
      std::for_each(
        this->y_arr_.begin(), this->y_arr_.end(),
        [&rh](Elem& y) { y *= rh; }
      );
      std::for_each(
        y_err_arr_.begin(), y_err_arr_.end(),
        [&rh](Elem& y) { y *= rh; }
      );
    }

    void Add(const WaveE& rh, const Elem factor=1.)
    {
      const double gap = (
        (rh.xs_->front() - this->xs_->front())
        / this->xs_->step()
      );
      if (std::abs(gap) > 1.) {
        throw std::range_error(
          "The x-ranges of two WaveObj are too far apart. Difference : "
          + std::to_string(rh.xs_->front() - this->xs_->front())
        );
      }

      for (std::size_t index = 0; index < size_; ++index) {
        this->y_arr_[index] += factor * rh.x_to_y((*xs_)[index]);
        this->y_err_arr_[index] = std::sqrt(
          std::pow(y_err_arr_[index], 2)
          + std::pow(factor * rh.x_to_y_err((*xs_)[index]), 2)
        );
      }
    }
  };


  template <typename Elem, std::size_t size_>
  class WaveObjAccumulator
  {
    using WaveA = WaveObjAccumulator<Elem, size_>;


  private:
    const Range<double>* xs_;
    std::size_t num_;
    std::array<Elem, size_> sum_arr_;
    std::array<Elem, size_> pow2_arr_;


  public:
    WaveObjAccumulator() = delete;

    explicit WaveObjAccumulator(const Range<double>& xs)
    : xs_(&xs), num_(0)
    {
      sum_arr_.fill(Elem(0));
      pow2_arr_.fill(Elem(0));
    }

    ~WaveObjAccumulator() = default;

    WaveObjAccumulator(const WaveObjAccumulator& rh) = delete;

    WaveObjAccumulator& operator=(const WaveA& rh) = delete;

    WaveObjAccumulator(WaveA&& rh) = default;

    WaveObjAccumulator& operator=(WaveA&& rh) = default;


    template <typename Elem2>
    void Push(
      const WaveObj<Elem2, size_>& wobj,
      const std::function<
        bool(const std::size_t index, const Elem2 value)
      >& whether_pushed
    ) noexcept
    {
      for (std::size_t index = 0; index < size_; ++index) {
        const Elem value = static_cast<Elem>(rh.x_to_y((*xs_)[index]));
        if (whether_pushed(index, value)) {
          sum_arr_[index] += value;
          pow2_arr_[index] += std::pow(value, 2);
        }
      }
      num_ += 1;
    }

    WaveObjErrors<Elem, size_> Write() const noexcept
    {
      const Elem num = static_cast<Elem>(num_);
      std::array<Elem, size_> y_arr;
      std::array<Elem, size_> y_err_arr;
      for (std::size_t index = 0; index < size_; ++index) {
        y_arr[index] = sum_arr_[index] / num;
        y_err_arr[index] = std::sqrt(
          (pow2_arr_[index] - num * std::pow(y_arr[index], 2))
          / (num - Elem(1))
        );
      }
      return {xs_, std::move(y_arr), std::move(y_err_arr)};
    }
  };
}



#endif // CXXWAVEOBJ_H
