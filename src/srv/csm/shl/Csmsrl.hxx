// ---------------------------------------------------------------------------
// - Csmsrl.hxx                                                              -
// - afnix:csm service - serial decoding definition                          -
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

#ifndef  AFNIX_CSMSRL_HXX
#define  AFNIX_CSMSRL_HXX

#include "Visa.hpp"
#include "Csmsid.hxx"
#include "Domain.hpp"
#include "Carrier.hpp"
#include "Tracker.hpp"
#include "Utility.hpp"
#include "Session.hpp"
#include "Delegate.hpp"
#include "Authority.hpp"
#include "Principal.hpp"

namespace afnix {
  
  // the csm dispatch function
  static Serial* srl_deod_csm (const t_word sid) {
    switch (sid) {
    case SRL_PART_SID:
      return new Part;
      break;
    case SRL_BLOB_SID:
      return new Blob;
      break;
    case SRL_BLOC_SID:
      return new Bloc;
      break;
    case SRL_CBLB_SID:
      return new Carrier;
      break;
    case SRL_DBLB_SID:
      return new Delegate;
      break;
    case SRL_TCKR_SID:
      return new Tracker;
      break;
    case SRL_COLN_SID:
      return new Collection;
      break;
    case SRL_DOMN_SID:
      return new Domain;
      break;
    case SRL_WOIS_SID:
      return new Whois;
      break;
    case SRL_WAIS_SID:
      return new Whatis;
      break;
    case SRL_CRED_SID:
      return new Credential;
      break;
    case SRL_SESS_SID:
      return new Session;
      break;
    case SRL_IDTY_SID:
      return new Identity;
      break;
    case SRL_AUTH_SID:
      return new Authority;
      break;
    case SRL_PCPL_SID:
      return new Principal;
      break;
    case SRL_XACT_SID:
      return new Act;
      break;
    case SRL_VISA_SID:
      return new Visa;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid csm sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_CSM, srl_deod_csm);
}

#endif
