#ifndef CXXPROGRESSBAR_H
#define CXXPROGRESSBAR_H

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <type_traits>



namespace csp
{
  template<typename Integral>
  class ProgressBar
  {
    static_assert(
      std::is_integral_v<Integral>,
      "Template parameter Integral must be integral type."
    );


    using steady_clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<steady_clock>;


  private:
    const Integral total_;
    const int bar_width_;

    Integral progress_;

    const time_point start_time_;

    std::mutex mtx_;


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

    static std::string DurationPrint(const time_point& time) noexcept
    {
      return DurationPrint(steady_clock::now() - time);
    }

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
    ProgressBar(const Integral total, const int bar_width = 70) noexcept
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

    ProgressBar(ProgressBar&& rh)
    : total_(rh.total_),
      bar_width_(rh.bar_width_),
      progress_(rh.progress_),
      start_time_(std::move(rh.start_time_)),
      mtx_(std::move(rh.mtx_))
    {
    }

    ProgressBar& operator=(const ProgressBar& rh) = delete;

    ProgressBar& operator=(ProgressBar&& rh) = delete;


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

    Integral get_total() const noexcept
    {
      return total_;
    }

    Integral get_progress() const noexcept
    {
      return progress_;
    }
  };
}



#endif // CXXPROGRESSBAR_H
