// ---------------------------------------------------------------------------
// - UdpClient.hpp                                                           -
// - afnix:net module - udp client socket class definition                   -
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

#ifndef  AFNIX_UDPCLIENT_HPP
#define  AFNIX_UDPCLIENT_HPP

#ifndef  AFNIX_UDPSOCKET_HPP
#include "UdpSocket.hpp"
#endif

namespace afnix {

  /// The UdpClient class is a socket class used by a client object. A Udp
  /// client is created by specifiying the server address and port. Once the
  /// client socket is created, the socket is connected so the standard
  /// read and write methods can be used. Note that for a connected
  /// socket the accept method can also be used.
  /// @author amaury darsch

  class UdpClient : public UdpSocket {
  public:
    /// create a udp socket and connect to the specified host.
    /// @param host the host name to connect
    /// @param port the port to listen
    UdpClient (const String& host, t_word port);
    
    /// create a udp socket and connect to the specified host.
    /// @param addr the ip address to connect
    /// @param port the port to listen
    UdpClient (const Address& addr, t_word port);
    
    /// @return the class name
    String repr (void) const;
    
  private:
    // make the copy constructor private
    UdpClient (const UdpClient&);
    // make the assignment operator private
    UdpClient& operator = (const UdpClient&);
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
