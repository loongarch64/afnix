// ---------------------------------------------------------------------------
// - Sheet.hpp                                                               -
// - afnix:sps module - sheet class definition                               -
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

#ifndef  AFNIX_SHEET_HPP
#define  AFNIX_SHEET_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_BUNDLE_HPP
#include "Bundle.hpp"
#endif

#ifndef  AFNIX_RECORD_HPP
#include "Record.hpp"
#endif

#ifndef  AFNIX_PRINTTABLE_HPP
#include "PrintTable.hpp"
#endif

namespace afnix {

  /// The Sheet class is a collection of records. Because of the record
  /// format, a sheet look like a 2-dimensional array of cells. Like the
  /// record, the sheet is defined with a name and a vector of records.
  /// For format purpose, the sheet has also three record fields, namelly,
  /// the info, head, and foot record.
  /// @author amaury darsch

  class Sheet : public Saveas {
  protected:
    /// the sheet tags
    Strvec   d_tags;
    /// the sheet signature
    String   d_sign;
    /// the header style
    Vector   d_hstl;
    /// the sheet marker
    Bundle   d_mark;
    /// the record header
    Record   d_head;
    /// the record footer
    Record   d_foot;
    /// the record body
    Vector   d_body;
    
  public:
    /// create an empty sheet
    Sheet (void);

    /// create a new sheet by name
    /// @param name the sheet name
    Sheet (const String& name);

    /// create a new sheet by name and info
    /// @param name the sheet name
    /// @param info the sheet info
    Sheet (const String& name, const String& info);

    /// copy construct this sheet
    /// @param that the sheet to copy
    Sheet (const Sheet& that);

    /// @return the object name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this sheet
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this sheet
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// assign a sheet to this one
    /// @paarm that the sheet to assign
    Sheet& operator = (const Sheet& that);

    /// reset this sheet
    void reset (void);

    /// clear this sheet
    void clear (void);

    /// add a sheet tag
    /// @param tag the tag to add
    void addtag (const String& tag);
    
    /// add a vector of literals in the sheet tag decsriptor
    /// @param argv the literal vector to add
    void addtag (const Vector* argv);

    /// @return the tags descriptor length
    long tagslen (void) const;

    /// @return true if the sheet tag exists
    bool istag (const String& tag) const;

    /// get a tag by index
    /// @param index the tag index
    String gettag (const long index) const;

    /// set the tags descriptor by index and literal
    /// @param index the tag index to set
    /// @param lobj  the literal object to set
    void settag (const long index, Literal* lobj);

    /// find a tag index by ag
    /// @param tag the tag to find
    long findtag (const String& tag) const;

    /// set a sheet signature
    /// @param sign the sign to set
    void setsign (const String& sign);

    /// get the sheet signature
    String getsign (void) const;

    /// set a header style by index
    /// @param index the head index
    /// @param hstl  the header style
    void sethstl (const long index, const Style& hstl);

    /// @return a header style by index
    Style* gethstl (const long index) const;

    /// @return the marker length
    long marklen (void) const;
    
    /// add a boolean marker
    /// @param bval the marker to add
    void addmark (const bool bval);

    /// add an integer marker
    /// @param ival the marker to add
    void addmark (const long ival);

    /// add a real marker
    /// @param rval the marker to add
    void addmark (const t_real rval);

    /// add a literal marker
    /// @param mark the marker to add
    void addmark (Literal* mark);

    /// @return the sheet marker literal
    Literal* getmark (void) const;

    /// @return the sheet marker by index
    Literal* getmark (const long midx) const;

    /// add a new object in the header (cell or literal)
    /// @param object the object to add
    void addhead (Object* object);

    /// add a vector of object in the header descriptor
    /// @param argv the object vector
    void addhead (const Vector* argv);

    /// @return a header cell by index
    Cell* gethead (const long index) const;

    /// @return a header literal by index
    Literal* maphead (const long index) const;

    /// set the record header by index and literal
    /// @param index the record index to set
    /// @param lobj  the literal object to set
    Cell* sethead (const long index, Literal* lobj);

    /// add a new object in the footer (cell or literal)
    /// @param object the object to add
    void addfoot (Object* object);

    /// add a vector of object in the footer descriptor
    /// @param argv the object vector
    void addfoot (const Vector* argv);

    /// @return a footer cell by index
    Cell* getfoot (const long index) const;

    /// @return a footer cell value by index
    Literal* mapfoot (const long index) const;

    /// set the record footer by index and literal
    /// @param index the info index to set
    /// @param lobj  the literal object to set
    Cell* setfoot (const long index, Literal* lobj);

    /// @return the sheet length
    long length (void) const;

    /// @return the number of columns
    long getcols (void) const;

    /// add a new record in this sheet
    /// @param rcd the record to add
    void add (Record* rcd);

    /// @return a record by index
    Record* get (const long index) const;

    /// @return a cell by row and column
    Cell* get (const long row, const long col) const;

    /// @return a cell literal by row and column
    Literal* map (const long row, const long col) const;

    /// set a record in this sheet by index
    /// @param index the record index
    /// @param rcd   the record to set
    void set (const long index, Record* rcd);

    /// set an object by row and column
    /// @param row    the record index
    /// @param col    the cell index in the record
    /// @param object the cell to set
    Cell* set (const long row, const long col , Object* object);

    /// add a vector of objects in this sheet
    /// @param argv the vector to add
    void adddata (const Vector* argv);

    /// remove a record by index
    /// @param index the record index to remove
    void remove (const long index);

    /// convert a sheet into a print sheet
    /// @param max the maximum number of rows
    /// @param start the row start index
    /// @param flag  the string or literal represenntation
    PrintTable* convert (long max, long start, bool flag) const;

    /// sort this sheet by ascending or descending order and column
    /// @param col  the column used for sorting
    /// @param mode the ascending mode
    void sort (const long col, const bool mode);

    /// link a sheet column into this sheet
    /// @param sheet the reference sheet
    /// @param col   the column index
    void lkcol (const Sheet* sheet, const long col);

    /// check if a row exists by column index and literal
    /// @param col  the column to check
    /// @param lobj the literal to check
    bool isrow (const long col, const Literal& lobj) const;

    /// find a row by column index and literal
    /// @param col  the column to check
    /// @param lobj the literal to check
    long rfind (const long col, const Literal& lobj) const;
    
    /// find a row by column index and literal or throw an exception
    /// @param col  the column to check
    /// @param lobj the literal to check
    long rlookup (const long col, const Literal& lobj) const;

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
