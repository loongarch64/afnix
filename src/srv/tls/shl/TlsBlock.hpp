// ---------------------------------------------------------------------------
// - TlsBlock.hpp                                                            -
// - afnix:tls service - tls block class definition                          -
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

#ifndef  AFNIX_TLSBLOCK_HPP
#define  AFNIX_TLSBLOCK_HPP

#ifndef  AFNIX_TLSINFOS_HPP
#include "TlsInfos.hpp"
#endif

namespace afnix {

  /// The TlsBlock class is an abstract class for the tls protocol. A block
  /// is defined by type and length. A byte accessor method is also provided.
  /// @author amaury darsch

  class TlsBlock : public TlsInfos {
  public:
    /// @return the block info as a plist
    Plist getinfo (void) const;

    /// @return the block type
    virtual t_byte gettype (void) const =0;

    /// @return the block length
    virtual long length (void) const =0;

    /// get a block byte by position
    /// @param pos the block position
    virtual t_byte getbyte (const long pos) const =0;

    /// map the block as a buffer
    virtual Buffer tobuffer (void) const;
    
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
