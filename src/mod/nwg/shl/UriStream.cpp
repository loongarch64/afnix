// ---------------------------------------------------------------------------
// - UriStream.cpp                                                           -
// - afnix:uri service - uri stream class implementation                     -
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

#include "UriStream.hpp"
#include "Exception.hpp"
#include "InputFile.hpp"
#include "OutputFile.hpp"
#include "HttpStream.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

namespace afnix {
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the uri file scheme
  static const String URI_FILE_SCHM = "file";
  // the uri http scheme
  static const String URI_HTTP_SCHM = "http";
  // the uri default path
  static const String URI_PATH_XDEF = "/";

  // get a file input stream
  static InputStream* uri_file_is (const Uri& uri) {
    try {
      // get the system path from the uri path
      String path = uri.getsysp ();
      // open a file for input
      return new InputFile (path);
    } catch (...) {
      throw Exception ("uristream-error", "cannot access uri", uri.getname ());
    }
  }

  // get a file output stream
  static OutputStream* uri_file_os (const Uri& uri) {
    try {
      // get the system path from the uri path
      String path = uri.getsysp ();
      // open a file for output
      return new OutputFile (path);
    } catch (...) {
      throw Exception ("uristream-error", "cannot access uri", uri.getname ());
    }
  }

  // get a http input stream
  static InputStream* uri_http_is (const Uri& uri, UriStream::t_iosm iosm) {
    // create a http request by uri
    HttpRequest hrq (uri);
    // get the uri host
    String host = uri.gethost ();
    // get the port
    long   port = uri.getport ();
    // open a tcp socket
    Object* xs = iosm (host, port);
    auto is = dynamic_cast<InputStream*>(xs);
    auto os = dynamic_cast<OutputStream*>(xs);
    if ((is == nullptr) || (os == nullptr)) return nullptr;
    // write the request
    hrq.write (*os);
    // create a response
    HttpResponse* hrs = new HttpResponse (*is);
    try {
      // check for another location
      if (hrs->ishloc () == false) {
	InputStream* result = new HttpStream (hrs, is);
	delete hrs;
	return result;
      } else {
	Uri huri = hrs->gethloc ();
	delete hrs;
	return uri_http_is (huri, iosm);
      }
      // invalid response
      throw Exception ("uristream-error", "cannot access uri", uri.getname ());
    } catch (...) {
      delete hrs;
      throw;
    }
  }
  
  // get the content input stream by uri

  static InputStream* uri_new_is (const Uri& uri, UriStream::t_iosm iosm) {
    // get the uri scheme
    String sch = uri.getscheme ();
    // select from the uri scheme
    if (sch == URI_FILE_SCHM) return uri_file_is (uri);
    if (sch == URI_HTTP_SCHM) return uri_http_is (uri, iosm);
    // invalid uri scheme to process
    throw Exception ("uristream-error", "invalid uri stream scheme", sch);
  }
  
  // get the content output stream by uri name

  static OutputStream* uri_new_os (const Uri& uri) {
    // get the uri scheme
    String sch = uri.getscheme ();
    // select from the uri scheme
    if (sch == URI_FILE_SCHM) return uri_file_os (uri);
    // invalid uri scheme to process
    throw Exception ("uristream-error", "invalid uri stream scheme", sch);
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                         -
  // -------------------------------------------------------------------------

  // create a null uri stream

  UriStream::UriStream (void) {
    p_iosm = nullptr;
  }
  
  // create a uri stream by input mapper

  UriStream::UriStream (t_iosm iosm) {
    p_iosm = iosm;
  }

  // create a input stream by uri

  InputStream* UriStream::istream (const Uri& uri) const {
    return uri_new_is (uri, p_iosm);
  }

  // create a input stream by string

  InputStream* UriStream::istream (const String& suri) const {
    // check for a valid uri - try by file
    if (Uri::isuri (suri) == false) return new InputFile (suri);
    // normalize the string uri
    String nuri = Uri::sysname (suri);
    // create a requets uri
    Uri uri = nuri;
    // create the input stream
    return uri_new_is (uri, p_iosm);
  }

  // create an output stream by uri

  OutputStream* UriStream::ostream (const Uri& uri) const {
    return uri_new_os (uri);
  }

  // create a output stream by string

  OutputStream* UriStream::ostream (const String& suri) const {
    // check for a valid uri - try by file
    if (Uri::isuri (suri) == false) return new OutputFile (suri);    
    // normalize the string uri
    String nuri = Uri::sysname (suri);
    // create a requets uri
    Uri uri = nuri;
    // create the output stream
    return uri_new_os (uri);
  } 
}
