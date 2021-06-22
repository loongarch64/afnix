// ---------------------------------------------------------------------------
// - Qr.hpp                                                                  -
// - afnix:mth module - Qr based solver class definitions                    -
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

#ifndef  AFNIX_QR_HPP
#define  AFNIX_QR_HPP

#ifndef  AFNIX_DIRECT_HPP
#include "Direct.hpp"
#endif

#ifndef  AFNIX_RGIVENS_HPP
#include "Rgivens.hpp"
#endif

namespace afnix {

  /// The Qr class is a class that implements a QR factorization process in
  /// the form of various methods that provides a fine grain process control.
  /// The general methodolofy os to copy the original matrix into a real
  /// sparse matrix, perform the factorization which results in a a list
  /// of givens factor (Q) and a sparse upper triangular matrix. When the
  /// factorization is done, the system can be solved by reversing it. Note
  /// that an upper triangular system is easily solved and the (Q) matrix is
  /// orthogonal. Note also that the Q matrix is stored in the transposed form.
  /// This means that reverse form is obtained by directly using Q while the
  /// normal form needs to be transposed since the Q matrix is orthogonal.
  /// @author amaury darsch

  class Qr : public Direct {
  private:
    /// the givens array
    Rgivens* p_q;
    /// the r matrix
    Rmi* p_r;
 
  public:
    /// create a default qr solver
    Qr (void);

    /// create a qr solver by lhs
    /// @param lhs the solver lhs
    Qr (Rmi* lhs);
    
    /// destroy this object
    ~Qr (void);

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

    /// factorize the lhs by range
    /// @param row the starting row
    /// @param rng thr row range
    void factorize (const t_long row, const t_long rng);

    /// update a factorization by lhs range
    /// @param row the starting row
    /// @param nru the number of rows
    void update (const t_long row, const t_long nru);

    /// update a factorization by vectors
    /// @param u the vector argument
    /// @param v the vector argument
    void update (const Rvi& u, const Rvi& v);

    /// @return the r matrix
    virtual Rmi* getr (void) const;

    /// @return the qr matrix product
    virtual Rmi* getqr (void) const;

    /// perform a reverse givens product
    /// @param x the vector argument
    virtual Rvi* rgivens (const Rvi& x) const;

    /// check the matrix diagonal for null elements
    virtual bool nulld (void) const;

  protected:
    /// no lock - perform a reverse givens vector product
    /// @param x the vector to update
    /// @param k the givens rotation index
    virtual void nlrg (Rvi& x, const long k) const;
 
    /// no lock - perform a reverse givens matrix product
    /// @param m the matrix to update
    /// @param k the givens rotation index
    virtual void nlrg (Rmi& m, const long k) const;

    /// no lock - perform a transposed givens matrix product
    /// @param m the matrix to update
    /// @param k the givens rotation index
    virtual void nltg (Rmi& m, const long k) const;

    /// no lock - nullify/update by vector
    /// @param w the vector argument
    virtual void nlnv (Rvi& w);

    /// no lock - nullify/update by matrix
    /// @param i the row coordinate
    /// @param j the col coordinate
    virtual void nlnm (const t_long i, const t_long j);

    /// no lock - factorize the upper-hessenberg r matrix
    virtual void nlnh (void);

  private:
    // make the copy constructor private
    Qr (const Qr&);
    // make the assignment operator private
    Qr& operator = (const Qr&);

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
