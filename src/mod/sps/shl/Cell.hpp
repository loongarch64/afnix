// ---------------------------------------------------------------------------
// - Cell.hpp                                                                -
// - afnix:sps module - cell class definition                                -
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

#ifndef  AFNIX_CELL_HPP
#define  AFNIX_CELL_HPP

#ifndef  AFNIX_STYLE_HPP
#include "Style.hpp"
#endif

#ifndef  AFNIX_SAVEAS_HPP
#include "Saveas.hpp"
#endif

namespace afnix {

  /// The Cell class is the foundation block of the afnix spreadsheet module. 
  /// A cell is a simple binding between a name and a literal object. For this
  /// reason a cell is a serializable object. The cell name does not have to
  /// exists for the cell to operate.
  /// @author amaury darsch

  class Cell : public Saveas {
  protected:
    /// the cell quark
    long d_quark;
    /// the const flag
    bool d_cflg;
    /// the cell object
    Literal* p_cobj;
    /// the cell style
    Style* p_cstl;

  public:
    /// create an empty cell
    Cell (void);

    /// create a cell with a literal
    /// @param cobj the cell object
    Cell (Literal* cobj);

    /// create a cell with a name and an object
    /// @param name the cell name
    /// @param cobj the cell object
    Cell (const String& name, Literal* cobj);
    
    /// copy construct this cell
    /// @param that the cell to copy
    Cell (const Cell& that);

    /// destroy this cell
    ~Cell (void);

    /// @return the object name
    String repr (void) const;

    /// @ return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this cell
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this cell
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// assign a cell to this one
    /// @paarm that the cell to assign
    Cell& operator = (const Cell& that);

    /// check that the cell match the quark
    /// @param quark the quark to match
    bool operator == (const long quark) const;

    /// check that the cell match the literal
    /// @param lobj the literal to match
    bool operator == (const Literal& lobj) const;

    /// reset this cell
    void reset (void);

    /// clear this cell
    void clear (void);

    /// @return the cell name
    String getname (void) const;

    /// set the cell name
    /// @param name the cell name to set
    void setname (const String& name);

    /// @return the cell value
    Literal* get (void) const;

    /// set the cell value
    /// @param cobj the cell object to set
    void set (Literal* cobj);

    /// set the constant cell value
    /// @param cobj the cell object to set
    /// @param cflg the const flag
    void set (Literal* cobj, const bool cflg);

    /// set the cell const flag
    /// @param cflg the const flag
    void setcflg (const bool cflg);

    /// @return the cell string literal
    String tostring (void) const;

    /// @return the cell style
    Style* getstyle (void) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
 
    /// set a const object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* cdef (Evaluable* zobj, Nameset* nset, Object* object);

    /// set an object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object);

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
