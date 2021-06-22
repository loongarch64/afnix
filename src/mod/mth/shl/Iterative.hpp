// ---------------------------------------------------------------------------
// - Iterative.hpp                                                           -
// - afnix:mth module - iterative linear solver abstract class definition    -
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

#ifndef  AFNIX_ITERATIVE_HPP
#define  AFNIX_ITERATIVE_HPP

#ifndef  AFNIX_SOLVER_HPP
#include "Solver.hpp"
#endif

namespace afnix {

  /// The Iterative class is an abstract class that is part of the linear
  /// solver family. The class encapsulates the iterative solver family.
  /// Among the iterative solver, one will find the stationnary solver like
  /// the Jacobi and the non stationnary solver like the Krylov solvers.
  /// @author amaury darsch

  class Iterative : public Solver {
  protected:
    /// the absolute precision
    t_real d_aps;
    /// the relative precision
    t_real d_rps;
    /// max number of iterations
    t_long d_mni;
    /// the preconditionner
    Rvi*   p_ovp;

  public:
    /// create a default iterative solver
    Iterative (void);

    /// copy construct this iterative solver
    /// @param that the object to copy
    Iterative (const Iterative& that);

    /// destroy this iterative solver
    ~Iterative (void);
    
    /// assign a solver to this one
    /// @param that the object to assign
    Iterative& operator = (const Iterative& that);

    /// reset this solver
    void reset (void);

    /// set the solver lhs
    /// @param lhs the left handside
    void setlhs (Rmi* lhs);
    
    /// set the maximum iterations
    /// @param mni the number of iterations
    virtual void setmni (const t_long mni);

    /// set the absolute precision
    /// @param aps the precision to set
    virtual void setaps (const t_real aps);

    /// @return the absolute precision
    virtual t_real getaps (void) const;

    /// set the relative precision
    /// @param rps the precision to set
    virtual void setrps (const t_real rps);

    /// @return the relative precision
    virtual t_real getrps (void) const;

    /// set the optional preconditionner
    /// @param ovp the preconditionner vector
    virtual void setovp (Rvi* ovp);

    /// @return the solver preconditionner
    virtual Rvi* getovp (void) const;

    /// set the solver lhs with preconditionning
    /// @param lhs the lhs to set
    /// @param pcf the preconditionning flag
    virtual void setlhp (Rmi* lhs, const bool pcf);

    /// @return a jacobi preconditionner
    virtual Rvi* getjp (void) const;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
