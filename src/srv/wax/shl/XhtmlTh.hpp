// ---------------------------------------------------------------------------
// - XhtmlTh.hpp                                                             -
// - afnix:wax service - xhtml th node class definition                      -
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

#ifndef  AFNIX_XHTMLTH_HPP
#define  AFNIX_XHTMLTH_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlTh class is a xhtml th node used in the table row node.
  /// @author amaury darsch

  class XhtmlTh : public XhtmlBase {
  public:
    /// create an empty th node
    XhtmlTh (void);

    /// create a xhtml th node with a class
    /// @param cls the node class
    XhtmlTh (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlTh (const XhtmlTh&);
    // make the assignment operator private
    XhtmlTh& operator = (const XhtmlTh&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
