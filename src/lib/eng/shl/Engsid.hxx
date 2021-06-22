// ---------------------------------------------------------------------------
// - Engsid.hxx                                                              -
// - afnix:eng - engine serial id definition                                 -
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

#ifndef  AFNIX_ENGSID_HXX
#define  AFNIX_ENGSID_HXX

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  // engine serial id [0000 0000 0000 0001][@0x0001U]
  static const t_word SRL_RESV_SID = 0x0000U; // reserved  id
  static const t_word SRL_CNST_SID = 0x0001U; // constant  id
  static const t_word SRL_LEXL_SID = 0x0002U; // lexical   id
  static const t_word SRL_QUAL_SID = 0x0003U; // qualified id
  static const t_word SRL_FORM_SID = 0x0004U; // form      id
  static const t_word SRL_CNTR_SID = 0x0005U; // counter   id
  static const t_word SRL_PMIS_SID = 0x0006U; // promise   id

  // the engine dispatch id
  static const t_word SRL_DEOD_ENG = 0x0001U;
}

#endif
