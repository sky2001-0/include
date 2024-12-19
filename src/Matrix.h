#ifndef CXXMATRIX_H
#define CXXMATRIX_H

#include <algorithm>
#include <array>
#include <complex>
#include <cstddef>
#include <functional>
#include <type_traits>



namespace csp
{
  template <typename Elem, std::size_t row_size_, std::size_t col_size_>
  class Matrix
  {
    static_assert(
      std::is_arithmetic_v<Elem> || std::is_same_v<Elem, std::complex<double>>,
      "Template parameter 'Elem' must be an arithmetic type or std::complex<double>."
    );


    using Mat = Matrix<Elem, row_size_, col_size_>;


  private:
    std::array<Elem, row_size_ * col_size_> arr_;

    static void Assert_if_not_regular() noexcept
    {
      static_assert(row_size_ == col_size_, "Matrix is not regular.");
    }


  public:
    Matrix()
    : Matrix(0)
    {
    }

    explicit Matrix(const Elem elem)
    {
      arr_.fill(elem);
    }

    ~Matrix() = default;

    Matrix(const Mat& rh) = default;

    Matrix(Mat&& rh) = default;

    Matrix& operator=(const Mat& rh) = default;

    Matrix& operator=(Mat&& rh) = default;


    Elem& get(const std::size_t i)
    {
      return arr_.at(i);
    }

    Elem cget(const std::size_t i) const
    {
      return arr_.at(i);
    }

    Elem& get(const std::size_t row, const std::size_t col)
    {
      return arr_.at(row * col_size_ + col);
    }

    Elem cget(const std::size_t row, const std::size_t col) const
    {
      return arr_.at(row * col_size_ + col);
    }

    Elem& getf(const std::size_t i) noexcept
    {
      return arr_[i];
    }

    Elem cgetf(const std::size_t i) const noexcept
    {
      return arr_[i];
    }

    Elem& getf(const std::size_t row, const std::size_t col) noexcept
    {
      return arr_[row * col_size_ + col];
    }

    Elem cgetf(
      const std::size_t row, const std::size_t col
    ) const noexcept
    {
      return arr_[row * col_size_ + col];
    }


    void operator+=(const Elem rh) noexcept
    {
      Assert_if_not_regular();

      for (std::size_t i = 0.; i < row_size_; ++i) {
        getf(i, i) += rh;
      }
    }

    Mat operator+(const Elem rh) const noexcept
    {
      Assert_if_not_regular();

      Mat result = *this;
      result += rh;
      return result;
    }

    void operator+=(const Mat rh) noexcept
    {
      Mat result;

      std::transform(
        arr_.begin(), arr_.end(), rh.arr_.begin(), arr_.begin(), std::plus<>()
      );
    }

    Mat operator+(const Mat rh) const noexcept
    {
      Mat result;

      std::transform(
        arr_.begin(), arr_.end(), rh.arr_.begin(), result.arr_.begin(),
        std::plus<>()
      );
      return result;
    }

    void operator-=(const Elem rh) noexcept
    {
      Assert_if_not_regular();

      for (std::size_t i = 0.; i < row_size_; ++i) {
        getf(i, i) -= rh;
      }
    }

    Mat operator-(const Elem rh) const noexcept
    {
      Assert_if_not_regular();

      Mat result = *this;
      result -= rh;
      return result;
    }

    void operator-=(const Mat rh) noexcept
    {
      Mat result;

      std::transform(
        arr_.begin(), arr_.end(), rh.arr_.begin(), arr_.begin(), std::minus<>()
      );
    }

    Mat operator-(const Mat rh) const noexcept
    {
      Mat result;

      std::transform(
        arr_.begin(), arr_.end(), rh.arr_.begin(), result.arr_.begin(),
        std::minus<>()
      );
      return result;
    }

    void operator*=(const Elem rh) noexcept
    {
      std::transform(
        arr_.begin(), arr_.end(), arr_.begin(),
        [rh](const Elem& elem) {
          return elem * rh;
        }
      );
    }

    Mat operator*(const Elem rh) const noexcept
    {
      Mat result;

      std::transform(
        arr_.begin(), arr_.end(), result.arr_.begin(),
        [rh](const Elem& elem) {
          return elem * rh;
        }
      );
      return result;
    }

    template <std::size_t col_size2>
    Matrix<Elem, row_size_, col_size2> operator*(
      const Matrix<Elem, col_size_, col_size2> rh
    ) const noexcept
    {
      Matrix<Elem, row_size_, col_size2> result;

      for (std::size_t row = 0; row < row_size_; ++row) {
        for (std::size_t col = 0; col < col_size2; ++col) {
          Elem val = 0.;
          for (std::size_t tmp = 0; tmp < col_size_; ++tmp) {
            val += cgetf(row, tmp) * rh.cgetf(tmp, col);
          }
          result.getf(row, col) = val;
        }
      }
      return result;
    }


    Elem trace() const noexcept
    {
      Assert_if_not_regular();

      Elem result;
      for (std::size_t i = 0; i < row_size_; ++i) {
        result += cgetf(i, i);
      }
      return result;
    }

    Mat commute(
      const Mat& rh
    ) const noexcept
    {
      Assert_if_not_regular();

      Mat result = (*this) * rh;
      Mat tmp = rh * (*this);
      result -= tmp;
      return result;
    }
  };
}



#endif // CXXMATRIX_H
