// ---------------------------------------------------------------------------
// - Locale.cpp                                                              -
// - standard object library - locale class implementation                   -
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

#include "Real.hpp"
#include "Date.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Strvec.hpp"
#include "Locale.hpp"
#include "Utility.hpp"
#include "Unicode.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputFile.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default locale name, info ands index
  static const String LC_NAME_DEF = "";
  static const String LC_INFO_DEF = "";
  static const long   LC_LIDX_DEF = -1L;
  // the default decimal characters
  static const t_quad LC_DPNT_DEF = '.';
  static const t_quad LC_DSEP_DEF = ' ';
  // the default date format
  static const Locale::t_dfmt LC_DFMT_DEF = Locale::DFMT_YYYYMMDD;
  static const t_quad LC_DMRK_DEF = '-';
  static const String LC_DRGX_DEF = "($d$d$d$d)-($d$d)-($d$d)";
  // the default object format/modifier
  static const Locale::t_ofmt LC_OFMT_DEF = Locale::OFMT_IDEM;
  static const Locale::t_omod LC_OMOD_DEF = Locale::OMOD_NONE;
  
  // the locale parameters separators
  static const String LC_PSEP_BRK = "&";
  static const String LC_PEQL_BRK = "=";
  static const String LC_NAME_PRM = "N";
  static const String LC_INFO_PRM = "I";  
  static const String LC_LIDX_PRM = "L";  
  static const String LC_DPNT_PRM = "D";
  static const String LC_DSEP_PRM = "S";
  static const String LC_DFMT_PRM = "C";
  static const String LC_DMRK_PRM = "M";
  static const String LC_OFMT_PRM = "O";
  static const String LC_OMOD_PRM = "X";

  // this procedure maps a string into a date format
  static Locale::t_dfmt lc_todfmt (const String& s) {
    if (s == "YYMMDD")   return Locale::DFMT_YYMMDD;
    if (s == "DDMMYY")   return Locale::DFMT_DDMMYY;
    if (s == "YYYYMMDD") return Locale::DFMT_YYYYMMDD;
    if (s == "DDMMYYYY") return Locale::DFMT_DDMMYYYY;
    throw Exception ("locale-error", "cannot map date format", s);
  }
  
  // this procedure maps a date format into a string
  static String lc_tostring (const Locale::t_dfmt dfmt) {
    String result;
    switch (dfmt) {
    case Locale::DFMT_YYMMDD:
      result = "YYMMDD";
      break;
    case Locale::DFMT_DDMMYY:
      result = "DDMMYY";
      break;
    case Locale::DFMT_YYYYMMDD:
      result = "YYYYMMDD";
      break;
    case Locale::DFMT_DDMMYYYY:
      result = "DDMMYYYY";
      break;
    }
    return result;
  }

  // this procedure maps an object format into a string
  static String lc_tostring (const Locale::t_ofmt ofmt) {
    String result;
    switch (ofmt) {
    case Locale::OFMT_IDEM:
      result = "IDEM";
      break;
    case Locale::OFMT_INTG:
      result = "INTG";
      break;
    case Locale::OFMT_REAL:
      result = "REAL";
      break;
    case Locale::OFMT_DATE:
      result = "DATE";
      break;
    }
    return result;
  }
  
  // this procedure maps an object format into a string
  static String lc_tostring (const Locale::t_omod omod) {
    String result;
    switch (omod) {
    case Locale::OMOD_NONE:
      result = "NONE";
      break;
    case Locale::OMOD_SHRT:
      result = "SHRT";
      break;
    }
    return result;
  }
  
  // this procedure creates a date regex by format and marker
  static Regex lc_toregex (const Locale::t_dfmt dfmt, const t_quad dmrk) {
    String result;
    switch (dfmt) {
    case Locale::DFMT_YYMMDD:
    case Locale::DFMT_DDMMYY:
      result  = "($d$d)";
      result += dmrk;
      result += "($d$d)";
      result += dmrk;
      result += "($d$d)";
      break;
    case Locale::DFMT_YYYYMMDD:
      result  = "($d$d$d$d)";
      result += dmrk;
      result += "($d$d)";
      result += dmrk;
      result += "($d$d)";
      break;
    case Locale::DFMT_DDMMYYYY:
      result  = "($d$d)";
      result += dmrk;
      result += "($d$d)";
      result += dmrk;
      result += "($d$d$d$d)";
      break;
    }
    return Regex (result);
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // get an object format by string
  
  Locale::t_ofmt Locale::toofmt (const String& sval) {
    if (sval == "IDEM") return Locale::OFMT_IDEM;
    if (sval == "INTG") return Locale::OFMT_INTG;
    if (sval == "REAL") return Locale::OFMT_REAL;
    if (sval == "DATE") return Locale::OFMT_DATE;
    throw Exception ("locale-error", "cannot map object format", sval);
  }
  
  // get an object modifier by string
  Locale::t_omod Locale::toomod (const String& sval) {
    if (sval == "NONE")     return Locale::OMOD_NONE;
    if (sval == "SHRT")     return Locale::OMOD_SHRT;
    throw Exception ("locale-error", "cannot map object modifier", sval);
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a new locale
  
  Locale::Locale (void) {
    reset ();
  }

  // create a new locale by format

  Locale::Locale (const String& s) {
    setprm (s);
  }
  
  // copy construct this locale
  
  Locale::Locale (const Locale& that) {
    that.rdlock ();
    try {
      // copy base locale
      Taggable::operator = (that);
      // copy locally
      d_lidx = that.d_lidx;
      d_dpnt = that.d_dpnt;
      d_dsep = that.d_dsep;
      d_dfmt = that.d_dfmt;
      d_dmrk = that.d_dmrk;
      d_drgx = that.d_drgx;
      d_ofmt = that.d_ofmt;
      d_omod = that.d_omod;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a locale to this one
  
  Locale& Locale::operator = (const Locale& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base locale
      Taggable::operator = (that);
      d_lidx = that.d_lidx;
      d_dpnt = that.d_dpnt;
      d_dsep = that.d_dsep;
      d_dfmt = that.d_dfmt;
      d_dmrk = that.d_dmrk;
      d_drgx = that.d_drgx;
      d_ofmt = that.d_ofmt;
      d_omod = that.d_omod;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Locale::repr (void) const {
    return "Locale";
  }
  
  // return a clone of this object

  Object* Locale::clone (void) const {
    return new Locale (*this);
  }

  // return the locale did

  t_word Locale::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the locale sid

  t_word Locale::getsid (void) const {
    return SRL_LOCL_SID;
  }

  // serialize this locale

  void Locale::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the taggable
      Taggable::wrstream (os);
      // save the message type
      Serial::wrlong (d_lidx, os);
      Serial::wrchar (d_dpnt, os);
      Serial::wrchar (d_dsep, os);
      Serial::wrbyte (d_dfmt, os);
      Serial::wrchar (d_dmrk, os);
      d_drgx.wrstream (os);
      Serial::wrbyte (d_ofmt, os);
      Serial::wrbyte (d_omod, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this locale

  void Locale::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the taggable
      Taggable::rdstream (is);
      // deserialize locally
      d_lidx = Serial::rdlong (is);
      d_dpnt = Serial::rdchar (is);
      d_dsep = Serial::rdchar (is);
      d_dfmt = static_cast <t_dfmt>(Serial::rdbyte (is));
      d_dmrk = Serial::rdchar (is);
      d_drgx.rdstream (is);
      d_ofmt = static_cast<t_ofmt>(Serial::rdbyte (is));
      d_omod = static_cast<t_omod>(Serial::rdbyte (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this locale

  void Locale::reset (void) {
    wrlock ();
    try {
      // locale defition
      d_name = LC_NAME_DEF;
      d_info = LC_INFO_DEF;
      d_lidx = LC_LIDX_DEF;
      // locale decimal
      d_dpnt = LC_DPNT_DEF;
      d_dsep = LC_DSEP_DEF;
      // locale date
      d_dfmt = LC_DFMT_DEF;
      d_dmrk = LC_DMRK_DEF;
      d_drgx = lc_toregex (d_dfmt, d_dmrk);
      // locale object default
      d_ofmt = LC_OFMT_DEF;
      d_omod = LC_OMOD_DEF;
      // youp la boum
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the locale index
  
  void Locale::setlidx (const long lidx) {
    rdlock ();
    try {
      d_lidx = lidx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the locale index
  
  long Locale::getlidx (void) const {
    rdlock ();
    try {
      long result = d_lidx;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the locale decimal point
  
  void Locale::setdpnt (const t_quad dpnt) {
    rdlock ();
    try {
      d_dpnt = dpnt;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the locale decimal point
  
  t_quad Locale::getdpnt (void) const {
    rdlock ();
    try {
      t_quad result = d_dpnt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the locale decimal separator
  
  void Locale::setdsep (const t_quad dsep) {
    rdlock ();
    try {
      d_dsep = dsep;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the locale decimal separator
  
  t_quad Locale::getdsep (void) const {
    rdlock ();
    try {
      t_quad result = d_dsep;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the locale date format
  
  void Locale::setdfmt (const String& dfmt) {
    rdlock ();
    try {
      d_dfmt = lc_todfmt  (dfmt);
      d_drgx = lc_toregex (d_dfmt, d_dmrk);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the locale date format
  
  String Locale::getdfmt (void) const {
    rdlock ();
    try {
      String result = lc_tostring (d_dfmt);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the locale date marker
  
  void Locale::setdmrk (const t_quad dmrk) {
    rdlock ();
    try {
      d_dmrk = dmrk;
      d_drgx = lc_toregex (d_dfmt, d_dmrk);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the locale decimal point
  
  t_quad Locale::getdmrk (void) const {
    rdlock ();
    try {
      t_quad result = d_dmrk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the locale object format
  
  void Locale::setofmt (const String& ofmt) {
    rdlock ();
    try {
      d_ofmt = toofmt (ofmt);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the locale object format
  
  String Locale::getofmt (void) const {
    rdlock ();
    try {
      String result = lc_tostring (d_ofmt);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the locale object modifier
  
  void Locale::setomod (const String& omod) {
    rdlock ();
    try {
      d_omod = toomod (omod);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the locale object modifier
  
  String Locale::getomod (void) const {
    rdlock ();
    try {
      String result = lc_tostring (d_omod);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the locale parameters

  void Locale::setprm (const String& lp) {
    wrlock ();
    try {
      // reset the locale
      reset ();
      // separate the definition
      Strvec pv = Strvec::split (lp, LC_PSEP_BRK);
      long plen = pv.length  ();
      // loop in the settings
      for (long k = 0L; k < plen; k++) {
	String pd = pv.get (k);
	Strvec dv = Strvec::split (pd, LC_PEQL_BRK);
	if (dv.length () != 2) {
	  throw Exception ("locale-error",
			   "invalid locale parameter definition", pd);
	}
	// extract parameter name and value
	String pn = dv.get (0);
	String pv = dv.get (1);
	// check for a name
	if (pn == LC_NAME_PRM) setname (pv);
	if (pn == LC_INFO_PRM) setinfo (pv);
	if (pn == LC_LIDX_PRM) setlidx ((long) Utility::tolong (pv));
	if (pn == LC_DPNT_PRM) {
	  if (pv.length () != 1) {
	    throw Exception ("locale-error", "invalid decimal point", pv);
	  }
	  setdpnt (pv[0]);
	}
	if (pn == LC_DSEP_PRM) {
	  if (pv.length () != 1) {
	    throw Exception ("locale-error", "invalid decimal separator", pv);
	  }
	  setdsep (pv[0]);
	}
	if (pn == LC_DFMT_PRM) setdfmt (pv);
	if (pn == LC_DMRK_PRM) {
	  if (pv.length () != 1) {
	    throw Exception ("locale-error", "invalid date marker", pv);
	  }
	  setdmrk (pv[0]);
	}
	if (pn == LC_OFMT_PRM) setofmt (pv);
	if (pn == LC_OMOD_PRM) setomod (pv);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the locale parameters

  String Locale::getprm (void) const {
    rdlock ();
    try {
      String result;
      // check for name
      if (d_name.isnil () == false) {
	if (result.isnil () == false) result += '&';
	result += LC_NAME_PRM; result += '='; result += d_name;
      }
      // check for info
      if (d_info.isnil () == false) {
	if (result.isnil () == false) result += '&';
	result += LC_INFO_PRM; result += '='; result += d_info;
      }
      // check for locale index
      if (d_lidx != LC_LIDX_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_LIDX_PRM; result += '=';
	result += Utility::tostring (d_lidx);
      }	
      // check for decimal point
      if (d_dpnt != LC_DPNT_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_DPNT_PRM; result += '='; result += d_dpnt;
      }
      // check for decimal separator
      if (d_dsep != LC_DSEP_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_DSEP_PRM; result += '='; result += d_dsep;
      }
      // check for date format
      if (d_dfmt != LC_DFMT_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_DFMT_PRM; result += '='; result += lc_tostring (d_dfmt);
      }
      // check for date marker
      if (d_dmrk != LC_DMRK_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_DMRK_PRM; result += '='; result += d_dmrk;
      }
      // check for object format
      if (d_ofmt != LC_OFMT_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_OFMT_PRM; result += '='; result += lc_tostring (d_ofmt);
      }
      // check for object modifier
      if (d_omod != LC_OMOD_DEF) {
	if (result.isnil () == false) result += '&';
	result += LC_OMOD_PRM; result += '='; result += lc_tostring (d_omod);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // convert a locale string to a real

  t_real Locale::toreal (const String& s) const {
    rdlock ();
    t_quad* sbuf = nullptr;
    try {
      // get a quad copy
      sbuf = s.toquad ();
      long len = Unicode::strlen (sbuf);
      // convert the buffer
      long k = 0L;
      for (long i = 0L; i < len; i++) {
	if (sbuf[i] == d_dsep) continue;
	sbuf[k++] = (sbuf[i] == d_dpnt) ? '.' : sbuf[i];
      }
      for (long i = k; i < len; i++) sbuf[i] = nilq;
      // make a string and convert it to a real
      String sval (sbuf); delete [] sbuf;
      t_real result = Utility::toreal (sval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert a locale string to a date

  t_long Locale::todate (const String& s) const {
    rdlock ();
    try {
      long year = 0L; long ymon = 0L; long mday = 0L;
      Regex rgx = d_drgx;
      switch (d_dfmt) {
      case DFMT_YYMMDD:
	if (rgx != s) {
	  throw Exception ("locale-error", "invalid date format", s);
	}
	year = rgx.getlong (0);
	year += (year < 50L) ? 2000L : 1900L;
	ymon = rgx.getlong (1);
	mday = rgx.getlong (2);
	break;
      case DFMT_DDMMYY:
	if (rgx != s) {
	  throw Exception ("locale-error", "invalid date format", s);
	}
	mday = rgx.getlong (0);
	ymon = rgx.getlong (1);
	year = rgx.getlong (2);
	year += (year < 50L) ? 2000L : 1900L;
	break;
      case DFMT_YYYYMMDD:
	if (rgx != s) {
	  throw Exception ("locale-error", "invalid date format", s);
	}
	year = rgx.getlong (0);
	ymon = rgx.getlong (1);
	mday = rgx.getlong (2);
	break;
      case DFMT_DDMMYYYY:
	if (rgx != s) {
	  throw Exception ("locale-error", "invalid date format", s);
	}
	mday = rgx.getlong (0);
	ymon = rgx.getlong (1);
	year = rgx.getlong (2);
	break;
      }
      // create a date and compute time
      t_long result = Date(year, ymon, mday).gettime (true);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert a string to an object

  Object* Locale::toobject (const String& s) const {
    rdlock();
    try {
      Object* result = nullptr;
      switch (d_ofmt) {
      case OFMT_IDEM:
	result = new String (s);
	break;
      case OFMT_INTG:
	result = new Integer (Utility::tolong (s));
	break;
      case OFMT_REAL:
	result = new Real (toreal (s));
	break;
      case OFMT_DATE:
	result = new Integer (todate (s));
	break;	
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 22;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_TOREAL  = zone.intern ("to-real");
  static const long QUARK_TODATE  = zone.intern ("to-date");
  static const long QUARK_SETPRM  = zone.intern ("set-parameters");
  static const long QUARK_GETPRM  = zone.intern ("get-parameters");
  static const long QUARK_SETNAME = zone.intern ("set-name");
  static const long QUARK_SETINFO = zone.intern ("set-info");
  static const long QUARK_GETINFO = zone.intern ("get-info");
  static const long QUARK_SETLIDX = zone.intern ("set-index");
  static const long QUARK_GETLIDX = zone.intern ("get-index");
  static const long QUARK_SETDFMT = zone.intern ("set-date-format");
  static const long QUARK_GETDFMT = zone.intern ("get-date-format");
  static const long QUARK_SETDMRK = zone.intern ("set-date-marker");
  static const long QUARK_GETDMRK = zone.intern ("get-date-marker");
  static const long QUARK_SETDPNT = zone.intern ("set-decimal-point");
  static const long QUARK_GETDPNT = zone.intern ("get-decimal-point");
  static const long QUARK_SETDSEP = zone.intern ("set-decimal-separator");
  static const long QUARK_GETDSEP = zone.intern ("get-decimal-separator");
  static const long QUARK_SETOFMT = zone.intern ("set-object-format");
  static const long QUARK_GETOFMT = zone.intern ("get-object-format");
  static const long QUARK_SETOMOD = zone.intern ("set-object-modifier");
  static const long QUARK_GETOMOD = zone.intern ("get-object-modifier");

  // create a new object in a generic way
  
  Object* Locale::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Locale;
    // check 1 argument
    if (argc == 1) {
      String s = argv->getstring (0);
      return new Locale (s);
    }
    throw Exception ("argument-error", "too many argument for locale");
  }

  // return true if the given quark is defined
  
  bool Locale::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object class with a set of arguments and a quark
  
  Object* Locale::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETDPNT) return new Character (getdpnt ());
      if (quark == QUARK_GETDSEP) return new Character (getdsep ());
      if (quark == QUARK_GETDMRK) return new Character (getdmrk ());
      if (quark == QUARK_GETLIDX) return new Integer (getlidx ());
      if (quark == QUARK_GETINFO) return new String  (getinfo ());
      if (quark == QUARK_GETDFMT) return new String  (getdfmt ());
      if (quark == QUARK_GETOFMT) return new String  (getofmt ());
      if (quark == QUARK_GETOMOD) return new String  (getomod ());
      if (quark == QUARK_GETPRM)  return new String  (getprm  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_TOREAL) {
	String s = argv->getstring (0);
	return new Real (toreal (s));
      }
      if (quark == QUARK_TODATE) {
	String s = argv->getstring (0);
	return new Integer (todate (s));
      }
      if (quark == QUARK_SETPRM) {
	String lp = argv->getstring (0);
	setprm (lp);
	return nullptr;
      }
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETINFO) {
	String info = argv->getstring (0);
	setinfo (info);
	return nullptr;
      }
      if (quark == QUARK_SETLIDX) {
	long lidx = argv->getlong (0);
	setlidx (lidx);
	return nullptr;
      }
      if (quark == QUARK_SETDPNT) {
	t_quad dpnt = argv->getchar (0);
	setdpnt (dpnt);
	return nullptr;
      }
      if (quark == QUARK_SETDSEP) {
	t_quad dsep = argv->getchar (0);
	setdsep (dsep);
	return nullptr;
      }
      if (quark == QUARK_SETDFMT) {
	String dfmt = argv->getstring (0);
	setdfmt (dfmt);
	return nullptr;
      }
      if (quark == QUARK_SETDMRK) {
	t_quad dmrk = argv->getchar (0);
	setdsep (dmrk);
	return nullptr;
      }
      if (quark == QUARK_SETOFMT) {
	String ofmt = argv->getstring (0);
	setofmt (ofmt);
	return nullptr;
      }
      if (quark == QUARK_SETOMOD) {
	String omod = argv->getstring (0);
	setomod (omod);
	return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
