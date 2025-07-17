#ifndef CXXTIME_H
#define CXXTIME_H

#include <atomic>
#include <chrono>
#include <cmath>
#include <concepts>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <type_traits>



namespace csp::time
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


  template <std::integral T>
  class ProgressBar
  {
    using steady_clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<steady_clock>;


  private:
    const T total_;
    const T step_;
    const int bar_width_;

    std::atomic<T> progress_;

    const time_point start_time_;

    std::mutex io_mutex_;


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

    void Draw(const T progress) const noexcept
    {
      std::lock_guard<std::mutex> lock(io_mutex_);

      std::stringstream ss;
      {
        const int progress_width = bar_width_ * progress / total_;

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
         << 100 * progress / total_
         << "% ";

      ss << DurationPrint(start_time_);

      if (progress == total_) {
        std::cout << ss.str() << std::endl;
      } else {
        std::cout << ss.str() << "\r";
        std::cout.flush();
      }
    }


  public:
    ProgressBar(const T total, const int bar_width = 70) noexcept
    : total_(total),
      step_(std::max<T>(1, total_ / 100)),
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

    ProgressBar(ProgressBar&& rh) = delete;

    ProgressBar& operator=(const ProgressBar& rh) = delete;

    ProgressBar& operator=(ProgressBar&& rh) = delete;


    void operator++() noexcept
    {
      T progress = progress_.fetch_add(1) + 1;

      if (progress == 1) {
        {
          std::lock_guard<std::mutex> lock(io_mutex_);
          std::cout
            << "Estimated time : "
            << DurationPrint((steady_clock::now() - start_time_) * total_)
            << std::endl;
        }
        Draw(progress);

      } else if (progress % step_ == 0 || progress == total_) {
        Draw(progress);
      }
    }

    T get_total() const noexcept
    {
      return total_;
    }

    T get_progress() const noexcept
    {
      return progress_.load();
    }
  };
}



#endif // CXXTIME_H
