// ---------------------------------------------------------------------------
// - XhtmlH2.hpp                                                             -
// - afnix:wax service - xhtml heading 1 node class definition               -
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

#ifndef  AFNIX_XHTMLH2_HPP
#define  AFNIX_XHTMLH2_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlH2 class is a xhtml heading 2 node used in the body element
  /// of a xhtml page. The heading node can be created with a style name.
  /// @author amaury darsch

  class XhtmlH2 : public XhtmlBase {
  public:
    /// create an empty heading node
    XhtmlH2 (void);

    /// create a xhtml heading node with a class
    /// @param cls the node class
    XhtmlH2 (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlH2 (const XhtmlH2&);
    // make the assignment operator private
    XhtmlH2& operator = (const XhtmlH2&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
