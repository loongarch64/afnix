// ---------------------------------------------------------------------------
// - Ami.hpp                                                                 -
// - afnix:mth module - abstact matrix interface definitions                 -
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

#ifndef  AFNIX_AMI_HPP
#define  AFNIX_AMI_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

namespace afnix {

  /// This Ami class is an abstract class that models the behavior of a
  /// typed matrix. Tha class defines the matrix size.
  /// @author amaury darsch

  class Ami : public virtual Serial {
  public:
    /// the matrix row size
    t_long d_rsiz;
    /// the matrix column size
    t_long d_csiz;

  public:
    /// create a null matrix
    Ami (void);

    /// create a square matrix by size
    /// @param size the matrix size
    Ami (const t_long size);

    /// create a matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    Ami (const t_long rsiz, const t_long csiz);

    /// copy construct this matrix
    /// @param that the matrix to copy
    Ami (const Ami& that);

    /// copy move this matrix
    /// @param that the matrix to move
    Ami (Ami&& that) noexcept;

    /// assign a matrix into this one
    /// @param that the matrix to assign
    Ami& operator = (const Ami& that);

    /// move a matrix into this one
    /// @param that the matrix to move
    Ami& operator = (Ami&& that) noexcept;
    
    /// @return the matrix row size
    virtual t_long getrsiz (void) const;

    /// @return the matrix column size
    virtual t_long getcsiz (void) const;

    /// @return true if teh matrix is square
    virtual bool issquare (void) const;
    
    /// reset this matrix
    virtual void reset (void) =0;
    
    /// clear this matrix
    virtual void clear (void) =0;
    
    /// resize this matrix
    /// @param rsiz the new matrix row size
    /// @param csiz the new matrix column size
    virtual void resize (const t_long rsiz, const t_long csiz) =0;

  public:
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
