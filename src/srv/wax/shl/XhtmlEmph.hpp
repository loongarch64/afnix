// ---------------------------------------------------------------------------
// - XhtmlEmph.hpp                                                           -
// - afnix:wax service - xhtml emphasize node class definition               -
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

#ifndef  AFNIX_XHTMLEMPH_HPP
#define  AFNIX_XHTMLEMPH_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlEmph class is a xhtml emphasize node used in the body element
  /// of a xhtml page. The emphasize node can be created with a style name.
  /// @author amaury darsch

  class XhtmlEmph : public XhtmlBase {
  public:
    /// create a default emph node
    XhtmlEmph (void);

    /// create a xhtml emph node with a class
    /// @param cls the node class
    XhtmlEmph (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlEmph (const XhtmlEmph&);
    // make the assignment operator private
    XhtmlEmph& operator = (const XhtmlEmph&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
