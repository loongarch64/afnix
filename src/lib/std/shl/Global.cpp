// ---------------------------------------------------------------------------
// - Global.cpp                                                              -
// - standard object library - global class implementation                   -
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

#include "Global.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the debug flag
  #ifdef AFNIX_DEBUG
  static bool cv_glo_dbug = true;
  #else
  static bool cv_glo_dbug = false;
  #endif
  
  // the assert flag
  static bool cv_glo_asrt = false;
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // set the debug flag

  void Global::setdbug (const bool dbug) {
    cv_glo_dbug = dbug;
  }

  // get the deug flag

  bool Global::getdbug (void) {
    return cv_glo_dbug;
  }

  // set the assert flag

  void Global::setasrt (const bool asrt) {
    cv_glo_asrt = asrt;
  }

  // get the assert flag

  bool Global::getasrt (void) {
    return cv_glo_asrt;
  }
}
