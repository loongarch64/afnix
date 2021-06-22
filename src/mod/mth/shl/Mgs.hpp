// ---------------------------------------------------------------------------
// - Mgs.hpp                                                                 -
// - afnix:mth module - Modified Gram-Schmidt class definitions              -
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

#ifndef  AFNIX_MGS_HPP
#define  AFNIX_MGS_HPP

#ifndef  AFNIX_DIRECT_HPP
#include "Direct.hpp"
#endif

namespace afnix {

  /// The Mgs class is a class that implements a QR factorization process
  /// with a modified Gram-Schmidt orthogonalization. The factorization
  /// is performed in place on a permuted block . The result is two block
  /// matrix (Q/R) which can be used to solve a linear system. Unlike other
  /// methods, this implementation operates with matrix block and is therefore
  /// subject to memory limitation. It is the method of choice for small or
  /// dense system.
  /// @author amaury darsch

  class Mgs : public Direct {
  private:
    /// the Q matrix
    Rmi* p_q;
    /// the r matrix
    Rmi* p_r;
 
  public:
    /// create a default mgs solver
    Mgs (void);

    /// create a mgs solver by lhs
    /// @param lhs the solver lhs
    Mgs (Rmi* lhs);
    
    /// destroy this object
    ~Mgs (void);

    /// @return the class name
    String repr (void) const;

    /// reset this solver
    void reset (void);

    /// clear this solver
    void clear (void);

    /// set the solver lhs
    /// @param lhs the left handsize
    void setlhs (Rmi* lhs);

    /// solve a linear system 
    /// @param rhs the right handside
    Rvi* solve (const Rvi& rhs);

    /// factorize the lhs
    void factorize (void);

    /// @return the r matrix
    virtual Rmi* getr (void) const;

    /// @return the qr matrix product
    virtual Rmi* getqr (void) const;

    /// check the matrix diagonal for null elements
    virtual bool nulld (void) const;

  protected:
    /// no lock - create a block matrix
    virtual void nlbm (void);
    /// no lock - factorize a block matrix
    virtual void nlbf (void);

    /// no lock - create a sparse matrix
    virtual void nlsm (void);
    /// no lock - factorize a sparse matrix
    virtual void nlsf (void);

  private:
    // make the copy constructor private
    Mgs (const Mgs&);
    // make the assignment operator private
    Mgs& operator = (const Mgs&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
