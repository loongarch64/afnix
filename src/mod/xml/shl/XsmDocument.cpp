// ---------------------------------------------------------------------------
// - XsmDocument.cpp                                                         -
// - afnix:xml module - xsm document class implementation                    -
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

#include "Vector.hpp"
#include "InputFile.hpp"
#include "Exception.hpp"
#include "XsmReader.hpp"
#include "XsmDocument.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure reads an input stream and return a vector
  static Vector* get_xdoc_tree (InputStream* is) {
    // create a result vector
    Vector* result = new Vector;
    // check the stream
    if (is == nullptr) return result;
    // create a new reader and parse the input stream
    XsmReader* xsmr = new XsmReader (is);
    try {
      while (true) {
	XsmNode* node = xsmr->getnode ();
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
      // get document vector
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

  // create a default document

  XsmDocument::XsmDocument (void) {
    p_tree = nullptr;
  }

  // create a document by name

  XsmDocument::XsmDocument (const String& name) {
    // create a reader
    d_name = name;
    Object::iref (p_tree = get_xdoc_tree (name));
  }

  // create a document by name and input stream

  XsmDocument::XsmDocument (const String& name, InputStream* is) {
    d_name = name;
    Object::iref (p_tree = get_xdoc_tree (is));
  }

  // return the document class name

  String XsmDocument::repr (void) const {
    return "XsmDocument";
  }


  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XsmDocument::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default document object
    if (argc == 0) return new XsmDocument;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XsmDocument (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the document name
      String name = argv->getstring (0);
      // get the object and check
      Object* obj = argv->get (1);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new XsmDocument (name, is);
      throw Exception ("type-error", 
		       "invalid object with xsm document constructor",
		       Object::repr (obj));	
    }
    throw Exception ("argument-error",
                     "too many argument with xsm document constructor");
  }
}
