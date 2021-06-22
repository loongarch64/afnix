// ---------------------------------------------------------------------------
// - Connectable.hpp                                                         -
// - afnix:net module - connectable abstract class definition                -
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

#ifndef  AFNIX_CONNECTABLE_HPP
#define  AFNIX_CONNECTABLE_HPP

#ifndef  AFNIX_SOCKPARAMS_HPP
#include "SockParams.hpp"
#endif

namespace afnix {

  /// The Connectable class is an abstract class for connectable socket. The
  /// object can be connected by host/port or address/port. Furthermore, the
  /// class is used in the autocom object for automatic socket connection.
  /// @author amaury darsch

  class Connectable : public virtual Object {
  public:
    /// create a default connectable
    Connectable (void) =default;
    
    /// copy move this connectable object
    /// @param that the connectable to move
    Connectable (Connectable&& that) noexcept;

    /// move a connectable into this one
    /// @param that the conneactable to move
    Connectable& operator = (Connectable&& that) noexcept;

    /// connect a socket by parameters
    /// @param prms the socket parameters
    virtual bool connect (const SockParams& prms) =0;
    
    /// connect a socket by host and port
    /// @param host the host name to connect
    /// @param port the port to listen
    virtual bool connect (const String& host, const t_word port) =0;

    /// connect a socket by address and port
    /// @param addr the ip address to connect
    /// @param port the port to listen
    virtual bool connect (const Address& addr, const t_word port) =0;
  };
}

#endif
