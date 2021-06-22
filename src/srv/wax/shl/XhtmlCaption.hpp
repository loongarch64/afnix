// ---------------------------------------------------------------------------
// - XhtmlCaption.hpp                                                        -
// - afnix:wax service - xhtml caption node class definition                 -
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

#ifndef  AFNIX_XHTMLCAPTION_HPP
#define  AFNIX_XHTMLCAPTION_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlCaption class is a xhtml caption node used in the table 
  /// element of a xhtml page. Most of the time the node is added automatically
  /// from the table.
  /// @author amaury darsch

  class XhtmlCaption : public XmlTag {
  public:
    /// create a xhtml caption node by text
    /// @param text the caption text
    XhtmlCaption (const String& text);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlCaption (const XhtmlCaption&);
    // make the assignment operator private
    XhtmlCaption& operator = (const XhtmlCaption&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
