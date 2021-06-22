// ---------------------------------------------------------------------------
// - XhtmlTelem.hpp                                                          -
// - afnix:wax service - xhtml table element node class definition           -
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

#ifndef  AFNIX_XHTMLTELEM_HPP
#define  AFNIX_XHTMLTELEM_HPP

#ifndef  AFNIX_PRINTTABLE_HPP
#include "PrintTable.hpp"
#endif

#ifndef  AFNIX_XHTMLTR_HPP
#include "XhtmlTr.hpp"
#endif

namespace afnix {

  /// The XhtmlTElem class is is an abstract class that implements the node
  /// behavior for the thead, tbody, tfoot and table xhtml elements. The table
  /// element node is designed to accumulate table row nodes. This 
  /// class cannot be constructed directly.
  /// @author amaury darsch

  class XhtmlTelem : public XhtmlBase {
  protected:
    /// the default tr class
    String d_trc;
    /// the default txc class
    String d_txc;

  protected:
    /// create a table element by tag name
    /// @param tag the tag name to use
    XhtmlTelem (const String& tag);

    /// create a table element node with a class
    /// @param tag the tag name to use
    /// @param cls the node class
    XhtmlTelem (const String& tag, const String& cls);

    /// create a table element node with a class and a default tr class
    /// @param tag the tag name to use
    /// @param cls the node class
    /// @param trc the default tr class
    XhtmlTelem (const String& tag, const String& cls, const String& trc);

    /// create a table element node with a class, a default tr and tx class
    /// @param tag the tag name to use
    /// @param cls the node class
    /// @param trc the default tr class
    /// @param txc the default tx class
    XhtmlTelem (const String& tag, const String& cls, 
		const String& trc, const String& txc);

  public:
    /// @return the class name
    String repr (void) const;

    /// set the default tr class
    /// @param trc the default tr class
    void settrc (const String& trc);

    /// set the default tx class
    /// @param txc the default tx class
    void settxc (const String& txc);

    /// @return a new table row
    XhtmlTr* newtr (void);

    /// @return a new table row by class name
    XhtmlTr* newtr (const String& cls);

    /// add a print table to this table
    /// @param tbl  the print table to add
    /// @param tflg the tag flag for uri import
    void addtbl (const PrintTable& tbl, const bool tflg);

  private:
    // make the copy constructor private
    XhtmlTelem (const XhtmlTelem&);
    // make the assignment operator private
    XhtmlTelem& operator = (const XhtmlTelem&);

  public:
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
