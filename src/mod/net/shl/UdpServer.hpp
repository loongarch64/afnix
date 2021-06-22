// ---------------------------------------------------------------------------
// - UdpServer.hpp                                                           -
// - afnix:net module - udp server socket class definition                   -
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

#ifndef  AFNIX_UDPSERVER_HPP
#define  AFNIX_UDPSERVER_HPP

#ifndef  AFNIX_UDPSOCKET_HPP
#include "UdpSocket.hpp"
#endif

namespace afnix {

  /// The UdpServer class is a socket class used to create a udp server
  /// object. A server object is created on the local host by specifiying
  /// the port to listen and eventually the backlog number. In the case of
  /// multi-homed host, the server address can also be specified.
  /// @author amaury darsch

  class UdpServer : public UdpSocket {
  public:
    /// create a udp server on an ephemeral port
    UdpServer (void);

    /// create a udp server at specified port
    /// @param port the port to listen
    UdpServer (t_word port);

    /// create a udp server by parameters
    /// @param prms the socket parameters
    UdpServer (const SockParams& prms);
    
    /// create a udp server with a host and port
    /// @param host the host to bind the server
    /// @param port the port to listen
    UdpServer (const String& host, t_word port);

    /// create a udp server with an address and port
    /// @param addr the address to bind the server
    /// @param port the port to listen
    UdpServer (const Address& addr, t_word port);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    UdpServer (const UdpServer&);
    // make the assignment operator private
    UdpServer& operator = (const UdpServer&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
