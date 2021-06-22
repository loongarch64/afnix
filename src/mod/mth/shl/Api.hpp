// ---------------------------------------------------------------------------
// - Api.hpp                                                                 -
// - afnix:mth module - abstract point interface definitions                 -
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

#ifndef  AFNIX_API_HPP
#define  AFNIX_API_HPP

#ifndef  AFNIX_SERIAL_HPP
#include "Serial.hpp"
#endif

namespace afnix {

  /// This Api class is an abstract class that models the behavior of a
  /// typed point. The class defines the point size.
  /// @author amaury darsch

  class Api : public virtual Serial {
  protected:
    /// the point size
    t_long d_size;

  public:
    /// create a null point
    Api (void);

    /// create a point by size
    /// @param size the point size
    Api (const t_long size);

    /// copy construct this point
    /// @param that the point to copy
    Api (const Api& that);

    /// copy move this point
    /// @param that the point to move
    Api (Api&& that) noexcept;

    /// assign a point into this one
    /// @param that the point to assign
    Api& operator = (const Api& that);

    /// move a point into this one
    /// @param that the point to move
    Api& operator = (Api&& that) noexcept;

    /// @return the point size
    virtual t_long getsize (void) const;

    /// reset this point
    virtual void reset (void) =0;

    /// clear this point
    virtual void clear (void) =0;

    /// resize this point
    /// @param size the new point size
    virtual void resize (const t_long size) =0;

    /// @return true if the point is nil
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
