// ---------------------------------------------------------------------------
// - XhtmlTable.hpp                                                          -
// - afnix:wax service - xhtml table node class definition                   -
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

#ifndef  AFNIX_XHTMLTABLE_HPP
#define  AFNIX_XHTMLTABLE_HPP

#ifndef  AFNIX_XHTMLTELEM_HPP
#include "XhtmlTelem.hpp"
#endif

namespace afnix {

  /// The XhtmlTable class is a xhtml table node. The table node is designed
  /// to accumulate table row nodes or column group (colgroup) nodes. The
  /// table can alsobe designed with a table head, body and foot nodes.
  /// @author amaury darsch

  class XhtmlTable : public XhtmlTelem {
  public:
    /// create an empty table node
    XhtmlTable (void);

    /// create a xhtml table node with a class
    /// @param cls the node class
    XhtmlTable (const String& cls);

    /// create a xhtml table node with a class and a default tr class
    /// @param cls the node class
    /// @param trc the default tr class
    XhtmlTable (const String& cls, const String& trc);

    /// create a xhtml table node with a class, a default tr and tx class
    /// @param cls the node class
    /// @param trc the default tr class
    /// @param txc the default tx class
    XhtmlTable (const String& cls, const String& trc, const String& txc);

    /// @return the class name
    String repr (void) const;

    /// set the table caption text
    /// @param text the caption text
    void setcaption (const String& text);

  private:
    // make the copy constructor private
    XhtmlTable (const XhtmlTable&);
    // make the assignment operator private
    XhtmlTable& operator = (const XhtmlTable&);

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
