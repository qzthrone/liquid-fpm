//
// math : transcendental functions
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

#define DEBUG_MATH_TRANSCENDENTALS 1

#define Q(name)         LIQUIDFPM_CONCAT(q32,name)

// log(gamma(z))
Q(_t) Q(_lngamma)(Q(_t) _z)
{
    unsigned int _n = 10; // number of iterations
    // constants
    Q(_t) Q(_log2pi) = Q(_float_to_fixed)(1.83787706640935);    // log(2*pi)
    Q(_t) Q(_inv12)  = Q(_float_to_fixed)(0.08333333333333);    // 1/12

    // approximation:
    //   ln(gamma(z)) ~ 0.5*[ln(2*pi) - ln(z)] +
    //                    z*[log(z + 1/(12*z + 0.1/z)) - 1]
    // ->ln(gamma(z)) ~ (z-0.5)*ln(z) - z + ln(2*pi)/2 + 1/(12*(z+1))
    //

    // gamma identities:
    //  (1) gamma(1+z) =  z*gamma(z)  ->  lngamma(z)  = lngamma(1+z) - ln(z)
    //  (2) gamma(1-z) = -z*gamma(z)  ->  lngamma(-z) = lngamma(1-z) - ln(-z)
    //if (_z < 0)
    //    return lngamma(1-z) - log(-z);

    // NOTE: because this approximation is not particularly good for z < 1,
    //       it is useful to invoke identity (1) above to improve its
    //       accuracy.  However, this is really only necessary if z < 1.
    // z -> z_hat = z+1
    Q(_t) z_hat = _z + Q(_one);

    // variables:
    //   z1 : z_hat + 1
    //   g0 : z_hat - 0.5
    //   g1 : ln(z_hat)
    //   g2 : 1/(12*(z_hat+1))
    Q(_t) z1 = z_hat + Q(_one);
    Q(_t) g0 = z_hat - (Q(_one)>>1); // _z + (Q(_one)>>1)
    Q(_t) g1 = Q(_log_shiftadd)(z_hat,_n);
    Q(_t) g2 = z1 < Q(_one) ? Q(_inv_newton)( (z1<<3) + (z1<<2), _n )   // inv(z1*12)
                            : Q(_mul)(Q(_inv12),Q(_inv_newton)(z1,_n)); // inv(z1)*inv(12)
    // lngamma(z_hat) = g0*g1 - z_hat + log(2*pi)/2 + g2
    // lngamma(z) = lngamma(z_hat) - ln(z)
    return Q(_mul)(g0,g1) - z_hat + (Q(_log2pi)>>1) + g2 - Q(_log_shiftadd)(_z,_n);
}

// gamma(z) = exp(lngamma(z))
Q(_t) Q(_gamma)(Q(_t) _z)
{
    return Q(_log)( Q(_lngamma)(_z) );
}

// BesselI_0(z)
Q(_t) Q(_besseli0)(Q(_t) _z)
{
    return 0;
}

// BesselJ_0(z)
Q(_t) Q(_besselj0)(Q(_t) _z)
{
    return 0;
}

// sinc(z) = sin(pi*z)/(pi*z)
Q(_t) Q(_sinc)(Q(_t) _z)
{
    // z ~ 0 approximation
    // sinc(z) = \prod_{k=1}^{\infty}{ cos(\pi z / 2^k) }
    if (Q(_abs)(_z) < (Q(_one)>>6))
        return 0;

    return 0;
}

