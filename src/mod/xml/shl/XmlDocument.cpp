// ---------------------------------------------------------------------------
// - XmlDocument.cpp                                                         -
// - afnix:xml module - xml document class implementation                    -
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
#include "InputFile.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlReader.hpp"
#include "XmlTexter.hpp"
#include "BlockBuffer.hpp"
#include "XmlDocument.hpp"
#include "InputOutput.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure reads an input stream and return a root text node
  static XmlRoot* get_rtxt_node (InputStream* is) {
    // check the stream
    if (is == nullptr) return nullptr;
    // create a new reader and parse the input stream
    XmlTexter* xmlt = new XmlTexter;
    try {
      // parse the input stream
      xmlt->parse (is);
      // get the root node and protect it
      XmlRoot* result = xmlt-> getroot ();
      // clean the reader and result
      Object::iref (result);
      delete xmlt;
      Object::tref (result);
      // this is it
      return result;
    } catch (...) {
      delete xmlt;
      throw;
    }
  }

  // this procedure reads an input file and returns a root text node
  // the reference counter for the node is already increased
  static XmlRoot* get_rtxt_node (const String& name) {
    // open the file
    InputFile is (name);
    // get the root node
    return get_rtxt_node (&is);
  }

  // this procedure reads an input stream and return a root node
  static XmlRoot* get_root_node (InputStream* is) {
    // check the stream
    if (is == nullptr) return nullptr;
    // create a new reader and parse the input stream
    XmlReader* xmlr = new XmlReader;
    try {
      // parse the input stream
      xmlr->parse (is);
      // get the root node and protect it
      XmlRoot* result = xmlr-> getroot ();
      // clean the reader and result
      Object::iref (result);
      delete xmlr;
      Object::tref (result);
      // this is it
      return result;
    } catch (...) {
      delete xmlr;
      throw;
    }
  }

  // this procedure reads an input file and returns a root node
  // the reference counter for the node is already increased
  static XmlRoot* get_root_node (const String& name) {
    // open the file
    InputFile is (name);
    // get the root node
    return get_root_node (&is);
  }

  // this procedure reads a buffer and returns a root node
  // the reference counter for the node is already increased
  static XmlRoot* get_root_node (const Buffer& sbuf) {
    // create an input stream
    InputOutput is (sbuf);
    // get the root node
    return get_root_node (&is);
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default document

  XmlDocument::XmlDocument (void) {
    p_root = nullptr;
  }

  // create a document by name

  XmlDocument::XmlDocument (const String& name) {
    p_root = nullptr;
    setroot (name);
  }

  // create a document by name and buffer

  XmlDocument::XmlDocument (const String& name, const Buffer& sbuf) {
    p_root = nullptr;
    setroot (name, sbuf);
  }

  // create a document by name and input stream

  XmlDocument::XmlDocument (const String& name, InputStream* is) {
    p_root = nullptr;
    setroot (name, is);
  }

  // create a document by name and root node

  XmlDocument::XmlDocument (const String& name, XmlRoot* root) {
    p_root = nullptr;
    setroot (name, root);
  }

  // copy construct this document

  XmlDocument::XmlDocument (const XmlDocument& that) {
    that.rdlock ();
    try {
      d_name = that.d_name;
      if (that.p_root == nullptr) {
	p_root = nullptr;
      } else {
	Object::iref(p_root = dynamic_cast <XmlRoot*> (that.p_root->copy ()));
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this document

  XmlDocument::~XmlDocument (void) {
    // release the root node
    if (p_root != nullptr) p_root->release ();
    // eventually clean the root node
    Object::dref (p_root);
  }

  // return the document class name

  String XmlDocument::repr (void) const {
    return "XmlDocument";
  }

  // get a clone of this object

  Object* XmlDocument::clone (void) const {
    return new XmlDocument (*this);
  }

  // assign a xml document to this one

  XmlDocument& XmlDocument::operator = (const XmlDocument& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // release the root node
      if (p_root != nullptr) p_root->release ();
      // eventually clean the root node
      Object::dref (p_root);
      // copy the document name
      d_name = that.d_name;
      // copy the root node
      if (that.p_root == nullptr) {
	p_root = nullptr;
      } else {
	Object::iref (p_root = dynamic_cast <XmlRoot*> (that.p_root->copy ()));
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the document name

  String XmlDocument::getname (void) const {
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

  // set the document name

  void XmlDocument::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the document root by name

  void XmlDocument::setroot (const String& name) {
    wrlock ();
    try {
      Object::dref (p_root);
      p_root = nullptr;
      d_name = name;
      Object::iref (p_root = get_root_node (name));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the document root by name and buffer

  void XmlDocument::setroot (const String& name, const Buffer& sbuf) {
    wrlock ();
    try {
      Object::dref (p_root);
      p_root = nullptr;
      d_name = name;
      Object::iref (p_root = get_root_node (sbuf));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the document root by name and input stream

  void XmlDocument::setroot (const String& name, InputStream* is) {
    wrlock ();
    try {
      Object::dref (p_root);
      p_root = nullptr;
      d_name = name;
      Object::iref (p_root = get_root_node (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the document root by name and xml root node

  void XmlDocument::setroot (const String& name, XmlRoot* root) {
    wrlock ();
    Object::dref (p_root);
    p_root = nullptr;
    try {
      d_name = name;
      Object::iref (p_root = root);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the document root text by name

  void XmlDocument::setrtxt (const String& name){
    wrlock ();
    try {
      // release the old tree
      if (p_root != nullptr) {
	p_root->release ();
	Object::dref (p_root);
	p_root = nullptr;
      }
      // bind the tree by name
      d_name = name;
      Object::iref (p_root = get_rtxt_node (name));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the document root text by name and input stream

  void XmlDocument::setrtxt (const String& name, InputStream* is) {
    wrlock ();
    Object::dref (p_root);
    p_root = nullptr;
    try {
      d_name = name;
      Object::iref (p_root = get_rtxt_node (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root node or create a new one

  XmlRoot* XmlDocument::newroot (const bool dflg) {
    wrlock ();
    try {
      if (p_root == nullptr) Object::iref (p_root = new XmlRoot (dflg));
      XmlRoot* result = p_root;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root node

  XmlRoot* XmlDocument::getroot (void) const {
    rdlock ();
    try {
      XmlRoot* result = p_root;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root node by removing the root declaration

  XmlRoot* XmlDocument::getbody (void) const {
    rdlock ();
    try {
      XmlRoot* result = (p_root == nullptr) ? nullptr : p_root->bdup ();
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
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETNAME = zone.intern ("set-name");
  static const long QUARK_SETROOT = zone.intern ("set-root");
  static const long QUARK_SETRTXT = zone.intern ("set-text");
  static const long QUARK_NEWROOT = zone.intern ("new-root");
  static const long QUARK_GETROOT = zone.intern ("get-root");
  static const long QUARK_GETBODY = zone.intern ("get-body");

  // create a new object in a generic way

  Object* XmlDocument::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default document object
    if (argc == 0) return new XmlDocument;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new XmlDocument (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      // get the document name
      String name = argv->getstring (0);
      // get the object and check
      Object* obj = argv->get (1);
      // check for a buffer
      Buffer* buf = dynamic_cast <Buffer*> (obj);
      if (buf != nullptr) return new XmlDocument (name, *buf);
      // check for a block buffer
      BlockBuffer* bb = dynamic_cast <BlockBuffer*> (obj);
      if (bb != nullptr) return new XmlDocument (name, *bb);
      // check for an input stream
      InputStream* is = dynamic_cast <InputStream*> (obj);
      if (is != nullptr) return new XmlDocument (name, is);
      // check for a root node
      XmlRoot* root = dynamic_cast <XmlRoot*> (obj);
      if (root != nullptr) return new XmlDocument (name, root);
      throw Exception ("type-error", 
		       "invalid object with document constructor",
		       Object::repr (obj));	
    }
    throw Exception ("argument-error",
                     "too many argument with xml document constructor");
  }

  // return true if the given quark is defined

  bool XmlDocument::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Nameable::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlDocument::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_NEWROOT) {
	rdlock ();
	try {
	  Object* result = newroot (false);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETROOT) {
	rdlock ();
	try {
	  Object* result = getroot ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETBODY) {
	rdlock ();
	try {
	  Object* result = getbody ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_NEWROOT) {
	rdlock ();
	try {
	  bool dflg = argv->getbool (0);
	  Object* result = newroot (dflg);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETROOT) {
	String name = argv->getstring (0);
	setroot (name);
	return nullptr;
      }
      if (quark == QUARK_SETRTXT) {
	String name = argv->getstring (0);
	setrtxt (name);
	return nullptr;
      }
    }

    // check for 2 argument
    if (argc == 2) {
      if (quark == QUARK_SETROOT) {
	// get the document name
	String name = argv->getstring (0);
	// get the input stream
	Object* obj = argv->get (1);
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", 
			   "invalid input stream object with set-root",
			   Object::repr (obj));
	}
	setroot (name, is);
	return nullptr;
      }
      if (quark == QUARK_SETRTXT) {
	// get the document name
	String name = argv->getstring (0);
	// get the input stream
	Object* obj = argv->get (1);
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", 
			   "invalid input stream object with set-root",
			   Object::repr (obj));
	}
	setrtxt (name, is);
	return nullptr;
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
