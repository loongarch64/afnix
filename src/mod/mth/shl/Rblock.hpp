// ---------------------------------------------------------------------------
// - Rblock.hpp                                                              -
// - afnix:mth module - real block matrix definitions                        -
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

#ifndef  AFNIX_RBLOCK_HPP
#define  AFNIX_RBLOCK_HPP

#ifndef  AFNIX_RMI_HPP
#include "Rmi.hpp"
#endif
 
#ifndef  AFNIX_RVECTOR_HPP
#include "Rvector.hpp"
#endif
 
namespace afnix {

  /// This Rblock class is the block implementation of the real matrix
  /// interface. The matrix is designed to support an array representation
  /// without any optimization for sparsity. Although the class is very
  /// efficient, it cannot grow as much as needed. Use with caution.
  /// @author amaury darsch

  class Rblock : public Rmi, public Viewable {
  public:
    /// generate a random matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Rblock* random (const t_long size, const t_real rmin,
			   const t_real rmax);
    
    /// generate a random matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param ddom the diagonally dominant flag
    static Rblock* random (const t_long size, const t_real rmin,
			   const t_real rmax, const bool   ddom);

    /// generate a random matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param ddom the diagonally dominant flag
    static Rblock* random (const t_long rsiz, const t_long csiz,
			   const t_real rmin, const t_real rmax,
			   const bool   ddom);
    
    /// generate a sparse matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    /// @param nzsz the non zero size
    static Rblock* sparse (const t_long size, const t_real rmin,
			   const t_real rmax, const t_long nzsz);
    
    /// add a matrix with another one
    /// @param mx the matrix argument
    /// @param my the matrix argument
    friend Rblock operator + (const Rblock& mx, const Rblock& my);

    /// substract a matrix with another one
    /// @param mx the matrix argument
    /// @param my the matrix argument
    friend Rblock operator - (const Rblock& mx, const Rblock& my);

    /// multiply a vector with a matrix
    /// @param m the matrix argument
    /// @param x the vector argument
    friend Rvector operator * (const Rblock& m, const Rvector& x);

    /// multiply two matrices
    /// @param mx the matrix argument
    /// @param my the matrix argument
    friend Rblock operator * (const Rblock& mx, const Rblock& my);

  private:
    /// the matrix block
    t_real* p_blok;

  public:
    /// create a null matrix
    Rblock (void);

    /// create a square matrix by size
    /// @param size the matrix size
    Rblock (const t_long size);

    /// create a matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    Rblock (const t_long rsiz, const t_long csiz);

    /// create a matrix by vector multiply
    /// @param u the vector argument
    /// @param v the vector argument
    Rblock (const Rvi& u, const Rvi& v);

    /// copy construct this matrix
    /// @param that the matrix to copy
    Rblock (const Rblock& that);

    /// destroy this matrix
    ~Rblock (void);

    /// assign a matrix to this one
    /// @param that the matrix to assign
    Rblock& operator = (const Rblock& that);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// return true if the matrix is null
    bool isnil (void) const;

    /// reset this matrix
    void reset (void);

    /// clear this matrix
    void clear (void);

    /// resize this matrix
    /// @param rsiz the new matrix row size
    /// @param csiz the new matrix column size
    void resize (const t_long rsiz, const t_long csiz);

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

    /// @return the viewable size
    long tosize (void) const;

    /// @return the viewable data
    t_byte* tobyte (void);
    
    /// @return the viewable data
    const t_byte* tobyte (void) const;
    
  public:
    /// no lock - set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    void nlset (const t_long row, const t_long col, const t_real val);

    /// no lock - get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    t_real nlget (const t_long row, const t_long col) const;

  private:
    // make the matrix iterator a friend
    friend class Rblockit;

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

  /// The Rblockit class is the iterator for the block real matrix class.
  /// Such iterator is constructed with the "makeit" method. The iterator
  /// is reset to the beginning of the matrix.
  /// @author amaury darsch

  class Rblockit : public Rmit {
  private:
    /// the matrix to iterate
    Rblock* p_mobj;
    /// the matrix row
    t_long  d_mrow;
    /// the matrix column
    t_long d_mcol;
    
  public:
    /// create a new iterator from a matrix
    /// @param mobj the matrix to iterate
    Rblockit (Rblock* vobj);

    /// create a new iterator by type
    /// @param mobj the matrix to iterate
    /// @param rmit the matrix iterator type
    Rblockit (Rblock* vobj, const t_rmit rmit);
    
    /// destroy this iterator
    ~Rblockit (void);
    
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
    Rblockit (const Rblockit&);
    // make the assignment operator private
    Rblockit& operator = (const Rblockit&);
  };
}

#endif
