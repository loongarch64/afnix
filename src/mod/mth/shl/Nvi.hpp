// ---------------------------------------------------------------------------
// - Nvi.hpp                                                                 -
// - afnix:mth module - numeral vector interface definitions                 -
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

#ifndef  AFNIX_NVI_HPP
#define  AFNIX_NVI_HPP

#ifndef  AFNIX_AVI_HPP
#include "Avi.hpp"
#endif

#ifndef  AFNIX_NUMERAL_HPP
#include "Numeral.hpp"
#endif

namespace afnix {

  /// This Nvi class is an abstract class that models the behavior of a
  /// numeral based vector. The class extends the abstract vector interface
  /// with specific numeral methods.
  /// @author amaury darsch

  class Nvi : public Avi {
  protected:
    /// the numeral type
    Numeral::t_numt d_numt;

  public:
    /// create a null vector
    Nvi (void);

    /// create a vector by size
    /// @param size the vector size
    Nvi (const t_long size);

    /// create a vector by size and type
    /// @param size the vector size
    /// @param numt the numeral type
    Nvi (const t_long size, const Numeral::t_numt numt);

    /// copy construct this vector
    /// @param that the vector to copy
    Nvi (const Nvi& that);

    /// copy move this vector
    /// @param that the vector to move
    Nvi (Nvi&& that) noexcept;

    /// assign a vector into this one
    /// @param that the vector to assign
    Nvi& operator = (const Nvi& that);

    /// move a vector into this one
    /// @param that the vector to move
    Nvi& operator = (Nvi&& that) noexcept;

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

    /// set the vector by numeral value
    /// @param val the value to set
    virtual void set (const Numeral& val);

    /// set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void set (const t_long pos, const Numeral& val);

    /// get a vector value by position
    /// @param pos the vector position
    virtual Numeral get (const t_long pos) const;

    /// get a vector integer value by position
    /// @param pos the vector position
    virtual t_long getlong (const t_long pos) const;

    /// get a vector real value by position
    /// @param pos the vector position
    virtual t_real getreal (const t_long pos) const;

    /// convert a numeral vector by type
    /// @param numt the target type
    virtual Numeral::t_numt convert (const Numeral::t_numt numt) =0;
    
  public:
    /// no lock - set a vector by position
    /// @param pos the vector position
    /// @param val the value to set
    virtual void nlset (const t_long pos, const Numeral& val) =0;

    /// no lock - get a vector value by position
    /// @param pos the vector position
    virtual Numeral nlget (const t_long pos) const =0;

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
