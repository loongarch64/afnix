// ---------------------------------------------------------------------------
// - XhtmlOption.hpp                                                         -
// - afnix:wax service - xhtml select hidden node class definition           -
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

#ifndef  AFNIX_XHTMLHIDDEN_HPP
#define  AFNIX_XHTMLHIDDEN_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlHidden class is a xhtml element used in the html form
  /// element. An hidden element is primarily defined with a label and
  /// a value and is not shown.
  /// @author amaury darsch

  class XhtmlHidden : public XmlTag {
  public:
    /// create a xhtml input hidden by name and value
    /// @param name the hidden name
    /// @param sval the hidden value
    XhtmlHidden (const String& name, const String& sval);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlHidden (const XhtmlHidden&);
    // make the assignment operator private
    XhtmlHidden& operator = (const XhtmlHidden&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
