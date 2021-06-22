// ---------------------------------------------------------------------------
// - t_transcoder.cpp                                                        -
// - standard object library - transcoder class tester                       -
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

#include "Transcoder.hpp"

int main (int, char**) {
  using namespace afnix;
  
  // create a transcoder
  Transcoder* coder = new Transcoder;

  // check for the default coder
  if (coder->gettmod () != Encoding::TMOD_NONE) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-1 codeset
  coder->settmod (Encoding::TMOD_5901);
  if (coder->gettmod () != Encoding::TMOD_5901) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-2 codeset
  coder->settmod (Encoding::TMOD_5902);
  if (coder->gettmod () != Encoding::TMOD_5902) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-3 codeset
  coder->settmod (Encoding::TMOD_5903);
  if (coder->gettmod () != Encoding::TMOD_5903) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-4 codeset
  coder->settmod (Encoding::TMOD_5904);
  if (coder->gettmod () != Encoding::TMOD_5904) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-5 codeset
  coder->settmod (Encoding::TMOD_5905);
  if (coder->gettmod () != Encoding::TMOD_5905) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-6 codeset
  coder->settmod (Encoding::TMOD_5906);
  if (coder->gettmod () != Encoding::TMOD_5906) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-7 codeset
  coder->settmod (Encoding::TMOD_5907);
  if (coder->gettmod () != Encoding::TMOD_5907) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-8 codeset
  coder->settmod (Encoding::TMOD_5908);
  if (coder->gettmod () != Encoding::TMOD_5908) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-9 codeset
  coder->settmod (Encoding::TMOD_5909);
  if (coder->gettmod () != Encoding::TMOD_5909) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-10 codeset
  coder->settmod (Encoding::TMOD_5910);
  if (coder->gettmod () != Encoding::TMOD_5910) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-11 codeset
  coder->settmod (Encoding::TMOD_5911);
  if (coder->gettmod () != Encoding::TMOD_5911) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // check for the iso-8859-13 codeset
  coder->settmod (Encoding::TMOD_5913);
  if (coder->gettmod () != Encoding::TMOD_5913) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }
  // check for the iso-8859-14 codeset
  coder->settmod (Encoding::TMOD_5914);
  if (coder->gettmod () != Encoding::TMOD_5914) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }
  // check for the iso-8859-15 codeset
  coder->settmod (Encoding::TMOD_5915);
  if (coder->gettmod () != Encoding::TMOD_5915) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }
  // check for the iso-8859-16 codeset
  coder->settmod (Encoding::TMOD_5916);
  if (coder->gettmod () != Encoding::TMOD_5916) return 1;
  for (long i = 0; i < 256; i++) {
    char cval = i;
    if (coder->valid (cval) == true) {
      t_quad uval = coder->encode (cval);
      if (coder->valid (uval) == false) return 1;
      if (coder->decode  (uval) != cval)  return 1;
    }
  }

  // delete everything
  delete coder;
  return 0;
}
