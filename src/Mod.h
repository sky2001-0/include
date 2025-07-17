#ifndef CXXMOD_H
#define CXXMOD_H

#include <cmath>
#include <vector>



namespace csp::math
{
  class ModInverse
  {
  // Class for computing modular inverses and combinations modulo a prime.
  // Precondition: `mod_` must be a positive prime number
  private:
    const int mod_;
    const int upper_;
    std::vector<int> invs_;

    void Initialize() noexcept
    {
      invs_[0] = 0;
      invs_[1] = 1;
      for (int i = 2; i <= upper_; ++i) {
        invs_[i] = mod_ - static_cast<int>(
          static_cast<long long>(mod_ / i) * invs_[mod_ % i] % mod_
        );
      }
    }


  public:
    ModInverse() = delete;

    ModInverse(const int mod, int upper = 0) noexcept
    : mod_(mod), upper_(upper == 0 ? mod_ / 2 : upper), invs_(upper_ + 1)
    {
      Initialize();
    }

    ~ModInverse() = default;

    ModInverse(const ModInverse& rh) = delete;

    ModInverse(ModInverse&& rh) = delete;

    ModInverse& operator=(const ModInverse& rh) = delete;

    ModInverse& operator=(ModInverse&& rh) = delete;


    int cget_mod() const noexcept
    {
      return mod_;
    }


    int Inv(const int i) const noexcept
    {
      if (i <= upper_) {
        return invs_[i];
      } else if (mod_ - i <= upper_){
        return mod_ - invs_[mod_ - i];
      }

      return 0;
    }

    long long Comb(const int n, int k)
    // Precondition: 0 <= k <= n, and both k and n-k must be <= upper_
    {
      if (k < 0 || k > n) {
        return 0;
      }

      k = std::min(k, n - k);
      long long ans = 1LL;

      for (int i = 1; i <= k; ++i) {
        ans = ans * (n + 1 - i) % mod_ * invs_[i] % mod_;
      }

      return ans;
    }
  };
}



#endif // CXXMOD_H