// ---------------------------------------------------------------------------
// - Socket.hpp                                                              -
// - afnix:net module - socket class definition                              -
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

#ifndef  AFNIX_SOCKETSTREAM_HPP
#define  AFNIX_SOCKETSTREAM_HPP

#ifndef  AFNIX_ADDRESS_HPP
#include "Address.hpp"
#endif

#ifndef  AFNIX_DUPLEIXSTREAM_HPP
#include "DupleixStream.hpp"
#endif


namespace afnix {

  /// The SocketStream class is an abstract base class for the socket class
  /// which extends the dupleix stream with the socket information methods.
  /// When it comes to create a socket, the control methods needs also to be
  /// added. Note that the socket options are not present here.
  /// @author amaury darsch

  class SocketStream : public DupleixStream {    
  public:
    /// create a default socket stream
    SocketStream (void) =default;
    
    /// @return true if we have an ipv6 socket
    virtual bool isipv6 (void) const =0;

    /// @return true if we can broadcast messages
    virtual bool isbcast (void) const =0;

    /// @return true if the socket is open
    virtual bool isopen (void) const =0;

    /// @return the socket address
    virtual Address* getsockaddr (void) const =0;

    /// @return the socket port
    virtual t_word getsockport (void) const =0;

    /// @return the socket authority
    virtual String getsockauth (void) const =0;

    /// @return the peer address
    virtual Address* getpeeraddr (void) const =0;

    /// @return the peer port
    virtual t_word getpeerport (void) const =0;

    /// @return the peer authority
    virtual String getpeerauth (void) const =0;

    /// @return the socket protocol
    virtual String getprotocol (void) const =0;

  private:
    // make the copy constructor private
    SocketStream (const SocketStream&) =delete;
    // make the assignment operator private
    SocketStream& operator = (const SocketStream&) =delete;

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
