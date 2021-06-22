// ---------------------------------------------------------------------------
// - t_bitset.cpp                                                            -
// - standard object library - bitset class tester                           -
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

#include "Bitset.hpp"

int main (int, char**) {
  using namespace afnix;

  // create a new bitset
  Bitset bs (256);
  for (long i = 0; i < 256; i++) {
    if (bs.ismark (i) == true) return 1;
  }

  // mark at position
  for (long i = 0; i < 256; i++) {
    if (bs.ismark (i) == true) return 1;
    bs.mark (i);
    if (bs.ismark (i) == false) return 1;
  }

  // clear and check
  for (long i = 0; i < 256; i++) bs.clear (i);
  for (long i = 0; i < 256; i++) { 
    if (bs.ismark (i) == true) return 1;
  }

  // reset the bitset with an octa
  bs.set (0x0123456789ABCDEFULL);
  if (bs.length () != 64) return 1;

  // check by byte
  if (bs.tobyte (0)  != 0xEF) return 1;
  if (bs.tobyte (8)  != 0xCD) return 1;
  if (bs.tobyte (16) != 0xAB) return 1;
  if (bs.tobyte (24) != 0x89) return 1;
  if (bs.tobyte (32) != 0x67) return 1;
  if (bs.tobyte (40) != 0x45) return 1;
  if (bs.tobyte (48) != 0x23) return 1;
  if (bs.tobyte (56) != 0x01) return 1;

  // extract at position and check
  Bitset sb = bs.subset (8);
  if (sb.tobyte (0) != 0xEF) return 1;
  sb = bs.subset (8, 8);
  if (sb.tobyte (0) != 0xCD) return 1;
  sb = bs.subset (8, 16);
  if (sb.tobyte (0) != 0xAB) return 1;
  sb = bs.subset (8, 24);
  if (sb.tobyte (0) != 0x89) return 1;
  sb = bs.subset (32, 32);
  if (sb.tobyte (0)  != 0x67) return 1;
  if (sb.tobyte (8)  != 0x45) return 1;
  if (sb.tobyte (16) != 0x23) return 1;
  if (sb.tobyte (24) != 0x01) return 1;
  
  // clamp the bitset and check the size
  if (bs.getcbi (true) != 56) return 1;
  bs.clamp (true);
  if (bs.length () != 57) return 1;
  
  // add a bit at position 0
  bs.add (0, false);
  if (bs.length ()  != 58)   return 1;
  if (bs.tobyte (0) != 0xDE) return 1;

  // reset the bitset with another octa
  bs.set (0x0000000000000321ULL);
  if (bs.length () != 64) return 1;
  bs.clamp (true);
  if (bs.length () != 10) return 1;

  // add a bit at position 15
  bs.add (15, true);
  if (bs.length ()  != 16)   return 1;
  if (bs.tobyte (0) != 0x21) return 1;
  if (bs.tobyte (8) != 0x83) return 1;

  // another test - used somewhere else...
  bs.set (0x00000000000000B4ULL);
  bs.clamp (true);
  bs.add (7, false);
  bs.add (15, true);
  if (bs.tobyte (0) != 0x34) return 1;
  if (bs.tobyte (8) != 0x81) return 1;
    
  // another test by string
  bs.set ("01101001");
  if (bs.length () != 8) return 1;
  if (bs.tobyte (0) != 0x69) return 1;

  // another test by string
  bs.set ("0b110101101001");
  if (bs.tobyte (0) != 0x69) return 1;
  if (bs.tobyte (8) != 0x0D) return 1;

  // create a test buffer
  Buffer buf (16);
  for (long i = 0; i < 16; i++) buf.add ((char) i);

  // create a bitset by ascending buffer
  bs = Bitset (buf, true);
  for (long i = 0; i < 16; i++) {
    if (bs.tobyte (i*8) != i) return 1;
  }
  // create a bitset by descending buffer
  bs = Bitset (buf, false);
  for (long i = 0; i < 16; i++) {
    if (bs.tobyte (i*8) != (16 - i - 1)) return 1;
  }

  // create a random bitset
  bs = Bitset::random (8192);
  if (bs.length () != 8192) return 1;

  // done
  return 0;
}
