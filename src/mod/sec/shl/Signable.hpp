// ---------------------------------------------------------------------------
// - Signable.hpp                                                            -
// - afnix:sec module - abstract signable object class definition            -
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

#ifndef  AFNIX_SIGNABLE_HPP
#define  AFNIX_SIGNABLE_HPP

#ifndef  AFNIX_KEY_HPP
#include "Key.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The Signable class is an abstract class which provides the interface
  /// a signable object. Such object must implement the tostream method as
  /// well as the sign and verify methods.
  /// @author amaury darsch

  class Signable : public virtual Object {
  public:
    /// create a default signable
    Signable (void) =default;
    
    /// copy move this signable object
    /// @param that the signable to move
    Signable (Signable&& that) noexcept;

    /// move a signable into this one
    /// @param that the signable to move
    Signable& operator = (Signable&& that) noexcept;

    /// serialize the signable object
    /// @param os the output stream
    virtual void wrsign (OutputStream& os) const =0;

    /// sign an object by key
    /// @param skey the signing key
    virtual bool sign (const Key& skey) =0;

    /// verify an object by key
    virtual bool verify (void) const =0;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
