// ---------------------------------------------------------------------------
// - HttpResponse.cpp                                                        -
// - afnix:nwg module - http response class implementation                   -
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

#include "Regex.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputBound.hpp"
#include "HttpResponse.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the http status code
  static const long HTTP_STAT_200 = 200; // OK
  static const long HTTP_STAT_201 = 201; // Created
  static const long HTTP_STAT_301 = 301; // Move Permanently
  static const long HTTP_STAT_302 = 302; // Found
  static const long HTTP_STAT_303 = 303; // See Other
  static const long HTTP_STAT_EMN = 300; // min error code
  static const long HTTP_STAT_EMX = 505; // max error code
  

  // the status line regex
  static const String HTTP_RESP_RGEX = "(HTTP/$d.$d)$b+($d+)$b+$N+";

  // this procedure returns the response status line
  static String http_status_line (InputStream& is, long& code) {
    // check for valid stream
    if (is.valid () == false) {
      throw Exception ("http-error", "http status timeout");
    }
    // get the status line
    String line = is.readln ();
    Regex    re = HTTP_RESP_RGEX;
    if (re == line) {
      String vers = re.getstring (0);
      String data = re.getstring (1);
      code = Utility::tolong (data);
      return vers;
    }
    throw Exception ("http-error", "invalid http status line", line);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default response

  HttpResponse::HttpResponse (void) : HttpProto (HTTP_SERVER) {
    reset ();
  }

  // create a http response by status code

  HttpResponse::HttpResponse (const long code) : HttpProto (HTTP_SERVER) {
    // reset protocol
    reset ();
    // set the status code
    d_code = code;
  }

  // create a http response by status code and type

  HttpResponse::HttpResponse (const long code, 
			      const String& type) : HttpProto (HTTP_SERVER) {
    // reset protocol
    reset ();
    // set the status code
    d_code = code;
    // add the content-type property
    hset (HEAD_CTYP, type);  
  }
    
  // create a http response by input stream
  
  HttpResponse::HttpResponse (InputStream& is) : HttpProto (HTTP_CLIENT) {
    parse (is);
  }

  // copy construct this http response

  HttpResponse::HttpResponse (const HttpResponse& that)  {
    that.rdlock ();
    try {
      HttpProto::operator = (that);
      d_code = that.d_code;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String HttpResponse::repr (void) const {
    return "HttpResponse";
  }

  // return a clone of this object

  Object* HttpResponse::clone (void) const {
    return new HttpResponse (*this);
  }

  // assign a http response to this one

  HttpResponse& HttpResponse::operator = (const HttpResponse& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      HttpProto::operator = (that);
      d_code = that.d_code;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset the response and reset

  void HttpResponse::reset (void) {
    wrlock ();
    try {
      HttpProto::reset ();
      d_code = 0;
      d_cjar.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse the response input stream

  void HttpResponse::parse (InputStream& is) {
    wrlock ();
    try {
      // reset everything
      reset ();
      // get the status version
      String vers = http_status_line (is, d_code);
      setvers (vers);
      // read the header
      hparse (is);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // write the response header to an output stream
  
  void HttpResponse::hwrite (OutputStream& os) const {
    rdlock ();
    try {
      // write the proto header
      HeaderPart::hwrite (os);
      // add the cookie from the cookie jar
      long clen = d_cjar.length ();
      for (long k = 0; k < clen; k++) {
	// get the cookie by index
	Cookie cook = d_cjar.get (k);
	// format the cookie string by version
	String cval;
	if (cook.getvers () == 0) {
	  cval = HttpProto::hfldtos (HEAD_COK0);
	} else if (cook.getvers () == 1) {
	  cval = HttpProto::hfldtos (HEAD_COK1);
	} else {
	  throw Exception ("http-error", "invalid cookie version");
	}
	cval += ": ";
	cval += cook.tostring ();
	// write the cookie value
	os.writeln (cval, true);
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the http response header to a buffer
  
  void HttpResponse::hwrite (Buffer& buf) const {
    rdlock ();
    try {
      // write the proto header
      HeaderPart::hwrite (buf);
      // add the cookie from the cookie jar
      long clen = d_cjar.length ();
      for (long k = 0; k < clen; k++) {
	// get the cookie by index
	Cookie cook = d_cjar.get (k);
	// format the cookie string by version
	String cval;
	if (cook.getvers () == 0) {
	  cval = HttpProto::hfldtos (HEAD_COK0);
	} else if (cook.getvers () == 1) {
	  cval = HttpProto::hfldtos (HEAD_COK1);
	} else {
	  throw Exception ("http-error", "invalid cookie version");
	}
	cval += ": ";
	cval += cook.tostring ();
	// write the cookie value
	buf.add (cval);
	buf.add (crlq);
	buf.add (eolq);
      }
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
	       
  // write the http response to an output stream
  
  void HttpResponse::write (OutputStream& os) const {
    rdlock ();
    try {
      // write the response
      String rsta = d_vers + ' ' + HttpProto::codetos (d_code);
      os.writeln (rsta, true);
      // write the proto
      HttpProto::write (os);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write the http response to a buffer
  
  void HttpResponse::write (Buffer& buf) const {
    rdlock ();
    try {
      // write the response
      String rsta = d_vers + ' ' + HttpProto::codetos (d_code);
      buf.add (rsta);
      buf.add (crlq);
      buf.add (eolq);
      // write the proto
      HttpProto::write (buf);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the status code is ok

  bool HttpResponse::isok (void) const {
    rdlock ();
    try {
      bool status = (d_code == HTTP_STAT_200);
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the error code is set

  bool HttpResponse::iserror (void) const {
    rdlock ();
    try {
      bool status = ((d_code >= HTTP_STAT_EMN) && (d_code <= HTTP_STAT_EMX));
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the status code is set

  bool HttpResponse::isstatus (void) const {
    rdlock ();
    try {
      bool status = (d_code <= 0) ? false : true;
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the response status by code

  void HttpResponse::setstatus (const long code) {
    wrlock ();
    try {
      d_code = code;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the response status by code and version

  void HttpResponse::setstatus (const long code, const String& vers) {
    wrlock ();
    try {
      // set the code
      d_code = code;
      // set the version
      setvers (vers);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the status code

  long HttpResponse::getstatus (void) const {
    rdlock ();
    try {
      long result = d_code;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // map a status code into a string

  String HttpResponse::mapstatus (void) const {
    rdlock ();
    try {
      String result = HttpProto::codetos (d_code);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // return true if there is something at another location

  bool HttpResponse::ishloc (void) const {
    rdlock ();
    try {
      // check first for a location property
      if (hexists (HEAD_MLOC) == false) {
	unlock ();
	return false;
      }
      // check the status code
      bool status = false;
      switch (d_code) {
      case HTTP_STAT_201:
      case HTTP_STAT_301:
      case HTTP_STAT_302:
      case HTTP_STAT_303:
	status = true;
	break;
      default:
	status = false;
	break;
      }
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a header location as a redirect

  void HttpResponse::sethloc (const String& uri) {
    wrlock ();
    try {
      // set a status 303 (see other)
      setstatus (HTTP_STAT_303);
      // set the redirection location
      hset (HEAD_MLOC, uri);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the header location value

  String HttpResponse::gethloc (void) const {
    rdlock ();
    try {
      String result = hmap (HEAD_MLOC);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a cookie in the response jar

  void HttpResponse::addcookie (const Cookie& cook) {
    wrlock ();
    try {
      d_cjar.add (cook);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------
  
  // the quark zone
  static const long QUARK_ZONE_LENGTH = 10;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_OKP      = zone.intern ("status-ok-p");
  static const long QUARK_HLOCP    = zone.intern ("location-p");
  static const long QUARK_ERRORP   = zone.intern ("status-error-p");
  static const long QUARK_STATUSP  = zone.intern ("status-p");
  static const long QUARK_SETCODE  = zone.intern ("set-status-code");
  static const long QUARK_GETCODE  = zone.intern ("get-status-code");
  static const long QUARK_MAPCODE  = zone.intern ("map-status-code");
  static const long QUARK_GETHLOC  = zone.intern ("get-location");
  static const long QUARK_SETHLOC  = zone.intern ("set-location");
  static const long QUARK_ADDCOOK  = zone.intern ("add-cookie");
  
  // create a new object in a generic way
  
  Object* HttpResponse::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new HttpResponse;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	long code = iobj->tolong ();
	return new HttpResponse (code);
      }
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new HttpResponse (*is);
      // invalid arguments
      throw Exception ("type-error", 
		       "invalid object for http response constructor",
		       Object::repr (obj));
    }
    // check for 2 arguments
    if (argc == 2) {
      long   code = argv->getlong (0);
      String type = argv->getstring (1);
      return new HttpResponse (code, type);
    }
    throw Exception ("argument-error", 
		     "too many arguments with http response constructor");
  }

  // return true if the given quark is defined
  
  bool HttpResponse::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? HttpProto::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* HttpResponse::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_OKP)      return new Boolean (isok      ());
      if (quark == QUARK_HLOCP)    return new Boolean (ishloc    ());
      if (quark == QUARK_ERRORP)   return new Boolean (iserror   ());
      if (quark == QUARK_STATUSP)  return new Boolean (isstatus  ());
      if (quark == QUARK_GETCODE)  return new Integer (getstatus ());
      if (quark == QUARK_MAPCODE)  return new String  (mapstatus ());
      if (quark == QUARK_GETHLOC)  return new String  (gethloc   ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCODE) {
	long code = argv->getlong (0);
	setstatus (code);
	return nullptr;
      }
      if (quark == QUARK_SETHLOC) {
	String uri = argv->getstring (0);
	sethloc (uri);
	return nullptr;
      }
      if (quark == QUARK_ADDCOOK) {
        Object*   obj = argv->get (0);
        Cookie*  cobj = dynamic_cast <Cookie*> (obj);
        if (cobj == nullptr) {
          throw Exception ("type-error", "invalid object with set-cookie",
                           Object::repr (obj));
        }
	addcookie (*cobj);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETCODE) {
	long   code = argv->getlong (0);
	String vers = argv->getstring (1);
	setstatus (code, vers);
	return nullptr;
      }
    }
    // call the http proto method
    return HttpProto::apply (zobj, nset, quark, argv);
  }
}
