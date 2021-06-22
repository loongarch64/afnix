// ---------------------------------------------------------------------------
// - XhtmlTd.hpp                                                             -
// - afnix:wax service - xhtml td node class definition                      -
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

#ifndef  AFNIX_XHTMLTD_HPP
#define  AFNIX_XHTMLTD_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlTd class is a xhtml td node used in the table row node.
  /// @author amaury darsch

  class XhtmlTd : public XhtmlBase {
  public:
    /// create an empty td node
    XhtmlTd (void);

    /// create a xhtml td node with a class
    /// @param cls the node class
    XhtmlTd (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlTd (const XhtmlTd&);
    // make the assignment operator private
    XhtmlTd& operator = (const XhtmlTd&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
