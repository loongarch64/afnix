// ---------------------------------------------------------------------------
// - Linear.hpp                                                              -
// - afnix:mth module - generalized linear solver class definitions          -
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

#ifndef  AFNIX_LINEAR_HPP
#define  AFNIX_LINEAR_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_SOLVER_HPP
#include "Solver.hpp"
#endif

namespace afnix {

  /// The Linear class is a general class designed to encapsulate various
  /// solver strategies. The implementation can be used to select a
  /// a particular solver or enable the execution of different one.
  /// The automatic verification mode can be enabled to automatically verify
  /// the computed solution. Such mode is generally used in debug mode.
  /// @author amaury darsch

  class Linear : public Solver {
  public:
    /// the default verification flag
    static const bool AVF_DEF;
    /// the default preconditioning flag
    static const bool PCF_DEF;

    /// the solver type
    enum t_styp {
      STYP_ALL, // all solvers
      STYP_DCT, // direct type solver
      STYP_ITR  // iterative type solve
    };

    /// map a string to a solver type
    static t_styp tostyp (const String& styp);

  private:
    /// the solvers vectors
    Vector d_slv;
    /// the verification flag
    bool d_avf;
    /// the preconditionning flag
    bool d_pcf;

  public:
    /// create a default linear solver
    Linear (void);

    /// create a linear solver by flag
    /// @param avf the verification flag
    Linear (const bool avf);

    /// create a linear solver by flags
    /// @param avf the verification flag
    /// @param pcf the preconditionning flag
    Linear (const bool avf, const bool pcf);

    /// create a linear solver by flags and type
    /// @param avf the verification flag
    /// @param pcf the preconditionning flag
    /// @param stp solver type
    Linear (const bool avf, const bool pcf, const t_styp stp);

    /// create a linear solver by flags, type and logger
    /// @param avf the verification flag
    /// @param pcf the preconditionning flag
    /// @param stp solver type
    /// @param slg the system logger
    Linear (const bool avf, const bool pcf, const t_styp stp, Logger* slg);

    /// copy construct this solver
    /// @param that the object to copy
    Linear (const Linear& that);

    /// assign a solver to this one
    /// @param that the object to assign
    Linear& operator = (const Linear& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this linear solver
    void reset (void);

    /// set the solver lhs
    /// @param lhs the lhs to set
    void setlhs (Rmi* lhs);

    /// solve a system by rhs
    /// @param rhs the right handside
    Rvi* solve (const Rvi& rhs);

    /// set the solver type
    /// @param styp the solver type
    virtual void setstyp (const t_styp styp);
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
