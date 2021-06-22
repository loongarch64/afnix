// ---------------------------------------------------------------------------
// - InputTimeout.cpp                                                        -
// - standard object library - timeout input stream class implementation     -
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

#include "InputTimeout.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the read blocking mode
  static const long DEF_BLOK_MODE = -1;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default input timeout object

  InputTimeout::InputTimeout (void) {
    d_tout = DEF_BLOK_MODE;
  }

  // get the stream timeout

  long InputTimeout::gettout (void) const {
    rdlock ();
    try {
      long result = d_tout;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the stream timeout

  void InputTimeout::settout (const long tout) {
    wrlock ();
    try {
      d_tout = tout;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
