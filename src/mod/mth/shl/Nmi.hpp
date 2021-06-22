// ---------------------------------------------------------------------------
// - Nmi.hpp                                                                 -
// - afnix:mth module - numeral matrix interface definitions                 -
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

#ifndef  AFNIX_NMI_HPP
#define  AFNIX_NMI_HPP

#ifndef  AFNIX_NVI_HPP
#include "Nvi.hpp"
#endif

#ifndef  AFNIX_AMI_HPP
#include "Ami.hpp"
#endif

namespace afnix {

  /// This Nmi class is an abstract class that models the behavior of a
  /// numeral based matrix. The class extends the abstract matrix interface
  /// with specific numeral methods.
  /// @author amaury darsch

  class Nmi : public Ami {
  protected:
    /// the numeral type
    Numeral::t_numt d_numt;
    
  public:
    /// create a null matrix
    Nmi (void);

    /// create a square matrix by size
    /// @param size the matrix size
    Nmi (const t_long size);

    /// create a matrix by size
    /// @param rsiz the row size
    /// @param csiz the column size
    Nmi (const t_long rsiz, const t_long csiz);

    /// create a matrix by size and type
    /// @param rsiz the row size
    /// @param csiz the column size
    /// @param numt the numeral type
    Nmi (const t_long rsiz, const t_long csiz, const Numeral::t_numt numt);

    /// copy construct this matrix
    /// @param that the matrix to copy
    Nmi (const Nmi& that);

    /// copy move this matrix
    /// @param that the matrix to move
    Nmi (Nmi&& that) noexcept;

    /// assign a matrix into this one
    /// @param that the matrix to assign
    Nmi& operator = (const Nmi& that);

    /// move a matrix into this one
    /// @param that the matrix to move
    Nmi& operator = (Nmi&& that) noexcept;
    
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

    /// set an identity matrix
    virtual void eye (void);
    
    /// set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    virtual void set (const t_long row, const t_long col, const Numeral& val);

    /// get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    virtual Numeral get (const t_long row, const t_long col) const;

    /// get a matrix integer value by position
    /// @param row the row position
    /// @param col the column position
    virtual t_long getlong (const t_long row, const t_long col) const;

    /// get a matrix real value by position
    /// @param row the row position
    /// @param col the column position
    virtual t_real getreal (const t_long row, const t_long col) const;

  public:
    /// no lock - set a matrix by position
    /// @param row the row position
    /// @param col the column position
    /// @param val the value to set
    virtual void nlset (const t_long row, const t_long col, 
			const Numeral& val) =0;

    /// no lock - get a matrix value by position
    /// @param row the row position
    /// @param col the column position
    virtual Numeral nlget (const t_long row, const t_long col) const =0;

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
