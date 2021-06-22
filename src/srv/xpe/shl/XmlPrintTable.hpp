// ---------------------------------------------------------------------------
// - XmlPrintTable.hpp                                                       -
// - afnix:xpe module - xml print table class definition                     -
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

#ifndef  AFNIX_XMLPRINTTABLE_HPP
#define  AFNIX_XMLPRINTTABLE_HPP

#ifndef  AFNIX_PRINTTABLE_HPP
#include "PrintTable.hpp"
#endif

#ifndef  AFNIX_XMLDOCUMENT_HPP
#include "XmlDocument.hpp"
#endif

namespace afnix {

  /// The XmlPrintTable class is a xml document class designed to hold a print
  /// table in the form of a xml tree. Normally the xml tree is created with the
  /// help of a print table object.
  /// @author amaury darsch

  class XmlPrintTable : public XmlDocument {
  private:
    /// the table tag name
    String   d_ttgn;
    /// the head tag name
    String   d_htgn;
    /// the body tag name
    String   d_btgn;
    /// the row tag name
    String   d_rtgn;
    /// the data tag name
    String   d_dtgn;

  public:
    /// create a default xml print table
    XmlPrintTable (void);

    /// create a xml print table by table
    /// @param ptbl the print table
    XmlPrintTable (const PrintTable& ptbl);

    /// create a xml pritn table by name and table
    /// @param name the xml table name
    /// @param ptbl the print table
    XmlPrintTable (const String& name, const PrintTable& ptbl);

    /// copy construct this xml print table
    /// @param that the xml print table to copy
    XmlPrintTable (const XmlPrintTable& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// set the table tag name
    /// @param ttgn the tag name to set
    void setttgn (const String& ttgn);

    /// @return the table tag name
    String getttgn (void) const;

    /// set the head tag name
    /// @param htgn the tag name to set
    void sethtgn (const String& htgn);

    /// @return the head tag name
    String gethtgn (void) const;

    /// set the body tag name
    /// @param btgn the tag name to set
    void setbtgn (const String& btgn);

    /// @return the body tag name
    String getbtgn (void) const;

    /// set the row tag name
    /// @param rtgn the tag name to set
    void setrtgn (const String& rtgn);

    /// @return the row tag name
    String getrtgn (void) const;

    /// set the data tag name
    /// @param dtgn the tag name to set
    void setdtgn (const String& dtgn);

    /// @return the data tag name
    String getdtgn (void) const;

    /// set the xml print table by table
    /// @param ptbl the table to set
    void setptbl (const PrintTable& ptbl);

  private:
    // make the assignment operator private
    XmlPrintTable& operator = (const XmlPrintTable&);

  public:
    /// create an object in a generic way
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
