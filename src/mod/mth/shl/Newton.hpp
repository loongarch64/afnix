// ---------------------------------------------------------------------------
// - Newton.hpp                                                              -
// - afnix:mth module - newton based zero solver class definitions           -
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

#ifndef  AFNIX_NEWTON_HPP
#define  AFNIX_NEWTON_HPP

#ifndef  AFNIX_RFI_HPP
#include "Rfi.hpp"
#endif

#ifndef  AFNIX_RNI_HPP
#include "Rni.hpp"
#endif

#ifndef  AFNIX_SOLVER_HPP
#include "Solver.hpp"
#endif

namespace afnix {

  /// The Newton class is a class that implements various newton based 
  /// zero solver for non linear system. With single real variable function,
  /// the Newton algorithm requires the function and it derivate. With multi
  /// variable function, a vector is used to represent the set of variables
  /// and the jacobian the set of derivatives in the function space. The
  /// newton method is an iterative local method, meaning that it works well
  /// when the initial point is closed to the solution. Obviously, things
  /// become interesting when the derivate is null.
  /// @author amaury darsch

  class Newton : public Object {
  private:
    /// the maximum number of iteration
    long d_mni;
    /// the linear solver
    Solver* p_lnr;

  public:
    /// create a default newton solver
    Newton (void);

    /// create a newton solver with a linear solver
    /// @param lnr the linear sover to use
    Newton (Solver* lnr);

    /// copy construct this solver
    /// @param that the object to copy
    Newton (const Newton& that);

    /// destroy this newton solver
    ~Newton (void);

    /// assign a solver to this one
    /// @param that the object to assign
    Newton& operator = (const Newton& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// set the newton linear solver
    /// @param lnr the linear solver
    virtual void setlnr (Solver* lnr);

    /// @return the linear solver
    virtual Solver* getlnr (void) const;

    /// set the number of newton iterations
    /// @param mni the numer of iterations
    virtual void setmni (const long mni);

    /// @return the number of newton iterations
    virtual long getmni (void) const;

    /// find a function root with the newton method
    /// @param fo the function object
    /// @param xi the initial point
    virtual t_real solve (const Rfi& fo, const t_real xi);

    /// find a system root with the newton method
    /// @param so the system object
    /// @param xi the initial point
    virtual Rvi* solve (const Rni& so, const Rvi& xi);

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
