// ---------------------------------------------------------------------------
// - Avi.hpp                                                                 -
// - afnix:mth module - abstract vector interface definitions                -
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

#ifndef  AFNIX_AVI_HPP
#define  AFNIX_AVI_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

namespace afnix {

  /// This Avi class is an abstract class that models the behavior of a
  /// typed vector. The class defines the vector size.
  /// @author amaury darsch

  class Avi : public virtual Serial {
  protected:
    /// the vector size
    t_long d_size;

  public:
    /// create a null vector
    Avi (void);

    /// create a vector by size
    /// @param size the vector size
    Avi (const t_long size);

    /// copy construct this vector
    /// @param that the vector to copy
    Avi (const Avi& that);

    /// copy move this vector
    /// @param that the vector to move
    Avi (Avi&& that) noexcept;

    /// assign a vector into this one
    /// @param that the vector to assign
    Avi& operator = (const Avi& that);

    /// move a vector into this one
    /// @param that the vector to move
    Avi& operator = (Avi&& that) noexcept;

    /// @return the vector size
    virtual t_long getsize (void) const;

    /// reset this vector
    virtual void reset (void) =0;

    /// clear this vector
    virtual void clear (void) =0;

    /// preset this vector
    virtual void preset (void) =0;

    /// resize this vector
    /// @param size the new vector size
    virtual void resize (const t_long size) =0;

    /// @return true if the vector is nil
    virtual bool isnil (void) const =0;
    
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
