// ---------------------------------------------------------------------------
// - XhtmlTitle.hpp                                                          -
// - afnix:wax service - xhtml title node class definition                   -
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

#ifndef  AFNIX_XHTMLTITLE_HPP
#define  AFNIX_XHTMLTITLE_HPP

#ifndef  AFNIX_XMLTAG_HPP
#include "XmlTag.hpp"
#endif

namespace afnix {

  /// The XhtmlTitle class is a xhtml title node used in the head node.
  /// @author amaury darsch

  class XhtmlTitle : public XmlTag {
  public:
    /// create a xhtml title node
    /// @param title the node title
    XhtmlTitle (const String& title);

    /// @return the class name
    String repr (void) const;

    /// set the title by value
    /// @param title the title to set
    void settitle (const String& title);

  private:
    // make the copy constructor private
    XhtmlTitle (const XhtmlTitle&);
    // make the assignment operator private
    XhtmlTitle& operator = (const XhtmlTitle&);

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
