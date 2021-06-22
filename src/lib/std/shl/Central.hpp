// ---------------------------------------------------------------------------
// - Central.hpp                                                             -
// - standard object library - central object control class definition       -
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

#ifndef  AFNIX_CENTRAL_HPP
#define  AFNIX_CENTRAL_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

#ifndef  AFNIX_COLLECTABLE_HPP
#include "Collectable.hpp"
#endif

namespace afnix {
  
  /// The Central class is a complex class design for the management of the
  /// object. First, the central object incorporates a reactor which is
  /// designed to produce quarks from a string object. Second, the central
  /// incorporates a confinement engine designed to confine the collectable
  /// objects created by the running system. From a design perspective, the
  /// central provides only static functions and therefore can be used
  /// internally anywhere.
  /// @author amaury darsch

  class Central {
  public:
    /// intern a string and return the associated quark
    /// @param name the name to intern
    static long intern (const String& name);

    /// map a quark to a string object
    /// @param quark the quark to map
    static String qmap (const long quark);

    /// confine a collectable object
    /// @param cobj the collectable object
    /// @return the confinement pointer
    static void* confine (Collectable* cobj);

    /// untangle a confined object
    /// @param cobj the collectable object
    /// @param cptr the confinement pointer
    static bool untangle (Collectable* cobj, void* cptr);
  };
}

#endif
