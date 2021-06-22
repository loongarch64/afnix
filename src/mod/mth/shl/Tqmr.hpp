// ---------------------------------------------------------------------------
// - Tqmr.hpp                                                                -
// - afnix:mth module - tqmr iterative solver class definition               -
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

#ifndef  AFNIX_TQMR_HPP
#define  AFNIX_TQMR_HPP

#ifndef  AFNIX_ITERATIVE_HPP
#include "Iterative.hpp"
#endif

namespace afnix {

  /// The Tqmr class is a iterative solver which is part of the Krylov
  /// family of non stationnary solvers. This solver is the transpose-free
  /// qmr implementation which operates with general purpose matrix.
  /// @author amaury darsch

  class Tqmr : public Iterative {
  public:
    /// create a default tqmr solver
    Tqmr (void);
    
    /// create a tqmr solver by lhs
    /// @param lhs the solver lhs
    Tqmr (Rmi* lhs);

    /// create a tqmr solver by lhs and preconditionner
    /// @param lhs the solver lhs
    /// @param ovp the preconditionner
    Tqmr (Rmi* lhs, Rvi* ovp);

    /// @return the class name
    String repr (void) const;

    /// solve a linear system 
    /// @param rhs the right handside
    Rvi* solve (const Rvi& rhs);

  private:
    // make the copy constructor private
    Tqmr (const Tqmr&) =delete;
    // make the assignment operator private
    Tqmr& operator = (const Tqmr&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
