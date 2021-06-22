// ---------------------------------------------------------------------------
// - XhtmlStyle.hpp                                                          -
// - afnix:wax service - xhtml style node class definition                   -
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

#ifndef  AFNIX_XHTMLSTYLE_HPP
#define  AFNIX_XHTMLSTYLE_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlStyle class is a xhtml style node used in the head node. The 
  /// style node is built with a xml data node that holds the formatted
  /// url string.
  /// @author amaury darsch

  class XhtmlStyle : public XmlTag {
  public:
    /// create a xhtml style node with a url
    /// @param url the url of the style
    XhtmlStyle (const String& url);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlStyle (const XhtmlStyle&);
    // make the assignment operator private
    XhtmlStyle& operator = (const XhtmlStyle&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
