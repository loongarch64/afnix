// ---------------------------------------------------------------------------
// - Rmd.hpp                                                                 -
// - afnix:mth module - real matrix datum class definitions                  -
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

#ifndef  AFNIX_RMD_HPP
#define  AFNIX_RMD_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Rmd class is the real matrix datum class that provides the 
  /// matrix value information by coordinates. A matrix datum is composed
  /// of a row, column and matrix values, thus making the class simple.
  /// @author amaury darsch

  class Rmd : public Object {
  protected:
    /// the row datum
    t_long d_row;
    /// the column datum
    t_long d_col;
    /// the value datum
    t_real d_val;

  public:
    /// create a default datum
    Rmd (void);

    /// create a datum by values
    /// @param row the row datum
    /// @param col the col datum
    /// @param val the val datum
    Rmd (const t_long row, const t_long col, const t_real val);

    /// copy construct this datum
    /// @param that the object to copy
    Rmd (const Rmd& that);

    /// assign a datum to this one
    /// @param that the object to assign
    Rmd& operator = (const Rmd& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// set the datum by values
    /// @param row the row datum
    /// @param col the col datum
    /// @param val the val datum
    virtual void set (const t_long row, const t_long col, const t_real val);

    /// set the datum matrix row
    /// @param row the row to set
    virtual void setrow (const t_long row);

    /// @return the datum matrix row
    virtual t_long getrow (void) const;

    /// set the datum matrix col
    /// @param col the column to set
    virtual void setcol (const t_long row);

    /// @return the datum matrix column
    virtual t_long getcol (void) const;

    /// set the datum matrix value
    /// @param val the value to set
    virtual void setval (const t_real val);

    /// @return the datum matrix value
    virtual t_real getval (void) const;

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
