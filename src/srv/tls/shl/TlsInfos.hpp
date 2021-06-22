// ---------------------------------------------------------------------------
// - TlsInfos.hpp                                                            -
// - afnix:tls service - tls info list base class definition                 -
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

#ifndef  AFNIX_TLSINFOS_HPP
#define  AFNIX_TLSINFOS_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The TlsInfos class is an abstract class for the tls protocol. A tls
  /// info object defines the 'reset' and 'getinfo' methods.
  /// @author amaury darsch

  class TlsInfos : public virtual Object {
  public:
    /// create a default tls infos
    TlsInfos (void) =default;
    
    /// copy move this tls infos
    /// @param that the tls infos to move
    TlsInfos (TlsInfos&& that) noexcept;

    /// move an tls infos into this one
    /// @param that the tls infos to move
    TlsInfos& operator = (TlsInfos&& that) noexcept;

    /// reset this object
    virtual void reset (void) =0;

    /// @return the info as a plist
    virtual Plist getinfo (void) const =0;

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
