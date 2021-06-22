// ---------------------------------------------------------------------------
// - XhtmlLi.hpp                                                             -
// - afnix:wax service - xhtml list item node class definition               -
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

#ifndef  AFNIX_XHTMLI_HPP
#define  AFNIX_XHTMLI_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlLi class is a xhtml list item node used in the ul element 
  /// of a xhtml page.
  /// @author amaury darsch

  class XhtmlLi : public XhtmlBase {
  public:
    /// create an empty list item node
    XhtmlLi (void);

    /// create a xhtml list item node with a class
    /// @param cls the node class
    XhtmlLi (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlLi (const XhtmlLi&);
    // make the assignment operator private
    XhtmlLi& operator = (const XhtmlLi&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
