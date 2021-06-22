// ---------------------------------------------------------------------------
// - XhtmlPara.hpp                                                           -
// - afnix:wax service - xhtml paragraph node class definition               -
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

#ifndef  AFNIX_XHTMLPARA_HPP
#define  AFNIX_XHTMLPARA_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlPara class is a xhtml paragraph node used in the body element
  /// of a xhtml page. The paragraph node can be created with a style name
  /// or as an empty node.
  /// @author amaury darsch

  class XhtmlPara : public XhtmlBase {
  public:
    /// create an empty para node
    XhtmlPara (void);

    /// create a xhtml para node with a class
    /// @param cls the node class
    XhtmlPara (const String& cls);

    /// create an empty xhtml para node
    /// @param eflg the empty flag to use
    XhtmlPara (const bool eflg);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlPara (const XhtmlPara&);
    // make the assignment operator private
    XhtmlPara& operator = (const XhtmlPara&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
