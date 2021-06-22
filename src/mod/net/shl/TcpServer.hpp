// ---------------------------------------------------------------------------
// - TcpServer.hpp                                                           -
// - afnix:net module - tcp server socket class definition                   -
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

#ifndef  AFNIX_TCPSERVER_HPP
#define  AFNIX_TCPSERVER_HPP

#ifndef  AFNIX_TCPSOCKET_HPP
#include "TcpSocket.hpp"
#endif

namespace afnix {

  /// The TcpServer class is an object class used to create a tcp server
  /// object. A server object is created on the local host by specifiying
  /// the port to listen and eventually the backlog number. In the case of
  /// multi-homed host, the server address can also be specified. 
  /// The "accept" method can be used to get a socket object when a
  /// connection is established.
  /// @author amaury darsch

  class TcpServer : public TcpSocket {
  public:
    /// create a default tcp server on a ephemeral port
    TcpServer (void);

    /// create a tcp server by init flag
    /// @param cflg the create flag
    TcpServer (const bool cflg);
    
    /// create a tcp server at specified port
    /// @param port the port to listen
    TcpServer (const t_word port);

    /// create a tcp server by parameters
    /// @param prms the socket parameters
    TcpServer (const SockParams& prms);
    
    /// create a tcp server with a host and port
    /// @param host the host to bind the server
    /// @param port the port to listen
    TcpServer (const String& host, const t_word port);

    /// create a tcp server with an address and port
    /// @param addr the address to bind the server
    /// @param port the port to listen
    TcpServer (const Address& addr, const t_word port);

    /// @return the class name
    String repr (void) const;

    /// listen on this tcp socket
    /// @param backlog the number of incoming connection
    virtual bool listen (const long backlog) const;

    /// @return an accepted connected tcp socket
    virtual SocketStream* accept (void) const;

  private:
    // make the copy constructor private
    TcpServer (const TcpServer&);
    // make the assignment operator private
    TcpServer& operator = (const TcpServer&);

  public:
    /// create a new object ina generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

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
