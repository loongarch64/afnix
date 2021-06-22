// ---------------------------------------------------------------------------
// - XhtmlCgr.hpp                                                            -
// - afnix:wax service - xhtml column group node class definition            -
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

#ifndef  AFNIX_XHTMLCGR_HPP
#define  AFNIX_XHTMLCGR_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlCgr class is a xhtml column group node used in the table
  /// element. The column group is designed to hold the col element.
  /// @author amaury darsch

  class XhtmlCgr : public XmlTag {
  public:
    /// create an empty cgr node
    XhtmlCgr (void);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlCgr (const XhtmlCgr&);
    // make the assignment operator private
    XhtmlCgr& operator = (const XhtmlCgr&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
