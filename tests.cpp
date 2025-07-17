#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>

#include <Color.h>
#include <Vector3.h>



void TestVector3()
{
  using V = csp::math::Vector3<double>;
  const double eps = 1e-6;

  assert(std::abs(V{1, 0, 0}.dot(V{0, 1, 0}.cross(V{0, 0, 1})) - 1.0) < eps);

  {
    V a{1, 2, 3}, b{4, 5, 6}, c{7, 8, 9};
    assert((a.cross(b.cross(c)) - (b * a.dot(c) - c * a.dot(b))).norm() < eps);
  }

  assert((V{1, 2, 3}.cross(V{2, 4, 6})).norm() < eps);

  assert((V{3, 0, 0}.normalize() * 3.0 - V{3, 0, 0}).norm() < eps);

  {
    V v = V{1, 2, 2}.normalize();
    assert(std::abs(v.dot(v) - 1.0) < eps);
  }

  {
    const V v{7.0, 8.3, 9.0};
    const auto [x, y, z] = v;
    assert(x == 7.0 && y == 8.3 && z == 9.0);
  }

  {
    V a{1, 2, 3}, b{1, 2, 3}, c{3, 2, 1};
    assert(a == b);
    assert(a != c);
  }

  {
    constexpr V a{1.0, 2.0, 3.0}, b{4.0, 5.0, 6.0};
    constexpr auto r = a + b;
    static_assert(r.x_ == 5.0 && r.y_ == 7.0 && r.z_ == 9.0);
  }
}


void TestColor()
{
  using csp::utils::Color;

  {
    Color c1(static_cast<unsigned char>(128), 64, 192);
    auto hsv = c1.to_hsv();
    Color c2(hsv);
    auto [r2, g2, b2] = c2.to_rgb();
    auto [r1, g1, b1] = c1.to_rgb();

    // 色空間変換後にある程度の誤差内で一致
    assert(std::abs(r1 - r2) <= 1);
    assert(std::abs(g1 - g2) <= 1);
    assert(std::abs(b1 - b2) <= 1);
  }

  {
    Color c(0xFFAA33);
    assert(c.to_hex() == 0xFFAA33);
  }

  {
    Color c(300.f, 0.5f, 0.8f);
    auto [h, s, v] = c.to_hsv();
    assert(std::abs(h - 300.f) < 1e-2);
    assert(std::abs(s - 0.5f) < 1e-2);
    assert(std::abs(v - 0.8f) < 1e-2);
  }

  {
    Color base(static_cast<unsigned char>(240), 100, 0);
    Color c1 = base.GenerateColor(1);
    Color c2 = base.GenerateColor(2);
    assert(c1.to_hex() != c2.to_hex());

    assert(c1.to_hex() == base.GenerateColor(1).to_hex());
  }

  {
    Color c(static_cast<unsigned char>(42), 128, 255);
    auto rgb = c.to_rgb();
    assert(rgb == std::make_tuple(42, 128, 255));
  }

  {
    Color a(static_cast<unsigned char>(12), 34, 56);
    Color b;
    b = a;
    assert(b.to_hex() == a.to_hex());
  }
}



int main()
{
  TestVector3();
  std::cout << "✅ All Vector3 tests passed." << std::endl;

  TestColor();
  std::cout << "✅ All Color tests passed." << std::endl;

  return 0;
}
