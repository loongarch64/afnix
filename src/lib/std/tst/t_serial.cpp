// ---------------------------------------------------------------------------
// - t_serial.cpp                                                            -
// - standard object library - serial class tester                           -
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

#include "Serial.hpp"
#include "String.hpp"

// ---------------------------------------------------------------------------
// - private section                                                         -
// ---------------------------------------------------------------------------

namespace afnix {
  static long TST_XDIM_SIZE = 64;
  static long TST_YDIM_SIZE = 64;
  static long TST_ARRY_SIZE = TST_XDIM_SIZE * TST_YDIM_SIZE;

  // test the byte serial array
  static bool tst_array_byte (void) {
    // create a byte serial array
    Serial::Array arry (TST_ARRY_SIZE, Serial::Array::ATYP_BYTE);
    // add a byte in the array
    for (long k = 0; k < TST_ARRY_SIZE; k++) arry.add ((t_byte) (k % 256));
    // test the array
    for (long k = 0; k < TST_ARRY_SIZE; k++) {
      if (arry.getbyte (k) != (t_byte) (k % 256)) return false;
    }
    return true;
  }

  // test the long serial array
  static bool tst_array_long (void) {
    // create a long serial array
    Serial::Array arry (TST_ARRY_SIZE, Serial::Array::ATYP_LONG);
    // add a long in the array
    for (long k = 0; k < TST_ARRY_SIZE; k++) arry.add ((t_long) k);
    // test the array
    for (long k = 0; k < TST_ARRY_SIZE; k++) {
      if (arry.getlong (k) != (t_long) k) return false;
    }
    return true;
  }

  // test the real serial array
  static bool tst_array_real (void) {
    // create a real serial array
    Serial::Array arry (TST_ARRY_SIZE, Serial::Array::ATYP_REAL);
    // add a real in the array
    for (long k = 0; k < TST_ARRY_SIZE; k++) arry.add ((t_real) k);
    // test the array
    for (long k = 0; k < TST_ARRY_SIZE; k++) {
      if (arry.getreal (k) != (t_real) k) return false;
    }
    return true;
  }

  // test the real point serial array
  static bool tst_array_rpt2 (void) {
    // create a real point serial array
    Serial::Array arry (TST_ARRY_SIZE, Serial::Array::ATYP_RPT2);
    // add a point in the array
    for (long x = 0; x < TST_XDIM_SIZE; x++) {
      for (long y = 0; y < TST_YDIM_SIZE; y++) {
	arry.add (x, y, (t_real) x * y);
      }
    }
    // test the array
    for (long x = 0; x < TST_XDIM_SIZE; x++) {
      for (long y = 0; y < TST_YDIM_SIZE; y++) {
	long index = x * TST_XDIM_SIZE + y;
	if (arry.getreal (index) != (t_real) x * y) return false;
      }
    }
    return true;
  }
}

int main (int, char**) {
  using namespace afnix;

  // test a byte array
  if (tst_array_byte () == false) return 1;
  // test a long array
  if (tst_array_long () == false) return 1;
  // test a real array
  if (tst_array_real () == false) return 1;
  // test a real point array
  if (tst_array_rpt2 () == false) return 1;

  // ok - everything is fine
  return 0;
}
