// ---------------------------------------------------------------------------
// - HttppProto.cpp                                                          -
// - afnix:nwg module - http protocol class implementation                   -
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

#include "Mime.hpp"
#include "Regex.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Utility.hpp"
#include "Loopable.hpp"
#include "HttpProto.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the compatible http version
  static const String HTTP_VERS_CPT0 = "HTTP/1.0";
  // the current http version
  static const String HTTP_VERS_CPT1 = "HTTP/1.1";
  // the default http version
  static const String HTTP_VERS_XDEF = HTTP_VERS_CPT1;

  /// the media type regex
  static const String HTTP_CTYP_RGEX = "(<$a-+>+/<$a-+>+)[;$N+]?";
  /// the charset regex
  static const String HTTP_CENC_RGEX = "$N+charset=(<$a->+)";

  // the http header field
  static const String HTTP_HEAD_ACPT = "Accept";
  static const String HTTP_HEAD_ACHS = "Accept-Charset";
  static const String HTTP_HEAD_AENC = "Accept-Encoding";
  static const String HTTP_HEAD_ALNG = "Accept-Language";
  static const String HTTP_HEAD_ARNG = "Accept-Ranges";
  static const String HTTP_HEAD_BAGE = "Age";
  static const String HTTP_HEAD_ALOW = "Allow";
  static const String HTTP_HEAD_AUTH = "Authorization";
  static const String HTTP_HEAD_CCTL = "Cache-Control";
  static const String HTTP_HEAD_CONN = "Connection";
  static const String HTTP_HEAD_CENC = "Content-Encoding";
  static const String HTTP_HEAD_CLNG = "Content-Language";
  static const String HTTP_HEAD_CLEN = "Content-Length";
  static const String HTTP_HEAD_CLOC = "Content-Location";
  static const String HTTP_HEAD_CMD5 = "Content-MD5";
  static const String HTTP_HEAD_CRNG = "Content-Range";
  static const String HTTP_HEAD_CTYP = "Content-Type";
  static const String HTTP_HEAD_DATE = "Date";
  static const String HTTP_HEAD_ETAG = "ETag";
  static const String HTTP_HEAD_EXPT = "Expect";
  static const String HTTP_HEAD_EXPR = "Expires";
  static const String HTTP_HEAD_FROM = "From";
  static const String HTTP_HEAD_HOST = "Host";
  static const String HTTP_HEAD_IFMT = "If-Match";
  static const String HTTP_HEAD_IFMS = "If-Modified-Since";
  static const String HTTP_HEAD_IFNM = "If-None-Match";
  static const String HTTP_HEAD_IFRG = "If-Range";
  static const String HTTP_HEAD_IFUS = "If-Unmodified-Since";
  static const String HTTP_HEAD_LMOD = "Last-Modified";
  static const String HTTP_HEAD_MLOC = "Location";
  static const String HTTP_HEAD_MFWD = "Max-Forwards";
  static const String HTTP_HEAD_PRGM = "Pragma";
  static const String HTTP_HEAD_PXTH = "Proxy-Authenticate";
  static const String HTTP_HEAD_PXAT = "Proxy-Authorization";
  static const String HTTP_HEAD_BRNG = "Range";
  static const String HTTP_HEAD_REFR = "Referer";
  static const String HTTP_HEAD_RAFT = "Retry-After";
  static const String HTTP_HEAD_SERV = "Server";
  static const String HTTP_HEAD_BDTE = "TE";
  static const String HTTP_HEAD_TRLR = "Trailer";
  static const String HTTP_HEAD_TENC = "Transfer-Encoding";
  static const String HTTP_HEAD_UPGR = "Upgrade";
  static const String HTTP_HEAD_UAGT = "User-Agent";
  static const String HTTP_HEAD_VARY = "Vary";
  static const String HTTP_HEAD_RVIA = "Via";
  static const String HTTP_HEAD_WRNG = "Warning";
  static const String HTTP_HEAD_WWWA = "WWW-Authenticate";
  static const String HTTP_HEAD_COK0 = "Set-Cookie";
  static const String HTTP_HEAD_COK1 = "Set-Cookie2";

  // the http status code
  static const String HTTP_STAT_C100 = "100 Continue";
  static const String HTTP_STAT_C101 = "101 Switching Protocols";
  static const String HTTP_STAT_C200 = "200 OK";
  static const String HTTP_STAT_C201 = "201 Created";
  static const String HTTP_STAT_C202 = "202 Accepted";
  static const String HTTP_STAT_C203 = "203 Non-Authoritative Information";
  static const String HTTP_STAT_C204 = "204 No Content";
  static const String HTTP_STAT_C205 = "205 Reset Content";
  static const String HTTP_STAT_C206 = "206 Partial Content";
  static const String HTTP_STAT_C300 = "300 Multiple Choices";
  static const String HTTP_STAT_C301 = "301 Moved Permanently";
  static const String HTTP_STAT_C302 = "302 Found";
  static const String HTTP_STAT_C303 = "303 See Other";
  static const String HTTP_STAT_C304 = "304 Not Modified";
  static const String HTTP_STAT_C305 = "305 Use Proxy";
  static const String HTTP_STAT_C307 = "307 Temporary Redirect";
  static const String HTTP_STAT_C400 = "400 Bad Request";
  static const String HTTP_STAT_C401 = "401 Unauthorized";
  static const String HTTP_STAT_C402 = "402 Payment Required";
  static const String HTTP_STAT_C403 = "403 Forbidden";
  static const String HTTP_STAT_C404 = "404 Not Found";
  static const String HTTP_STAT_C405 = "405 Method Not Allowed";
  static const String HTTP_STAT_C406 = "406 Not Acceptable";
  static const String HTTP_STAT_C407 = "407 Proxy Authentication Required";
  static const String HTTP_STAT_C408 = "408 Request Time-out";
  static const String HTTP_STAT_C409 = "409 Conflict";
  static const String HTTP_STAT_C410 = "410 Gone";
  static const String HTTP_STAT_C411 = "411 Length Required";
  static const String HTTP_STAT_C412 = "412 Precondition Failed";
  static const String HTTP_STAT_C413 = "413 Request Entity Too Large";
  static const String HTTP_STAT_C414 = "414 Request-URI Too Large";
  static const String HTTP_STAT_C415 = "415 Unsupported Media Type";
  static const String HTTP_STAT_C416 = "416 Requested range not satisfiable";
  static const String HTTP_STAT_C417 = "417 Expectation Failed";
  static const String HTTP_STAT_C500 = "500 Internal Server Error";
  static const String HTTP_STAT_C501 = "501 Not Implemented";
  static const String HTTP_STAT_C502 = "502 Bad Gateway";
  static const String HTTP_STAT_C503 = "503 Service Unavailable";
  static const String HTTP_STAT_C504 = "504 Gateway Time-out";
  static const String HTTP_STAT_C505 = "505 HTTP Version not supported";

  // this procedure check if a media type is defined
  static bool http_is_media (const String& pval) {
    // extract the content value
    Regex re = HTTP_CTYP_RGEX;
    if (re == pval) return true;
    // not defined
    return false;
  }

  // this procedure extract the media type from the header
  static String http_get_media (const String& pval) {
    // extract the content value
    Regex re = HTTP_CTYP_RGEX;
    if (re == pval) return re.getstring (0);
    throw Exception ("http-error", "cannot extract media type", pval);
  }

  // this procedure returns true if the encoding mode is defined in the header
  static bool http_is_emod (const String& pval) {
    // extract the content value
    Regex re = HTTP_CENC_RGEX;
    if (re == pval) return true;
    // not defined
    return false;
  }

  // this procedure extract the encoding mode from the header
  static String http_get_emod (const String& pval) {
    // extract the encoding value
    Regex re = HTTP_CENC_RGEX;
    if (re == pval) return re.getstring (0);
    throw Exception ("http-error", "cannot extract encoding mode", pval);
  }

  // -------------------------------------------------------------------------
  // - public section                                                         -
  // -------------------------------------------------------------------------

  // get the default http version

  String HttpProto::getxdef (void) {
    return HTTP_VERS_XDEF;
  }

  // map a header field to a string

  String HttpProto::hfldtos (const t_hfld hfld) {
    switch (hfld) {
    case HEAD_ACPT: return HTTP_HEAD_ACPT;
    case HEAD_ACHS: return HTTP_HEAD_ACHS;
    case HEAD_AENC: return HTTP_HEAD_AENC;
    case HEAD_ALNG: return HTTP_HEAD_ALNG;
    case HEAD_ARNG: return HTTP_HEAD_ARNG;
    case HEAD_BAGE: return HTTP_HEAD_BAGE;
    case HEAD_ALOW: return HTTP_HEAD_ALOW;
    case HEAD_AUTH: return HTTP_HEAD_AUTH;
    case HEAD_CCTL: return HTTP_HEAD_CCTL;
    case HEAD_CONN: return HTTP_HEAD_CONN;
    case HEAD_CENC: return HTTP_HEAD_CENC;
    case HEAD_CLNG: return HTTP_HEAD_CLNG;
    case HEAD_CLEN: return HTTP_HEAD_CLEN;
    case HEAD_CLOC: return HTTP_HEAD_CLOC;
    case HEAD_CMD5: return HTTP_HEAD_CMD5;
    case HEAD_CRNG: return HTTP_HEAD_CRNG;
    case HEAD_CTYP: return HTTP_HEAD_CTYP;
    case HEAD_DATE: return HTTP_HEAD_DATE;
    case HEAD_ETAG: return HTTP_HEAD_ETAG;
    case HEAD_EXPT: return HTTP_HEAD_EXPT;
    case HEAD_EXPR: return HTTP_HEAD_EXPR;
    case HEAD_FROM: return HTTP_HEAD_FROM;
    case HEAD_HOST: return HTTP_HEAD_HOST;
    case HEAD_IFMT: return HTTP_HEAD_IFMT;
    case HEAD_IFMS: return HTTP_HEAD_IFMS;
    case HEAD_IFNM: return HTTP_HEAD_IFNM;
    case HEAD_IFRG: return HTTP_HEAD_IFRG;
    case HEAD_IFUS: return HTTP_HEAD_IFUS;
    case HEAD_LMOD: return HTTP_HEAD_LMOD;
    case HEAD_MLOC: return HTTP_HEAD_MLOC;
    case HEAD_MFWD: return HTTP_HEAD_MFWD;
    case HEAD_PRGM: return HTTP_HEAD_PRGM;
    case HEAD_PXTH: return HTTP_HEAD_PXTH;
    case HEAD_PXAT: return HTTP_HEAD_PXAT;
    case HEAD_BRNG: return HTTP_HEAD_BRNG;
    case HEAD_REFR: return HTTP_HEAD_REFR;
    case HEAD_RAFT: return HTTP_HEAD_RAFT;
    case HEAD_SERV: return HTTP_HEAD_SERV;
    case HEAD_BDTE: return HTTP_HEAD_BDTE;
    case HEAD_TRLR: return HTTP_HEAD_TRLR;
    case HEAD_TENC: return HTTP_HEAD_TENC;
    case HEAD_UPGR: return HTTP_HEAD_UPGR;
    case HEAD_UAGT: return HTTP_HEAD_UAGT;
    case HEAD_VARY: return HTTP_HEAD_VARY;
    case HEAD_RVIA: return HTTP_HEAD_RVIA;
    case HEAD_WRNG: return HTTP_HEAD_WRNG;
    case HEAD_WWWA: return HTTP_HEAD_WWWA;
    case HEAD_COK0: return HTTP_HEAD_COK0;
    case HEAD_COK1: return HTTP_HEAD_COK1;
    default:
      break;
    }
    throw Exception ("http-error", "invalid http head field");
  }

  // map a http status code to a string

  String HttpProto::codetos (const long code) {
    switch (code) {
    case 100: return HTTP_STAT_C100;
    case 101: return HTTP_STAT_C101;
    case 200: return HTTP_STAT_C200;
    case 201: return HTTP_STAT_C201;
    case 202: return HTTP_STAT_C202;
    case 203: return HTTP_STAT_C203;
    case 204: return HTTP_STAT_C204;
    case 205: return HTTP_STAT_C205;
    case 206: return HTTP_STAT_C206;
    case 300: return HTTP_STAT_C300;
    case 301: return HTTP_STAT_C301;
    case 302: return HTTP_STAT_C302;
    case 303: return HTTP_STAT_C303;
    case 304: return HTTP_STAT_C304;
    case 305: return HTTP_STAT_C305;
    case 307: return HTTP_STAT_C307;
    case 400: return HTTP_STAT_C400;
    case 401: return HTTP_STAT_C401;
    case 402: return HTTP_STAT_C402;
    case 403: return HTTP_STAT_C403;
    case 404: return HTTP_STAT_C404;
    case 405: return HTTP_STAT_C405;
    case 406: return HTTP_STAT_C406;
    case 407: return HTTP_STAT_C407;
    case 408: return HTTP_STAT_C408;
    case 409: return HTTP_STAT_C409;
    case 410: return HTTP_STAT_C410;
    case 411: return HTTP_STAT_C411;
    case 412: return HTTP_STAT_C412;
    case 413: return HTTP_STAT_C413;
    case 414: return HTTP_STAT_C414;
    case 415: return HTTP_STAT_C415;
    case 416: return HTTP_STAT_C416;
    case 417: return HTTP_STAT_C417;
    case 500: return HTTP_STAT_C500;
    case 501: return HTTP_STAT_C501;
    case 502: return HTTP_STAT_C502;
    case 503: return HTTP_STAT_C503;
    case 504: return HTTP_STAT_C504;
    case 505: return HTTP_STAT_C505;
    default:
      break;
    }
    throw Exception ("http-error", "invalid http status code", 
		     Utility::tostring (code));
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default http protocol

  HttpProto::HttpProto (void) {
    reset ();
  }

  // create a http protocol by type

  HttpProto::HttpProto (const t_ptyp ptyp) {
    reset ();
    d_ptyp = ptyp;
  }

  // copy construct this http protocol

  HttpProto::HttpProto (const HttpProto& that) {
    that.rdlock ();
    try {
      // set base type
      HeaderPart::operator = (that);
      // copy locally
      d_vers = that.d_vers;
      d_ptyp = that.d_ptyp;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a http protocol to this one

  HttpProto& HttpProto::operator = (const HttpProto& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // set base header
      HeaderPart::operator = (that);
      // copy locally
      d_vers = that.d_vers;
      d_ptyp = that.d_ptyp;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // reset this protocol version

  void HttpProto::reset (void) {
    wrlock ();
    try {
      // reset base
      HeaderPart::reset ();
      // reset locally
      d_vers = HTTP_VERS_XDEF;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the http protocol version

  void HttpProto::setvers (const String& vers) {
    wrlock ();
    try {
      if ((vers != HTTP_VERS_CPT0) && (vers != HTTP_VERS_CPT1)) {
	throw Exception ("http-error", "invalid protocol version", vers);
      }
      d_vers = vers;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the http protocol version

  String HttpProto::getvers (void) const {
    rdlock ();
    try {
      String result = d_vers;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if a header property exists by field

  bool HttpProto::hexists (const t_hfld hfld) const {
    rdlock ();
    try {
      // map the header field
      String name = HttpProto::hfldtos (hfld);
      // look for the name
      bool result = HeaderPart::hexists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a header property by field and literal

  void HttpProto::hset (const t_hfld hfld, const Literal& lval) {
    wrlock ();
    try {
      // map the header field
      String name = HttpProto::hfldtos (hfld);
      // set the header by name
      HeaderPart::hset (name, lval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header property by field
  
  Property* HttpProto::hfind (const t_hfld hfld) const {
    rdlock ();
    try {
      // map the header field
      String name = HttpProto::hfldtos (hfld);
      // find the property by name
      Property* result = HeaderPart::hfind (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a header property by field or throw an exception
  
  Property* HttpProto::hlookup (const t_hfld hfld) const {
    rdlock ();
    try {
      // map the header field
      String name = HttpProto::hfldtos (hfld);
      // look by name
      Property* result = HeaderPart::hlookup (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a header property value by field
  
  String HttpProto::hmap (const t_hfld hfld) const {
    rdlock ();
    try {
      // map the header field
      String name = HttpProto::hfldtos (hfld);
      // mp by name
      String result = HeaderPart::hmap (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the media type is defined

  bool HttpProto::ismedia (void) const {
    rdlock ();
    try {
      // set the result
      bool result = false;
      // check for the content type property
      if (hexists (HEAD_CTYP) == true) {
	// get the content type property
	String pval = hmap (HEAD_CTYP);
	// test the media type
	result = http_is_media (pval);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the media type

  String HttpProto::getmedia (void) const {
    rdlock ();
    try {
      // set the result
      String result = Mime::getmdef ();
      // check for the content type property
      if (hexists (HEAD_CTYP) == true) {
	// get the content type property
	String pval = hmap (HEAD_CTYP);
	// map the media type
	result = http_get_media (pval);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the encoding mode is defined in the header

  bool HttpProto::isemod (void) const {
    rdlock ();
    try {
      // set the result
      bool result = false;
      // check for the content type property
      if (hexists (HEAD_CTYP) == true) {
	// get the content type property
	String pval = hmap (HEAD_CTYP);
	// test the encoding mode
	result = http_is_emod (pval);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the content encoding mode if any

  String HttpProto::getemod (void) const {
    rdlock ();
    try {
      String result = Encoding::getdem ();
      // check for the content type property
      if (hexists (HEAD_CTYP) == true) {
	// get the content type property
	String pval = hmap (HEAD_CTYP);
	// map the encoding mode
	result = http_get_emod (pval);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if content length is defined

  bool HttpProto::isclen (void) const {
    rdlock ();
    try {
      bool result = hexists (HEAD_CLEN);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the header content length

  long HttpProto::getclen (void) const {
    rdlock ();
    try {
      long result = 0;
      // check for a content lenth property
      if (hexists (HEAD_CLEN) == true) {
	// get the content length property
	String pval = hmap (HEAD_CLEN);
	// convert to an integer
	result = Utility::tolong(pval);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the content stream

  String HttpProto::getcstr (InputStream& is) {
    wrlock ();
    char* cbuf = nullptr;
    try {
      // extract the content length
      long clen = getclen ();
      // create the content buffer
      cbuf = new char[clen+1];
      // try to read it
      if (is.copy (cbuf, clen) != clen) {
	throw Exception ("http-error", "canno parse content buffer");
      }
      cbuf[clen] = nilc;
      // map the result
      String result = cbuf;
      delete [] cbuf;
      // unlock and result
      unlock ();
      return result;
    } catch (...) {
      delete [] cbuf;
      unlock ();
      throw;
    }
  }

  // write the http proto to the output stream

  void HttpProto::write (OutputStream& os) const {
    rdlock ();
    try {
      // write the header
      hwrite (os);
      // end the header
      os.newline (true);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the http proto to a buffer

  void HttpProto::write (Buffer& buf) const {
    rdlock ();
    try {
      // write the header
      hwrite (buf);
      // end the header
      buf.add (crlc);
      buf.add (eolc);
      // done
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
  static const long QUARK_PARSE    = zone.intern ("parse");
  static const long QUARK_WRITE    = zone.intern ("write");
  static const long QUARK_SETVERS  = zone.intern ("set-version");  
  static const long QUARK_GETVERS  = zone.intern ("get-version");  
  static const long QUARK_EMODP    = zone.intern ("encoding-mode-p");
  static const long QUARK_MEDIAP   = zone.intern ("media-type-p");
  static const long QUARK_GETEMOD  = zone.intern ("get-encoding-mode");
  static const long QUARK_GETCLEN  = zone.intern ("get-content-length");
  static const long QUARK_GETCSTR  = zone.intern ("get-content-string");
  static const long QUARK_GETMEDIA = zone.intern ("get-media-type");

  // return true if the given quark is defined

  bool HttpProto::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? HeaderPart::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* HttpProto::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETVERS)  return new String  (getvers  ());
      if (quark == QUARK_EMODP)    return new Boolean (isemod   ());
      if (quark == QUARK_MEDIAP)   return new Boolean (ismedia  ());
      if (quark == QUARK_GETEMOD)  return new String  (getemod  ());
      if (quark == QUARK_GETCLEN)  return new Integer (getclen  ());
      if (quark == QUARK_GETMEDIA) return new String  (getmedia ());
      if (quark == QUARK_WRITE) {
	auto lobj = dynamic_cast<Loopable*>(zobj);
	OutputStream* os = (lobj == nullptr) ? nullptr : lobj->getos ();
	if (os != nullptr) write (*os);
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETVERS) {
        String vers = argv->getstring (0);
	setvers (vers);
	return nullptr;
      }
      if (quark == QUARK_PARSE) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", 
			   "invalid object as an input stream to parse",
			   Object::repr (obj));
	}
	parse (*is);
	return nullptr;
      }
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  write (*os);
	  return nullptr;
	}
	// check for a buffer
	Buffer* buf = dynamic_cast <Buffer*> (obj);
	if (buf != nullptr) {
	  write (*buf);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with write",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETCSTR) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", 
			   "invalid object as an input stream to parse",
			   Object::repr (obj));
	}
	return new String (getcstr (*is));
      }
    }
    // call the header part method
    return HeaderPart::apply (zobj, nset, quark, argv);
  }
}
