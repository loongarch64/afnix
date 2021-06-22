// ---------------------------------------------------------------------------
// - Nblock.hpp                                                              -
// - afnix:mth module - numeral block matrix definitions                     -
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

#ifndef  AFNIX_NBLOCK_HPP
#define  AFNIX_NBLOCK_HPP

#ifndef  AFNIX_NMI_HPP
#include "Nmi.hpp"
#endif
 
#ifndef  AFNIX_NVECTOR_HPP
#include "Nvector.hpp"
#endif
 
namespace afnix {

  /// This Nblock class is the block implementation of the numeral matrix
  /// interface. The matrix is designed to support an array representation
  /// without any optimization for sparsity. Although the class is very
  /// efficient, it cannot grow as much as needed. Use with caution.
  /// @author amaury darsch

  class Nblock : public Nmi, public Viewable {
  public:
    /// generate a random matrix by size
    /// @param size the matrix size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Nblock* random (const t_long size, const Numeral& rmin,
			   const Numeral& rmax);

    /// generate a random matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    /// @param rmin the minimum value
    /// @param rmax the maximum value
    static Nblock* random (const t_long   rsiz, const t_long   csiz,
			   const Numeral& rmin, const Numeral& rmax);

  protected:
    /// the matrix block
    union {
      /// the integer matrix
      int* p_iblk;
      /// the float block
      float* p_fblk;
      /// the byte block
      t_byte* p_bblk;
    };
    /// the column order flag
    bool d_cord;
    /// the line padding
    long d_lpad;
    /// the block padding
    long d_bpad;
    
  public:
    /// create a null matrix
    Nblock (void);

    /// create a square matrix by size
    /// @param size the matrix size
    Nblock (const t_long size);

    /// create a matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    Nblock (const t_long rsiz, const t_long csiz);

    /// create a matrix by size and type
    /// @param rsiz the row size
    /// @param csiz the column size
    /// @param numt the numeral type
    Nblock (const t_long rsiz, const t_long csiz, const Numeral::t_numt numt);

    /// create a matrix by size, type and column order
    /// @param size the  matrix size
    /// @param numt the numeral type
    /// @param cord the column order
    Nblock (const t_long size, const Numeral::t_numt numt, const bool cord);

    /// copy construct this matrix
    /// @param that the matrix to copy
    Nblock (const Nblock& that);

    /// destroy this matrix
    ~Nblock (void);

    /// assign a matrix to this one
    /// @param that the matrix to assign
    Nblock& operator = (const Nblock& that);
    
    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// @return the serial did
    t_word getdid (void) const override;

    /// @return the serial sid
    t_word getsid (void) const override;

    /// reset this matrix
    void reset (void) override;

    /// clear this matrix
    void clear (void) override;

    /// resize this matrix
    /// @param rsiz the new matrix row size
    /// @param csiz the new matrix column size
    void resize (const t_long rsiz, const t_long csiz) override;

    /// @return the viewable size
    long tosize (void) const override;

    /// @return the viewable data
    t_byte* tobyte (void) override;
    
    /// @return the viewable data
    const t_byte* tobyte (void) const override;
    
  public:
    /// no lock - set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    void nlset (const t_long row,const t_long col,const Numeral& val) override;

    /// no lock - get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    Numeral nlget (const t_long row, const t_long col) const override;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
