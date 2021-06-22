// ---------------------------------------------------------------------------
// - Ivi.hpp                                                                 -
// - afnix:mth module - integer vector interface definitions                 -
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

#ifndef  AFNIX_IVI_HPP
#define  AFNIX_IVI_HPP

#ifndef  AFNIX_AVI_HPP
#include "Avi.hpp"
#endif

#ifndef  AFNIX_CPI_HPP
#include "Cpi.hpp"
#endif

namespace afnix {

  /// This Ivi class is an abstract class that models the behavior of a
  /// integer based vector. The class extends the abstract vector interface
  /// with specific integer methods.
  /// accessor and mutator methods.
  /// @author amaury darsch

  class Ivi : public Avi {
  public:
    /// create a null vector
    Ivi (void) =default;

    /// create a vector by size
    /// @param size the vector size
    Ivi (const t_long size);

    /// copy construct this vector
    /// @param that the vector to copy
    Ivi (const Ivi& that);

    /// copy move this vector
    /// @param that the vector to move
    Ivi (Ivi&& that) noexcept;

    /// assign a vector into this one
    /// @param that the vector to assign
    Ivi& operator = (const Ivi& that);

    /// move a vector into this one
    /// @param that the vector to move
    Ivi& operator = (Ivi&& that) noexcept;

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

    /// compare two vectors
    /// @param  x the vector argument
    /// @return true if they are equals
    virtual bool operator == (const Ivi& x) const;

    /// compare two vectors
    /// @param  x the vector argument
    /// @return true if they are not equals
    virtual bool operator != (const Ivi& x) const;

    /// add a vector by a scalar
    /// @param s the scalar value
    virtual Ivi& operator += (const long s);

    /// add a vector with a vector
    /// @param x the vector argument
    virtual Ivi& operator += (const Ivi& x);

    /// substract a vector by a scalar
    /// @param s the scalar value
    virtual Ivi& operator -= (const long s);

    /// multiply a vector by a scalar
    /// @param s the scalar value
    virtual Ivi& operator *= (const long s);

    /// @return true if the vector is nil
    bool isnil (void) const override;
    
    /// copy a vector into this one
    /// @param x the vector to copy
    virtual Ivi& cpy (const Ivi& x);

    /// compare a vector value
    /// @param pos the vector position
    /// @param val the value to compare
    virtual bool cmp (const t_long pos, const long val) const;

    /// set a vector by value
    /// @param val the value to set
    virtual void set (const long val);

    /// set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void set (const t_long pos, const long val);

    /// get a vector value by position
    /// @param pos the vector position
    virtual long get (const t_long pos) const;

    /// add a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    virtual Ivi& add (const Ivi& x, const long s);

    /// add a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Ivi& add (const Ivi& x, const Ivi& y);

    /// add a vector with another scaled one
    /// @param x the vector argument
    /// @param y the vector argument
    /// @param s the scalar factor
    virtual Ivi& add (const Ivi& x, const Ivi& y, const long s);

    /// substract a vector with a scalar
    /// @param x the vector argument
    /// @param s the scalar factor
    virtual Ivi& sub (const Ivi& x, const long s);

    /// substract a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Ivi& sub (const Ivi& x, const Ivi& y);

    /// multiply a vector with a scaled vector
    /// @param x the vector to multiply
    /// @param s the scaling factor
    virtual Ivi& mul (const Ivi& x, const long s);

    /// multiply a vector with another one
    /// @param x the vector argument
    /// @param y the vector argument
    virtual Ivi& mul (const Ivi& x, const Ivi& y);

    /// add equal with a vector
    /// @param x the vector to add
    virtual Ivi& aeq (const Ivi& x);

    /// add equal with a scaled vector
    /// @param x the vector to add
    /// @param s the scaling factor
    virtual Ivi& aeq (const Ivi& x, const long s);

    /// rescale equal with a vector
    /// @param x the vector to add
    /// @param s the scaling factor
    virtual Ivi& req (const Ivi& x, const long s);

    /// permutate this vector
    /// @param p the permutation object
    virtual Ivi* permutate (const Cpi& p) const;

    /// reverse this vector permutation
    /// @param p the permutation object
    virtual Ivi* reverse (const Cpi& p) const;

  public:
    /// no lock - set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void nlset (const t_long pos, const long val) =0;
    
    /// no lock - get a vector value by position
    /// @param pos the vector position
    virtual long nlget (const t_long pos) const =0;

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
