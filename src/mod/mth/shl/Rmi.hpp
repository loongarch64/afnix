// ---------------------------------------------------------------------------
// - Rmi.hpp                                                                 -
// - afnix:mth module - real matrix interface definitions                    -
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

#ifndef  AFNIX_RMI_HPP
#define  AFNIX_RMI_HPP

#ifndef  AFNIX_RVI_HPP
#include "Rvi.hpp"
#endif

#ifndef  AFNIX_AMI_HPP
#include "Ami.hpp"
#endif

#ifndef  AFNIX_ITERABLE_HPP
#include "Iterable.hpp"
#endif

namespace afnix {

  /// This Rmi class is an abstract class that models the behavior of a
  /// real based matrix. . The class extends the abstract matrix interface with
  /// specific real methods.
  /// @author amaury darsch

  class Rmi : public Ami, public Iterable {
  public:
    /// create a null matrix
    Rmi (void) =default;

    /// create a square matrix by size
    /// @param size the matrix size
    Rmi (const t_long size);

    /// create a matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    Rmi (const t_long rsiz, const t_long csiz);

    /// copy construct this matrix
    /// @param that the matrix to copy
    Rmi (const Rmi& that);

    /// copy move this matrix
    /// @param that the matrix to move
    Rmi (Rmi&& that) noexcept;

    /// assign a matrix into this one
    /// @param that the matrix to assign
    Rmi& operator = (const Rmi& that);

    /// move a matrix into this one
    /// @param that the matrix to move
    Rmi& operator = (Rmi&& that) noexcept;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;
    
    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// reset this matrix
    void reset (void) override;
    
    /// clear this matrix
    void clear (void) override;

    /// compare two matrices
    /// @param  x the vector argument
    /// @return true if they are equals
    virtual bool operator == (const Rmi& x) const;

    /// compare two matrices
    /// @param  x the vector argument
    /// @return true if they are not equals
    virtual bool operator != (const Rmi& x) const;

    /// @return the matrix row barrier
    virtual t_long getrowb (void) const;

    /// @return the matrix column barrier
    virtual t_long getcolb (void) const;
    
    /// return true if the matrix is null
    virtual bool isnil (void) const;

    /// clear a matrix zone
    /// @param row the stating row
    /// @param col the starting column
    /// @param rsz the row size
    /// @param csz the column size
    
    virtual void clear (const t_long row, const t_long col,
			const t_long rsz, const t_long csz);

    /// clear the lower triangular part
    virtual void clt (void);

    /// compress the matrix if possible
    virtual bool compress (void);

    /// create a zero identical matrix
    virtual Rmi* zeros (void) const =0;

    /// set an identity matrix
    virtual void eye (void);
    
    /// copy a matrix row into a vector
    /// @param row the row to copy
    virtual Rvi* cpr (const t_long row) const;

    /// copy a matrix column into a vector
    /// @param col the column to copy
    virtual Rvi* cpc (const t_long col) const;

    /// compare two matrices upto a precision
    /// @param mx the matrix argument
    virtual bool cmp (const Rmi& x) const;

    /// check if a matrix row is null
    virtual bool isnrow (const t_long row) const;

    /// check if a matrix column is null
    virtual bool isncol (const t_long row) const;

    /// @return the matrix frobenius norm
    virtual t_real norm (void) const;

    /// fill a matrix by copy
    /// @param m the matrix to fill
    virtual void fill (Rmi& m) const;

    /// set a matrix by copy
    /// @param m the matrix to copy
    virtual void set (const Rmi& m);

    /// set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    virtual void set (const t_long row, const t_long col, const t_real val);

    /// get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    virtual t_real get (const t_long row, const t_long col) const;

    /// multiply a matrix with a vector and a scaling factor
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& mul (Rvi& r, const Rvi& x, const t_real s) const;

    /// multiply a transposed matrix with a vector and a scaling factor
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& tmul (Rvi& r, const Rvi& x, const t_real s) const;

    /// fill this matrix with random values
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    virtual void random (const t_real rmin, const t_real rmax);

    /// fill this matrix with sparse random values
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param nzsz the non zero size
    virtual void sparse (const t_real rmin, const t_real rmax, 
			 const t_long nzsz);

    /// make this matrix diagonal dominant
    /// @param df the dominance factor
    virtual void toddom (const t_real df);

    /// perform an upper triangular multiplication
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& utmul (Rvi& r, const Rvi& x) const;

