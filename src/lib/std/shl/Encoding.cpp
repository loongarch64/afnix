// ---------------------------------------------------------------------------
// - Encoding.cpp                                                            -
// - standard object library - encoding system class implementation          -
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

#include "Encoding.hpp"
#include "Exception.hpp"
#include "cloc.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the byte mode encoding
  static const t_byte ENC_BYTE_COD = 0x01;
  static const t_byte ENC_UTF8_COD = 0x08;
  
  // the natural encoding
  static const String ENC_MODE_DEF = "DEFAULT";

  // the natural encoding
  static const String DEF_MODE_UTF = "UTF-8";

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // get the encoding code by mode

  t_byte Encoding::tocode (const t_emod emod) {
    t_byte result = 0x00;
    switch (emod) {
    case EMOD_BYTE:
      result = ENC_BYTE_COD;
      break;
    case EMOD_UTF8:
      result = ENC_UTF8_COD;
      break;
    }
    return result;
  }

  // get the encoding mode by code

  Encoding::t_emod Encoding::toemod (const t_byte code) {
    switch (code) {
    case ENC_BYTE_COD:
      return EMOD_BYTE;
      break;
    case ENC_UTF8_COD:
      return EMOD_UTF8;
      break;
    default:
      break;
    }
    throw Exception ("encoding-error", "invalid encoding code to map");
  }
  
  // map the encoding mode by name

  Encoding::t_emod Encoding::toemod (const String& mode) {
    // map to upper case
    String umod = mode.toupper ();
    // check for utf mode
    if ((umod == "UTF-8") || (umod == "UTF_8")) return Encoding::EMOD_UTF8;
    // default to byte mode
    return Encoding::EMOD_BYTE;
  }

  // get the transcoding mode by name

  Encoding::t_tmod Encoding::totmod (const String& mode) {
    // map to upper case
    String umod = mode.toupper ();
    t_tmod tmod = TMOD_NONE;
    if ((umod == "DEFAULT") ||
	(umod == "ASCII")) {
      tmod = TMOD_NONE;
    } else if ((umod == "ISO-8859-1") ||
	       (umod == "ISO_8859_1")) {
      tmod = Encoding::TMOD_5901;
    } else if ((umod == "ISO-8859-2") ||
	       (umod == "ISO_8859_2")) {
      tmod = Encoding::TMOD_5902;
    } else if ((umod == "ISO-8859-3") ||
	       (umod == "ISO_8859_3")) {
      tmod = Encoding::TMOD_5903;
    } else if ((umod == "ISO-8859-4") ||
	       (umod == "ISO_8859_4")) {
      tmod = Encoding::TMOD_5904;
    } else if ((umod == "ISO-8859-5") ||
	       (umod == "ISO_8859_5")) {
      tmod = Encoding::TMOD_5905;
    } else if ((umod == "ISO-8859-6") ||
	       (umod == "ISO_8859_6")) {
      tmod = Encoding::TMOD_5906;
    } else if ((umod == "ISO-8859-7") ||
	       (umod == "ISO_8859_7")) {
      tmod = Encoding::TMOD_5907;
    } else if ((umod == "ISO-8859-8") ||
	       (umod == "ISO_8859_8")) {
      tmod = Encoding::TMOD_5908;
    } else if ((umod == "ISO-8859-9") ||
	       (umod == "ISO_8859_9")) {
      tmod = Encoding::TMOD_5909;
    } else if ((umod == "ISO-8859-10") ||
	       (umod == "ISO_8859_10")) {
      tmod = Encoding::TMOD_5910;
    } else if ((umod == "ISO-8859-11") ||
	       (umod == "ISO_8859_11")) {
      tmod = Encoding::TMOD_5911;
    } else if ((umod == "ISO-8859-13") ||
	       (umod == "ISO_8859_13")) {
      tmod = Encoding::TMOD_5913;
    } else if ((umod == "ISO-8859-14") ||
	       (umod == "ISO_8859_14")) {
      tmod = Encoding::TMOD_5914;
    } else if ((umod == "ISO-8859-15") ||
	       (umod == "ISO_8859_15")) {
      tmod = Encoding::TMOD_5915;
    } else if ((umod == "ISO-8859-16") ||
	       (umod == "ISO_8859_16")) {
      tmod = Encoding::TMOD_5916;
    } else if ((umod == "UTF-8") ||
	       (umod == "UTF_8")) {
      tmod = Encoding::TMOD_NONE;
    } else {
      throw Exception ("mode-error", "invalid transcoding mode", mode);
    }
    return tmod;
  }

  // get the system encoding mode

  Encoding::t_emod Encoding::getsem (void) {
    // get the system codeset
    t_codeset codeset = c_getcset ();
    // map the code set
    switch (codeset) {
    case LOC_ASC_00:
    case LOC_ISO_01:
    case LOC_ISO_02:
    case LOC_ISO_03:
    case LOC_ISO_04:
    case LOC_ISO_05:
    case LOC_ISO_06:
    case LOC_ISO_07:
    case LOC_ISO_08:
    case LOC_ISO_09:
    case LOC_ISO_10:
    case LOC_ISO_11:
    case LOC_ISO_13:
    case LOC_ISO_14:
    case LOC_ISO_15:
    case LOC_ISO_16:
      return EMOD_BYTE;
      break;
    case LOC_UTF_08:
      return EMOD_UTF8;
      break;
    default:
      break;
    }
    return EMOD_BYTE;
  }

  // get the system transcoding mode

  Encoding::t_tmod Encoding::getstm (void) {
    // get the system codeset
    t_codeset codeset = c_getcset ();
    // map the code set
    switch (codeset) {
    case LOC_ASC_00:
      return TMOD_NONE;
      break;
    case LOC_ISO_01:
      return TMOD_5901;
      break;
    case LOC_ISO_02:
      return TMOD_5902;
      break;
    case LOC_ISO_03:
      return TMOD_5903;
      break;
    case LOC_ISO_04:
      return TMOD_5904;
      break;
    case LOC_ISO_05:
      return TMOD_5905;
      break;
    case LOC_ISO_06:
      return TMOD_5906;
      break;
    case LOC_ISO_07:
      return TMOD_5907;
      break;
    case LOC_ISO_08:
      return TMOD_5908;
      break;
    case LOC_ISO_09:
      return TMOD_5909;
      break;
    case LOC_ISO_10:
      return TMOD_5910;
      break;
    case LOC_ISO_11:
      return TMOD_5911;
      break;
    case LOC_ISO_13:
      return TMOD_5913;
      break;
    case LOC_ISO_14:
      return TMOD_5914;
      break;
    case LOC_ISO_15:
      return TMOD_5915;
      break;
    case LOC_ISO_16:
      return TMOD_5916;
      break;
    default:
      return TMOD_NONE;
      break;
    }
    return TMOD_NONE;
  }

  // get the natural encoding mode

  String Encoding::getdem (void) {
    return ENC_MODE_DEF;
  }
  
  // get the natural encoding mode
  
  String Encoding::getnem (void) {
    return DEF_MODE_UTF;
  }
}
