// ---------------------------------------------------------------------------
// - Rmatrix.hpp                                                             -
// - afnix:mth module - real matrix definitions                              -
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

#ifndef  AFNIX_RMATRIX_HPP
#define  AFNIX_RMATRIX_HPP

#ifndef  AFNIX_RMI_HPP
#include "Rmi.hpp"
#endif

#ifndef  AFNIX_MUTEX_HPP
#include "Mutex.hpp"
#endif
 
#ifndef  AFNIX_RVECTOR_HPP
#include "Rvector.hpp"
#endif
 
namespace afnix {

  /// This Rmatrix class is the default implementation of the real matrix
  /// interface. The matrix is designed to support a sparse representation
  /// the form of blocks aggregation. The block index is obtained from the
  /// matrix index with the help of field index. The lower index is derived
  /// from the lower 32 bits while the higher index is derived from the higher
  /// 32 bits if needed. The implementation automatically adapt itself with
  /// a 32 bits or 64 bits long integer. The lower 32 bits indexes are derived
  /// as LB[4] (lower bottom - 4 bits), CB[8] (center-bottom 8 bits), CC[8]
  /// (center-center 8 bits), CT[8] (center-top - 8 bits) and LT[4] (lower-top
  /// 4 bits). The upper block (HB) has a variable size.
  /// @author amaury darsch

  class Rmatrix : public Rmi {
  public:
    /// generate a random matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Rmatrix* random (const t_long size, const t_real rmin,
			    const t_real rmax);

    /// generate a random matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param ddom the diagonally dominant flag
    static Rmatrix* random (const t_long size, const t_real rmin,
			    const t_real rmax, const bool   ddom);

    /// generate a sparse matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param nzsz the non zero size
    static Rmatrix* sparse (const t_long size, const t_real rmin,
			    const t_real rmax, const t_long nzsz);
    
    /// add a matrix with another one
    /// @param mx the matrix argument
    /// @param my the matrix argument
    friend Rmatrix operator + (const Rmatrix& mx, const Rmatrix& my);

    /// substract a matrix with another one
    /// @param mx the matrix argument
    /// @param my the matrix argument
    friend Rmatrix operator - (const Rmatrix& mx, const Rmatrix& my);

    /// multiply a vector with a matrix
    /// @param m the matrix argument
    /// @param x the vector argument
    friend Rvector operator * (const Rmatrix& m, const Rvector& x);

    /// multiply two matrices
    /// @param mx the matrix argument
    /// @param my the matrix argument
    friend Rmatrix operator * (const Rmatrix& mx, const Rmatrix& my);

  public:
    /// the internal lb type
    using t_rmlb = t_real*;
    /// the internal cb type
    using t_rmcb = t_rmlb*;
    /// the internal cc type
    using t_rmcc = t_rmcb*;
    /// the internal ct type
    using t_rmct = t_rmcc*;
    /// the internal lt type
    using t_rmlt = t_rmct*;
    /// the internal hb type
    using t_rmhb = t_rmlt*;

  private:
    /// the sparse matrix
    t_rmhb p_rmhb;
    /// the block mutex
    Mutex* p_rmbm;
  
  public:
    /// create a null matrix
    Rmatrix (void);

    /// create a square matrix by size
    /// @param size the matrix size
    Rmatrix (const t_long size);

    /// create a matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    Rmatrix (const t_long rsiz, const t_long csiz);

    /// create a matrix by vector multiply
    /// @param u the vector argument
    /// @param v the vector argument
    Rmatrix (const Rvi& u, const Rvi& v);

    /// copy construct this matrix
    /// @param that the matrix to copy
    Rmatrix (const Rmatrix& that);

    /// destroy this matrix
    ~Rmatrix (void);

