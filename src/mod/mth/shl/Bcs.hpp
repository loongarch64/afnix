// ---------------------------------------------------------------------------
// - Bcs.hpp                                                                 -
// - afnix:mth module - bcs iterative solver class definition                -
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

#ifndef  AFNIX_BCS_HPP
#define  AFNIX_BCS_HPP

#ifndef  AFNIX_ITERATIVE_HPP
#include "Iterative.hpp"
#endif

namespace afnix {

  /// The Bcs class is a iterative solver which is part of the Krylov
  /// family of non stationnary solvers. This solver is the bi-conjugate
  /// gradient stabilized implementation which operates with general
  /// purpose matrix.
  /// @author amaury darsch

  class Bcs : public Iterative {
  public:
    /// create a default bcs solver
    Bcs (void);
    
    /// create a bcs solver by lhs
    /// @param lhs the solver lhs
    Bcs (Rmi* lhs);

    /// create a bcs solver by lhs and preconditionner
    /// @param lhs the solver lhs
    /// @param ovp the preconditionner
    Bcs (Rmi* lhs, Rvi* ovp);

    /// @return the class name
    String repr (void) const;

    /// solve a linear system 
    /// @param rhs the right handside
    Rvi* solve (const Rvi& rhs);

  private:
    // make the copy constructor private
    Bcs (const Bcs&);
    // make the assignment operator private
    Bcs& operator = (const Bcs&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
