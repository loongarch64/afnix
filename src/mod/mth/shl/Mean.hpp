// ---------------------------------------------------------------------------
// - Mean.hpp                                                                -
// - afnix:mth module - samples mean class definition                        -
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

#ifndef  AFNIX_MEAN_HPP
#define  AFNIX_MEAN_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// The Mean class is a class that implements the computation of a mean
  /// for a sample source. The class maintains its own state and therefore
  /// do support update.
  /// @author amaury darsch

  class Mean : public Object {
  protected:
    /// the cumulative value
    t_real d_cval;
    /// the number of samples
    long d_snum;
    
  public:
    /// create a default mean
    Mean (void);

    /// copy construct this mean
    /// @param that the object to copy
    Mean (const Mean& that);

    /// copy move this mean
    /// @param that the object to move
    Mean (Mean&& that);
    
    /// assign a mean to this one
    /// @param that the object to assign
    Mean& operator = (const Mean& that);

    /// move a mean to this one
    /// @param that the object to assign
    Mean& operator = (Mean&& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this mean
    void reset (void);

    /// @return the cumulative value
    t_real getcval (void) const;
    
    /// @return the number of samples
    long getsnum (void) const;

    /// @return the current mean
    t_real getmean (void) const;

    /// push a new sample value
    /// @param sval the value to push
    void push (const t_real sval);

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
