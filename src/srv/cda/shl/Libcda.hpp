// ---------------------------------------------------------------------------
// - Libcda.hpp                                                              -
// - afnix:cda service - static entry point declaration                      -
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

#ifndef  AFNIX_LIBCDA_HPP
#define  AFNIX_LIBCDA_HPP

#ifndef  AFNIX_INTERP_HPP
#include "Interp.hpp"
#endif

namespace afnix {

  /// this file contains the module initialization routine for the afnix
  /// collect data analytics service
  /// @author amaury darsch

  /// initialize the afnix:cda service
  /// @param interp the current interpreter
  /// @param argv   the arguments vector
  Object* init_afnix_cda (Interp* interp, Vector* argv);
}

#endif
