// ---------------------------------------------------------------------------
// - Engsrl.hxx                                                              -
// - afnix:eng - engine serial decoding definition                           -
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

#ifndef  AFNIX_ENGSRL_HXX
#define  AFNIX_ENGSRL_HXX

#include "Form.hpp"
#include "Engsid.hxx"
#include "Utility.hpp"
#include "Lexical.hpp"
#include "Counter.hpp"
#include "Promise.hpp"
#include "Constant.hpp"
#include "Reserved.hpp"
#include "Qualified.hpp"

namespace afnix {
  
  // the engine dispatch function
  static Serial* srl_deod_eng (const t_word sid) {
    switch (sid) {
    case SRL_RESV_SID:
      return new Reserved;
      break;
    case SRL_CNST_SID:
      return new Constant;
      break;
    case SRL_LEXL_SID:
      return new Lexical;
      break;
    case SRL_QUAL_SID:
      return new Qualified;
      break;
    case SRL_FORM_SID:
      return new Form;
      break;
    case SRL_CNTR_SID:
      return new Counter;
      break;
    case SRL_PMIS_SID:
      return new Promise;
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid engine sid code",
		     Utility::tohexa (sid, true, true));
  }
  
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_ENG, srl_deod_eng);
}

#endif
