// ---------------------------------------------------------------------------
// - Record.hpp                                                              -
// - afnix:sps module - record class definition                              -
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

#ifndef  AFNIX_RECORD_HPP
#define  AFNIX_RECORD_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_CELL_HPP
#include "Cell.hpp"
#endif

namespace afnix {

  /// The Record class is a collection of cell. Like a cell a record can be
  /// bound to a name. Adding cell to a record is a simple process, like
  /// adding object to a vector.
  /// @author amaury darsch

  class Record : public Saveas {
  protected:
    /// the record name
    long   d_quark;
    /// the transaction number
    long   d_trnum;
    /// the cell vector
    Vector d_vcell;
    /// the record style
    Style* p_style;

  public:
    /// create an empty record
    Record (void);

    /// create a new record with a name
    /// @param name the record name
    Record (const String& name);

    /// copy construct this record
    /// @param that the record to copy
    Record (const Record& that);

    /// destroy this record
    ~Record (void);

    /// @return the object name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this record
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this record
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// assign a record to this one
    /// @param that the record to assign
    Record& operator = (const Record& that);

    /// reset this record
    void reset (void);

    /// clear this record
    void clear (void);

    /// @return the record name
    String getname (void) const;

    /// set the record name
    /// @param name the name to set
    void setname (const String& name);

    /// set the a cell name by index
    /// @param name the name to set
    void setname (const long index, const String& name);

    /// set the transaction number
    /// @param trnum the number to set
    void setrnum (const long trnum);

    /// @return the transaction number
    long getrnum (void) const;

    /// @return the record style
    Style* getstyle (void) const;
    
    /// @return the cell style by index
    Style* getstyle (const long index) const;

    /// @return the record length
    long length (void) const;

    /// add a new cell in this record
    /// @param cell the cell to add
    void add (Cell* cell);

    /// add a new cell by literal
    /// @param lobj the literal to add
    void add (Literal* lobj);

    /// add a new cell by name and literal
    /// @param name the cell name to add
    /// @param lobj the literal to add
    void add (const String& name, Literal* lobj);

    /// add an object in this record
    /// @param object the object to add
    void add (Object* object);

    /// add a vector of objects
    /// @param argv the vector of objects to add
    void add (const Vector* argv);

    /// @return a cell by index
    Cell* get (const long index) const;

    /// @return a cell literal by index
    Literal* map (const long index) const;

    /// set a cell in this record by index
    /// @param index the cell index
    /// @param cell  the cell to set
    void set (const long index, Cell* cell);

    /// set a cell record by index and literal
    /// @param index the literal index
    /// @param lobj  the literal to use
    Cell* set (const long index, Literal* lobj);

    /// set an object in this record by index
    /// @param index  the object index
    /// @param object the object to use
    Cell* set (const long index, Object* object);

    /// find a cell by quark
    /// @param name the cell to find
    Cell* find (const long quark) const;

    /// find a cell by name
    /// @param name the cell to find
    Cell* find (const String& name) const;

    /// find a cell by quark or throw an exception
    /// @param name the cell to find
    Cell* lookup (const long quark) const;

    /// find a cell by name or throw an exception
    /// @param name the cell to find
    Cell* lookup (const String& name) const;

    /// get a cell index by quark
    /// @param quark the cell quark to find
    long getindex (const long quark) const;

    /// get a cell index by name
    /// @param name the cell name to find
    long getindex (const String& name) const;
    
    /// check if a literal object match a cell value
    /// @param index the record index
    /// @param lobj  the literal object to check
    bool isequal (const long index, const Literal& lobj) const;

    /// sort this record by ascending or descending order
    /// @param mode the ascending mode
    void sort (const bool mode);

  public:
    /// cerate a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// evaluate this object with a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    Object* eval (Evaluable* zobj, Nameset* nset, const long quark);

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
