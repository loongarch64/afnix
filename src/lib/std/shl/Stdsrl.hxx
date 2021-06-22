// ---------------------------------------------------------------------------
// - Stdsrl.hxx                                                              -
// - afnix:std - standard serial decoding definition                         -
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

#ifndef  AFNIX_STDSRL_HXX
#define  AFNIX_STDSRL_HXX

#include "Set.hpp"
#include "Byte.hpp"
#include "Cons.hpp"
#include "List.hpp"
#include "Real.hpp"
#include "Regex.hpp"
#include "Plist.hpp"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Locale.hpp"
#include "Utility.hpp"
#include "Strfifo.hpp"
#include "Numeral.hpp"
#include "Relatif.hpp"
#include "Boolean.hpp"
#include "Message.hpp"
#include "Character.hpp"
#include "HashTable.hpp"
#include "NameTable.hpp"
#include "Structure.hpp"
#include "QuarkZone.hpp"
#include "PrintTable.hpp"
#include "InputStream.hpp"
#include "Stdsid.hxx"

namespace afnix {

  // the standard dispatch function
  static Serial* srl_deod_std (const t_word sid) {
    switch (sid) {
    case SRL_NILP_SID:
      return nullptr;
      break;
    case SRL_BOOL_SID:
      return new Boolean;
      break;
    case SRL_BYTE_SID:
      return new Byte;
      break;
    case SRL_EOSC_SID:
      return nullptr ;
      break;
    case SRL_INTG_SID:
      return new Integer;
      break;
    case SRL_RELT_SID:
      return new Relatif;
      break;
    case SRL_REAL_SID:
      return new Real;
      break;
    case SRL_CHAR_SID:
      return new Character;
      break;
    case SRL_STRG_SID:
      return new String;
      break;
    case SRL_REGX_SID:
      return new Regex;
      break;
    case SRL_BUFR_SID:
      return new Buffer;
      break;
    case SRL_CONS_SID:
      return new Cons;
      break;
    case SRL_VECT_SID:
      return new Vector;
      break;
    case SRL_OSET_SID:
      return new Set;
      break;
    case SRL_NTBL_SID:
      return new NameTable;
      break;
    case SRL_STRV_SID:
      return new Strvec;
      break;
    case SRL_PROP_SID:
      return new Property;
      break;
    case SRL_PLST_SID:
      return new Plist;
      break;
    case SRL_LIST_SID:
      return new List;
      break;
    case SRL_STRF_SID:
      return new Strfifo;
      break;
    case SRL_PTBL_SID:
      return new PrintTable;
      break;
    case SRL_MESG_SID:
      return new Message;
      break;
    case SRL_STYL_SID:
      return new Style;
      break;
    case SRL_NUMR_SID:
      return new Numeral;
      break;
    case SRL_HASH_SID:
      return new HashTable;
      break;
    case SRL_LOCL_SID:
      return new Locale;
      break;
    case SRL_STRC_SID:
      return new Structure;
      break;
    case SRL_OBLK_SID:
    case SRL_BBLK_SID:
    case SRL_LBLK_SID:
    case SRL_RBLK_SID:
    case SRL_RPT2_SID:
      throw Exception ("serial-error", "cannot map internal serial block");
      break;
    default:
      break;
    }
    throw Exception ("serial-error", "invalid standard sid code",
		     Utility::tohexa (sid, true, true));
  }
  // bind the dispatch function
  static t_word SRL_DEOD_DID = Serial::addsd (SRL_DEOD_STD, srl_deod_std);
}

#endif
