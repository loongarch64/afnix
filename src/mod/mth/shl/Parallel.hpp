// ---------------------------------------------------------------------------
// - Parallel.hpp                                                            -
// - afnix:mth module - paraller linear solver abstract class definition     -
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

#ifndef  AFNIX_PARALLEL_HPP
#define  AFNIX_PARALLEL_HPP

#ifndef  AFNIX_SOLVER_HPP
#include "Solver.hpp"
#endif

namespace afnix {

  /// The Parallel class is an abstract class that is part of the linear
  /// solver family. The class encapsulates the parallel solver family.
  /// A parallel architectured solver is generally implemented as a distributed
  /// solver although it can be run locally. Most of the parallel solver that
  /// are available are fine pieces of software engineering.
  /// @author amaury darsch

  class Parallel : public Solver {
  protected:
    /// the number of tasks
    long d_tnum;

  public:
    /// create a default parallel solver
    Parallel (void);

    /// reset this solver
    void reset (void);

    /// set the number of tasks
    /// @param tnum the number of tasks
    virtual void settnum (const long tnum);

    /// @return the number of tasks
    virtual long gettnum (void) const;

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
