// ---------------------------------------------------------------------------
// - Cookie.cpp                                                              -
// - afnix:nwg module - http cookie class implementation                     -
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
#include "Strvec.hpp"
#include "Vector.hpp"
#include "Cookie.hpp"
#include "Utility.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default cookie version
  static const long   CKY_VERS_DEF = 0;
  // the default cookie path
  static const String CKY_PATH_DEF = "/";
  // the default cookie port
  static const long   CKY_PORT_DEF = -1;
  // the default cookie maximum age
  static const t_long CKY_MAGE_DEF = -1LL;
  // the default cookie expiration time
  static const t_long CKY_ETIM_DEF = 0LL;
  // the default cookie discard flag
  static const bool   CKY_DISF_DEF = false;
  // the default cookie secuer flag
  static const bool   CKY_SECF_DEF = false;
  // the default cookie http only flag
  static const bool   CKY_HNLY_DEF = false;
  
  // get the maximum age from the expiration time
  static t_long get_mage (const t_long etim) {
    // get the current time
    t_long ctim = Time::gettclk ();
    t_long mage = etim - ctim;
    if (mage < 0) mage = -1LL;
    return mage;
  }

  // format the date for a cookie expire time
  static String get_cookie_date (const t_long etim, const t_long mage) {
    // prepare for the time object
    Date date;
    // handle the 0 maximum age (for discard)
    if (mage == 0LL) {
      date.setdate (1970, 1, 1);
      return date.toweb ();
    }
    // process normal date
    if (etim == 0LL) {
      date.add (mage);
    } else {
      date.settime (etim);
    }
    // get the web date
    return date.toweb ();
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a cookie with a string header value

  Cookie::Cookie (const String& hval) {
    parse (hval);
  }

  // create a cookie with a name/value pair

  Cookie::Cookie (const String& name, const String& value) {
    reset ();
    d_name = name;
    d_cval = value;
  }

  // create a cookie with a name/value pair

  Cookie::Cookie (const String& name, const String& value, const t_long mage) {
    reset ();
    d_name = name;
    d_cval = value;
    d_mage = (mage < 0LL) ?  CKY_MAGE_DEF : mage;
  }

  // copy construct this cookie

  Cookie::Cookie (const Cookie& that) {
    that.rdlock ();
    try {
      d_vers = that.d_vers;
      d_name = that.d_name;
      d_cval = that.d_cval;
      d_path = that.d_path;
      d_hdom = that.d_hdom;
      d_port = that.d_port;
      d_comt = that.d_comt;
      d_curl = that.d_curl;
      d_mage = that.d_mage;
      d_etim = that.d_etim;
      d_disf = that.d_disf;
      d_secf = that.d_secf;
      d_hnly = that.d_hnly;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Cookie::repr (void) const {
    return "Cookie";
  }

  // return a  clone of this object

  Object* Cookie::clone (void) const {
    return new Cookie (*this);
  }

  // assign a cookie to this one

  Cookie& Cookie::operator = (const Cookie& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_vers = that.d_vers;
      d_name = that.d_name;
      d_cval = that.d_cval;
      d_path = that.d_path;
      d_hdom = that.d_hdom;
      d_port = that.d_port;
      d_comt = that.d_comt;
      d_curl = that.d_curl;
      d_mage = that.d_mage;
      d_etim = that.d_etim;
      d_disf = that.d_disf;
      d_secf = that.d_secf;
      d_hnly = that.d_hnly;
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this cookie to factory value

  void Cookie::reset (void) {
    wrlock ();
    try {
      d_vers = CKY_VERS_DEF;
      d_name = "";
      d_cval = "";
      d_path = CKY_PATH_DEF;
      d_hdom = "";
      d_port = CKY_PORT_DEF;
      d_comt = "";
      d_curl = "";
      d_mage = CKY_MAGE_DEF;
      d_etim = CKY_ETIM_DEF;
      d_disf = CKY_DISF_DEF;
      d_secf = CKY_SECF_DEF;
      d_hnly = CKY_HNLY_DEF;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // parse a string header value

  void Cookie::parse (const String& hval) {
    wrlock ();
    try {
      // reset everythnig
      reset  ();
      // check for nothing first
      if (hval.isnil () == true) {
	unlock ();
	return;
      }
      // split the string with individual components
      Strvec svec = Strvec::split (hval, ';');
      long   slen = svec.length ();
      // the property regex
      const Regex re ("($a+)=?($N*)");
      // loop in the proprties vectors
      for (long k = 0; k < slen; k++) {
	// get the stripped property
	String prop = svec.get(k).strip ();
	if (prop.isnil () == true) continue;
	// split the property name/value
	if (re != prop) {
	  throw Exception ("cookie-error", "invalid cookie property",prop);
	}
	// extract the name and value
	String pnam = re.getstring(0).strip().tolower();
	String pval = re.getstring(1).strip (" \t\"");
	// check for a version key
	if (pnam == "version") {
	  long vers = Utility::tolong (pval);
	  setvers (vers);
	  continue;
	}
	// check for a path key
	if (pnam == "path") {
	  setpath (pval);
	  continue;
	}
	// check for a domain key
	if (pnam == "domain") {
	  setdomain (pval);
	  continue;
	}
	// check for a port key
	if (pnam == "port") {
	  long port = Utility::tolong (pval);
	  setport (port);
	  continue;
	}
	// check for a comment key
	if (pnam == "comment") {
	  setcomt (pval);
	  continue;
	}
	// check for a comment url key
	if (pnam == "commenturl") {
	  setcurl (pval);
	  continue;
	}
	// check for an expiration time
	if (pnam == "expires") {
	  Date d; d.setweb (pval);
	  setetim (d.gettime (true));
	  continue;
	}
	// check for a discard flag key
	if (pnam == "discard") {
	  setdisf (true);
	  continue;
	}
	// check for a secure flag key
	if (pnam == "secure") {
	  setsecf (true);
	  continue;
	}
	// check for a http only flag key
	if (pnam == "httponly") {
	  sethnly (true);
	  continue;
	}
	// cookie property name/value pair
	d_name = re.getstring(0).strip();
	d_cval = re.getstring(1).strip().rmquote ();
	continue;
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a http cookie string representation

  String Cookie::tostring (void) const {
    rdlock ();
    try {
      // set cookie name/value pair
      if ((d_name.isnil () == true) || (d_cval.isnil () == true)) {
	unlock ();
	throw Exception ("cookie-error", "invalid cookie name or value");
      }
      String result = d_name.tostring () + '=' + d_cval.tostring ();
      // set the cookie version
      if (d_vers == 1) {
	result += "; Version=1";
      }
      // set the cookie expiration time (version 0) - expire with 0 mage
      if (((d_mage >= 0) || (d_etim > 0)) && (d_vers == 0)) {
	result = result + "; Expires=" + get_cookie_date (d_etim, d_mage);
      }
      // set cookie maximum age (version 1)
      if (d_vers == 1) {
	t_long mage = (d_etim == 0LL) ? d_mage : get_mage (d_etim);
	if (mage >= 0) result = result + "; Max-Age=" + mage;
      }
      // set cookie path
      if (d_path.isnil () == false) {
	result = result + "; Path=" + d_path.tostring ();
      }
      // set cookie domain
      if (d_hdom.isnil () == false) {
	result = result + "; Domain=" + d_hdom.tostring ();
      }
      // set cookie port (version 1)
      if ((d_port >= 0) && (d_vers == 1)) {
	result = result + "; Port=" + '"' + d_port + '"';
      }
      // set the cookie comment
      if (d_comt.isnil () == false) {
	result = result + "; Comment=" + d_comt.tostring ();
      }
      // set the cookie url comment
      if (d_curl.isnil () == false) {
	result = result + "; CommentURL=" + d_curl.tostring ();
      }
      // set the cookie discard flag (version 1)
      if ((d_disf == true) &&(d_vers == 1)) {
	result += "; Discard";
      }      
      // set the cookie secure flag
      if (d_secf == true) {
	result += "; Secure";
      }
      // set the cookie http only flag
      if (d_hnly == true) {
	result += "; HttpOnly";
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie version

  void Cookie::setvers (const long vers) {
    wrlock ();
    try {
      // check version
      if ((vers != 0) && (vers != 1)) {
	throw Exception ("cookie-error", "illegal cookie version");
      }
      d_vers = vers;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie version

  long Cookie::getvers (void) const {
    rdlock ();
    try {
      long result = d_vers;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie name
  
  void Cookie::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie name

  String Cookie::getname (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie value
  
  void Cookie::setvalue (const String& value) {
    wrlock ();
    try {
      d_cval = value;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie value

  String Cookie::getvalue (void) const {
    rdlock ();
    try {
      String result = d_cval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie maximum age
  
  void Cookie::setmage (const t_long mage) {
    wrlock ();
    try {
      d_mage = (mage < 0LL) ? -1LL : mage;
      d_etim = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie maximum age

  t_long Cookie::getmage (void) const {
    rdlock ();
    try {
      t_long result = d_mage;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie expire time
  
  void Cookie::setetim (const t_long etim) {
    wrlock ();
    try {
      d_mage = -1LL;
      d_etim = etim;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie expiration time

  t_long Cookie::getetim (void) const {
    rdlock ();
    try {
      t_long result = d_etim;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie path
  
  void Cookie::setpath (const String& path) {
    wrlock ();
    try {
      d_path = path;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie path

  String Cookie::getpath (void) const {
    rdlock ();
    try {
      String result = d_path;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie domain
  
  void Cookie::setdomain (const String& hdom) {
    wrlock ();
    try {
      d_hdom = hdom;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie domain

  String Cookie::getdomain (void) const {
    rdlock ();
    try {
      String result = d_hdom;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie port
  
  void Cookie::setport (const long port) {
    wrlock ();
    try {
      d_port = (port < 0) ? -1 : port;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie port

  long Cookie::getport (void) const {
    rdlock ();
    try {
      long result = d_port;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie comment
  
  void Cookie::setcomt (const String& comt) {
    wrlock ();
    try {
      d_comt = comt;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie comment

  String Cookie::getcomt (void) const {
    rdlock ();
    try {
      String result = d_comt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie comment url
  
  void Cookie::setcurl (const String& curl) {
    wrlock ();
    try {
      d_curl = curl;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie comment url

  String Cookie::getcurl (void) const {
    rdlock ();
    try {
      String result = d_curl;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie discard flag
  
  void Cookie::setdisf (const bool flag) {
    wrlock ();
    try {
      d_disf = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie discard flag

  bool Cookie::getdisf (void) const {
    rdlock ();
    try {
      bool result = d_disf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cookie secure flag
  
  void Cookie::setsecf (const bool secf) {
    wrlock ();
    try {
      d_secf = secf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie secure flag

  bool Cookie::getsecf (void) const {
    rdlock ();
    try {
      bool result = d_secf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the cookie http only flag
  
  void Cookie::sethnly (const bool hnly) {
    wrlock ();
    try {
      d_hnly = hnly;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cookie http only flag

  bool Cookie::gethnly (void) const {
    rdlock ();
    try {
      bool result = d_hnly;
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
  static const long QUARK_ZONE_LENGTH = 29;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_PARSE    = zone.intern ("parse");
  static const long QUARK_GETVERS  = zone.intern ("get-version");
  static const long QUARK_SETVERS  = zone.intern ("set-version");
  static const long QUARK_GETNAME  = zone.intern ("get-name");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETMAGE  = zone.intern ("get-max-age");
  static const long QUARK_SETMAGE  = zone.intern ("set-max-age");
  static const long QUARK_GETETIM  = zone.intern ("get-expire-time");
  static const long QUARK_SETETIM  = zone.intern ("set-expire-time");
  static const long QUARK_GETPATH  = zone.intern ("get-path");
  static const long QUARK_SETPATH  = zone.intern ("set-path");
  static const long QUARK_GETHDOM  = zone.intern ("get-domain");
  static const long QUARK_SETHDOM  = zone.intern ("set-domain");
  static const long QUARK_GETPORT  = zone.intern ("get-port");
  static const long QUARK_SETPORT  = zone.intern ("set-port");
  static const long QUARK_GETCOMT  = zone.intern ("get-comment");
  static const long QUARK_SETCOMT  = zone.intern ("set-comment");
  static const long QUARK_GETCURL  = zone.intern ("get-comment-url");
  static const long QUARK_SETCURL  = zone.intern ("set-comment-url");
  static const long QUARK_GETDISF  = zone.intern ("get-discard");
  static const long QUARK_SETDISF  = zone.intern ("set-discard");
  static const long QUARK_GETSECF  = zone.intern ("get-secure");
  static const long QUARK_SETSECF  = zone.intern ("set-secure");
  static const long QUARK_GETHNLY  = zone.intern ("get-http-only");
  static const long QUARK_SETHNLY  = zone.intern ("set-http-only");
  static const long QUARK_TOSTRING = zone.intern ("to-string");
  static const long QUARK_GETVALUE = zone.intern ("get-value");
  static const long QUARK_SETVALUE = zone.intern ("set-value");

  // create a new object in a generic way

  Object* Cookie::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String hval  = argv->getstring (0);
      return new Cookie (hval);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name  = argv->getstring (0);
      String value = argv->getstring (1);
      return new Cookie (name, value);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name  = argv->getstring (0);
      String value = argv->getstring (1);
      long   mage  = argv->getlong (2);
      return new Cookie (name, value, mage);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with cookie");
  }

  // return true if the given quark is defined

  bool Cookie::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Cookie::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETVERS)  return new Integer (getvers   ());
      if (quark == QUARK_GETNAME)  return new String  (getname   ());
      if (quark == QUARK_GETMAGE)  return new Integer (getmage   ());
      if (quark == QUARK_GETETIM)  return new Integer (getetim   ());
      if (quark == QUARK_GETHDOM)  return new String  (getdomain ());
      if (quark == QUARK_GETPATH)  return new String  (getpath   ());
      if (quark == QUARK_GETPORT)  return new Integer (getport   ());
      if (quark == QUARK_GETCOMT)  return new String  (getcomt   ());
      if (quark == QUARK_GETCURL)  return new String  (getcurl   ());
      if (quark == QUARK_GETDISF)  return new Boolean (getdisf   ());
      if (quark == QUARK_GETSECF)  return new Boolean (getsecf   ());
      if (quark == QUARK_GETHNLY)  return new Boolean (gethnly   ());
      if (quark == QUARK_GETVALUE) return new String  (getvalue  ());
      if (quark == QUARK_TOSTRING) return new String  (tostring  ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	String hval = argv->getstring (0);
	parse (hval);
	return nullptr;
      }
      if (quark == QUARK_SETVERS) {
	long vers = argv->getlong (0);
	setvers (vers);
	return nullptr;
      }
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETVALUE) {
	String value = argv->getstring (0);
	setvalue (value);
	return nullptr;
      }
      if (quark == QUARK_SETMAGE) {
	t_long mage = argv->getlong (0);
	setmage (mage);
	return nullptr;
      }
      if (quark == QUARK_SETETIM) {
	t_long etim = argv->getlong (0);
	setetim (etim);
	return nullptr;
      }      
      if (quark == QUARK_SETPATH) {
	String path = argv->getstring (0);
	setpath (path);
	return nullptr;
      }      
      if (quark == QUARK_SETHDOM) {
	String hdom = argv->getstring (0);
	setdomain (hdom);
	return nullptr;
      }   
      if (quark == QUARK_SETPORT) {
	long port = argv->getlong (0);
	setport (port);
	return nullptr;
      }   
      if (quark == QUARK_SETCOMT) {
	String comt = argv->getstring (0);
	setcomt (comt);
	return nullptr;
      }
      if (quark == QUARK_SETCURL) {
	String curl = argv->getstring (0);
	setcurl (curl);
	return nullptr;
      }
      if (quark == QUARK_SETDISF) {
	bool flag = argv->getbool (0);
	setdisf (flag);
	return nullptr;
      }
      if (quark == QUARK_SETSECF) {
	bool secf = argv->getbool (0);
	setsecf (secf);
	return nullptr;
      }
      if (quark == QUARK_SETHNLY) {
	bool hnly = argv->getbool (0);
	sethnly (hnly);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
