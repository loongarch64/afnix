// ---------------------------------------------------------------------------
// - Folio.hpp                                                               -
// - afnix:sps module - folio class definition                               -
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

#ifndef  AFNIX_FOLIO_HPP
#define  AFNIX_FOLIO_HPP

#ifndef  AFNIX_XREF_HPP
#include "Xref.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_SHEET_HPP
#include "Sheet.hpp"
#endif

namespace afnix {

  /// The Folio class is a folio of sheets. Because of the sheet format, 
  /// a folio look like a 3-dimensional array of cells. Like the 
  /// sheet, the folio is defined with a name and a vector of sheets.
  /// @author amaury darsch

  class Folio : public Saveas {
  private:
    /// the folio properties
    Plist  d_prop;
    /// the sheet vector
    Vector d_vsht;

  public:
    /// create an empty folio
    Folio (void);

    /// create a new folio by name
    /// @param name the folio name
    Folio (const String& name);

    /// create a new folio by name and info
    /// @param name the folio name
    /// @param info the folio info
    Folio (const String& name, const String& info);

    /// create a new folio with a stream
    /// @param is the input stream to use
    Folio (InputStream* is);

    /// @return the object name
    String repr (void) const;

    /// reset the folio
    void reset (void);

    /// write the folio to an output stream
    /// @param os the output stream
    void write (OutputStream& os);

    /// serialize this folio
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this folio
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the folio property list
    Plist getplist (void) const;

    /// @return true if the property exists
    bool isprop (const String& name);

    /// @return the number of folio properties
    long lenprop (void) const;

    /// @return a folio property by index
    Property* getprop (const long index) const;

    /// @return a folio property by name
    Property* findprop (const String& name) const;

    /// @return a folio property by name or throw an exception
    Property* lookprop (const String& name) const;

    /// @return a folio property value by name
    String getpval (const String& name) const;

    /// add a folio property
    /// @param prop the property to add
    void addprop (Property* prop);

    /// add a property by name and value
    /// @param name the property name to add
    /// @param lval the property value to add
    void addprop (const String& name, const Literal& lval);

    /// set a property by name and value
    /// @param name the property name to set
    /// @param lval the property value to set
    void setprop (const String& name, const Literal& lval);

    /// @return the folio length
    long length (void) const;

    /// add a new sheet in this folio
    /// @param sheet the sheet to add
    void add (Sheet* sheet);

    /// @return a sheet by index
    Sheet* get (const long index) const;

    /// set a folio by index and sheet
    /// @param index the sheet index
    /// @param sheet the sheet to set
    void set (const long index, Sheet* sheet);

    /// @return true if a tag exists by tag
    bool istag (const String& tag) const;

    /// @return the sheet index by tag
    Index* getsidx (const String& tag) const;

    /// find a sheet by tag - the first sheet found is returned
    /// @param tag the sheet tag used for filtering
    Sheet* find (const String& tag) const;

    /// find a sheet by tag or throw an exception - the first sheet 
    /// found is returned
    /// @param tag the sheet tag used for filtering
    Sheet* lookup (const String& tag) const;

    /// filter a folio by tag and get a new folio
    /// @param tag the tag used to filter the folio
    Folio* filter (const String& tag) const;

    /// @return a full xref table
    Xref* getxref (void) const;

    /// @return a xref table by tag
    Xref* getxref (const String& tag) const;

    /// @return a xref table by coordinate
    Xref* getxref (const long cidx) const;

    /// @return a xref table by coordinate
    Xref* getxref (const long cidx, const long ridx) const;

  private:
    // make the copy constructor private
    Folio (const Folio&);
    // make the assignment operator private
    Folio& operator = (const Folio&);

  public:
    /// create a new object in a generic way
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
