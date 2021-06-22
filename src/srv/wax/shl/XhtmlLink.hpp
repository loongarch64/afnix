// ---------------------------------------------------------------------------
// - XhtmlLink.hpp                                                           -
// - afnix:wax service - xhtml link node class definition                    -
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

#ifndef  AFNIX_XHTMLLINK_HPP
#define  AFNIX_XHTMLLINK_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlLink class is a xhtml link node used in the head node. The
  /// link node is an empty node with several attributes. The most important
  /// one is the 'href' attribute that specifies the link uri. Other attributes
  /// like 'type' or 'rel' can also be set at construction.
  /// @author amaury darsch

  class XhtmlLink : public XmlTag {
  public:
    /// create a xhtml link node by reference
    /// @param href the href attribute
    XhtmlLink (const String& href);

    /// create a xhtml link node by reference and type
    /// @param href the href attribute
    /// @param type the type attribute
    XhtmlLink (const String& href, const String& type);

    /// create a xhtml link node by reference, type and relation
    /// @param href the href attribute
    /// @param type the type attribute
    /// @param lrel the link relation attribute
    XhtmlLink (const String& href, const String& type, const String& lrel);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlLink (const XhtmlLink&);
    // make the assignment operator private
    XhtmlLink& operator = (const XhtmlLink&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
