// ---------------------------------------------------------------------------
// - Npi.hpp                                                                 -
// - afnix:mth module - numeral point interface definitions                  -
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

#ifndef  AFNIX_NPI_HPP
#define  AFNIX_NPI_HPP

#ifndef  AFNIX_API_HPP
#include "Api.hpp"
#endif

#ifndef  AFNIX_NUMERAL_HPP
#include "Numeral.hpp"
#endif

namespace afnix {

  /// This Npi class is an abstract class that models the behavior of a
  /// numeral based point. The class extends the abstract point interface
  /// with specific numeral methods.
  /// @author amaury darsch

  class Npi : public Api {
  protected:
    /// the numeral type
    Numeral::t_numt d_numt;

  public:
    /// create a null point
    Npi (void);

    /// create a point by size
    /// @param size the point size
    Npi (const t_long size);

    /// create a point by size and type
    /// @param size the point size
    /// @param numt the numeral type
    Npi (const t_long size, const Numeral::t_numt numt);

    /// copy construct this point
    /// @param that the point to copy
    Npi (const Npi& that);

    /// copy move this point
    /// @param that the point to move
    Npi (Npi&& that) noexcept;

    /// assign a point into this one
    /// @param that the point to assign
    Npi& operator = (const Npi& that);

    /// move a point into this one
    /// @param that the point to move
    Npi& operator = (Npi&& that) noexcept;

    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;

    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// reset this point
    void reset (void) override;

    /// clear this point
    void clear (void) override;

    /// @return true if the point is nil
    bool isnil (void) const override;
    
    /// set the point by numeral value
    /// @param val the value to set
    virtual void set (const Numeral& val);

    /// set a point by position
    /// @param pos the point position
    /// @param val the value to set
    virtual void set (const t_long pos, const Numeral& val);

    /// get a point value by position
    /// @param pos the point position
    virtual Numeral get (const t_long pos) const;

    /// get a point integer value by position
    /// @param pos the point position
    virtual t_long getlong (const t_long pos) const;

    /// get a point real value by position
    /// @param pos the point position
    virtual t_real getreal (const t_long pos) const;
    
  public:
    /// no lock - set a point by position
    /// @param pos the point position
    /// @param val the value to set
    virtual void nlset (const t_long pos, const Numeral& val) =0;

    /// no lock - get a point value by position
    /// @param pos the point position
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
