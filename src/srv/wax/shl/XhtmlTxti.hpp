// ---------------------------------------------------------------------------
// - XhtmlTxti.hpp                                                           -
// - afnix:wax service - xhtml text italic node class definition             -
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

#ifndef  AFNIX_XHTMLTXTI_HPP
#define  AFNIX_XHTMLTXTI_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlTxti class is a xhtml text italic node used in the body element
  /// of a xhtml page.
  /// @author amaury darsch

  class XhtmlTxti : public XhtmlBase {
  public:
    /// create an empty text italic node
    XhtmlTxti (void);

    /// create a xhtml text italic node with a class
    /// @param cls the node class
    XhtmlTxti (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlTxti (const XhtmlTxti&);
    // make the assignment operator private
    XhtmlTxti& operator = (const XhtmlTxti&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
