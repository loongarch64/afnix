// ---------------------------------------------------------------------------
// - Algebra.hpp                                                             -
// - afnix:mth module - algebraic algorithm definitions                      -
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

#ifndef  AFNIX_ALGEBRA_HPP
#define  AFNIX_ALGEBRA_HPP

#ifndef  AFNIX_IVI_HPP
#include "Ivi.hpp"
#endif

#ifndef  AFNIX_NVI_HPP
#include "Nvi.hpp"
#endif

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif

#ifndef  AFNIX_NMI_HPP
#include "Nmi.hpp"
#endif

#ifndef  AFNIX_RMI_HPP
#include "Rmi.hpp"
#endif

#ifndef  AFNIX_CPI_HPP
#include "Cpi.hpp"
#endif

namespace afnix {

  /// The Algebra class is a class that implements various algorithms used
  /// in algebra calculus. This is the default implementation which is often
  /// optmized by underlying class.
  /// @author amaury darsch

  class Algebra {
  public:
    /// @return true if a vector is nil
    static bool isnil (const Ivi& v);

    /// @return true if a vector is nil
    static bool isnil (const Nvi& v);

    /// @return true if a vector is nil
    static bool isnil (const Rvi& v);
    
    /// copy a vector into another one
    /// @param r the result vector
    /// @param x the vector argument
    static void cpy (Ivi& r, const Ivi& x);

    /// copy a vector into another one
    /// @param r the result vector
    /// @param x the vector argument
    static void cpy (Rvi& r, const Rvi& x);

    /// compare two vectors
    /// @param x the vector argument
    /// @param y the vector argument
    static bool eql (const Ivi& x, const Ivi& y);

    /// compare two vectors
    /// @param x the vector argument
    /// @param y the vector argument
    static bool eql (const Rvi& x, const Rvi& y);
    
    /// compare two vectors by precision
    /// @param x the vector argument
    /// @param y the vector argument
    static bool cmp (const Rvi& x, const Rvi& y);
    
    /// compute the vector dot product
    /// @param x the vector argument
    /// @param y the vector argument
    static t_real dot (const Rvi& x, const Rvi& y);

    /// compute the vector dot product with Kahan's algorithm
    /// @param x the vector argument
    /// @param y the vector argument
    static t_real kdot (const Rvi& x, const Rvi& y);

    /// compute the vector norm
    /// @param x the vector argument
    static t_real norm (const Nvi& x);

    /// compute the vector norm
    /// @param x the vector argument
    static t_real norm (const Rvi& x);
    
    /// add a vector with a scalar
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void add (Ivi& r, const Ivi& x, const long s);

    /// add a vector with a scalar
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void add (Rvi& r, const Rvi& x, const t_real s);

    /// add a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void add (Ivi& r, const Ivi& x, const Ivi& y);

    /// add a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void add (Rvi& r, const Rvi& x, const Rvi& y);

    /// add a vector with another scaled one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    /// @param s the scalar factor
    static void add (Ivi& r, const Ivi& x, const Ivi& y, const long s);

    /// add a vector with another scaled one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    /// @param s the scalar factor
    static void add (Rvi& r, const Rvi& x, const Rvi& y, const t_real s);

    /// substract a vector with a scalar
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void sub (Ivi& r, const Ivi& x, const long s);

    /// substract a vector with a scalar
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void sub (Rvi& r, const Rvi& x, const t_real s);

    /// substract a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void sub (Ivi& r, const Ivi& x, const Ivi& y);

    /// substract a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void sub (Rvi& r, const Rvi& x, const Rvi& y);

    /// multiply a vector with a scalar
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void mul (Ivi& r, const Ivi& x, const long s);

    /// multiply a vector with a scalar
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void mul (Rvi& r, const Rvi& x, const t_real s);

    /// multiply a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void mul (Ivi& r, const Ivi& x, const Ivi& y);

    /// multiply a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void mul (Rvi& r, const Rvi& x, const Rvi& y);

    /// divide a vector with another one
    /// @param r the result vector
    /// @param x the vector argument
    /// @param y the vector argument
    static void div (Rvi& r, const Rvi& x, const Rvi& y);

    /// add equal with a vector
    /// @param r the result vector
    /// @param x the vector argument
    static void aeq (Ivi& r, const Ivi& x);

