// ---------------------------------------------------------------------------
// - XsmDocument.hpp                                                         -
// - afnix:xml module - xsm document class definition                        -
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

#ifndef  AFNIX_XSMDOCUMENT_HPP
#define  AFNIX_XSMDOCUMENT_HPP

#ifndef  AFNIX_XSMTREE_HPP
#include "XsmTree.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The XsmDocument class is the document class that maintains a
  /// xsm document from a xsm tree. Generally the class is constructed with
  /// a file name or a name and an input stream that is used for parsing the 
  /// input data. When the class is built, all methods n the xsm tree can be
  /// used to access the xsm node objects.
  /// @author amaury darsch

  class XsmDocument : public XsmTree {
  public:
    /// create a default document
    XsmDocument (void);

    /// create an document by name
    /// @param name the document file name
    XsmDocument (const String& name);

    /// create an document by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    XsmDocument (const String& name, InputStream* is);

    /// @return the document class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XsmDocument (const XsmDocument&);
    // make the assignment operator private
    XsmDocument& operator = (const XsmDocument&);

  public:
    /// create an object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
