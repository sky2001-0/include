#ifndef CXXMATRIX_HH
#define CXXMATRIX_HH

#include <algorithm>
#include <array>
#include <complex>
#include <type_traits>



namespace csp
{
  namespace math
  {
    template <typename ElemType, std::size_t row_size_, std::size_t col_size_>
    class Matrix final
    {
      static_assert(
        (
          std::is_arithmetic_v<ElemType>
          || std::is_same_v<ElemType, std::complex<double>>
        ),
        "Template parameter 'ElemType' must be an arithmetic type or std::complex<double>."
      );

      static void Assert_if_not_regular() noexcept
      {
        static_assert(
          row_size_ == col_size_,
          "Matrix is not regular."
        );
      }

    private:
      std::array<ElemType, row_size_ * col_size_> arr_;


    public:
      Matrix() = default;

      explicit Matrix(const ElemType elem)
      {
        arr_.fill(elem);
      }

      ~Matrix() = default;

      Matrix(const Matrix<ElemType, row_size_, col_size_>& rh) = default;

      Matrix(Matrix<ElemType, row_size_, col_size_>&& rh) = default;

      Matrix& operator=(
        const Matrix<ElemType, row_size_, col_size_>& rh
      ) = default;

      Matrix& operator=(
        Matrix<ElemType, row_size_, col_size_>&& rh
      ) = default;


      ElemType& get(const std::size_t i)
      {
        return arr_.at(i);
      }

      ElemType cget(const std::size_t i) const
      {
        return arr_.at(i);
      }

      ElemType& get(const std::size_t row, const std::size_t col)
      {
        return arr_.at(row * col_size_ + col);
      }

      ElemType cget(const std::size_t row, const std::size_t col) const
      {
        return arr_.at(row * col_size_ + col);
      }

      ElemType& getf(const std::size_t i) noexcept
      {
        return arr_[i];
      }

      ElemType cgetf(const std::size_t i) const noexcept
      {
        return arr_[i];
      }

      ElemType& getf(const std::size_t row, const std::size_t col) noexcept
      {
        return arr_[row * col_size_ + col];
      }

      ElemType cgetf(
        const std::size_t row, const std::size_t col
      ) const noexcept
      {
        return arr_[row * col_size_ + col];
      }


      void operator+=(const ElemType rh) noexcept
      {
        Assert_if_not_regular();

        for (std::size_t i = 0.; i < row_size_; ++i) {
          getf(i, i) += rh;
        }
      }

      Matrix<ElemType, row_size_, col_size_> operator+(
        const ElemType rh
      ) const noexcept
      {
        Assert_if_not_regular();

        Matrix<ElemType, row_size_, col_size_> result = *this;
        result += rh;
        return result;
      }

      void operator+=(
        const Matrix<ElemType, row_size_, col_size_> rh
      ) noexcept
      {
        Matrix<ElemType, row_size_, col_size_> result;

        std::transform(
          arr_.begin(), arr_.end(), rh.arr_.begin(), arr_.begin(),
          [](const ElemType& elem1, const ElemType& elem2) {
            return elem1 + elem2;
          }
        );
      }

      Matrix<ElemType, row_size_, col_size_> operator+(
        const Matrix<ElemType, row_size_, col_size_> rh
      ) const noexcept
      {
        Matrix<ElemType, row_size_, col_size_> result;

        std::transform(
          arr_.begin(), arr_.end(), rh.arr_.begin(), result.arr_.begin(),
          [](const ElemType& elem1, const ElemType& elem2) {
            return elem1 + elem2;
          }
        );
        return result;
      }

      void operator-=(const ElemType rh) noexcept
      {
        Assert_if_not_regular();

        for (std::size_t i = 0.; i < row_size_; ++i) {
          getf(i, i) -= rh;
        }
      }

      Matrix<ElemType, row_size_, col_size_> operator-(
        const ElemType rh
      ) const noexcept
      {
        Assert_if_not_regular();

        Matrix<ElemType, row_size_, col_size_> result = *this;
        result -= rh;
        return result;
      }

      void operator-=(
        const Matrix<ElemType, row_size_, col_size_> rh
      ) noexcept
      {
        Matrix<ElemType, row_size_, col_size_> result;

        std::transform(
          arr_.begin(), arr_.end(), rh.arr_.begin(), arr_.begin(),
          [](const ElemType& elem1, const ElemType& elem2) {
            return elem1 - elem2;
          }
        );
      }

      Matrix<ElemType, row_size_, col_size_> operator-(
        const Matrix<ElemType, row_size_, col_size_> rh
      ) const noexcept
      {
        Matrix<ElemType, row_size_, col_size_> result;

        std::transform(
          arr_.begin(), arr_.end(), rh.arr_.begin(), result.arr_.begin(),
          [](const ElemType& elem1, const ElemType& elem2) {
            return elem1 - elem2;
          }
        );
        return result;
      }

      void operator*=(const ElemType rh) noexcept
      {
        std::transform(
          arr_.begin(), arr_.end(), arr_.begin(),
          [rh](const ElemType& elem) {
            return elem * rh;
          }
        );
      }

      Matrix<ElemType, row_size_, col_size_> operator*(
        const ElemType rh
      ) const noexcept
      {
        Matrix<ElemType, row_size_, col_size_> result;

        std::transform(
          arr_.begin(), arr_.end(), result.arr_.begin(),
          [rh](const ElemType& elem) {
            return elem * rh;
          }
        );
        return result;
      }

      template <std::size_t col_size2>
      Matrix<ElemType, row_size_, col_size2> operator*(
        const Matrix<ElemType, col_size_, col_size2> rh
      ) const noexcept
      {
        Matrix<ElemType, row_size_, col_size2> result;

        for (std::size_t row = 0; row < row_size_; ++row) {
          for (std::size_t col = 0; col < col_size2; ++col) {
            ElemType val = 0.;
            for (std::size_t tmp = 0; tmp < col_size_; ++tmp) {
              val += cgetf(row, tmp) * rh.cgetf(tmp, col);
            }
            result.getf(row, col) = val;
          }
        }
        return result;
      }


      ElemType trace() const noexcept
      {
        Assert_if_not_regular();

        ElemType result;
        for (std::size_t i = 0; i < row_size_; ++i) {
          result += cgetf(i, i);
        }
        return result;
      }

      Matrix<ElemType, row_size_, col_size_> commute(
        const Matrix<ElemType, row_size_, col_size_>& rh
      ) const noexcept
      {
        Assert_if_not_regular();

        Matrix<ElemType, row_size_, col_size_> result = (*this) * rh;
        Matrix<ElemType, row_size_, col_size_> tmp = rh * (*this);
        result -= tmp;
        return result;
      }
    };
  }
}



#endif // CXXMATRIX_HH
