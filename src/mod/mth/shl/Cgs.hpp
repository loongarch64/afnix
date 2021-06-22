// ---------------------------------------------------------------------------
// - Cgs.hpp                                                                 -
// - afnix:mth module - cgs iterative solver class definition                -
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

#ifndef  AFNIX_CGS_HPP
#define  AFNIX_CGS_HPP

#ifndef  AFNIX_ITERATIVE_HPP
#include "Iterative.hpp"
#endif

namespace afnix {

  /// The Cgs class is a iterative solver which is part of the Krylov
  /// family of non stationnary solvers. This solver is the conjugate
  /// gradient squared implementation which operates with general
  /// purpose matrix.
  /// @author amaury darsch

  class Cgs : public Iterative {
  public:
    /// create a default cgs solver
    Cgs (void);
    
    /// create a cgs solver by lhs
    /// @param lhs the solver lhs
    Cgs (Rmi* lhs);

    /// create a cgs solver by lhs and preconditionner
    /// @param lhs the solver lhs
    /// @param ovp the preconditionner
    Cgs (Rmi* lhs, Rvi* ovp);

    /// @return the class name
    String repr (void) const;

    /// solve a linear system 
    /// @param rhs the right handside
    Rvi* solve (const Rvi& rhs);

  private:
    // make the copy constructor private
    Cgs (const Cgs&);
    // make the assignment operator private
    Cgs& operator = (const Cgs&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
