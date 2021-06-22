// ---------------------------------------------------------------------------
// - XhtmlBody.hpp                                                           -
// - afnix:wax service - xhtml body node class definition                    -
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

#ifndef  AFNIX_XHTMLBODY_HPP
#define  AFNIX_XHTMLBODY_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlBody class is a xhtml body node used for the design of a xhtml
  /// document page. The class is designed to be filled with other xhtml nodes.
  /// @author amaury darsch

  class XhtmlBody : public XmlTag {
  public:
    /// create an empty xhtml body
    XhtmlBody (void);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlBody (const XhtmlBody&);
    // make the assignment operator private
    XhtmlBody& operator = (const XhtmlBody&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
