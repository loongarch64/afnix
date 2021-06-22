// ---------------------------------------------------------------------------
// - Nai.hpp                                                                 -
// - afnix:mth module - numeral array interface definitions                  -
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

#ifndef  AFNIX_NAI_HPP
#define  AFNIX_NAI_HPP

#ifndef  AFNIX_AAI_HPP
#include "Aai.hpp"
#endif

#ifndef  AFNIX_NUMERAL_HPP
#include "Numeral.hpp"
#endif

namespace afnix {

  /// This Nai class is an abstract class that models the behavior of a
  /// numeral based array. The class extends the abstract array interface
  /// with specific numeral methods.
  /// @author amaury darsch

  class Nai : public Aai {
  protected:
    /// the numeral type
    Numeral::t_numt d_numt;
    
  public:
    /// create a null array
    Nai (void);

    /// create an array by size
    /// @param size the array size
    Nai (const t_long size);

    /// create an array by size
    /// @param size the array size
    /// @param strd the array stride
    Nai (const t_long size, const long strd);

    /// create a array by stride and type
    /// @param strd the array stride
    /// @param numt the numeral type
    Nai (const long strd, const Numeral::t_numt numt);

    /// create a array by size and type
    /// @param size the array size
    /// @param strd the array stride
    /// @param numt the numeral type
    Nai (const t_long size, const long strd, const Numeral::t_numt numt);

    /// copy construct this array
    /// @param that the array to copy
    Nai (const Nai& that);

    /// copy move this array
    /// @param that the array to move
    Nai (Nai&& that) noexcept;

    /// assign a array into this one
    /// @param that the array to assign
    Nai& operator = (const Nai& that);

    /// move a array into this one
    /// @param that the array to move
    Nai& operator = (Nai&& that) noexcept;
    
    /// serialize this object
    /// @param os the output stream
    void wrstream (OutputStream& os) const override;
    
    /// deserialize this object
    /// @param is the input stream
    void rdstream (InputStream& os) override;

    /// reset this array
    void reset (void) override;
    
    /// clear this array
    void clear (void) override;

    /// set an array by position
    /// @param aidx the array index
    /// @param sidx the stride index
    /// @param nval the value to set
    virtual void set (const t_long aidx, const long sidx, const Numeral& nval);

    /// get an array value by position
    /// @param aidx the array index
    /// @param sidx the stride index
    virtual Numeral get (const t_long aidx, const long sidx) const;

    /// get an array integer value by position
    /// @param aidx the array index
    /// @param sidx the stride index
    virtual t_long getlong (const t_long aidx, const long sidx) const;

    /// get a array real value by position
    /// @param aidx the array index
    /// @param sidx the stride index
    virtual t_real getreal (const t_long aidx, const long sidx) const;

  public:
    /// no lock - set a array by position
    /// @param aidx the array index
    /// @param sidx the stride index
    /// @param nval the value to set
    virtual void nlset (const t_long aidx, const long sidx, 
			const Numeral& nval) =0;

    /// no lock - get a array value by position
    /// @param aidx the array index
    /// @param sidx the stride index
    virtual Numeral nlget (const t_long aidx, const long sidx) const =0;

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
