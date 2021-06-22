// ---------------------------------------------------------------------------
// - Direct.hpp                                                              -
// - afnix:mth module - direct linear solver abstract class definition       -
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

#ifndef  AFNIX_DIRECT_HPP
#define  AFNIX_DIRECT_HPP

#ifndef  AFNIX_CPI_HPP
#include "Cpi.hpp"
#endif

#ifndef  AFNIX_SOLVER_HPP
#include "Solver.hpp"
#endif

namespace afnix {

  /// The Direct class is an abstract class that is part of the linear solver
  /// family. The class encapsulates the direct solver family.
  /// @author amaury darsch

  class Direct : public Solver {
  protected:
    /// the permutation
    Cpi* p_cpi;

  public:
    /// create a direct solver
    Direct (void);

    /// destroy this solver
    ~Direct (void);

    /// set the solver lhs
    /// @param lhs the left handside
    void setlhs (Rmi* lhs);

    /// set the solver permutation
    /// @param cpi the permutation object
    virtual void setcpi (Cpi* cpi);

    /// @return the solver permutation
    virtual Cpi* getcpi (void) const;

    /// factorize the lhs
    virtual void factorize (void) =0;

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
