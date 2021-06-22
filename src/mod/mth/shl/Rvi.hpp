// ---------------------------------------------------------------------------
// - Rvi.hpp                                                                 -
// - afnix:mth module - real vector interface definitions                    -
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

#ifndef  AFNIX_RVI_HPP
#define  AFNIX_RVI_HPP

#ifndef  AFNIX_AVI_HPP
#include "Avi.hpp"
#endif

#ifndef  AFNIX_CPI_HPP
#include "Cpi.hpp"
#endif

namespace afnix {

  /// This Rvi class is an abstract class that models the behavior of a
  /// real based vector. The class extends the abstract vector interface with
  /// specific real methods.
  /// @author amaury darsch

  class Rvi : public Avi {
  public:
    /// create a null vector
    Rvi (void) =default;

    /// create a vector by size
    /// @param size the vector size
    Rvi (const t_long size);

    /// copy construct this vector
    /// @param that the vector to copy
    Rvi (const Rvi& that);

    /// copy move this vector
    /// @param that the vector to move
    Rvi (Rvi&& that) noexcept;

    /// assign a vector into this one
    /// @param that the vector to assign
    Rvi& operator = (const Rvi& that);

    /// move a vector into this one
    /// @param that the vector to move
    Rvi& operator = (Rvi&& that) noexcept;

    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// reset this vector
    void reset (void) override;

    /// clear this vector
    void clear (void) override;

    /// @return true if the vector is nil
    bool isnil (void) const override;

    /// compare two vectors
    /// @param  x the vector argument
    /// @return true if they are equals
    virtual bool operator == (const Rvi& x) const;

    /// compare two vectors
    /// @param  x the vector argument
    /// @return true if they are not equals
    virtual bool operator != (const Rvi& x) const;

    /// add a vector by a scalar
    /// @param s the scalar value
    virtual Rvi& operator += (const t_real s);

    /// add a vector with a vector
    /// @param x the vector argument
    virtual Rvi& operator += (const Rvi& x);

    /// substract a vector by a scalar
    /// @param s the scalar value
    virtual Rvi& operator -= (const t_real s);

    /// multiply a vector by a scalar
    /// @param s the scalar value
    virtual Rvi& operator *= (const t_real s);

    /// compute the vector dot product
    /// @param x the vector argument
    virtual t_real operator ^ (const Rvi& x) const;

    /// copy a vector into this one
    /// @param x the vector to copy
    virtual Rvi& cpy (const Rvi& x);

    /// compare a vector value
    /// @param pos the vector position
    /// @param val the value to compare
    virtual bool cmp (const t_long pos, const t_real val) const;

    /// compare two vectors upto a precision
    /// @param x the vector argument
    virtual bool cmp (const Rvi& x) const;

    /// @return the vector norm
    virtual t_real norm (void) const;

    /// set a vector by value
    /// @param val the value to set
    virtual void set (const t_real val);

    /// set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void set (const t_long pos, const t_real val);

    /// get a vector value by position
    /// @param pos the vector position
    virtual t_real get (const t_long pos) const;

    /// add a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    virtual Rvi& add (const Rvi& x, const t_real s);

    /// add a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Rvi& add (const Rvi& x, const Rvi& y);

    /// add a vector with another scaled one
    /// @param x the vector argument
    /// @param y the vector argument
    /// @param s the scalar factor
    virtual Rvi& add (const Rvi& x, const Rvi& y, const t_real s);

    /// substract a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    virtual Rvi& sub (const Rvi& x, const t_real s);

    /// substract a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Rvi& sub (const Rvi& x, const Rvi& y);

    /// multiply a vector with a scaled vector
    /// @param x the vector to multiply
    /// @param s the scaling factor
    virtual Rvi& mul (const Rvi& x, const t_real s);

    /// multiply a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Rvi& mul (const Rvi& x, const Rvi& y);

    /// divide a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Rvi& div (const Rvi& x, const Rvi& y);

    /// add equal with a vector
    /// @param x the vector to add
    virtual Rvi& aeq (const Rvi& x);

    /// add equal with a scaled vector
    /// @param x the vector to add
    /// @param s the scaling factor
    virtual Rvi& aeq (const Rvi& x, const t_real s);

    /// rescale equal with a vector
    /// @param x the vector to add
    /// @param s the scaling factor
    virtual Rvi& req (const Rvi& x, const t_real s);

    /// normalize this vector
    virtual Rvi& normalize (void);

    /// permutate this vector
    /// @param p the permutation object
    virtual Rvi* permutate (const Cpi& p) const;

    /// reverse this vector permutation
    /// @param p the permutation object
    virtual Rvi* reverse (const Cpi& p) const;

    /// perform a givens vector update
    /// @param i the row coordinate
    /// @param j the column coordinate
    /// @param c the givens ii/jj coefficient
    /// @param s the givens ij/ji coefficient
    virtual void givens (const t_long i, const t_long j, 
			 const t_real c, const t_real s);

  public:
    /// no lock - set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void nlset (const t_long pos, const t_real val) =0;

    /// no lock - get a vector value by position
    /// @param pos the vector position
    virtual t_real nlget (const t_long pos) const =0;

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
}

#endif
