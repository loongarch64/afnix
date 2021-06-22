// ---------------------------------------------------------------------------
// - t_blockbuffer.cpp                                                       -
// - standard object library - block buffer class tester                     -
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

#include "Ascii.hpp"
#include "Exception.hpp"
#include "BlockBuffer.hpp"

int main (int, char**) {
  using namespace afnix;

  // create an unbound buffer
  BlockBuffer ubuf;
  ubuf.setrflg (true);

  // check the size
  if (ubuf.length () != 0) return 1;
  
  // add a string and check
  if (ubuf.Buffer::add ("afnix") != 5) return 1;
  if (ubuf.length  () != 5) return 1;
  if (ubuf.getrcnt () != 0) return 1;
  if (ubuf.getwcnt () != 5) return 1;

  // check for string representation
  if (ubuf.tostring () != "afnix") return 1;

  // create a bound buffer
  BlockBuffer bbuf (4);

  // check the size
  if (bbuf.length () != 0) return 1;
  
  // add a string and check
  if (bbuf.Buffer::add ("afnix") != 4) return 1;
  if (bbuf.length  () != 4) return 1;
  if (bbuf.getrcnt () != 0) return 1;
  if (bbuf.getwcnt () != 4) return 1;

  // check for full
  if (bbuf.full () == false) return 1;

  // check for string representation
  if (bbuf.tostring () != "afni") return 1;

  // reset the bound buffer and copy
  bbuf.reset ();
  if (bbuf.copy (String ("afnix")) != 4) return 1;
  if (bbuf.length  () != 4) return 1;
  if (bbuf.getrcnt () != 0) return 1;
  if (bbuf.getwcnt () != 4) return 1;
  if (bbuf.copy (String ("x")) != 1) return 1;
  if (bbuf.length  () != 1) return 1;
  if (bbuf.getrcnt () != 0) return 1;
  if (bbuf.getwcnt () != 5) return 1;

  // check for full
  if (bbuf.full () == true) return 1;

  // success
  return 0;
}
