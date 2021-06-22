// ---------------------------------------------------------------------------
// - XhtmlRef.hpp                                                            -
// - afnix:wax service - xhtml ref (a) node class definition                 -
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

#ifndef  AFNIX_XHTMLREF_HPP
#define  AFNIX_XHTMLREF_HPP

#ifndef  AFNIX_XHTMLBASE_HPP
#include "XhtmlBase.hpp"
#endif

namespace afnix {

  /// The XhtmlRef class is a xhtml reference node used in the body 
  /// elementt of a xhtml page. The node can be used to
  /// create hyperlink that references objects by a uri. 
  /// @author amaury darsch

  class XhtmlRef : public XhtmlBase {
  public:
    /// create an empty ref node
    XhtmlRef (void);

    /// create a xhtml ref node with a uri
    /// @param uri the reference uri
    XhtmlRef (const String& uri);

    /// create a xhtml ref node with a uri and text
    /// @param uri the reference uri
    /// @param txt the reference text
    XhtmlRef (const String& uri, const String& txt);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlRef (const XhtmlRef&);
    // make the assignment operator private
    XhtmlRef& operator = (const XhtmlRef&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
