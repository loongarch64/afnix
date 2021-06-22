// ---------------------------------------------------------------------------
// - XhtmlOption.hpp                                                         -
// - afnix:wax service - xhtml select option node class definition           -
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

#ifndef  AFNIX_XHTMLOPTION_HPP
#define  AFNIX_XHTMLOPTION_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlOption class is a xhtml element used in the html select
  /// element. An option element is primarily defined with a label and
  /// a value.
  /// @author amaury darsch

  class XhtmlOption : public XmlTag {
  public:
    /// create a xhtml label and value
    /// @param lbal the option label
    /// @param sval the option value
    XhtmlOption (const String& lval, const String& sval);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlOption (const XhtmlOption&);
    // make the assignment operator private
    XhtmlOption& operator = (const XhtmlOption&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
