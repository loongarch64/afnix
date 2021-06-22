// ---------------------------------------------------------------------------
// - Global.hpp                                                              -
// - standard object library - global class definition                       -
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

#ifndef  AFNIX_GLOBAL_HPP
#define  AFNIX_GLOBAL_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The global class is a collection of global configuration parameters.
  /// Most of the parameters are set at the interpreter launch. Although they
  /// can be changed dynamically.
  /// @author amaury darsch

  class Global {
  public:
    /// the global debug flag
    /// @param dbug the debug flag
    static void setdbug (const bool dbug);

    /// @return the debug flag
    static bool getdbug (void);

    /// set the assert flag
    /// @param asrt the assert flag
    static void setasrt (const bool asrt);

    /// @return the assert flag
    static bool getasrt (void);
  };
}

#endif
