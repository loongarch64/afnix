// ---------------------------------------------------------------------------
// - t_ccnv.cpp                                                              -
// - standard platform library - conversion function tester                  -
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

#include "ccnv.hpp"

int main (int, char**) {
  using namespace afnix;

  // test number conversion
  char* buf = c_lltoa (0x1000000000LL);
  bool  flg = false;
  if (c_atoll (buf, flg) != 0x1000000000LL) return 1;
  if (flg == false) return 1;
  delete [] buf;

  // test word conversion
  t_byte warray[4];
  c_whton (0x1234, warray);
  if (warray[1] != (t_byte) 0x34)       return 1;
  if (warray[0] != (t_byte) 0x12)       return 1;
  if (c_wntoh (warray) != 0x1234)       return 1;

  // test quad conversion
  t_byte qarray[4];
  c_qhton (0x12345678,qarray);
  if (qarray[3] != (t_byte) 0x78)       return 1;
  if (qarray[2] != (t_byte) 0x56)       return 1;
  if (qarray[1] != (t_byte) 0x34)       return 1;
  if (qarray[0] != (t_byte) 0x12)       return 1;
  if (c_qntoh (qarray) != 0x12345678)   return 1;

  // test octa conversion
  t_byte oarray[8];
  c_ohton (0x0123456789abcdefLL,oarray);
  if (oarray[7] != (t_byte) 0xef)       return 1;
  if (oarray[6] != (t_byte) 0xcd)       return 1;
  if (oarray[5] != (t_byte) 0xab)       return 1;
  if (oarray[4] != (t_byte) 0x89)       return 1;
  if (oarray[3] != (t_byte) 0x67)       return 1;
  if (oarray[2] != (t_byte) 0x45)       return 1;
  if (oarray[1] != (t_byte) 0x23)       return 1;
  if (oarray[0] != (t_byte) 0x01)       return 1;
  if (c_ontoh (oarray) != 0x0123456789abcdefLL) return 1;

  // here it is
  return 0;
}
