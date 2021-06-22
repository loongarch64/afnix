// ---------------------------------------------------------------------------
// - TxtUtils.cpp                                                            -
// - afnix:txt module - text utilities implementation                        -
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

#include "Buffer.hpp"
#include "TxtUtils.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure checks if a character belong to an array
  static inline bool isin (const t_quad c, const t_quad* cs) {
    while (*cs != nilq) {
      if (*cs++ == c) return true;
    }
    return false;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // indent a string with start and end controls

  String TxtUtils::indent (const String& src, const String& scc, 
			   const String& ecc, const String& ibs) {
    // prepare the control arrays
    const t_quad* sca = scc.toquad ();
    const t_quad* eca = ecc.toquad ();
    try {
      // create a working buffer
      Buffer buf (Encoding::EMOD_UTF8);
      // initialize indent level
      long lvl = 0;
      // indent the string
      long len = src.length ();
      for (long i = 0; i < len; i++) {
	const t_quad c = src[i];
	const t_quad l = (i > 0) ? src[i-1] : nilq;
	// check increased indent
	if (isin (c, sca) == true) {
	  if (l == eolq) {
	    for (long j = 0; j < lvl; j++) buf.add (ibs);
	  } 
	  lvl++;
	  buf.add (c);
	  continue;
	}
	// check decreased indent
	if (isin (c, eca) == true) {
	  if (lvl > 0) lvl--;
	  if (l == eolq) {
	    for (long j = 0; j < lvl; j++) buf.add (ibs);
	  }
	  buf.add (c);
	  continue;
	}
	// check if we indent
	if (l == eolq) {
	  for (long j = 0; j < lvl; j++) buf.add (ibs);
	}
	// add the character
	buf.add (c);
      }
      // clean buffers
      delete [] sca;
      delete [] eca;
      // here is the string
      return buf.tostring ();
    } catch (...) {
      delete [] sca;
      delete [] eca;
      throw;
    }
  }
}
