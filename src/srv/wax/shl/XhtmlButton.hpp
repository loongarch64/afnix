// ---------------------------------------------------------------------------
// - XhtmlButton.hpp                                                         -
// - afnix:wax service - xhtml button node class definition                  -
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

#ifndef  AFNIX_XHTMLBUTTON_HPP
#define  AFNIX_XHTMLBUTTON_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlButton class is a xhtml button node which can be used to
  /// trigger an action.
  /// @author amaury darsch

  class XhtmlButton : public XhtmlBase {
  public:
    /// create an empty button node
    XhtmlButton (void);

    /// create a xhtml button node with a name
    /// @param name the button name
    XhtmlButton (const String& name);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlButton (const XhtmlButton&);
    // make the assignment operator private
    XhtmlButton& operator = (const XhtmlButton&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
