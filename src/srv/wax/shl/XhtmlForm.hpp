// ---------------------------------------------------------------------------
// - XhtmlForm.hpp                                                           -
// - afnix:wax service - xhtml form node class definition                    -
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

#ifndef  AFNIX_XHTMLFORM_HPP
#define  AFNIX_XHTMLFORM_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlForm class is a xhtml form node used in the html body. The
  /// form node is used to aggregate user controls as well as other nodes.
  /// A form has an action and a method. The action is an hyperlink while
  /// the methid is a submission method which can be either "GET" or "POST".
  /// @author amaury darsch

  class XhtmlForm : public XmlTag {
  public:
    /// create a xhtml form by action and method
    /// @param href the action hyperlink reference
    /// @param meth the method form
    XhtmlForm (const String& href, const String& meth);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlForm (const XhtmlForm&);
    // make the assignment operator private
    XhtmlForm& operator = (const XhtmlForm&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
