#ifndef PHYSCONSTANT_H
#define PHYSCONSTANT_H



namespace u
{
  inline constexpr double s  = 1.;
  inline constexpr double ms = 1e-3 * s;
  inline constexpr double us = 1e-6 * s;
  inline constexpr double ns = 1e-9 * s;
  inline constexpr double ps = 1e-12 * s;
  inline constexpr double fs = 1e-15 * s;

  inline constexpr double min = 60. * s;
  inline constexpr double hour = 3600. * s;

  inline constexpr double Hz  = 1. / s;
  inline constexpr double kHz = 1e+3 * Hz;
  inline constexpr double MHz = 1e+6 * Hz;
  inline constexpr double GHz = 1e+9 * Hz;
  inline constexpr double THz = 1e+12 * Hz;
  inline constexpr double PHz = 1e+15 * Hz;

  inline constexpr double m  = 1.;
  inline constexpr double cm = 1e-2 * m;
  inline constexpr double mm = 1e-3 * m;
  inline constexpr double um = 1e-6 * m;
  inline constexpr double nm = 1e-9 * m;
  inline constexpr double pm = 1e-12 * m;
  inline constexpr double km = 1e+3 * m;

  inline constexpr double kg = 1.;
  inline constexpr double g = 1e-3 * kg;
  inline constexpr double mg = 1e-6 * kg;

  inline constexpr double A = 1.;
  inline constexpr double mA = 1e-3 * A;

  inline constexpr double K = 1.;
  inline constexpr double mK = 1e-3 * K;

  inline constexpr double mol = 1.;

  inline constexpr double C = A * s;

  inline constexpr double J  = kg * m * m / s / s;
  inline constexpr double mJ = 1e-3 * J;
  inline constexpr double uJ = 1e-6 * J;

  inline constexpr double W  = J / s;
  inline constexpr double mW = 1e-3 * W;
  inline constexpr double kW = 1e+3 * W;

  inline constexpr double V = J / C;
  inline constexpr double F = C * C / J;

  inline constexpr double pi = 3.141592653589793; // circle ratio
  inline constexpr double tau = 2. * pi;
  inline constexpr double sigma_1 = 0.68268949213708590; // 1 sigma probability

  inline constexpr double c = 2.99792458e+8 * m / s; // speed of light in vacuum
  inline constexpr double h = 6.62607015e-34 * J * s; // elementary charge
  inline constexpr double e = 1.602176634e-19 * C; // Planck constant
  inline constexpr double k_B = 1.380649e-23 * J / K; // Boltzmann constant
  inline constexpr double N_A = 6.02214076e+23 / mol; // Avogadro constant

  inline constexpr double eV  = e * V;
  inline constexpr double meV = 1e-3 * eV;
  inline constexpr double keV = 1e+3 * eV;
  inline constexpr double MeV = 1e+6 * eV;

  inline constexpr double T_0 = 273.15 * K; // constant between absolute and Celsius temperature
  inline constexpr double hbar = h / tau; // reduced Planck constant
  inline constexpr double epsilon_0 = 8.8541878128e-12 * F / m; // electric constant
  inline constexpr double m_e = 9.1093837015e-31 * kg; // mass of electron
  inline constexpr double m_p = 1.67262192595e-27 * kg; // mass of proton
  inline constexpr double P_0 = 101325. * J / m / m / m; // standard atmosphere
  inline constexpr double a_0 = (2. * epsilon_0 * h * hbar) / (m_e * e * e); // Bohr radius
  inline constexpr double alpha = e * e / (2. * epsilon_0 * h * c); // fine-structure constant
}



#endif // PHYSCONSTANT_H
