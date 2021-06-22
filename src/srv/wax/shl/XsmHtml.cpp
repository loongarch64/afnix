// ---------------------------------------------------------------------------
// - XsmHtml.cpp                                                             -
// - afnix:wax service - xsm html tree class implementation                  -
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
#include "XsmHtml.hpp"
#include "InputFile.hpp"
#include "Exception.hpp"
#include "XsmReader.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------
  
  // the meta tag name
  static const String XSM_META_NAME = "meta";
  // the http-equiv attribute name
  static const String XSM_HTTP_ATTR = "http-equiv";
  // the content attribute
  static const String XSM_CNNT_ATTR = "content";
  // the content-type attribute value
  static const String XSM_CNTT_XVAL = "content-type";
  /// the content regex
  static const String XSM_CNTT_REGX = "text/html$b*[;$b*charset=(<$a->+)]?";

  // this procedure returns checks a node for a content type and eventually
  // mute the input stream. the node must be a normal node, defined with a
  // tag 'meta' and a 'http-equiv' attribute with a 'content-type'.
  static void set_xdoc_meta (InputStream* is, XsmNode* node) {
    // check the node and stream
    if ((is == nullptr) || (node == nullptr)) return;
    // check if we have a tag node
    if (node->istag () == false) return;
    // check if we have a meta tag
    if (node->getname (true) != XSM_META_NAME) return;
    // check if we have a http-equiv attribute
    Property* prop = node->getattr (XSM_HTTP_ATTR, true);
    if (prop == nullptr) return;
    // check the property value for a content type
    if (prop->getpval().tolower() != XSM_CNTT_XVAL) {
      Object::dref (prop);
      return;
    }
    // get the content attribute
    Object::dref (prop);
    prop = node->getattr (XSM_CNNT_ATTR, true);
    if (prop == nullptr) return;
    // check the content value
    String pval = prop->getpval().strip ();
    // clean the property
    Object::dref (prop);
    // check for the charset
    Regex re = XSM_CNTT_REGX;
    if (re == pval) {
      String emod = re.getstring (0);
      is->setemod (emod);
    }
  }

  // this procedure reads an input stream and return a vector
  static Vector* get_xdoc_tree (InputStream* is) {
    // create a result vector
    Vector* result = new Vector;
    // check the stream
    if (is == nullptr) return result;
    // create a new reader and parse the input stream
    XsmReader* xsmr = new XsmReader (is, true);
    try {
      while (true) {
	// get the node
	XsmNode* node = xsmr->getnode ();
	// eventually fix the stream
	set_xdoc_meta (is, node);
	// check and add
	if (node == nullptr) break;
	result->add (node);
      }
      delete xsmr;
      return result;
    } catch (...) {
      delete xsmr;
      delete result;
      throw;
    }
  }

  // this procedure reads an input file and returns a root node
  // the reference counter for the node is already increased
  static Vector* get_xdoc_tree (const String& name) {
    // open the file
    InputStream* is = new InputFile (name);
    try {
      // protect the stream
      Object::iref (is);
      // by default we operate in UTF-8 mode
      is->setemod (Encoding::EMOD_UTF8);
      // get tree vector
      Vector* result = get_xdoc_tree (is);
      // release the stream
      Object::dref (is);
      return result;
    } catch (...) {
      Object::dref (is);
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default html tree

  XsmHtml::XsmHtml (void) {
    p_tree = nullptr;
  }

  // create a html tree by name

  XsmHtml::XsmHtml (const String& name) {
    // create a reader
    d_name = name;
    Object::iref (p_tree = get_xdoc_tree (name));
  }

  // create a html tree by name and input stream

  XsmHtml::XsmHtml (const String& name, InputStream* is) {
    d_name = name;
    Object::iref (p_tree = get_xdoc_tree (is));
  }

  // return the html tree class name

  String XsmHtml::repr (void) const {
    return "XsmHtml";
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XsmHtml::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default html tree object
    if (argc == 0) return new XsmHtml;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XsmHtml (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the html tree name
      String name = argv->getstring (0);
      // get the object and check
      Object* obj = argv->get (1);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new XsmHtml (name, is);
      throw Exception ("type-error", 
		       "invalid object with xsm html tree constructor",
		       Object::repr (obj));	
    }
    throw Exception ("argument-error",
                     "too many argument with xsm html tree constructor");
  }
}
