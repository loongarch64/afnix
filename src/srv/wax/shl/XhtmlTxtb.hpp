// ---------------------------------------------------------------------------
// - XhtmlTxtb.hpp                                                           -
// - afnix:wax service - xhtml text bold node class definition               -
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

#ifndef  AFNIX_XHTMLTXTB_HPP
#define  AFNIX_XHTMLTXTB_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlTxtb class is a xhtml text bold node used in the body element
  /// of a xhtml page.
  /// @author amaury darsch

  class XhtmlTxtb : public XhtmlBase {
  public:
    /// create an empty text bold node
    XhtmlTxtb (void);

    /// create a xhtml text bold node with a class
    /// @param cls the node class
    XhtmlTxtb (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlTxtb (const XhtmlTxtb&);
    // make the assignment operator private
    XhtmlTxtb& operator = (const XhtmlTxtb&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
