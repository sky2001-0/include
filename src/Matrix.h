#ifndef CXXMATRIX_H
#define CXXMATRIX_H

#include <algorithm>
#include <array>
#include <complex>
#include <concepts>
#include <cstddef>
#include <functional>
#include <ranges>
#include <type_traits>



namespace csp::math
{
  template <typename Elem, std::size_t kRow, std::size_t kCol>
    requires (
      std::is_arithmetic_v<Elem> || std::is_same_v<Elem, std::complex<double>>
    )
  class Matrix
  {
    using Mat = Matrix<Elem, kRow, kCol>;


  private:
    std::array<Elem, kRow * kCol> arr_;


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
      return arr_.at(row * kCol + col);
    }

    Elem cget(const std::size_t row, const std::size_t col) const
    {
      return arr_.at(row * kCol + col);
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
      return arr_[row * kCol + col];
    }

    Elem cgetf(const std::size_t row, const std::size_t col) const noexcept
    {
      return arr_[row * kCol + col];
    }


    Mat operator-() const noexcept
    {
      Mat mat = *this;
      mat *= Elem(-1);
      return mat;
    }

    void operator+=(const Elem rh) noexcept
      requires (kRow == kCol)
    {
      for (std::size_t i = 0; i < kRow; ++i) {
        getf(i, i) += rh;
      }
    }

    Mat operator+(const Elem rh) const noexcept
      requires (kRow == kCol)
    {
      Mat mat = *this;
      mat += rh;
      return mat;
    }

    template <typename Elem2, std::size_t kRow2, std::size_t kCol2>
      requires (kRow2 == kCol2)
    friend constexpr Matrix<Elem2, kRow2, kCol2> operator+(
      const Elem2 lh, const Matrix<Elem2, kRow2, kCol2>& rh
    ) noexcept
    {
      return rh + lh;
    }

    void operator+=(const Mat& rh) noexcept
    {
      std::ranges::transform(arr_, rh.arr_, arr_.begin(), std::plus<>());
    }

    Mat operator+(const Mat& rh) const noexcept
    {
      Mat mat;
      std::ranges::transform(arr_, rh.arr_, mat.arr_.begin(), std::plus<>());
      return mat;
    }

    void operator-=(const Elem rh) noexcept
      requires (kRow == kCol)
    {
      for (std::size_t i = 0; i < kRow; ++i) {
        getf(i, i) -= rh;
      }
    }

    Mat operator-(const Elem rh) const noexcept
      requires (kRow == kCol)
    {
      Mat mat = *this;
      mat -= rh;
      return mat;
    }

    template <typename Elem2, std::size_t kRow2, std::size_t kCol2>
      requires (kRow2 == kCol2)
    friend constexpr Matrix<Elem2, kRow2, kCol2> operator-(
      const Elem2 lh, const Matrix<Elem2, kRow2, kCol2>& rh
    ) noexcept
    {
      return -(rh - lh);
    }

    void operator-=(const Mat& rh) noexcept
    {
      std::ranges::transform(arr_, rh.arr_, arr_.begin(), std::minus<>());
    }

    Mat operator-(const Mat& rh) const noexcept
    {
      Mat mat;
      std::ranges::transform(arr_, rh.arr_, mat.arr_.begin(), std::minus<>());
      return mat;
    }

    void operator*=(const Elem rh) noexcept
    {
      std::ranges::for_each(arr_,
        [rh](Elem& elem) {
          elem *= rh;
        }
      );
    }

    Mat operator*(const Elem rh) const noexcept
    {
      Mat mat;
      std::ranges::transform(arr_, mat.arr_.begin(),
        [rh](const Elem& elem) {
          return elem * rh;
        }
      );
      return mat;
    }

    template <typename Elem2, std::size_t kRow2, std::size_t kCol2>
    friend constexpr Matrix<Elem2, kRow2, kCol2> operator*(
      const Elem2 lh, const Matrix<Elem2, kRow2, kCol2>& rh
    ) noexcept
    {
      return rh * lh;
    }

    template <std::size_t kCol2>
    Matrix<Elem, kRow, kCol2> operator*(
      const Matrix<Elem, kCol, kCol2>& rh
    ) const noexcept
    {
      Matrix<Elem, kRow, kCol2> result;

      for (std::size_t row = 0; row < kRow; ++row) {
        for (std::size_t col = 0; col < kCol2; ++col) {
          Elem val = 0.;
          for (std::size_t tmp = 0; tmp < kCol; ++tmp) {
            val += cgetf(row, tmp) * rh.cgetf(tmp, col);
          }
          result.getf(row, col) = val;
        }
      }
      return result;
    }


    Elem trace() const noexcept
      requires (kRow == kCol)
    {
      Elem result = 0;
      for (std::size_t i = 0; i < kRow; ++i) {
        result += cgetf(i, i);
      }
      return result;
    }

    Mat commute(const Mat& rh) const noexcept
      requires (kRow == kCol)
    {
      Mat result = (*this) * rh;
      Mat tmp = rh * (*this);
      result -= tmp;
      return result;
    }
  };
}



#endif // CXXMATRIX_H
