// ---------------------------------------------------------------------------
// - Infix.hpp                                                               -
// - afnix:mth module - infix notation parser class definition               -
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

#ifndef  AFNIX_INFIX_HPP
#define  AFNIX_INFIX_HPP

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif

namespace afnix {

  /// This Infx class is an infix notation expression parsing class. The
  /// class provides the base methods for simple object generation from a
  /// string.
  /// @author amaury darsch

  class Infix {
  public:
    /// parse a string as a real vector
    /// @param s the string to parse
    static Rvi* torvi (const String& s);

    /// convert a vector to an infix string
    /// @param v the vector to convert
    static String tostring (const Rvi& v);
  };
}

#endif
