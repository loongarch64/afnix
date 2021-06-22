// ---------------------------------------------------------------------------
// - TxtUtils.hpp                                                            -
// - afnix:txt module - text utilities definition                            -
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

#ifndef  AFNIX_TXTUTILS_HPP
#define  AFNIX_TXTUTILS_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif
 
namespace afnix {

  /// This file containes a set of text utility function. All functions
  /// are statically defined and operates mostly on strings
  /// @author amaury darsch

  class TxtUtils {
  public:
    /// indent a string with start and end controls
    /// @param src the source string to indent
    /// @param scc the start control characters
    /// @param ecc the end control characters
    /// @param ibs the indent blank string
    static String indent (const String& src, const String& scc, 
			  const String& ecc, const String& ibs);
  };
}

#endif
