// ---------------------------------------------------------------------------
// - XsmHtml.hpp                                                             -
// - afnix:wax service - xsm html document class definition                  -
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

#ifndef  AFNIX_XSMHTML_HPP
#define  AFNIX_XSMHTML_HPP

#ifndef  AFNIX_XSMTREE_HPP
#include "XsmTree.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The XsmHtml class is the document class that maintains a xsm html 
  /// tree along with its associated list of nodes and other useful 
  /// information. The class is similar to the XsmDocument object but 
  /// provides specific support for the html environment.In particular,
  /// special care is taken with the meta tag that might affect the
  /// content type.
  /// @author amaury darsch

  class XsmHtml : public XsmTree {
  public:
    /// create a default document
    XsmHtml (void);

    /// create an document by name
    /// @param name the document file name
    XsmHtml (const String& name);

    /// create an document by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    XsmHtml (const String& name, InputStream* is);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XsmHtml (const XsmHtml&);
    // make the assignment operator private
    XsmHtml& operator = (const XsmHtml&);

  public:
    /// create an object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
