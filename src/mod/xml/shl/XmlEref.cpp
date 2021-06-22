// ---------------------------------------------------------------------------
// - XmlEref.cpp                                                             -
// - afnix:xml module - xml entity reference node class implementation       -
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

#include "XmlEref.hpp"
#include "XmlSystem.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default reference node

  XmlEref::XmlEref (void) {
  }

  // create a reference node by value

  XmlEref::XmlEref (const String& xref) : XmlRef (xref) {
  }

  // return the document class name

  String XmlEref::repr (void) const {
    return "XmlEref";
  }

  // get a clone of this node

  Object* XmlEref::clone (void) const {
    rdlock ();
    try {
      // create a new node
      XmlEref* result = new XmlEref;
      // copy the ref arguments
      result->d_xref = d_xref;
      // update the node info
      result->d_eflg = d_eflg;
      result->d_lnum = d_lnum;
      result->d_snam = d_snam;
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the reference node content as a text

  String XmlEref::totext (void) const {
    rdlock ();
    String xref = "&";
    xref += d_xref;
    xref += ";";
    String result = xref;
    unlock ();
    return result;
  }

  // write a eref node to a buffer

  void XmlEref::write (Buffer& buf) const {
    rdlock ();
    try {
      buf.add ("&");
      buf.add (d_xref);
      buf.add (";");
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a eref node to an output stream

  void XmlEref::write (OutputStream& os) const {
    rdlock ();
    try {
      os.write ("&");
      os.write (d_xref);
      os.write (";");
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* XmlEref::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlEref;
    // check for 1 argument
    if (argc == 1) {
      String xref = argv->getstring (0);
      return new XmlEref (xref);
    }
    throw Exception ("argument-error",
                     "too many arguments with xml eref node constructor");
  }
}
