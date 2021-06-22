// ---------------------------------------------------------------------------
// - XhtmlTxtt.hpp                                                           -
// - afnix:wax service - xhtml text teletype node class definition           -
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

#ifndef  AFNIX_XHTMLTXTT_HPP
#define  AFNIX_XHTMLTXTT_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlTxtt class is a xhtml text teletype node used in the body
  /// element of a xhtml page.
  /// @author amaury darsch

  class XhtmlTxtt : public XhtmlBase {
  public:
    /// create an empty text teletype node
    XhtmlTxtt (void);

    /// create a xhtml text teletype node with a class
    /// @param cls the node class
    XhtmlTxtt (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlTxtt (const XhtmlTxtt&);
    // make the assignment operator private
    XhtmlTxtt& operator = (const XhtmlTxtt&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