    /// assign a matrix to this one
    /// @param that the matrix to assign
    Rmatrix& operator = (const Rmatrix& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;
    
    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const; 

    /// @return the matrix row barrier
    t_long getrowb (void) const;

    /// @return the matrix column barrier
    t_long getcolb (void) const;

    /// return true if the matrix is null
    bool isnil (void) const;

    /// reset this matrix
    void reset (void);

    /// clear this matrix
    void clear (void);

    /// clear this matrix lower triangular part
    void clt (void);

    /// resize this matrix
    /// @param rsiz the new matrix row size
    /// @param csiz the new matrix column size
    void resize (const t_long rsiz, const t_long csiz);

    /// compress the matrix if possible
    bool compress (void);

    /// create a zero identical matrix
    Rmi* zeros (void) const;

    /// copy a matrix row into a vector
    /// @param row the row to copy
    Rvi* cpr (const t_long row) const;

    /// copy a matrix column into a vector
    /// @param col the column to copy
    Rvi* cpc (const t_long col) const;

    /// @return a new iterator for this matrix
    Iterator* makeit (void);

    /// permutate this matrix
    /// @param p the permutation object
    Rmi* permutate (const Cpi& p) const;

    /// reverse permutate this matrix
    /// @param p the permutation object
    Rmi* reverse (const Cpi& p) const;

  public:
    /// no lock - clear a matrix zone
    /// @param row the stating row
    /// @param col the starting column
    /// @param rsz the row size
    /// @param csz the column size
    void nlclear (const t_long row, const t_long col,
		  const t_long rsz, const t_long csz);

    /// no lock - set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    void nlset (const t_long row, const t_long col, const t_real val);

    /// no lock - get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    t_real nlget (const t_long row, const t_long col) const;

    /// no lock - perform a givens matrix update
    /// @param i the row coordinate
    /// @param j the column coordinate
    /// @param c the givens ii/jj coefficient
    /// @param s the givens ij/ji coefficient
    /// @param pflg the partial flag
    void nlgivens (const t_long i, const t_long j, const t_real c, 
		   const t_real s, const bool pflg);

  private:
    // make the matrix iterator a friend
    friend class Rmatrixit;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object);

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };

  /// The Rmatrixit class is the iterator for the real matrix class. Such 
  /// iterator is constructed with the "makeit" method. The iterator
  /// is reset to the beginning of the matrix.
  /// @author amaury darsch

  class Rmatrixit : public Rmit {
  private:
    /// the matrix to iterate
    Rmatrix* p_mobj;
    /// the hb size    
    long d_hbsz;
    /// the original move row
    t_long d_mrow;
    /// the original move column
    t_long d_mcol;
    /// the hb block index
    long d_bihb;
    /// the lt block index
    long d_bilt;
    /// the ct block index
    long d_bict;
    /// the cc block index
    long d_bicc;
    /// the cl block index
    long d_bicb;
    /// the lb block index
    long d_bilb;
    /// the hb mcnext barrier 
    long d_hbib;
    /// the lt mcnext barrier 
    long d_ltib;
    /// the ct mcnext barrier 
    long d_ctib;
    /// the cc index barrier 
    long d_ccib;
    /// the cb index barrier 
    long d_cbib;
    /// the lb index barrier 
    long d_lbib;
    /// the lt col max barrier 
    long d_ltcm;
    /// the ct col max barrier 
    long d_ctcm;
    /// the cc col max barrier 
    long d_cccm;
    /// the cb col max barrier 
    long d_cbcm;
    /// the lb col max barrier 
    long d_lbcm;
    /// the lt col reset barrier 
    long d_ltrb;
    /// the ct col reset barrier 
    long d_ctrb;
    /// the cc col reset barrier 
    long d_ccrb;
    /// the cb col reset barrier 
    long d_cbrb;
    /// the lb col reset barrier 
    long d_lbrb;
    
  public:
    /// create a new iterator from a matrix
    /// @param mobj the matrix to iterate
    Rmatrixit (Rmatrix* vobj);

    /// create a new iterator by type
    /// @param mobj the matrix to iterate
    /// @param rmit the matrix iterator type
    Rmatrixit (Rmatrix* vobj, const t_rmit rmit);
    
    /// destroy this matrix iterator
    ~Rmatrixit (void);
    
    /// @return the class name
    String repr (void) const;
    
    /// reset the iterator to the begining
    void begin (void);
    
    /// reset the iterator to the end
    void end (void);
        
    /// @return the object at the current position
    Object* getobj (void) const;
    
  public:
    /// no lock - move the matrix iterator to the next position
    void nlnext (void);
    
    /// no lock - move the matrix iterator to the previous position
    void nlprev (void);

    /// @return true if the iterator is at the end (no lock)
    bool nlend (void) const;
    
    /// no lock - move the iterator to a point
    /// @param row the matrix row coordinate
    /// @param col the matrix col coordinate
    void nlmove (const t_long row, const t_long col);

    /// @return the row coordinate at the iterator position (no lock)
    t_long nlgrow (void) const;

    /// @return the column coordinate at the iterator position (no lock)
    t_long nlgcol (void) const;

    /// set the matrix at the iterator position (no lock)
    /// @param val the value to set
    void nlsval (const t_real val);

    /// @return the value at the iterator position (no lock)
    t_real nlgval (void) const;

  private:
    // move the iterator to the next position (no lock)
    void   mvnext (void);
    // move the iterator to the next row position (no lock)
    void   mrnext (void);
    // move the iterator to the next column position (no lock)
    void   mcnext (void);
    // move the iterator to the previous position (no lock)
    void   mvprev (void);
    // move the iterator to the previous row position (no lock)
    void   mrprev (void);
    // move the iterator to the previous column position (no lock)
    void   mcprev (void);

  private:
    // make the copy constructor private
    Rmatrixit (const Rmatrixit&);
    // make the assignment operator private
    Rmatrixit& operator = (const Rmatrixit&);
  };
}

#endif
