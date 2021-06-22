// ---------------------------------------------------------------------------
// - Solver.hpp                                                              -
// - afnix:mth module - linear solver abstract class definition              -
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

#ifndef  AFNIX_SOLVER_HPP
#define  AFNIX_SOLVER_HPP

#ifndef  AFNIX_ANALYTIC_HPP
#include "Analytic.hpp"
#endif

namespace afnix {

  /// The Solver class is an abstract class dedicated to linear solver.
  /// The class provides the base 'solve' method which computes the
  /// solution of a linear system if one exists.
  /// @author amaury darsch

  class Solver : public Object {
  protected:
    /// the lhs matrix
    Rmi*      p_lhs;
    /// the solver logger
    Logger*   p_slg;
    /// the solver analytic
    Analytic* p_sla;

  public:
    /// create a default solver
    Solver (void);
 
    /// copy construct this solver
    /// @param that the object to copy
    Solver (const Solver& that);

    /// destroy this solver
    ~Solver (void);

    /// assign a solver to this one
    /// @param that the object to assign
    Solver& operator = (const Solver& that);

    /// reset this solver
    virtual void reset (void);

    /// clear this solver
    virtual void clear (void);

    /// set the solver lhs
    /// @param lhs the lhs to set
    virtual void setlhs (Rmi* lhs);

    /// @return the solver lhs
    virtual Rmi* getlhs (void) const;

    /// set the solver logger
    /// @param slg the solver logger
    virtual void setslg (Logger* slg);

    /// @return the solver logger
    virtual Logger* getslg (void) const;

    /// preprocess a rhs before using it
    /// @param rhs the rhs ro preprocess
    virtual void prerhs (Rvi& rhs) const;

    /// solve a linear system
    /// @param rhs the right handside
    virtual Rvi* solve (const Rvi& rhs)=0;

    /// solve a linear system
    /// @param lhs the left handside
    /// @param rhs the right handside
    virtual Rvi* solve (Rmi* lhs, const Rvi& rhs);

    /// verify a system with a solution and a rhs
    /// @param rhs the right handside
    /// @param   x the original solution
    virtual bool verify (const Rvi& rhs, const Rvi& x) const;

    /// check a system prior being solved
    /// @param rhs the right handside
    virtual bool check (const Rvi& rhs) const;

    /// check a system prior being solved
    /// @param lhs the left handside
    /// @param rhs the right handside
    virtual bool check (Rmi* lhs, const Rvi& rhs) const;

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
