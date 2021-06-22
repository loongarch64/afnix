// ---------------------------------------------------------------------------
// - XhtmlUl.hpp                                                             -
// - afnix:wax service - xhtml ul node class definition                      -
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

#ifndef  AFNIX_XHTMLUL_HPP
#define  AFNIX_XHTMLUL_HPP

#ifndef  AFNIX_XHTMLI_HPP
#include "XhtmlLi.hpp"
#endif

namespace afnix {

  /// The XhtmlUl class is a xhtml ul node used in the body element 
  /// of a xhtml page. The "ul" node is a xhtml tag designed to manage
  /// list in conjonction with the "item" tag.
  /// @author amaury darsch

  class XhtmlUl : public XhtmlBase {
  private:
    /// the default item class
    String d_xcls;

  public:
    /// create an empty ul node
    XhtmlUl (void);

    /// create a xhtml ul node with a class
    /// @param cls the node class
    XhtmlUl (const String& cls);

    /// create a xhtml ul node with a class and a item class
    /// @param cls the node class
    /// @param xcls the item class
    XhtmlUl (const String& cls, const String& xcls);

    /// @return the class name
    String repr (void) const;

    /// create a default item
    XhtmlLi* newitem (void);

    /// create a default by class
    /// @param cls the item class to use
    XhtmlLi* newitem (const String& cls);

  private:
    // make the copy constructor private
    XhtmlUl (const XhtmlUl&);
    // make the assignment operator private
    XhtmlUl& operator = (const XhtmlUl&);

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
