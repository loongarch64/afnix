// ---------------------------------------------------------------------------
// - XhtmlHr.hpp                                                             -
// - afnix:wax service - xhtml hr node class definition                      -
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

#ifndef  AFNIX_XHTMLHR_HPP
#define  AFNIX_XHTMLHR_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlHr class is a xhtml hr node used in the body element 
  /// of a xhtml page. The hr node is a xhtml horizontal ruler element.
  /// @author amaury darsch

  class XhtmlHr : public XhtmlBase {
  public:
    /// create an empty hr node
    XhtmlHr (void);

    /// create a xhtml hr node with a class
    /// @param cls the node class
    XhtmlHr (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlHr (const XhtmlHr&);
    // make the assignment operator private
    XhtmlHr& operator = (const XhtmlHr&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
