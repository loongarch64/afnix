// ---------------------------------------------------------------------------
// - Dipsrl.hxx                                                              -
// - afnix:dip service - serial decoding definition                          -
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

#ifndef  AFNIX_DIPSRL_HXX
#define  AFNIX_DIPSRL_HXX

#include "Dipsid.hxx"
#include "Pixmap.hpp"
#include "Mixmap.hpp"
#include "Utility.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // the dip dispatch function
  static Serial* srl_deod_dip (const t_word sid) {
    switch (sid) {
    case SRL_PIXL_SID:
      return new Pixel;
      break;
    case SRL_PIXM_SID:
      return new Pixmap;
      break;
    case SRL_MIXM_SID:
      return new Mixmap;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid dip sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_DIP, srl_deod_dip);
}

#endif
