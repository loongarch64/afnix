// ---------------------------------------------------------------------------
// - Rpolynom.hpp                                                            -
// - afnix:mth module - real polynom class definitions                       -
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

#ifndef  AFNIX_RPOLYNOM_HPP
#define  AFNIX_RPOLYNOM_HPP

#ifndef  AFNIX_RFI_HPP
#include "Rfi.hpp"
#endif

namespace afnix {

  /// The Rpolynom class is a class that encapuslates the functionality
  /// of a single real polynom. The class provides the support for the
  /// real function interface by binding the polynom coefficient. By 
  /// default a null polynom of degre 0 is created. The class supports
  /// the concept of negative degree polynom.
  /// @author amaury darsch

  class Rpolynom : public Rfi {
  private:
    /// the positive degree
    long d_pdgr;
    /// the negative degree
    long d_ndgr;
    /// the polynom coefficients
    t_real* p_coef;

  public:
    /// create a default null polynom
    Rpolynom (void);

    /// create a polynom by signed degree
    /// @param sdgr the signed degree
    Rpolynom (const long sdgr);

    /// create a polynom by positive and negative degree
    /// @param pdgr the positive degree
    /// @param ndgr the negative degree
    Rpolynom (const long pdgr, const long ndgr);

    /// copy construct this real polynom
    /// @param that the object to copy
    Rpolynom (const Rpolynom& that);
    
    /// destroy this real polynom
    ~Rpolynom (void);

    /// @return the class name
    String repr (void) const;
    
    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a real polynom to this one
    /// @param that the object to assign
    Rpolynom& operator = (const Rpolynom& that);

    /// compute the function by value
    /// @param x the evaluation real point
    t_real compute (const t_real x) const;

    /// compute the derivate by value
    /// @param x the evaluation real point
    t_real derivate (const t_real x) const;

    /// @return the polynom positive degree
    virtual long getpdgr (void) const;

    /// @return the polynom negative degree
    virtual long getndgr (void) const;

    /// set the polynom coefficient by index
    /// @param index the coefficient index
    /// @param coef  the coefficient value
    virtual void setcoef (const long index, const t_real coef);

    /// get the polynom coefficient by index
    /// @param index the coefficient index
    virtual t_real getcoef (const long index) const;

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