    /// perform an upper triangular division
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& utdiv (Rvi& r, const Rvi& x) const;

    /// perform a lower triangular multiplication
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& ltmul (Rvi& r, const Rvi& x) const;

    /// perform a lower triangular division
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& ltdiv (Rvi& r, const Rvi& x) const;

    /// permutate this matrix
    /// @param p the permutation object
    virtual Rmi* permutate (const Cpi& p) const;

    /// reverse permutate this matrix
    /// @param p the permutation object
    virtual Rmi* reverse (const Cpi& p) const;

    /// perform a givens matrix update
    /// @param i the row coordinate
    /// @param j the column coordinate
    /// @param c the givens ii/jj coefficient
    /// @param s the givens ij/ji coefficient
    /// @param pflg the partial flag
    virtual void givens (const t_long i, const t_long j, 
			 const t_real c, const t_real s, const bool pflg);

  public:
    /// no lock - clear a matrix zone
    /// @param row the stating row
    /// @param col the starting column
    /// @param rsz the row size
    /// @param csz the column size
    virtual void nlclear (const t_long row, const t_long col,
			  const t_long rsz, const t_long csz);

    /// no lock - set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    virtual void nlset (const t_long row, const t_long col, 
			const t_real val) =0;

    /// no lock - get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    virtual t_real nlget (const t_long row, const t_long col) const =0;

    /// no lock - multiply a matrix with a vector and a scaling factor
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& nlmul (Rvi& r, const Rvi& x, const t_real s) const;
 
    /// no lock - multiply a transposed matrix with a vector and scaling factor
    /// @param r the result vector
    /// @param x the vector argument
    /// @param s the scaling factor
    virtual Rvi& nltmul (Rvi& r, const Rvi& x, const t_real s) const;

    /// perform a givens matrix update
    /// @param i the row coordinate
    /// @param j the column coordinate
    /// @param c the givens ii/jj coefficient
    /// @param s the givens ij/ji coefficient
    /// @param pflg the partial flag
    virtual void nlgivens (const t_long i, const t_long j, 
			   const t_real c, const t_real s, const bool pflg);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object) override;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
    
  };

  /// The Rmit class is the iterator for the real matrix class. Such 
  /// iterator is constructed with the "makeit" method. The iterator
  /// is reset to the beginning of the matrix.
  /// @author amaury darsch

  class Rmit : public Iterator {
  public:
    /// the matrix iterator type
    enum t_rmit {
      RMIT_SEQ, // sequential iterator
      RMIT_ROW, // row iterator
      RMIT_COL  // column iterator
    };

  protected:
    /// the iterator type
    t_rmit d_rmit;

  public:
    /// move the matrix iterator to the next position
    void next (void) override;
    
    /// move the matrix iterator to the previous position
    void prev (void) override;

    /// @return true if the iterator is at the end
    bool isend (void) const override;

    /// move the iterator to a matrix point
    /// @param row the matrix row coordinate
    /// @param col the matrix col coordinate
    virtual void move (const t_long row, const t_long col);

    /// @return the iterator row coordinate
    virtual t_long getrow (void) const;

    /// @return the iterator column coordinate
    virtual t_long getcol (void) const;

    /// set the matrix at the iterator position
    /// @param val the value to set
    virtual void setval (const t_real val);

    /// @return the iterator matrix value
    virtual t_real getval (void) const;

    /// set the iterator type
    /// @param rmit th iterator type
    virtual void settype (const t_rmit rmit);
    
  public:
    /// move the matrix iterator to the next position (no lock)
    virtual void nlnext (void) =0;
    
    /// move the matrix iterator to the previous position (no lock)
    virtual void nlprev (void) =0;

    /// @return true if the iterator is at the end (no lock)
    virtual bool nlend (void) const =0;

    /// move the iterator to a matrix point (no lock)
    /// @param row the matrix row coordinate
    /// @param col the matrix col coordinate
    virtual void nlmove (const t_long row, const t_long col) =0;

    /// @return the iterator row coordinate (no lock)
    virtual t_long nlgrow (void) const =0;

    /// @return the iterator column coordinate (no lock)
    virtual t_long nlgcol (void) const =0;

    /// set the matrix at the iterator position (no lock)
    /// @param val the value to set
    virtual void nlsval (const t_real val) =0;

    /// @return the iterator matrix value (no lock)
    virtual t_real nlgval (void) const =0;

  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
