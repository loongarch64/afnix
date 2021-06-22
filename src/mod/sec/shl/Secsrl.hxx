// ---------------------------------------------------------------------------
// - Secsrl.hxx                                                              -
// - afnix:sec module - serial decoding definition                           -
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

#ifndef  AFNIX_SECSRL_HXX
#define  AFNIX_SECSRL_HXX

#include "Secsid.hxx"
#include "Keyring.hpp"
#include "Utility.hpp"
#include "Signature.hpp"

namespace afnix {
  
  // the sec dispatch function
  static Serial* srl_deod_sec (const t_word sid) {
    switch (sid) {
    case SRL_SKEY_SID:
      return new Key;
      break;
    case SRL_SIGN_SID:
      return new Signature;
      break;
    case SRL_KRNG_SID:
      return new Keyring;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid sec sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_SEC, srl_deod_sec);
}

#endif
