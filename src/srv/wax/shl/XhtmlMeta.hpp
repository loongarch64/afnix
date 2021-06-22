// ---------------------------------------------------------------------------
// - XhtmlMeta.hpp                                                           -
// - afnix:wax service - xhtml meta node class definition                    -
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

#ifndef  AFNIX_XHTMLMETA_HPP
#define  AFNIX_XHTMLMETA_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlMeta class is a xhtml meta node used in the head node. The
  /// meta data node is an empty node with two attributes which are the
  /// descriptor name and the content value. The meta data is stored 
  /// interanlly as a xml attribute.
  /// @author amaury darsch

  class XhtmlMeta : public XmlTag {
  public:
    /// create a xhtml meta node by name and content
    /// @param name the name attribute
    /// @param cnts the content attribute
    XhtmlMeta (const String& name, const String& cnts);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlMeta (const XhtmlMeta&);
    // make the assignment operator private
    XhtmlMeta& operator = (const XhtmlMeta&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
