// ---------------------------------------------------------------------------
// - t_relatif.cpp                                                           -
// - standard object library - relatif class tester module                   -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Relatif.hpp"
#include "Exception.hpp"

int main (int, char**) {
  using namespace afnix;

  // create relatif by values
  Relatif x = 0;           Relatif sx ("0");
  Relatif y = 1;           Relatif sy ("1");
  Relatif z = -1;          Relatif sz ("-1");
  Relatif t = 2;           Relatif st ("2");
  Relatif a = 65536;       Relatif sa ("65536");
  Relatif b = 1024;        Relatif sb ("1024");
  Relatif c = 256;         Relatif sc ("256");
  Relatif d = 128;         Relatif sd ("128");
  Relatif e = -a;
  Relatif f = -b;
  Relatif g = -c;
  Relatif h = -d;
  Relatif l = 123456789;   Relatif sl ("123456789");
  Relatif o = 0x700000000ULL;

  // check for native integer conversion
  if (x.tolong () !=      0)          return 1;
  if (y.tolong () !=      1)          return 1;
  if (z.tolong () !=     -1)          return 1;
  if (a.tolong () !=  65536)          return 1;
  if (b.tolong () !=   1024)          return 1;
  if (c.tolong () !=    256)          return 1;
  if (d.tolong () !=    128)          return 1;
  if (e.tolong () != -65536)          return 1;
  if (f.tolong () !=  -1024)          return 1;
  if (g.tolong () !=   -256)          return 1;
  if (h.tolong () !=   -128)          return 1;
  if (l.tolong () != 123456789)       return 1;

  // check for native integer conversion
  if (sx.tolong () !=      0)         return 1;
  if (sy.tolong () !=      1)         return 1;
  if (sz.tolong () !=     -1)         return 1;
  if (sa.tolong () !=  65536)         return 1;
  if (sb.tolong () !=   1024)         return 1;
  if (sc.tolong () !=    256)         return 1;
  if (sd.tolong () !=    128)         return 1;
  if (sl.tolong () != 123456789)      return 1;

  // check for string to relatif conversion
  if ((Relatif ("0"))      !=    0)      return 1;
  if ((Relatif ("1"))      !=    1)      return 1;
  if ((Relatif ("-1"))     !=   -1)      return 1;
  if ((Relatif ("2000"))   != 2000)      return 1;
  if ((Relatif ("256"))    !=  256)      return 1;
  if ((Relatif ("0xff"))   !=  255)      return 1;
  if ((Relatif ("0b1111")) !=   15)      return 1;

  // check for relatif to string conversion
  if (x.tostring () !=     "0")          return 1;
  if (y.tostring () !=     "1")          return 1;
  if (z.tostring () !=    "-1")          return 1;
  if (a.tostring () != "65536")          return 1;
  if (b.tostring () !=  "1024")          return 1;
  if (c.tostring () !=   "256")          return 1;
  if (d.tostring () !=   "128")          return 1;

  // simple comparison
  if ((x < x) == true)                   return 1;
  if ((x < y) == false)                  return 1;
  if ((x < z) == true)                   return 1;
  if ((x < t) == false)                  return 1;
  if ((a < a) == true)                   return 1;
  if ((a < b) == true)                   return 1;
  if ((a < c) == true)                   return 1;
  if ((a < d) == true)                   return 1;
  if ((a < e) == true)                   return 1;
  if ((b < f) == true)                   return 1;
  if ((c < g) == true)                   return 1;
  if ((d < h) == true)                   return 1;

  if ((x < x.tolong ()) == true)      return 1;
  if ((x < y.tolong ()) == false)     return 1;
  if ((x < z.tolong ()) == true)      return 1;
  if ((x < t.tolong ()) == false)     return 1;
  if ((a < a.tolong ()) == true)      return 1;
  if ((a < b.tolong ()) == true)      return 1;
  if ((a < c.tolong ()) == true)      return 1;
  if ((a < d.tolong ()) == true)      return 1;
  if ((a < e.tolong ()) == true)      return 1;
  if ((b < f.tolong ()) == true)      return 1;
  if ((c < g.tolong ()) == true)      return 1;
  if ((d < h.tolong ()) == true)      return 1;

  if ((x <= x) == false)                 return 1;
  if ((x <= x) == false)                 return 1;
  if ((l <= l) == false)                 return 1;
  if ((x <= y) == false)                 return 1;
  if ((x <= z) == true)                  return 1;
  if ((x <= t) == false)                 return 1;
  if ((a <= a) == false)                 return 1;
  if ((a <= b) == true)                  return 1;
  if ((a <= c) == true)                  return 1;
  if ((a <= d) == true)                  return 1;
  if ((a <= e) == true)                  return 1;
  if ((b <= f) == true)                  return 1;
  if ((c <= g) == true)                  return 1;
  if ((d <= h) == true)                  return 1;

  if ((x <= x.tolong ()) == false)    return 1;
  if ((x <= x.tolong ()) == false)    return 1;
  if ((l <= l.tolong ()) == false)    return 1;
  if ((x <= y.tolong ()) == false)    return 1;
  if ((x <= z.tolong ()) == true)     return 1;
  if ((x <= t.tolong ()) == false)    return 1;
  if ((a <= a.tolong ()) == false)    return 1;
  if ((a <= b.tolong ()) == true)     return 1;
  if ((a <= c.tolong ()) == true)     return 1;
  if ((a <= d.tolong ()) == true)     return 1;
  if ((a <= e.tolong ()) == true)     return 1;
  if ((b <= f.tolong ()) == true)     return 1;
  if ((c <= g.tolong ()) == true)     return 1;
  if ((d <= h.tolong ()) == true)     return 1;

  if ((x > x) == true)                   return 1;
  if ((x > y) == true)                   return 1;
  if ((x > z) == false)                  return 1;
  if ((x > t) == true)                   return 1;
  if ((a > a) == true)                   return 1;
  if ((a > b) == false)                  return 1;
  if ((a > c) == false)                  return 1;
  if ((a > d) == false)                  return 1;
  if ((a > e) == false)                  return 1;
  if ((b > f) == false)                  return 1;
  if ((c > g) == false)                  return 1;
  if ((d > h) == false)                  return 1;

  if ((x > x.tolong ()) == true)      return 1;
  if ((x > y.tolong ()) == true)      return 1;
  if ((x > z.tolong ()) == false)     return 1;
  if ((x > t.tolong ()) == true)      return 1;
  if ((a > a.tolong ()) == true)      return 1;
  if ((a > b.tolong ()) == false)     return 1;
  if ((a > c.tolong ()) == false)     return 1;
  if ((a > d.tolong ()) == false)     return 1;
  if ((a > e.tolong ()) == false)     return 1;
  if ((b > f.tolong ()) == false)     return 1;
  if ((c > g.tolong ()) == false)     return 1;
  if ((d > h.tolong ()) == false)     return 1;

  if ((x >= x) == false)                 return 1;
  if ((l >= l) == false)                 return 1;
  if ((x >= y) == true)                  return 1;
  if ((x >= z) == false)                 return 1;
  if ((x >= t) == true)                  return 1;
  if ((a >= a) == false)                 return 1;
  if ((a >= b) == false)                 return 1;
  if ((a >= c) == false)                 return 1;
  if ((a >= d) == false)                 return 1;
  if ((a >= e) == false)                 return 1;
  if ((b >= f) == false)                 return 1;
  if ((c >= g) == false)                 return 1;
  if ((d >= h) == false)                 return 1;

  if ((x >= x.tolong ()) == false)    return 1;
  if ((l >= l.tolong ()) == false)    return 1;
  if ((x >= y.tolong ()) == true)     return 1;
  if ((x >= z.tolong ()) == false)    return 1;
  if ((x >= t.tolong ()) == true)     return 1;
  if ((a >= a.tolong ()) == false)    return 1;
  if ((a >= b.tolong ()) == false)    return 1;
  if ((a >= c.tolong ()) == false)    return 1;
  if ((a >= d.tolong ()) == false)    return 1;
  if ((a >= e.tolong ()) == false)    return 1;
  if ((b >= f.tolong ()) == false)    return 1;
  if ((c >= g.tolong ()) == false)    return 1;
  if ((d >= h.tolong ()) == false)    return 1;

  // simple mode tests
  if (x.iszero () == false)              return 1;
  if (x.iseven () == false)              return 1;
  if (x.isodd  () == true)               return 1;
  if (y.iszero () == true)               return 1;
  if (y.iseven () == true)               return 1;
  if (y.isodd  () == false)              return 1;

  // simple negation check
  if (-x != x)                           return 1;
  if (-a != e)                           return 1;
  if (-b != f)                           return 1;
  if (-c != g)                           return 1;
  if (-d != h)                           return 1;

  // simple add tests
  if ((x + y)     != 1)                  return 1;
  if ((y + z)     != 0)                  return 1;
  if ((x + z)     != -1)                 return 1;
  if ((x + y + z) != 0)                  return 1;

  // simple sub tests
  if ((x - y)     != -1)                 return 1;
  if ((y - z)     != 2)                  return 1;
  if ((x - z)     != 1)                  return 1;
  if ((x - y - z) != 0)                  return 1;

  // simple mul tests
  if ((x * y)     != 0)                  return 1;
  if ((y * z)     != -1)                 return 1;
  if ((x * z)     != 0)                  return 1;
  if ((x * y * z) != 0)                  return 1;

  // simple div tests
  if ((x / y)     != 0)                  return 1;
  if ((y / z)     != -1)                 return 1;
  if ((a / a)     != 1)                  return 1;
  if ((a / b)     != 64)                 return 1;
  if ((a / c)     != 256)                return 1;

  // simple modulo tests
  if ((x % y)     != 0)                  return 1;
  if ((y % z)     != 0)                  return 1;
  if ((a % a)     != 0)                  return 1;
  if ((a % b)     != 0)                  return 1;
  if ((a % c)     != 0)                  return 1;

  // simple in place tests
  Relatif lx = x;
  if ((lx += x)   != 0)                  return 1;
  if ((lx += y)   != y)                  return 1;
  if ((lx -= y)   != 0)                  return 1;
  Relatif ly = y;
  if ((ly *= y)   != y)                  return 1;
  if ((ly *= l)   != l)                  return 1;
  if ((ly /= l)   != y)                  return 1;

  // increments & decrements tests
  Relatif ix = 0;
  if (++ix  != 1)                        return 1;
  if ((ix++ != 1) && (ix != 2))          return 1;
  if (--ix  != 1)                        return 1;
  if ((ix-- != 1) && (ix != 0))          return 1;

  // symetric add check
  if ((a - e)     !=  (65536 << 1))      return 1;
  if ((b - f)     !=  ( 1024 << 1))      return 1;
  if ((c - g)     !=  (  256 << 1))      return 1;
  if ((d - h)     !=  (  128 << 1))      return 1;
  if ((e - a)     != -(65536 << 1))      return 1;
  if ((f - b)     != -( 1024 << 1))      return 1;
  if ((g - c)     != -(  256 << 1))      return 1;
  if ((h - d)     != -(  128 << 1))      return 1;

  // symetric sub check
  if ((a + e)     != 0)                  return 1;
  if ((b + f)     != 0)                  return 1;
  if ((c + g)     != 0)                  return 1;
  if ((d + h)     != 0)                  return 1;
  if ((e + a)     != 0)                  return 1;
  if ((f + b)     != 0)                  return 1;
  if ((g + c)     != 0)                  return 1;
  if ((h + d)     != 0)                  return 1;

  // reverse add check
  if ((c + h)     !=  128)               return 1;
  if ((h + c)     !=  128)               return 1;
  if ((g + d)     != -128)               return 1;
  if ((d + g)     != -128)               return 1;

  // reverse sub check
  if ((c - h)     !=  384)               return 1;
  if ((h - c)     != -384)               return 1;
  if ((g - d)     != -384)               return 1;
  if ((d - g)     !=  384)               return 1;
  
  // bitwise operators check
  if (~x != 0xFFFFFFFFUL)                return 1;
  if (~y != 0xFFFFFFFEUL)                return 1;
  if ((x & y) != 0x00000000UL)           return 1;
  if ((x | y) != 0x00000001UL)           return 1;
  if ((x ^ y) != 0x00000001UL)           return 1;

  // check for abs
  if (a.abs () != a)                     return 1;
  if (e.abs () != a)                     return 1;

  // simple left shift tests
  if ((x << 8)   != 0)                   return 1;
  if ((y << 1)   != 2)                   return 1;
  if ((y << 8)   != 256)                 return 1;
  if ((y << 9)   != 512)                 return 1;
  if ((y << 10)  != 1024)                return 1;

  // simple in place left shift tests
  Relatif slx = x;
  Relatif sly = y;
  if ((slx <<= 8) != 0)                  return 1;
  if ((sly <<= 1) != 2)                  return 1;
  if ((sly <<= 7) != 256)                return 1;
  if ((sly <<= 1) != 512)                return 1;
  if ((sly <<= 1) != 1024)               return 1;

  // simple shift right tests
  if ((x >> 8)   != 0)                   return 1;
  if ((y >> 1)   != 0)                   return 1;
  if ((b >> 8)   != 4)                   return 1;
  if ((c >> 1)   != 128)                 return 1;
  if ((c >> 2)   != 64)                  return 1;

  // simple in place right shift tests
  Relatif srx = x;
  Relatif sry = y;
  Relatif srb = b;
  Relatif src = c;
  if ((srx >>= 8)   != 0)                return 1;
  if ((sry >>= 1)   != 0)                return 1;
  if ((srb >>= 8)   != 4)                return 1;
  if ((src >>= 1)   != 128)              return 1;
  if ((src >>= 1)   != 64)               return 1;

  // check for lsb
  if (x.getlsb () != 0)                  return 1;
  if (y.getlsb () != 1)                  return 1;
  if (z.getlsb () != 1)                  return 1;
  if (a.getlsb () != 17)                 return 1;
  if (b.getlsb () != 11)                 return 1;
  if (c.getlsb () != 9)                  return 1;
  if (l.getlsb () != 1)                  return 1;
  if (o.getlsb () != 33)                 return 1;

  // check for msb
  if (x.getmsb () != 0)                  return 1;
  if (y.getmsb () != 1)                  return 1;
  if (z.getmsb () != 1)                  return 1;
  if (a.getmsb () != 17)                 return 1;
  if (b.getmsb () != 11)                 return 1;
  if (c.getmsb () != 9)                  return 1;

  // check the byte buffer size
  if (x.getbbs () != 1)                  return 1;
  if (y.getbbs () != 1)                  return 1;
  if (z.getbbs () != 1)                  return 1;
  if (a.getbbs () != 3)                  return 1;
  if (b.getbbs () != 2)                  return 1;
  if (c.getbbs () != 2)                  return 1;

  // check for bit position
  if (x.getbit (0)  == true)             return 1;
  if (x.getbit (7)  == true)             return 1;
  if (y.getbit (0)  == false)            return 1;
  if (y.getbit (1)  == true)             return 1;
  if (y.getbit (7)  == true)             return 1;
  if (z.getbit (1)  == true)             return 1;
  if (z.getbit (7)  == true)             return 1;
  if (a.getbit (0)  == true)             return 1;
  if (a.getbit (16) == false)            return 1;

  // set bit position
  Relatif yb = y;
  yb.setbit (1, true);
  if (yb.tolong () != 3)              return 1;
  yb.setbit (1, false);
  if (yb.tolong () != 1)              return 1;

  // large add tests
  Relatif ra;
  Relatif rb;
  for (long i = 0; i < 256; i++) {
    long j = 65336 + i;
    ra = i;
    rb = j;
    if ((ra + j)  != (i + j))            return 1;
    if ((rb + i)  != (i + j))            return 1;
    if ((j + ra)  != (j + i))            return 1;
    if ((i + rb)  != (i + j))            return 1;
    if ((ra + rb) != (i + j))            return 1;
  }

  // large sub tests
  for (long i = 0; i < 256; i++) {
    long j = 65336 + i;
    ra = i;
    rb = j;
    if ((ra - j)  != (i - j))            return 1;
    if ((rb - i)  != (j - i))            return 1;
    if ((j  - ra) != (j - i))            return 1;
    if ((i  - rb) != (i - j))            return 1;
    if ((ra - rb) != (i - j))            return 1;
    if ((rb - ra) != (j - i))            return 1;
  }
  
  // large mul tests
  for (long i = 0; i < 256; i++) {
    long j = 65336 + i;
    ra = i;
    rb = j;
    if ((ra * j)  != (i * j))            return 1;
    if ((i  * rb) != (i * j))            return 1;
    if ((ra * rb) != (i * j))            return 1;
  }

  // large div tests
  for (long i = 1; i <= 256; i++) {
    long j = 1048576 + i;
    ra = j;
    rb = i;
    if ((ra / i)  != (j / i))            return 1;
    if ((ra / rb) != (j / i))            return 1;
  }

  // large modulo tests
  for (long i = 1; i <= 256; i++) {
    long j = 1048576 + i;
    ra = j;
    rb = i;
    if ((ra % i)  != (j % i))            return 1;
    if ((ra % rb) != (j % i))            return 1;
  }

  // large hexadecimal tests
  String hx0 = "0x1234567890ABCDEF";
  String hx1 = "0x1234567890ABCDEF00";
  String hx2 = "0x1234567890";
  String hx3 = "0x91A2B3C4855E6F7800000000000";
  Relatif hx (hx0);
  if (hx.tohexa () != hx0)               return 1;
  Relatif hxl = hx << 8;
  Relatif hxr = hx >> 24;
  if (hxl.tohexa () != hx1)              return 1;
  if (hxr.tohexa () != hx2)              return 1;
  hx <<= 47;
  if (hx.tohexa  () != hx3)              return 1;
  hx >>= 47;
  if (hx.tohexa  () != hx0)              return 1;

  // power and modular exponentiation
  if (t.pow (0) != 1)                    return 1;
  if (t.pow (1) != 2)                    return 1;
  if (t.pow (10) != 1024)                return 1;
  if (t.pow (12, 13)   != 1)             return 1;
  if (t.pow (4 , 13)   != 3)             return 1;
  if (t.pow (560, 561) != 1)             return 1;

  if (Relatif::mme (2, 12, 13)    != 1)  return 1;
  if (Relatif::mme (2, 560, 561)  != 1)  return 1;
  if (Relatif::mme (174, 55, 221) != 47) return 1;

  // test with random relatif
  for (long k = 1; k < 256; k++) {
    Relatif x = 2;
    Relatif e = Relatif::random (k);
    Relatif m = Relatif::random (k, true);
    if (x.pow (e, m) != Relatif::mme (x, e, m)) return 1;
  }
  
  // gcd test checks
  if (Relatif::gcd (15, 5)       != 5)   return 1;
  if (Relatif::gcd (1029, 1071)  != 21)  return 1;
  if (Relatif::gcd (6035, 20451) != 17)  return 1;

  if (Relatif::lcm (4,  6) != 12)        return 1;
  if (Relatif::lcm (21, 6) != 42)        return 1;

  if (Relatif::mmi (3, 11) != 4)         return 1;

  // random number checks
  Relatif ornd = Relatif::random (8, true);
  if (ornd.getmsb () != 8)               return 1;
  if (ornd.isodd  () == false)           return 1;
  if (ornd.iseven () == true)            return 1;

  Relatif ernd = Relatif::random (256, false);
  if (ernd.getmsb () != 256)             return 1;
  if (ernd.isodd  () == true)            return 1;
  if (ernd.iseven () == false)           return 1;

  // test with a random relatif
  Relatif u = Relatif::random (1024);
  Relatif v = Relatif::random (512);

  // msb check
  if (u.getmsb () != 1024)               return 1;
  if (v.getmsb () != 512)                return 1;

  // sum test
  Relatif s = u + v;
  if ((s - u) != v)                      return 1;
  if ((s - v) != u)                      return 1;

  // mul/div/mod tests
  Relatif m = u * v;
  if ((m / u) != v)                      return 1;
  if ((m / v) != u)                      return 1;
  if ((u / m) != 0)                      return 1;
  if ((v / m) != 0)                      return 1;
  if ((m % u) != 0)                      return 1;
  if ((m % v) != 0)                      return 1;
  if ((u % m) != u)                      return 1;
  if ((v % m) != v)                      return 1;

  // squaring test
  if (u.pow (2) != (u * u))              return 1;
  if (v.pow (2) != (v * v))              return 1;
  if (u.pow (t) != (u * u))              return 1;
  if (v.pow (t) != (v * v))              return 1;

  // test with a random relatif
  Relatif w = Relatif::random (1024, true);
  if (w.getmsb () != 1024)               return 1;
  if ((w % 2) != 1)                      return 1;

  // check relatif random
  if (Relatif::random(w) > w)            return 1;
  
  // check byte representation
  t_byte rbuf[128];
  if (w.toubuf (rbuf, 128) != 128)       return 1;
  if (w != Relatif (rbuf, 128))          return 1;
  if (w.tobyte (rbuf, 128) != 128)       return 1;
  if (w != Relatif (rbuf, 128))          return 1;

  // check byte buffer
  Relatif xb = (y << 32);
  if (xb.tobyte (rbuf, 128) != 5)        return 1;
  for (long i = 0; i < 128-5; i++) {
    if (rbuf[i] != nilc) return 1;
  }
  if (rbuf[123] != 0x01)                 return 1;
  if (rbuf[124] != nilc)                 return 1;
  if (rbuf[125] != nilc)                 return 1;
  if (rbuf[126] != nilc)                 return 1;
  if (rbuf[127] != nilc)                 return 1;

  // check a relatif by value
  for (long i = 0; i < 128; i++) rbuf[i] = 0x00;
  rbuf[0] = 1;
  Relatif r1 (rbuf, 1);
  if (r1 != 1) return 1;
  // check size 2
  rbuf[0] = 2;
  rbuf[1] = 1;
  Relatif r2 (rbuf, 2);
  if (r2 != 513) return 1;
  // check size 3
  rbuf[0] = 3;
  rbuf[1] = 2;
  rbuf[2] = 1;
  Relatif r3 (rbuf, 3);
  if (r3 != 197121) return 1;
  // check size 4
  rbuf[0] = 4;
  rbuf[1] = 3;
  rbuf[2] = 2;
  rbuf[3] = 1;
  Relatif r4 (rbuf, 4);
  if (r4 != 67305985) return 1;

  // check a relatif by complemented value
  for (long i = 0; i < 128; i++) rbuf[i] = 0x00;
  // check size 1 - unsigned
  rbuf[0] = 1;
  Relatif ru1 (rbuf, 1, false);
  if (ru1 != 1) return 1;
  // check size 1 - signed
  rbuf[0] = 0x80;
  Relatif rs1 (rbuf, 1, true);
  if (rs1 != -128) return 1;
  // check size 2 - unsigned
  rbuf[0] = 0xFF;
  rbuf[1] = 0xFF;
  Relatif ru2 (rbuf, 2, false);
  if (ru2 != 65535) return 1;
  // check size 2 - signed
  Relatif rs2 (rbuf, 2, true);
  if (rs2 != -1) return 1;
  // check size 4 - unsigned
  rbuf[0] = 0xFF;
  rbuf[1] = 0xFF;
  rbuf[2] = 0xFF;
  rbuf[3] = 0xFF;
  // check size 4 - signed
  Relatif rs4 (rbuf, 4, true);
  if (rs4 != -1) return 1;

  // check with sign conversion
  t_byte sbuf[4];
  if (rs4.tosbuf (sbuf, 4) != 1) return 1;
  if (sbuf[0] != 0xFF) return 1;
  // check size 3 signed
  Relatif rs3 = -65536;
  if (rs3.tosbuf (sbuf, 4) != 3) return 1;
  if (sbuf[0] != 0xFF) return 1;
  if (sbuf[1] != 0x00) return 1;
  if (sbuf[2] != 0x00) return 1;

  // check large integer
  rbuf[0] = 0x12;
  rbuf[1] = 0x34;
  rbuf[2] = 0x56;
  rbuf[3] = 0x78;
  rbuf[4] = 0x90;
  // check size 5 - signed
  Relatif rs5 (rbuf, 5, true);
  if (rs5 != 0x1234567890LL) return 1;

  // everything is fine
  return 0;
}
