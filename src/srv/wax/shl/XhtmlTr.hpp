// ---------------------------------------------------------------------------
// - XhtmlTr.hpp                                                             -
// - afnix:wax service - xhtml tr node class definition                      -
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

#ifndef  AFNIX_XHTMLTR_HPP
#define  AFNIX_XHTMLTR_HPP

#ifndef  AFNIX_XHTMLTH_HPP
#include "XhtmlTh.hpp"
#endif

#ifndef  AFNIX_XHTMLTD_HPP
#include "XhtmlTd.hpp"
#endif

namespace afnix {

  /// The XhtmlTr class is a xhtml tr node used in the table node. The table
  /// row node (tr) is designed to accumulate table head (th) or table data 
  /// (td) nodes.
  /// @author amaury darsch

  class XhtmlTr : public XhtmlBase {
  private:
    /// the default th class
    String d_thc;
    /// the default td class
    String d_tdc;

  public:
    /// create an empty tr node
    XhtmlTr (void);

    /// create a xhtml tr node with a class
    /// @param cls the node class
    XhtmlTr (const String& cls);

    /// create a xhtml tr node with a class and a default td class
    /// @param cls the node class
    /// @param txc the default tx class
    XhtmlTr (const String& cls, const String& txc);

    /// @return the class name
    String repr (void) const;

    /// set the default th class
    /// @param thc the default th class
    void setthc (const String& thc);

    /// set the default td class
    /// @param tdc the default td class
    void settdc (const String& tdc);

    /// set the default th and td class
    /// @param txc the default td and tx classes
    void settxc (const String& txc);

    /// @return a new table head
    XhtmlTh* newth (void);

    /// @return a new table head by class name
    XhtmlTh* newth (const String& cls);

    /// @return a new table data
    XhtmlTd* newtd (void);

    /// @return a new table data by class name
    XhtmlTd* newtd (const String& cls);

  private:
    // make the copy constructor private
    XhtmlTr (const XhtmlTr&);
    // make the assignment operator private
    XhtmlTr& operator = (const XhtmlTr&);

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