    /// add equal with a vector
    /// @param r the result vector
    /// @param x the vector argument
    static void aeq (Rvi& r, const Rvi& x);

    /// add equal with a scaled vector
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void aeq (Ivi& r, const Ivi& x, const long s);

    /// add equal with a scaled vector
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void aeq (Rvi& r, const Rvi& x, const t_real s);

    /// rescale equal with a vector
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void req (Ivi& r, const Ivi& x, const long s);

    /// rescale equal with a vector
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scalar factor
    static void req (Rvi& r, const Rvi& x, const t_real s);

    /// normalize a vector
    /// @param r the result vector
    /// @param x the vector to normalize
    static void normalize (Rvi& r, const Rvi& x);
    
    /// clear a matrix lower triangular part
    static void clt (Rmi& m);

    /// copy a matrix into another one
    /// @param mr the result matrix
    /// @param mx the matrix argument
    static void cpy (Rmi& mr, const Rmi& mx);

    /// copy a matrix row into a vector
    /// @param r   the result vector
    /// @param m   the matrix argument
    /// @param row the row to copy
    static void cpr (Rvi& r, const Rmi& m, const t_long row);

    /// copy a vector as a matrix row
    /// @param m   the result matrix
    /// @param x   the vector argument
    /// @param row the row to copy
    static void cpr (Rmi& m, const Rvi& x, const t_long row);

    /// copy a matrix column into a vector
    /// @param r   the result vector
    /// @param m   the matrix argument
    /// @param col the column to copy
    static void cpc (Rvi& r, const Rmi& m, const t_long col);

    /// copy a vector as a matrix column
    /// @param m   the result matrix
    /// @param x   the vector argument
    /// @param col the column to copy
    static void cpc (Rmi& m, const Rvi& x, const t_long col);

    /// compare two matrices
    /// @param mx the matrix argument
    /// @param my the matrix argument
    static bool eql (const Rmi& mx, const Rmi& my);

    /// compare two matrices by precision
    /// @param mx the matrix argument
    /// @param my the matrix argument
    static bool cmp (const Rmi& mx, const Rmi& my);

    /// check if a matrix row is null
    /// @param m the matrix argument
    /// @param row the matrix row
    static bool isnrow (const Rmi& m, const t_long row);

    /// check if a matrix column is null
    /// @param m the matrix argument
    /// @param col the matrix row
    static bool isncol (const Rmi& m, const t_long col);

    /// compute the matrix freobenius norm
    /// @param m the matrix argument
    static t_real norm (const Rmi& m);
    
    /// check if the matrix has null diagonal elements
    /// @param m the matrix argument
    static bool nulld (const Rmi& m);

    /// add a matrix with another one
    /// @param mr the result matrix
    /// @param mx the matrix argument
    /// @param my the matrix argument
    static void add (Rmi& mr, const Rmi& mx, const Rmi& my);

    /// substract a matrix with another one
    /// @param mr the result matrix
    /// @param mx the matrix argument
    /// @param my the matrix argument
    static void sub (Rmi& mr, const Rmi& mx, const Rmi& my);

    /// multiply two vectors as a matrix
    /// @param mr the result matrix
    /// @param u  the vector argument
    /// @param v  the second argument
    static void mul (Rmi& mr, const Rvi& u, const Rvi& v);

    /// multiply a matrix with a vector and a scaling factor
    /// @param r the result vector
    /// @param m the matrix argument
    /// @param x the vector argument
    /// @param s the scaling factor
    static void mul (Rvi& r, const Rmi& m, const Rvi& x, const t_real s);

    /// multiply two matrices
    /// @param mr the result matrix
    /// @param mx the matrix argument
    /// @param my the matrix argument
    static void mul (Rmi& mr, const Rmi& mx, const Rmi& my);

    /// multiply a transposed matrix with a vector and a scaling factor
    /// @param r the result vector
    /// @param m the matrix argument
    /// @param x the vector argument
    /// @param s the scaling factor
    static void tmul (Rvi& r, const Rmi& m, const Rvi& x, const t_real s);

    /// add equal a matrix with a vector at a column position
    /// @param m   the matrix to update
    /// @param v   the vector argument
    /// @param s   the scaling factor
    /// @param col the column to update
    static void cva (Rmi& m, const Rvi& x, const t_real s, const t_long col);

