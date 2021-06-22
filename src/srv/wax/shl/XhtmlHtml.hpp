// ---------------------------------------------------------------------------
// - XhtmlHtml.hpp                                                           -
// - afnix:wax service - xhtml html node class definition                    -
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

#ifndef  AFNIX_XHTMLHTML_HPP
#define  AFNIX_XHTMLHTML_HPP

#ifndef  AFNIX_XHTMLHEAD_HPP
#include "XhtmlHead.hpp"
#endif

#ifndef  AFNIX_XHTMLBODY_HPP
#include "XhtmlBody.hpp"
#endif

namespace afnix {

  /// The XhtmlHtml class is a xhtml html node used for the design of a xhtml
  /// document page. At construction, the html node is initialized with a 
  /// head node and a body node. Because a valid xhtml document must contain 
  /// a title, the constructor takes at least a title argument.
  /// @author amaury darsch

  class XhtmlHtml : public XmlTag {
  public:
    /// create a xhtml html node with a title
    /// @param title the node title
    XhtmlHtml (const String& title);

    /// @return the class name
    String repr (void) const;

    /// @return the xhtml head node
    XhtmlHead* gethead (void) const;

    /// @return the xhtml body node
    XhtmlBody* getbody (void) const;

  private:
    // make the copy constructor private
    XhtmlHtml (const XhtmlHtml&);
    // make the assignment operator private
    XhtmlHtml& operator = (const XhtmlHtml&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
