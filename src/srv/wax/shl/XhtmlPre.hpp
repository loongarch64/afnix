// ---------------------------------------------------------------------------
// - XhtmlPre.hpp                                                            -
// - afnix:wax service - xhtml pre node class definition                     -
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

#ifndef  AFNIX_XHTMLPRE_HPP
#define  AFNIX_XHTMLPRE_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlPre class is a xhtml pre node used in the body element 
  /// of a xhtml page. The pre node is a xhtml preformatted element.
  /// @author amaury darsch

  class XhtmlPre : public XhtmlBase {
  public:
    /// create an empty pre node
    XhtmlPre (void);

    /// create a xhtml pre node with a class
    /// @param cls the node class
    XhtmlPre (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlPre (const XhtmlPre&);
    // make the assignment operator private
    XhtmlPre& operator = (const XhtmlPre&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
