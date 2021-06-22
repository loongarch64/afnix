// ---------------------------------------------------------------------------
// - Wgtsrl.hxx                                                              -
// - afnix:wgt module - serial decoding definition                           -
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

#ifndef  AFNIX_WGTSRL_HXX
#define  AFNIX_WGTSRL_HXX

#include "Wgtsid.hxx"
#include "Utility.hpp"
#include "Exception.hpp"
#include "Conditional.hpp"

namespace afnix {
  
  // the wgt dispatch function
  static Serial* srl_deod_wgt (const t_word sid) {
    switch (sid) {
    case SRL_COND_SID:
      return new Conditional;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid wgt sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_WGT, srl_deod_wgt);
}

#endif