    /// add equal a matrix with a vector at a row position
    /// @param m   the matrix to update
    /// @param v   the vector argument
    /// @param s   the scaling factor
    /// @param row the row to update
    static void rva (Rmi& m, const Rvi& x, const t_real s, const t_long row);

    /// initialize an identity matrix
    static void eye (Nmi& mr);
    
    /// initialize an identity matrix
    static void eye (Rmi& mr);

    /// create a random vector with bounds
    /// @param r    the result vector
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static void random (Ivi& r, const long rmin, const long rmax);

    /// create a random vector with bounds
    /// @param r    the result vector
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static void random (Nvi& r, const Numeral& rmin, const Numeral& rmax);

    /// create a random vector with bounds
    /// @param r    the result vector
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static void random (Rvi& r, const t_real rmin, const t_real rmax);

    /// create a random matrix with bounds
    /// @param mr   the result matrix
    /// @param nmin the minimum value
    /// @param nmax the maximum value
    static void random (Nmi& mr, const Numeral& rmin, const Numeral& rmax);

    /// create a random matrix with bounds
    /// @param mr   the result matrix
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static void random (Rmi& mr, const t_real rmin, const t_real rmax);

    /// create a sparse matrix with bounds
    /// @param mr   the result matrix
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param nzsz the non zero size
    static void sparse (Rmi& mr, const t_real rmin, const t_real rmax,
			const t_long nzsz);

    /// make a matrix diagonaly dominant
    /// @param mr the result matrix
    /// @param df the diagonal factor to add
    static void toddom (Rmi& mr, const t_real df);

    /// perform a givens vector update
    /// @param r the vector to update
    /// @param i the row coordinate
    /// @param j the column coordinate
    /// @param c the givens ii/jj coefficient
    /// @param s the givens ij/ji coefficient
    static void givens (Rvi& r, const t_long i, const t_long j, 
			const t_real c, const t_real s);

    /// perform a givens matrix update
    /// @param m the matrix to update
    /// @param i the row coordinate
    /// @param j the column coordinate
    /// @param c the givens ii/jj coefficient
    /// @param s the givens ij/ji coefficient
    /// @param pflg the partial flag
    static void givens (Rmi& m, const t_long i, const t_long j, 
			const t_real c, const t_real s, const bool pflg);

    /// perform an upper triangular multiplication
    /// @param r the result vector
    /// @param m the matrix argument
    /// @param x the vector argument
    static void utmul (Rvi& r, const Rmi& m, const Rvi& x);

    /// perform an upper triangular division
    /// @param r the result vector
    /// @param m the matrix argument
    /// @param x the vector argument
    static void utdiv (Rvi& r, const Rmi& m, const Rvi& x);

    /// perform a lower triangular multiplication
    /// @param r the result vector
    /// @param m the matrix argument
    /// @param x the vector argument
    static void ltmul (Rvi& r, const Rmi& m, const Rvi& x);

    /// perform a lower triangular division
    /// @param r the result vector
    /// @param m the matrix argument
    /// @param x the vector argument
    static void ltdiv (Rvi& r, const Rmi& m, const Rvi& x);
    
    /// perform a vector permutation
    /// @param r the result vector
    /// @param x the vector argument
    /// @param p the permutation argument
    static void permutate (Ivi& r, const Ivi& x, const Cpi& p);

    /// perform a vector permutation
    /// @param r the result vector
    /// @param x the vector argument
    /// @param p the permutation argument
    static void permutate (Rvi& r, const Rvi& x, const Cpi& p);

    /// perform a matrix permutation
    /// @param r the result matrix
    /// @param m the matrix argument
    /// @param p the permutation argument
    static void permutate (Rmi& r, const Rmi& m, const Cpi& p);

    /// perform a reverse vector permutation
    /// @param r the result vector
    /// @param x the vector argument
    /// @param p the permutation argument
    static void reverse (Ivi& r, const Ivi& x, const Cpi& p);

    /// perform a reverse vector permutation
    /// @param r the result vector
    /// @param x the vector argument
    /// @param p the permutation argument
    static void reverse (Rvi& r, const Rvi& x, const Cpi& p);

    /// perform a reverse matrix permutation
    /// @param r the result matrix
    /// @param m the matrix argument
    /// @param p the permutation argument
    static void reverse (Rmi& r, const Rmi& m, const Cpi& p);
  };
}

#endif
