// ---------------------------------------------------------------------------
// - TcpClient.hpp                                                           -
// - afnix:net module - tcp client socket class definition                   -
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

#ifndef  AFNIX_TCPCLIENT_HPP
#define  AFNIX_TCPCLIENT_HPP

#ifndef  AFNIX_TCPSOCKET_HPP
#include "TcpSocket.hpp"
#endif

#ifndef  AFNIX_CONNECTABLE_HPP
#include "Connectable.hpp"
#endif

namespace afnix {

  /// The TcpClient class is a socket class used by a client object. A Tcp
  /// client is created by specifiying the server address and port. Once the
  /// client socket is created, the read and write methods can be used.
  /// @author amaury darsch

  class TcpClient : public TcpSocket, public Connectable {
  public:
    /// create a tcp client by parameters
    /// @param prms the socket parameters
    TcpClient (const SockParams& prms);
    
    /// create a tcp client and connect to the specified host.
    /// @param host the host name to connect
    /// @param port the port to listen
    TcpClient (const String& host, const t_word port);

    /// create a tcp client and connect to the specified host.
    /// @param addr the ip address to connect
    /// @param port the port to listen
    TcpClient (const Address& addr, const t_word port);

    /// @return the class name
    String repr (void) const override;

    /// connect this client by parameters
    /// @param prms the socket parameters
    bool connect (const SockParams& prms) override;
    
    /// connect this client by host and port
    /// @param host the host name to connect
    /// @param port the port to listen
    bool connect (const String& host, const t_word port) override;

    /// connect this client by address and port
    /// @param addr the ip address to connect
    /// @param port the port to listen
    bool connect (const Address& addr, const t_word port) override;
    
  private:
    // make the copy constructor private
    TcpClient (const TcpClient&);
    // make the assignment operator private
    TcpClient& operator = (const TcpClient&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
