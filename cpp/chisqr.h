
/*
    Implementation of the Chi-Square Distribution &
        Incomplete Gamma Function in C

        Written By Jacob Wells
        July 31, 2012
    Based on the formulas found here:

    Wikipedia - Incomplete Gamma Function -> Evaluation formulae -> Connection
   with Kummer's confluent hypergeometric function
    http://en.wikipedia.org/wiki/Regularized_Gamma_function#Connection_with_Kummer.27s_confluent_hypergeometric_function

    Wikipedia - Chi-squared Distribution -> Cumulative distribution function
    http://en.wikipedia.org/wiki/Chi-squared_distribution#Cumulative_distribution_function

    These functions are placed in the Public Domain, and may be used by anyone,
   anywhere, for any reason, absolutely free of charge.

*/

#include <stdio.h>
#include <math.h>
/** gamma function
 * taken from http://www.crbond.com/math.htm (Gamma function in C/C++ for real
 * arguments, ported from Zhang and Jin)
 * returns 1e308 if argument is a negative integer or 0 or if argument exceeds
 * 171.
 * @param x     argument
 * @return      Gamma(argument)
 */
static double gamma0(double x) {
  /* gamma function.
  Algorithms and coefficient values from "Computation of Special
  Functions", Zhang and Jin, John Wiley and Sons, 1996.
  (C) 2003, C. Bond. All rights reserved.
  taken from http://www.crbond.com/math.htm */
  int i, k, m;
  double ga, gr, r = 1.0, z;

  static double g[] = {1.0,
                       0.5772156649015329,
                       -0.6558780715202538,
                       -0.420026350340952e-1,
                       0.1665386113822915,
                       -0.421977345555443e-1,
                       -0.9621971527877e-2,
                       0.7218943246663e-2,
                       -0.11651675918591e-2,
                       -0.2152416741149e-3,
                       0.1280502823882e-3,
                       -0.201348547807e-4,
                       -0.12504934821e-5,
                       0.1133027232e-5,
                       -0.2056338417e-6,
                       0.6116095e-8,
                       0.50020075e-8,
                       -0.11812746e-8,
                       0.1043427e-9,
                       0.77823e-11,
                       -0.36968e-11,
                       0.51e-12,
                       -0.206e-13,
                       -0.54e-14,
                       0.14e-14};

  if (x > 171.0)
    return 1e308; // This value is an overflow flag.
  if (x == int(x)) {
    if (x > 0.0) {
      ga = 1.0; // use factorial
      for (i = 2; i < x; i++) {
        ga *= i;
      }
    } else
      ga = 1e308;
  } else {
    if (fabs(x) > 1.0) {
      z = fabs(x);
      m = int(z);
      r = 1.0;
      for (k = 1; k <= m; k++) {
        r *= (z - k);
      }
      z -= m;
    } else
      z = x;
    gr = g[24];
    for (k = 23; k >= 0; k--) {
      gr = gr * z + g[k];
    }
    ga = 1.0 / (gr * z);
    if (fabs(x) > 1.0) {
      ga *= r;
      if (x < 0.0) {
        ga = -M_PI / (x * ga * sin(M_PI * x));
      }
    }
  }
  return ga;
}

/** incomplete gamma function
 * taken from http://www.crbond.com/math.htm (Incomplete Gamma function, ported
 * from Zhang and Jin)
 * @param a
 * @param x
 * @param gin
 * @param gim
 * @param gip
 * @return
 */
static int incog(double a, double x, double& gin, double& gim, double& gip) {
  double xam, r, s, ga, t0;
  int k;

  if ((a < 0.0) || (x < 0))
    return 1;
  xam = -x + a * log(x);
  if ((xam > 700) || (a > 170.0))
    return 1;
  if (x == 0.0) {
    gin = 0.0;
    gim = gamma0(a);
    gip = 0.0;
    return 0;
  }
  if (x <= 1.0 + a) {
    s = 1.0 / a;
    r = s;
    for (k = 1; k <= 60; k++) {
      r *= x / (a + k);
      s += r;
      if (fabs(r / s) < 1e-15)
        break;
    }
    gin = exp(xam) * s;
    ga = gamma0(a);
    gip = gin / ga;
    gim = ga - gin;
  } else {
    t0 = 0.0;
    for (k = 60; k >= 1; k--) {
      t0 = (k - a) / (1.0 + k / (x + t0));
    }
    gim = exp(xam) / (x + t0);
    ga = gamma0(a);
    gin = ga - gim;
    gip = 1.0 - gim / ga;
  }
  return 0;
}

/** function converting Chi^2 value to corresponding p-value
 * taken from
 * http://www.codeproject.com/Articles/432194/How-to-Calculate-the-Chi-Squared-P-Value
 * note: do not use full implementation from above website, it is not precise
 * enough for small inputs
 * @param Dof   degrees of freedom
 * @param Cv    Chi^2 value
 * @return      p-value
 */
static double chisqr(int Dof, double Cv) {
  if (Cv < 0 || Dof < 1) {
    return 1;
  }
  double K = (double(Dof)) * 0.5;
  double X = Cv * 0.5;
  if (Dof == 2) {
    return exp(-1.0 * X);
  }
  double gin, gim, gip;
  incog(K, X, gin, gim, gip); // compute incomplete gamma function
  double PValue = gim;
  PValue /= gamma0(K); // divide by gamma function value
  return PValue;
}
