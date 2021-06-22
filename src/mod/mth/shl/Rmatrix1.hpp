// ---------------------------------------------------------------------------
// - Rmatrix1.hpp                                                            -
// - afnix:mth module - real matrix 1 definitions                            -
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

#ifndef  TANIX_RMATRIX1_HPP
#define  TANIX_RMATRIX1_HPP

#ifndef  AFNIX_RBLOCK_HPP
#include "Rblock.hpp"
#endif

#ifndef  AFNIX_RPOINT1_HPP
#include "Rpoint1.hpp"
#endif

#ifndef  AFNIX_RVECTOR1_HPP
#include "Rvector1.hpp"
#endif

namespace afnix {
  
  /// The Rmatrix1 class is a square matrix of size 1 designed to operate
  /// on vector 1 and point 1 objects.
  /// @author amaury darsch

  class Rmatrix1 : public Rblock {
  public:
    /// create an identity matrix
    Rmatrix1 (void);

    /// copy construct this matrix
    /// @param that the point to copy
    Rmatrix1 (const Rmatrix1& that);

    /// assign a point to this one
    /// @param that the point to assign
    Rmatrix1& operator = (const Rmatrix1& that);
    
    /// @return the class name
    String repr (void) const override;
    
    /// @return a clone of this object
    Object* clone (void) const override;

    /// multiply a matrix with a vector
    /// @param v the vector argument
    Rvector1 operator * (const Rvector1& v) const;

    /// multiply a matrix with a point
    /// @param p the point argument
    Rpoint1 operator * (const Rpoint1& p) const;
    
    /// multiply a matrix with a matrix
    /// @param m the matrix argument
    Rmatrix1 operator * (const Rmatrix1& m) const;
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
