// ---------------------------------------------------------------------------
// - HttpRequest.cpp                                                         -
// - afnix:nwg module - http request class implementation                    -
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
#include "Strvec.hpp"
#include "Boolean.hpp"
#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "HttpRequest.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the nil http version
  static const String HTTP_VERS_NULL = "";
  // the default http request uri
  static const String HTTP_RURI_NULL = "";
  // the default http request uri
  static const String HTTP_RURI_XDEF = "/";
  // the default http options uri
  static const String HTTP_RURI_OPTS = "*";
  // the standard request methods
  static const String HTTP_RMTH_XGET = "GET";  
  static const String HTTP_RMTH_HEAD = "HEAD";  
  static const String HTTP_RMTH_POST = "POST";  
  static const String HTTP_RMTH_TRCE = "TRACE";  
  static const String HTTP_RMTH_OPTS = "OPTIONS";  
  // the default http method
  static const String HTTP_RMTH_XDEF = HTTP_RMTH_XGET;
  // the http content length attribute
  static const String HTTP_CLEN_ATTR = "Content-Length";
  // the http connection type attribute
  static const String HTTP_CTYP_ATTR = "Content-Type";
  // the http host attribute
  static const String HTTP_HOST_ATTR = "Host";
  // the http connection attribute
  static const String HTTP_CONN_ATTR = "Connection";
  // the http connection value
  static const String HTTP_CONN_XDEF = "close";
  // the cookie attribute
  static const String HTTP_COOK_ATTR = "Cookie";
  // the form url encoded type
  static const String HTTP_CTYP_FORM = "application/x-www-form-urlencoded";
  // the multipart/form data type
  static const String HTTP_CTYP_PART = "multipart/form-data";

  // the maximum http request size
  static const long   HTTP_RQST_LSIZ = 8192L;
  
  // this procedure returns the default request uri by method
  static String http_rmth_ruri (const String rmth) {
    if (rmth == HTTP_RMTH_XGET) return HTTP_RURI_XDEF;
    if (rmth == HTTP_RMTH_HEAD) return HTTP_RURI_XDEF;
    if (rmth == HTTP_RMTH_POST) return HTTP_RURI_XDEF;
    if (rmth == HTTP_RMTH_TRCE) return HTTP_RURI_XDEF;
    if (rmth == HTTP_RMTH_OPTS) return HTTP_RURI_OPTS;
    return HTTP_RURI_NULL;
  }

  // this procedure maps a uri to a request uri
  static String http_uri_ruri (const Uri& uri) {
    String result = uri.getpenc ();
    if (result.isnil () == true) result = HTTP_RURI_XDEF;
    return result;
  }

  // this procedure parse a request line
  static Strvec http_parse_request (InputStream& is) {
    // check for valid stream
    if (is.valid () == false) {
      throw Exception ("http-error", "http request timeout");
    }
    // get the request line
    String line = is.readln ();
    if (line.length () > HTTP_RQST_LSIZ) {
      throw Exception ("htpp-error", "http request is too large");
    }
    // split the components
    Strvec result = Strvec::split (line);
    // check length and protocol
    if ((result.length () == 0) || (result.length () > 3)) {
      throw Exception ("http-error", "invalid request line", line);
    }
    // here is the result
    return result;
  }

  // this procedure parse a query string if any
  static String http_parse_query (const String& rmth, const String& ruri,
				  const Plist&  head, const Buffer* rbuf) {
    // prepare result
    String result;
    // eventually normalize the request uri
    String nuri = "http:";
    Regex  suri ("($l$a*):($N+)");
    nuri = (suri == ruri) ? ruri : nuri + ruri;
    // process the get method
    if (rmth ==  HTTP_RMTH_XGET) {
      // create a uri by request uri
      Uri uri (nuri);
      // extract the query string
      result = uri.getquery ();
    }
    // process the post method
    if (rmth ==  HTTP_RMTH_POST) {
      // check for valid buffer
      if (rbuf == nullptr) return result;
      // check if the content type is application/x-www-form-urlencoded
      if (head.exists (HTTP_CTYP_ATTR) == false) return result;
      if (head.getpval (HTTP_CTYP_ATTR) != HTTP_CTYP_FORM) return result;
      // use the buffer as the content string
      result = rbuf->tostring ();
    }
    return result;
  }

  // this procedure read the content into a buffer
  static Buffer* http_read_content (const Plist& head, InputStream& is) {
    Buffer* result = nullptr;
    try {
      // get the content length
      if (head.exists (HTTP_CLEN_ATTR) == false) return nullptr;
      long clen = head.tolong (HTTP_CLEN_ATTR);
      // check for nil
      if (clen <= 0) return nullptr;
      // allocate a buffer by size
      char* data = new char[clen];
      // copy the stream into the buffer
      if (is.copy (data, clen) != clen) {
	delete [] data;
	throw Exception ("http-error", 
			 "inconsistent content length in request");
      }
      result = new Buffer (clen, clen, data);
      // done
      return result;
    } catch (...) {
      delete result;
      throw;
    }
  }

  // this procedure check if a content-type is multipart and get the boundary
  static String http_get_multipart_boundary (const Plist& head) {
    // check for a content type
    if (head.exists (HTTP_CTYP_ATTR) == false) return "";
    // get the content type
    String cval = head.getpval (HTTP_CTYP_ATTR);
    Strvec pvec = Strvec::split (cval, ";", false);
    if (pvec.length () != 2) return "";
    String part = pvec.get(0).strip ();
    if (part != HTTP_CTYP_PART) return "";
    Strvec bvec = Strvec::split (pvec.get(1).strip (), "=", false);
    if (bvec.length () != 2) return "";
    if (bvec.get(0).strip () != "boundary") return "";
    return bvec.get(1).strip ();
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default request
  
  HttpRequest::HttpRequest (void) : HttpProto (HTTP_CLIENT) {
    p_rbuf = nullptr;
    reset ();
  }

  // create a http request by method
  
  HttpRequest::HttpRequest (const String& rmth) : HttpProto (HTTP_CLIENT) {
    p_rbuf = nullptr;
    // reset protocol
    reset ();
    // set the request data
    d_rmth = rmth;
    d_ruri = http_rmth_ruri (d_rmth);
  }
  
  // create a http request by uri

  HttpRequest::HttpRequest (const Uri& uri) : HttpProto (HTTP_CLIENT) {
    p_rbuf = nullptr;
    // reset protocol
    reset ();
    // set the request data
    d_rmth = HTTP_RMTH_XDEF;  
    d_ruri = http_uri_ruri (uri);
    // set the default header
    HeaderPart::hset (HTTP_HOST_ATTR, uri.getauth ());
    HeaderPart::hset (HTTP_CONN_ATTR, HTTP_CONN_XDEF);
  }

  // create a http request by method and uri name
  
  HttpRequest::HttpRequest (const String& rmth,
			    const String& ruri) : HttpProto (HTTP_CLIENT) {
    p_rbuf = nullptr;
    // reset protocol
    reset ();
    // set the request data
    d_rmth = rmth;
    d_ruri = ruri;
  }
  
  // create a http request by method and uri

  HttpRequest::HttpRequest (const String& rmth, 
			    const Uri& uri) : HttpProto (HTTP_CLIENT) {
    p_rbuf = nullptr;
    // reset protocol
    reset ();
    // set the request data
    d_rmth = rmth;
    d_ruri = http_uri_ruri (uri);
    // set the default header
    HeaderPart::hset (HTTP_HOST_ATTR, uri.getauth ());
    HeaderPart::hset (HTTP_CONN_ATTR, HTTP_CONN_XDEF);
  }

  HttpRequest::HttpRequest (InputStream& is) : HttpProto (HTTP_SERVER) {
    p_rbuf = nullptr;
    parse (is);
  }

  // copy construct this http request

  HttpRequest::HttpRequest (const HttpRequest& that) {
    that.rdlock ();
    try {
      HttpProto::operator = (that);
      d_rmth = that.d_rmth;
      d_ruri = that.d_ruri;
      p_rbuf = (that.p_rbuf == nullptr) ? nullptr : new Buffer (*that.p_rbuf);
      Object::iref (p_rbuf);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this object

  HttpRequest::~HttpRequest (void) {
    Object::dref (p_rbuf);
  }

  // return the class name

  String HttpRequest::repr (void) const {
    return "HttpRequest";
  }

  // return a clone of this object

  Object* HttpRequest::clone (void) const {
    return new HttpRequest (*this);
  }

  // assign a http request to this one

  HttpRequest& HttpRequest::operator = (const HttpRequest& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      HttpProto::operator = (that);
      d_rmth = that.d_rmth;
      d_ruri = that.d_ruri;
      Object::dref (p_rbuf); p_rbuf = nullptr;
      p_rbuf = (that.p_rbuf == nullptr) ? nullptr : new Buffer (*that.p_rbuf);
      Object::iref (p_rbuf);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset the request data and header

  void HttpRequest::reset (void) {
    wrlock ();
    try {
      HttpProto::reset ();
      d_rmth = HTTP_RMTH_XDEF;
      d_ruri = http_rmth_ruri (d_rmth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse the request input stream

  void HttpRequest::parse (InputStream& is) {
    wrlock ();
    try {
      // reset everything
      reset ();
      // parse the input stream
      Strvec rvec = http_parse_request (is);
      // get the request length
      long   rlen = rvec.length (); 
      // set the request value
      d_rmth = (rlen < 1) ? HTTP_RMTH_XDEF : rvec.get (0);
      d_ruri = (rlen < 2) ? http_rmth_ruri (d_rmth) : rvec.get (1);
      if (rlen == 3) {
	setvers (rvec.get (2));
      } else {
	d_vers = HTTP_VERS_NULL;
      }
      // read the header
      if (rlen == 3) hparse (is);
      // fill the request buffer
      p_rbuf = http_read_content (d_head, is);
      Object::iref (p_rbuf);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write the http request to an output stream
  
  void HttpRequest::write (OutputStream& os) const {
    rdlock ();
    try {
      // write the request
      String rmth = d_rmth + ' ' + d_ruri + ' ' + d_vers;
      os.writeln (rmth, true);
      // write the header
      HttpProto::write (os);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write the http request to a buffer
  
  void HttpRequest::write (Buffer& buf) const {
    rdlock ();
    try {
      // write the request
      String rmth = d_rmth + ' ' + d_ruri + ' ' + d_vers;
      buf.add (rmth);
      buf.add (crlq);
      buf.add (eolq);
      // write the header
      HttpProto::write (buf);
      // done
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the request method
  
  void HttpRequest::setrmth (const String& rmth) {
    wrlock ();
    try {
      d_rmth = rmth;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the request method

  String HttpRequest::getrmth (void) const {
    rdlock ();
    try {
      String result = d_rmth;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
			       
  // set the request uri
  
  void HttpRequest::setruri (const String& ruri) {
    wrlock ();
    try {
      d_ruri = ruri;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the request uri
  
  String HttpRequest::getruri (void) const {
    rdlock ();
    try {
      String result = d_ruri;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the request query string

  String HttpRequest::getrqry (void) const {
    rdlock ();
    try {
      String result = http_parse_query (d_rmth, d_ruri, d_head, p_rbuf);
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
 
  // check if a cookie is defined

  bool HttpRequest::iscookie (void) const {
    rdlock ();
    try {
      bool result = d_head.exists (HTTP_COOK_ATTR);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a cookie object from the header

  String HttpRequest::getcookie (void) const {
    rdlock ();
    try {
      // check for a cookie string
      if (d_head.exists (HTTP_COOK_ATTR) == false) {
	throw Exception ("http-error", "cannot find cookie in request header");
      }
      // get the cookie header string
      String result = d_head.getpval (HTTP_COOK_ATTR);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the request buffer if any

  Buffer* HttpRequest::getrbuf (void) const {
    rdlock ();
    try {
      Buffer* result = p_rbuf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  MultiPart* HttpRequest::getmpart (void) {
    wrlock ();
    try {
      // check for a boundary string
      String bnds = http_get_multipart_boundary (d_head);
      // get the multipart content
      MultiPart* result = nullptr;
      if ((bnds.isnil () == false) && (p_rbuf != nullptr)) {
	result = new MultiPart (bnds, *p_rbuf);
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
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_SETRMTH = zone.intern ("set-method");
  static const long QUARK_GETRMTH = zone.intern ("get-method");
  static const long QUARK_SETRURI = zone.intern ("set-uri");
  static const long QUARK_GETRURI = zone.intern ("get-uri");
  static const long QUARK_GETRQRY = zone.intern ("get-query");
  static const long QUARK_ISCOOKP = zone.intern ("cookie-p");
  static const long QUARK_GETCOOK = zone.intern ("get-cookie");
  static const long QUARK_GETRBUF = zone.intern ("get-buffer");
  static const long QUARK_GETMPRT = zone.intern ("get-multipart-content");
  
  // create a new object in a generic way
  
  Object* HttpRequest::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new HttpRequest;
    // check for 1 argument
    if (argc == 1) {
      // check for a string
      Object*  obj = argv->get (0);
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new HttpRequest (*sobj);
      // check for a uri
      Uri* uobj = dynamic_cast <Uri*> (obj);
      if (uobj != nullptr) return new HttpRequest (*uobj);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new HttpRequest (*is);
      throw Exception ("type-error", 
		       "invalid object with http request constructor",
		       Object::repr (obj));
    }
    //check for 2 arguments
    if (argc == 2) {
      // get the command
      String rmth = argv->getstring (0);
      // check for a string
      Object*  obj = argv->get (1);
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new HttpRequest (rmth, *sobj);
      // check for a uri
      Uri* uobj = dynamic_cast <Uri*> (obj);
      if (uobj != nullptr) return new HttpRequest (rmth, *uobj);
      throw Exception ("type-error", 
		       "invalid object with http request constructor",
		       Object::repr (obj));
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with http request constructor");
  }

  // return true if the given quark is defined
  
  bool HttpRequest::isquark (const long quark, const bool hflg) const {
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
  
  Object* HttpRequest::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETRMTH) return new String  (getrmth ());
      if (quark == QUARK_GETRURI) return new String  (getruri ());
      if (quark == QUARK_GETRQRY) return new String  (getrqry ());
      if (quark == QUARK_ISCOOKP) return new Boolean (iscookie  ());
      if (quark == QUARK_GETCOOK) return new String  (getcookie ());
      if (quark == QUARK_GETMPRT) return getmpart ();
      if (quark == QUARK_GETRBUF) {
	rdlock ();
	try {
	  Object* result = getrbuf ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETRMTH) {
	String rmth = argv->getstring (0);
	setrmth (rmth);
	return nullptr;
      }
      if (quark == QUARK_SETRURI) {
	String ruri = argv->getstring (0);
	setrmth (ruri);
	return nullptr;
      }
    }
    // call the http proto method
    return HttpProto::apply (zobj, nset, quark, argv);
  }
}
