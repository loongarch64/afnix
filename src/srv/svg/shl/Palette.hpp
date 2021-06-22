// ---------------------------------------------------------------------------
// - Palette.hpp                                                             -
// - afnix:svg service - color palette class definition                      -
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

#ifndef  AFNIX_PALETTE_HPP
#define  AFNIX_PALETTE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Palette class is the color transformation and palette class.
  /// The class defines a standard set of color which can be used for
  /// direct mapping in rgb coordinates.
  /// @author amaury darsch

  class Palette {
  public:
    /// get a color name by index
    /// @param index the color index
    static String getname (const long index);

    /// get a rgb color by index
    /// @param index the color index
    static String getrgb (const long index);
  };
}

#endif
