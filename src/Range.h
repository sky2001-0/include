#ifndef CXXRANGE_H
#define CXXRANGE_H

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <vector>



namespace csp
{
  template <typename NumberLike = std::size_t>
  class Range
  {
    static_assert(
      std::is_arithmetic_v<NumberLike>,
      "Template parameter 'NumberLike' must be arithmeric."
    );


  public:
    class Iterator;


  private:
    const NumberLike front_;
    const NumberLike step_;
    const std::size_t size_;


  public:
    Range() = delete;

    explicit Range(const std::size_t size) noexcept
    : front_(0), step_(1), size_(size)
    {
    }

    Range(const NumberLike front, const NumberLike end) noexcept
    : front_(front), step_(1),
      size_(
        [front, end]() -> std::size_t {
          if (front <= end) {
            return (end - front) / NumberLike(1);
          } else {
            return 0;
          }
        }()
      )
    {
    }

    Range(
      const NumberLike& front,
      const NumberLike& step,
      const std::size_t& size
    ) noexcept
    : front_(front), step_(step), size_(size)
    {
    }

    Range(
      const NumberLike front,
      const NumberLike back_end,
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

    ~Range() = default;

    Range(const Range<NumberLike>& rh) = delete;

    Range(Range<NumberLike>&& rh)
    : front_(rh.front_), step_(rh.step_), size_(rh.size_)
    {
    }

    Range& operator=(const Range<NumberLike>& rh) = delete;

    Range& operator=(Range<NumberLike>&& rh) = delete;


    NumberLike front() const noexcept { return front_; }

    NumberLike back() const noexcept { return (*this)[size_ - 1]; }

    std::size_t size() const noexcept { return size_; }

    NumberLike step() const noexcept { return step_; }

    Iterator begin() const noexcept { return Iterator(*this, 0); }

    Iterator end() const noexcept { return Iterator(*this, size_); }

    NumberLike operator[](const std::size_t index) const noexcept
    {
      return front_ + index * step_;
    }

    NumberLike at(const std::size_t index) const
    {
      if (index >= size_) {
        throw std::range_error("Range range error.");
      }
      return (*this)[index];
    }

    NumberLike i_to_x(const NumberLike i) const noexcept
    {
      return front_ + i * step_;
    }

    NumberLike x_to_i(const NumberLike x) const noexcept
    {
      return (x - front_) / step_;
    }

    bool in_range(const NumberLike x) const noexcept
    {
      const NumberLike i = x_to_i(x);
      return 0 <= i && i <= size_ - 1;
    }

    std::vector<NumberLike> ToVector() const noexcept
    {
      std::vector<NumberLike> vec(size_);
      for (std::size_t i = 0; i < size_; ++i) {
        vec[i] = (*this)[i];
      }
      return vec;
    }


    class Iterator
    {
    private:
      const Range<NumberLike>* obj_;
      std::size_t index_;


    public:
      Iterator() = delete;

      Iterator(
        const Range<NumberLike>& obj, const std::size_t index
      ) noexcept
      : obj_(&obj), index_(index)
      {}

      ~Iterator() = default;

      Iterator(const Iterator& rh) = default;

      Iterator(Iterator&& rh) = default;

      Iterator& operator=(const Iterator& rh) = default;

      Iterator& operator=(Iterator&& rh) = default;


      std::size_t operator()() const noexcept { return index_; }

      NumberLike operator*() const noexcept { return (*obj_)[index_]; }

      Iterator& operator++() noexcept
      {
        ++index_;
        return *this;
      }

      Iterator operator++(int) noexcept
      {
        Iterator old(*this);
        ++*this;
        return old;
      }

      bool operator!=(const Iterator& rh) const noexcept
      {
        return index_ != rh.index_;
      }
    };
  };
}



#endif // CXXRANGE_H
