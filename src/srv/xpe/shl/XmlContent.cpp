// ---------------------------------------------------------------------------
// - XmlContent.cpp                                                          -
// - afnix:xpe module - xml content class implementation                     -
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

#include "Evaluable.hpp"
#include "TcpClient.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputFile.hpp"
#include "OutputFile.hpp"
#include "XmlContent.hpp"
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
  static InputStream* get_file_istream (const Uri& uri) {
    try {
      // get the system path from the uri path
      String path = uri.getsysp ();
      // open a file for input
      return new InputFile (path);
    } catch (...) {
      throw Exception ("stream-error", "cannot access uri", uri.getname ());
    }
  }

  // get a file input stream with a uri path
  static InputStream* get_file_istream (const String& nuri) {
    try {
      // open a file for input
      return new InputFile (nuri);
    } catch (...) {
      throw Exception ("stream-error", "cannot access uri", nuri);
    }
  }

  // get a file output stream
  static OutputStream* get_file_ostream (const Uri& uri) {
    try {
      // get the system path from the uri path
      String path = uri.getsysp ();
      // open a file for output
      return new OutputFile (path);
    } catch (...) {
      throw Exception ("stream-error", "cannot access uri", uri.getname ());
    }
  }

  // get a file output stream with a uri path
  static OutputStream* get_file_ostream (const String& nuri) {
    try {
      // open a file for output
      return new OutputFile (nuri);
    } catch (...) {
      throw Exception ("stream-error", "cannot access uri", nuri);
    }
  }

  // get a http input stream
  static InputStream* get_http_istream (const Uri& uri) {
    // create a http request by uri
    HttpRequest hrq (uri);
    // get the uri host
    String host = uri.gethost ();
    // get the port
    long   port = uri.getport ();
    // open a tcp socket
    TcpClient* s = new TcpClient (host, port);
    // write the request
    hrq.write (*s);
    // create a response
    HttpResponse* hrs = new HttpResponse (*s);
    try {
      // check for another location
      if (hrs->ishloc () == false) {
	InputStream* result = new HttpStream (hrs, s);
	delete hrs;
	return result;
      } else {
	Uri huri = hrs->gethloc ();
	delete hrs;
	return get_http_istream (huri);
      }
      // invalid response
      throw Exception ("http-error", "cannot access uri", uri.getname ());
    } catch (...) {
      delete hrs;
      throw;
    }
  }

  // get the content input stream by uri name

  static InputStream* get_content_istream (const String& unam) {
    // create a uri by name
    Uri uri = unam;
    // get the uri scheme
    String sch = uri.getscheme ();
    // select from the uri scheme
    if (sch == URI_FILE_SCHM) return get_file_istream (uri);
    if (sch == URI_HTTP_SCHM) return get_http_istream (uri);
    // invalid uri scheme to process
    throw Exception ("scheme-error", "invalid xml content uri scheme", sch);
  }

  // get the content output stream by uri name

  static OutputStream* get_content_ostream (const String& unam) {
    // create a uri by name
    Uri uri = unam;
    // get the uri scheme
    String sch = uri.getscheme ();
    // select from the uri scheme
    if (sch == URI_FILE_SCHM) return get_file_ostream (uri);
    // invalid uri scheme to process
    throw Exception ("scheme-error", "invalid xml content uri scheme", sch);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a xml content for writing

  XmlContent::XmlContent (void) {
    p_puri = nullptr;
  }

  // create a xml content by name

  XmlContent::XmlContent (const String& name) {
    // save the name
    d_name = name;
    p_puri = nullptr;
    // normalize the document name
    d_nuri = Uri::sysname (name);
    // get the input stream
    InputStream* is = get_content_istream (d_nuri);
    Object::iref (is);
    try {
      // set the document root
      setroot (d_nuri, is);
      // clean the stream
      Object::dref (is);
    } catch (...) {
      Object::dref (is);
      throw;
    }
  }

  // create a xml content by name and input stream

  XmlContent::XmlContent (const String& name, InputStream* is) {
    // save the name
    d_name = name;
    p_puri = nullptr;
    // normalize the document name
    d_nuri = Uri::sysname (name);
    // set the document root
    setroot (d_nuri, is);
  }

  // create a xml content by uri name and path

  XmlContent::XmlContent (const String& name, const UriPath& puri) {
    // save the name
    d_name = name;
    p_puri = new UriPath (puri);
    // normalize the document name
    d_nuri = puri.mapruri (name);
    // get the input stream
    InputStream* is = get_file_istream (d_nuri);
    Object::iref (is);
    try {
      // set the document root
      setroot (d_nuri, is);
      // clean the stream
      Object::dref (is);
    } catch (...) {
      Object::dref (is);
      throw;
    }
  }

  // create a xml content by name and document name

  XmlContent::XmlContent (const String& name, const String& docn) {
    // save the name
    d_name = name;
    p_puri = nullptr;
    // normalize the document name
    d_nuri= Uri::sysname (docn);
    // get the input stream
    InputStream* is = get_content_istream (d_nuri);
    Object::iref (is);
    try {
      // set the document root
      setroot (d_nuri, is);
      // clean the stream
      Object::dref (is);
    } catch (...) {
      Object::dref (is);
      throw;
    }
  }

  // create a xml content by name, document name and encoding mode

  XmlContent::XmlContent (const String& name, const String& docn,
			  const String& emod) {
    // save the name
    d_name = name;
    p_puri = nullptr;
    // normalize the document name
    d_nuri = Uri::sysname (docn);
    // get the input stream
    InputStream* is = get_content_istream (d_nuri);
    Object::iref (is);
    try {
      // set the encoding mode
      is->setemod (emod);
      // set the document root text
      setrtxt (d_nuri, is);
      // clean the stream
      Object::dref (is);
    } catch (...) {
      Object::dref (is);
      throw;
    }
  }

  // copy construct this content

  XmlContent::XmlContent (const XmlContent& that) {
    that.rdlock ();
    try {
      XmlDocument::operator = (that);
      d_nuri = that.d_nuri;
      Object::iref (p_puri = that.p_puri);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
   
  // destroy this object

  XmlContent::~XmlContent (void) {
    Object::dref (p_puri);
  }

  // get the document normalized uri

  String XmlContent::getnuri (void) const {
    rdlock ();
    try {
      String result = d_nuri;
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }
 
  // get the document name

  String XmlContent::getdocn (void) const {
    rdlock ();
    try {
      String result = XmlDocument::getname ();
      unlock ();
      return result;
    } catch(...) {
      unlock ();
      throw;
    }
  }

  // write a xml content to a buffer

  void XmlContent::write (Buffer& buf) {
    wrlock ();
    try {
      // reset the buffer
      buf.reset ();
      // get the document root
      XmlRoot* root = newroot (true);
      // write the document root
      root->write (buf);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // write a xml content by name

  void XmlContent::write (const String& name) {
    wrlock ();
    OutputStream* os = nullptr;
    try {
      // save the name
      d_name = name;
      p_puri = nullptr;
      // normalize the document name
      d_nuri = Uri::sysname (name);
      // get the output stream
      Object::iref (os = get_content_ostream (d_nuri));
      if (os == nullptr) {
	throw Exception ("write-error", "nil stream for xml content write");
      }
      // get the document root
      XmlRoot* root = newroot (true);
      // write the document root
      root->write (*os);
      // close and clean the stream
      Object::dref (os);
      unlock ();
    } catch (...) {
      Object::dref (os);
      unlock ();
      throw;
    }
  }

  // write a xml content by name and output stream

  void XmlContent::write (const String& name, OutputStream* os) {
    wrlock ();
    try {
      if (os == nullptr) {
	throw Exception ("write-error", "nil stream for xml content write");
      }
      // save the name
      d_name = name;
      p_puri = nullptr;
      // normalize the document name
      d_nuri = Uri::sysname (name);
      // get the document root
      XmlRoot* root = newroot (true);
      // write the document root
      root->write (*os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a xml content by uri name and path

  void XmlContent::write (const String& name, const UriPath& puri) {
    wrlock ();
    OutputStream* os = nullptr;
    try {
      // save the name
      d_name = name;
      p_puri = new UriPath (puri);
      // normalize the document name
      d_nuri = puri.mapruri (name);
      // get the output stream
      Object::iref (os = get_file_ostream (d_nuri));
      if (os == nullptr) {
	throw Exception ("write-error", "nil stream for xml content write");
      }
      // get the document root
      XmlRoot* root = newroot (true);
      // write the document root
      root->write (*os);
      // close and clean the stream
      Object::dref (os);
      unlock ();
    } catch (...) {
      Object::dref (os);
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
  static const long QUARK_WRITE   = zone.intern ("write");
  static const long QUARK_GETNURI = zone.intern ("get-document-uri");
  static const long QUARK_GETDOCN = zone.intern ("get-document-name");

  // create a new object in a generic way

  Object* XmlContent::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new XmlContent;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XmlContent (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      Object* obj = argv->get (1);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new XmlContent (name, *sobj);
      // check for an uri path
      UriPath* pobj = dynamic_cast <UriPath*> (obj);
      if (pobj != nullptr) return new XmlContent (name, *pobj);
      // invalid object
      throw Exception ("type-error", "invalid oject with xml content",
		       Object::repr (obj));
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      String docn = argv->getstring (1);
      String emod = argv->getstring (2);
      return new XmlContent (name, docn, emod);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with xml processor constructor");
  }

  // return true if the given quark is defined

  bool XmlContent::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlDocument::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlContent::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNURI) return new String (getnuri ());
      if (quark == QUARK_GETDOCN) return new String (getdocn ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_WRITE) {
	Object* obj = argv->get (0);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  write (*sobj);
	  return nullptr;
	}
	// check for a buffer
	Buffer* bobj = dynamic_cast <Buffer*> (obj);
	if (bobj != nullptr) {
	  write (*bobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with xml content write",
			 Object::repr (obj));
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_WRITE) {
	String name = argv->getstring (0);
	Object* obj = argv->get (1);
	// check for an output stream
	OutputStream* os = dynamic_cast <OutputStream*> (obj);
	if (os != nullptr) {
	  write (name, os);
	  return nullptr;
	}
	// check for a uri path
	UriPath* urip = dynamic_cast <UriPath*> (obj);
	if (urip != nullptr) {
	  write (name, *urip);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with xml content write",
			 Object::repr (obj));
      }
    }
    // call the xml document method
    return XmlDocument::apply (zobj, nset, quark, argv);
  }
}
