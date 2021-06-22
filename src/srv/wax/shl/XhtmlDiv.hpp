// ---------------------------------------------------------------------------
// - XhtmlDiv.hpp                                                            -
// - afnix:wax service - xhtml div node class definition                     -
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

#ifndef  AFNIX_XHTMLDIV_HPP
#define  AFNIX_XHTMLDIV_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlDiv class is a xhtml div node used in the body element 
  /// of a xhtml page. The div node is a xhtml grouping element.
  /// @author amaury darsch

  class XhtmlDiv : public XhtmlBase {
  public:
    /// create an empty div node
    XhtmlDiv (void);

    /// create an empty div node by empty flag
    /// @param eflg the empty flag to set
    XhtmlDiv (const bool eflg);

    /// create a xhtml div node with a class
    /// @param cls the node class
    XhtmlDiv (const String& cls);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlDiv (const XhtmlDiv&);
    // make the assignment operator private
    XhtmlDiv& operator = (const XhtmlDiv&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
