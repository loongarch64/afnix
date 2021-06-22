// ---------------------------------------------------------------------------
// - Aai.hpp                                                                 -
// - afnix:mth module - abstact array interface definitions                  -
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

#ifndef  AFNIX_AAI_HPP
#define  AFNIX_AAI_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

namespace afnix {

  /// This Aai class is an abstract class that models the behavior of a
  /// typed array. Unlike a block, an array is a collection of typed values
  /// organised by lines.
  /// @author amaury darsch

  class Aai : public virtual Serial {
  public:
    /// the array size
    t_long d_size;
    /// the array stride
    long d_strd;

  public:
    /// create a null array
    Aai (void);

    /// create an array by size
    /// @param size the array size
    Aai (const t_long size);

    /// create a array by size and stride
    /// @param size the array size
    /// @param strd the array stride
    Aai (const t_long size, const long strd);

    /// copy construct this array
    /// @param that the array to copy
    Aai (const Aai& that);

    /// copy move this array
    /// @param that the array to move
    Aai (Aai&& that) noexcept;

    /// assign a array into this one
    /// @param that the array to assign
    Aai& operator = (const Aai& that);

    /// move a array into this one
    /// @param that the array to move
    Aai& operator = (Aai&& that) noexcept;
    
    /// @return the array size
    virtual t_long getsize (void) const;

    /// @return the array stride
    virtual long getstrd (void) const;

    /// reset this array
    virtual void reset (void) =0;
    
    /// clear this array
    virtual void clear (void) =0;
    
    /// resize this array
    /// @param size the new array size
    virtual void resize (const t_long size) =0;

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
