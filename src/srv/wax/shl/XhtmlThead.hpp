// ---------------------------------------------------------------------------
// - XhtmlThead.hpp                                                          -
// - afnix:wax service - xhtml thead node class definition                   -
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

#ifndef  AFNIX_XHTMLTHEAD_HPP
#define  AFNIX_XHTMLTHEAD_HPP

#ifndef  AFNIX_XHTMLTELEM_HPP
#include "XhtmlTelem.hpp"
#endif

namespace afnix {

  /// The XhtmlThead class is a xhtml thead node. The thead node is
  /// designed to accumulate table rows nodes. The class acts almost like
  /// the xhtml table class.
  /// @author amaury darsch

  class XhtmlThead : public XhtmlTelem {
  public:
    /// create an empty thead node
    XhtmlThead (void);

    /// create a xhtml thead node with a class
    /// @param cls the node class
    XhtmlThead (const String& cls);

    /// create a xhtml thead node with a class and a default tr class
    /// @param cls the node class
    /// @param trc the default tr class
    XhtmlThead (const String& cls, const String& trc);

    /// create a xhtml thead node with a class, a default tr and tx class
    /// @param cls the node class
    /// @param trc the default tr class
    /// @param txc the default tx class
    XhtmlThead (const String& cls, const String& trc, const String& txc);

    /// @return the class name
    String repr (void) const;
    
  private:
    // make the copy constructor private
    XhtmlThead (const XhtmlThead&);
    // make the assignment operator private
    XhtmlThead& operator = (const XhtmlThead&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
