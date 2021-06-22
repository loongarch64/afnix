// ---------------------------------------------------------------------------
// - SolverFactory.hpp                                                       -
// - afnix:mth module - solver factory abstract class definition             -
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

#ifndef  AFNIX_SOLVERFACTORY_HPP
#define  AFNIX_SOLVERFACTORY_HPP

#ifndef  AFNIX_SOLVER_HPP
#include "Solver.hpp"
#endif

namespace afnix {

  /// The SolverFactory class is an abstract class that acts as a solver
  /// factory. It is up to the implementation to decide how the solver shall
  /// be generated although all solver should be bound with a logger.
  /// @author amaury darsch

  class SolverFactory : public Object {
  protected:
    /// the factory logger
    Logger* p_slg;

  public:
    /// create a default factory
    SolverFactory (void);

    /// create a factory with a logger
    /// @param slg the system logger
    SolverFactory (Logger* slg);
 
    /// copy construct this factory
    /// @param that the object to copy
    SolverFactory (const SolverFactory& that);

    /// destroy this factory
    ~SolverFactory (void);

    /// assign a solver to this one
    /// @param that the object to assign
    SolverFactory& operator = (const SolverFactory& that);
    
    /// set the system logger
    /// @param slg the system logger
    virtual void setslg (Logger* slg);

    /// @return the system logger
    virtual Logger* getslg (void) const;

    /// @return a new solver
    virtual Solver* newslv (void) const =0;

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
