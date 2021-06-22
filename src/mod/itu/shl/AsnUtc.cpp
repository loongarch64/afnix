// ---------------------------------------------------------------------------
// - AsnUtc.cpp                                                              -
// - afnix:itu module - asn utc time node class implementation               -
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

#include "Date.hpp"
#include "Regex.hpp"
#include "Vector.hpp"
#include "AsnUtc.hpp"
#include "AsnNode.hxx"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the base regex
  static const String ASN_UTC_BASE = 
    "($d$d)($d$d)($d$d)($d$d)($d$d)([$d$d]?)";
  // the zone part regex
  static const String ASN_UTC_ZONE = "([<+->$d$d$d$d]?)";
  // the zulu part regex
  static const String ASN_UTC_ZULU = "([Z]?)";

  // check if the zulu flag is set
  static bool asn_get_zulu (const Regex& re) {
    // check the regex length
    if (re.length () != 7) {
      throw Exception ("asn-error", "invalid internal time regex object");
    }
    // check if we have the zulu time
    String zval = re.getstring (6);
    return (zval == 'Z');
  }

  // check if the seconds flag is set
  static bool asn_get_sflg (const Regex& re) {
    // check the regex length
    if (re.length () != 7) {
      throw Exception ("asn-error", "invalid internal time regex object");
    }
    // check if we have the seconds
    String sval = re.getstring (5);
    return (sval.isnil () == false);
  }

  // convert a regex value into a time
  static t_long asn_get_tclk (const Regex& re) {
    // get the zulu flag
    bool zulu = asn_get_zulu (re);
    // get the seconds flag
    bool sflg = asn_get_sflg (re);
    // get the date component2
    long year = re.getlong (0);
    if (year < 50) {
      year += 2000;
    } else {
      year += 1900;
    }
    long ymon = re.getlong (1);
    long mday = re.getlong (2);
    long hour = re.getlong (3);
    long mins = re.getlong (4);
    long secs = sflg ? re.getlong (5): 0L;
    // create a date representation
    Date date (year, ymon, mday, hour, mins, secs);
    // extract final value
    return date.gettime (zulu);
  }

  // convert a regex value into a zone time
  static t_long asn_get_zclk (const Regex& re) {
    // get the seconds flag
    bool sflg = asn_get_sflg (re);
    // get the date component2
    long year = re.getlong (0);
    if (year < 50) {
      year += 2000;
    } else {
      year += 1900;
    }
    long ymon = re.getlong (1);
    long mday = re.getlong (2);
    long hour = re.getlong (3);
    long mins = re.getlong (4);
    long secs = sflg ? re.getlong (5) : 0L;
    // create a date representation
    Date date (year, ymon, mday, hour, mins, secs);
    // extract final value
    return date.gettime (true);
  }

  // convert a regex value into a zone part
  static long asn_get_zone (const Regex& re) {
    // check for existence
    String zval = re.getstring (6);
    if (zval.isnil () == true) return 0L;
    // extract the zone component
    const Regex zre ("<+->($d$d)($d$d)");
    if (zre != zval) {
      throw Exception ("asn-error", "invalid internal zone object", zval);
    }
    long result = zre.getlong (0) * 3600 + zre.getlong (1) * 60;
    return (zval[0] == '-') ? -result : result;
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnUtc::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get a string representation
      String sval = tostring ();
      // write the string
      switch (encr) {
      case ASN_BER:
      case ASN_CER:
      case ASN_DER:
	buf.add (sval);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnUtc::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get a string representation
      String sval = tostring ();
      // write the string
      switch (encr) {
      case ASN_BER:
      case ASN_CER:
      case ASN_DER:
	os.write (sval);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn utc time node

  AsnUtc::AsnUtc (void) : AsnNode (ASN_UNIV_UTCS) {
    reset ();
  }
  
  // create an asn utc time by value

  AsnUtc::AsnUtc (const String& sval) : AsnNode (ASN_UNIV_UTCS) {
    // reset the node
    reset ();
    // set the date
    setdate (sval);
  }

  // create an asn utc time by node and buffer

  AsnUtc::AsnUtc (const AsnNode& node, const Buffer& cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_UTCS) || (d_cstf != false) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn utc time node state");
    }
    // convert the buffer to a string
    String sval = cbuf.tostring ();
    // set the date
    setdate (sval);
  }

  // copy construct this asn utc time node

  AsnUtc::AsnUtc (const AsnUtc& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_tclk = that.d_tclk;
      d_utcf = that.d_utcf;
      d_sflg = that.d_sflg;
      d_zflg = that.d_zflg;
      d_zone = that.d_zone;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnUtc::repr (void) const {
    return "AsnUtc";
  }

  // return a clone of this object

  Object* AsnUtc::clone (void) const {
    return new AsnUtc (*this);
  }
  
  // assign an asn utc time to this one

  AsnUtc& AsnUtc::operator = (const AsnUtc& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_tclk = that.d_tclk;
      d_utcf = that.d_utcf;
      d_sflg = that.d_sflg;
      d_zflg = that.d_zflg;
      d_zone = that.d_zone;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this asn utc time node

  void AsnUtc::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and values
      d_tagn = ASN_UNIV_UTCS;
      // get the utc time
      d_tclk = Time::gettclk ();
      d_utcf = true;
      d_sflg = true;
      d_zflg = false;
      d_zone = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn utc time content length

  t_long AsnUtc::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // get a string representation
      String sval = tostring ();
      // extract length
      t_long result = sval.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the utc flag is set

  bool AsnUtc::isutc (void) const {
    rdlock ();
    try {
      bool result = d_utcf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the utc time node by value

  void AsnUtc::setdate (const String& sval) {
    wrlock ();
    try {
      char* sbuf = sval.tochar ();
      if (sbuf == nullptr) sbuf = nullptr;
      // check for local time or zulu
      Regex lre (ASN_UTC_BASE + ASN_UTC_ZULU);
      if (lre == sval) {
	d_tclk = asn_get_tclk (lre);
	d_utcf = asn_get_zulu (lre);
	d_sflg = asn_get_sflg (lre);
	d_zflg = false;
	d_zone = d_utcf ? 0L : (long) Time::getzone ();
	unlock ();
	return;
      }
      // check for time difference
      Regex dre (ASN_UTC_BASE + ASN_UTC_ZONE);
      if (dre == sval) {
	// get the time components
	d_tclk = asn_get_zclk (dre);
	d_utcf = false;
	d_sflg = asn_get_sflg (dre);
	d_zflg = true;
	d_zone = asn_get_zone (dre);
	// adjust the zone
	d_tclk += d_zone;
	unlock ();
	return;
      }
      // invalid format
      throw Exception ("asn-error", "invalid utc time", sval);
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a time representation

  t_long AsnUtc::totime (void) const {
    rdlock ();
    try {
      t_long result = d_tclk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // get a string representation

  String AsnUtc::tostring (void) const {
    rdlock ();
    try {
      // compute wall clock with zone
      t_long wclk = d_tclk;
      if (d_utcf == false) wclk -= d_zone;
      // format the date
      String result = Date::todfmt (wclk, "", "", "");
      // remove first 2 years digits
      result = result.rsubstr (2);
      // eventually remove the seconds
      if (d_sflg == false) result = result.lsubstr (10);
      // add the zulu part if needed
      if (d_utcf == true) result += "Z";
      // add the zone part if needed
      if (d_zflg == true) result += Time::tozfmt (d_zone, "");
      // unlock and return
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_UTCP     = zone.intern ("utc-p");
  static const long QUARK_TOTIME   = zone.intern ("to-time");
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way
 
  Object* AsnUtc::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnUtc;
    // check for 1 argument
    if (argc == 1) {
      String sval = argv->getstring (0);
      return new AsnUtc (sval);
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn utc time constructor");
  }

  // return true if the given quark is defined

  bool AsnUtc::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? AsnNode::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* AsnUtc::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_UTCP)     return new Boolean (isutc ());
      if (quark == QUARK_TOTIME)   return new Integer (totime ());
      if (quark == QUARK_TOSTRING) return new String  (tostring ());
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
