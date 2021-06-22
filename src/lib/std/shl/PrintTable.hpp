// ---------------------------------------------------------------------------
// - PrintTable.hpp                                                          -
// - standard object library - printable table class definition              -
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

#ifndef  AFNIX_PRINTTABLE_HPP
#define  AFNIX_PRINTTABLE_HPP

#ifndef  AFNIX_STYLE_HPP
#include "Style.hpp"
#endif

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The PrintTable class is a special class that can be used to format
  /// a table for output. At construction the number of columns is given.
  /// The default constructor create a print table of one column. Any
  /// literal object can be added to the table and are formated according
  /// to a format descriptor. Once the table is created, it can be written
  /// to an output stream or a buffer.
  /// @author amaury darsch

  class PrintTable : public virtual Serial {
  private:
    /// the table size
    long d_size;
    /// the number of columns
    long d_cols;
    /// the number of rows
    long d_rows;
    /// the global style
    Style d_gstl;
    /// the table body
    String** p_body;
    /// the table tags
    String** p_stag;
    /// the table head
    String* p_head;
    /// the table foot
    String* p_foot;
    /// the column style
    Style** p_cstl;
    /// the column maximum width
    long*   p_cwth;

  public:
    /// create a default print table
    PrintTable (void);

    /// create a print table with a number of columns
    /// @param cols the number of columns
    PrintTable (const long cols);

    /// create a print table with a number of columns and rows
    /// @param cols the number of columns
    /// @param rows the number of rows
    PrintTable (const long cols, const long rows);

    /// destroy this print table
    ~PrintTable (void);

    /// @return the class name
    String repr (void) const;

    /// @return the print table did
    t_word getdid (void) const;

    /// @return the print table sid
    t_word getsid (void) const;

    /// serialize this print table to an output stream
    /// @param os the output stream to write
    void wrstream (OutputStream& os) const;

    /// deserialize a print table from an input stream
    /// @param is the input steam to read in
    void rdstream (InputStream& is);

    /// @return true if the head is defined
    bool ishead (void) const;

    /// @return true if the foot is defined
    bool isfoot (void) const;

    /// @return the number of rows
    long getrows (void) const;

    /// @return the number of columns
    long getcols (void) const;

    /// set the global style
    /// @param gstl the style to set
    void setstyle (const Style& gstl);

    /// @return the global style
    Style getstyle (void) const;

    /// set the column style
    /// @param col  the column index
    /// @param cstl the style to set
    void setstyle (const long col, const Style& cstl);

    /// @return the column style
    Style getstyle (const long col) const;

    /// add a row to the table and return the row index
    long add (void);
    
    /// set a head value by column
    /// @param col the table column
    /// @param val the table value
    void sethead (const long col, const String& val);

    /// get a head value by column
    /// @param col the table column
    String gethead (const long col) const;

    /// set a foot value by column
    /// @param col the table column
    /// @param val the table value
    void setfoot (const long col, const String& val);

    /// get a foot value by column
    /// @param col the table column
    String getfoot (const long col) const;

    /// set a value by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param val the table value
    void set (const long row, const long col, const String& val);

    /// set a value by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param val the table value
    void set (const long row, const long col, const long val);

    /// set a value by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param val the table value
    void set (const long row, const long col, const t_long val);

    /// set a value by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param val the table value
    void set (const long row, const long col, const t_octa val);

    /// set a value by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param val the table value
    void set (const long row, const long col, const t_real val);

    /// set a literal by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param obj the literal object to set
    void set (const long row, const long col, Literal* obj);
    
    /// get a value by row and column
    /// @param row the table row
    /// @param col the table column
    String get (const long row, const long col) const;

    /// get a truncated value by row and column
    /// @param row the table row
    /// @param col the table column
    String truncate (const long row, const long col) const;

    /// check if a tag is set
    /// @param row the table row
    /// @param col the table column
    bool istag (const long row, const long col) const;

    /// set a tag by row and column
    /// @param row the table row
    /// @param col the table column
    /// @param tag the tag to set
    void settag (const long row, const long col, const String& tag);

    /// get a tag value by row and column
    /// @param row the table row
    /// @param col the table column
    String gettag (const long row, const long col) const;

    /// add columns to the print table
    /// @param cnum the number of columns to add
    void addcols (const long cnum);

    /// @return a formatted row suitable for dumping
    String dump (const long row) const;

    /// dump the table into a buffer
    void dump (Buffer& buffer) const;

    /// dump the table for an output stream
    void dump (OutputStream& os) const;

    /// @return a formatted head suitable for printing
    String hformat (void) const;

    /// @return a formatted foot suitable for printing
    String fformat (void) const;

    /// @return a formatted row suitable for printing
    String format (const long row) const;

    /// format the table into a buffer
    void format (Buffer& buffer) const;

    /// format the table for an output stream
    void format (OutputStream& os) const;

    /// resize this table
    /// @param size the new table size
    void resize (const long size);

    /// merge a print table into this one
    /// @param ptbl the table to merge
    void merge (const PrintTable& ptbl);

    /// merge a print table into this one
    /// @param ptbl the table to merge
    /// @param hflg the header flag
    /// @param fflg the footer flag    
    void merge (const PrintTable& ptbl, const bool hflg, const bool fflg);

    /// locate a row by string value
    /// @param sval the string to search
    long locate (const String& sval) const;
    
    /// locate a row by string value and column
    /// @param sval the string to search
    /// @param col  the string column
    long locate (const String& sval, const long col) const;
    
  private:
    // make the copy constructor private
    PrintTable (const PrintTable&);
    // make the assignment operator private
    PrintTable& operator = (const PrintTable&);

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
