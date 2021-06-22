// ---------------------------------------------------------------------------
// - TlsHeader.hpp                                                           -
// - afnix:tls service - tls record header class definition                  -
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

#ifndef  AFNIX_TLSHEADER_HPP
#define  AFNIX_TLSHEADER_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_TLSBLOCK_HPP
#include "TlsBlock.hpp"
#endif

namespace afnix {

  /// The TlsHeader class is the base class for the tls protocol. A header
  /// record is defined by a two byte version and the record length. What
  /// happen next is up to the content decoder. Note that the record type
  /// which is the first byte of the record message does not have to be 
  /// stored since it obtained directly from the derived class.
  /// @author amaury darsch

  class TlsHeader : public TlsBlock {
  public:
    /// @return the header info as a plist
    Plist getinfo (void) const;

    /// @return the header major version
    virtual t_byte getmajor (void) const =0;

    /// @return the header minor version
    virtual t_byte getminor (void) const =0;

    /// @return the version as a string
    virtual String toversion (void) const;

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
