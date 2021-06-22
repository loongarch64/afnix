// ---------------------------------------------------------------------------
// - XhtmlCol.hpp                                                            -
// - afnix:wax service - xhtml col node class definition                     -
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

#ifndef  AFNIX_XHTMLCOL_HPP
#define  AFNIX_XHTMLCOL_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlCol class is a xhtml col node used in the table column group
  /// element. The col element is empty.
  /// @author amaury darsch

  class XhtmlCol : public XmlTag {
  public:
    /// create an empty col node
    XhtmlCol (void);

    /// create a col node with a width attribute
    /// @param width the width attribute to set
    XhtmlCol (const String& width);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlCol (const XhtmlCol&);
    // make the assignment operator private
    XhtmlCol& operator = (const XhtmlCol&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
