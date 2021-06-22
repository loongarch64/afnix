// ---------------------------------------------------------------------------
// - Multicast.hpp                                                           -
// - afnix:net module - multicast udp socket class definition                -
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

#ifndef  AFNIX_MULTICAST_HPP
#define  AFNIX_MULTICAST_HPP

#ifndef  AFNIX_UDPSOCKET_HPP
#include "UdpSocket.hpp"
#endif

namespace afnix {

  /// The Multicast class is a socket class used to create a multicast
  /// udp socket. The difference with a simple udp client, is that the 
  /// socket is bound on the port and a multicast join operation is
  /// performed. All other methods of the udp socket are available.
  /// @author amaury darsch

  class Multicast : public UdpSocket {
  public:
    /// create a multicast udp socketind host 
    /// @param host the host name to connect
    Multicast (const String& host);

    /// create a multicast udp socket by address
    /// @param addr the ip address to connect
    Multicast (const Address& addr);

    /// create a multicast udp socket by host and port
    /// @param host the host name to connect
    /// @param port the port to listen
    Multicast (const String& host, t_word port);

    /// create a multicast udp socketby address and port
    /// @param addr the ip address to connect
    /// @param port the port to listen
    Multicast (const Address& addr, t_word port);

    /// destroy this multicast client
    ~Multicast (void);

    /// @return the class name
    String repr (void) const;

    /// close this multicast client
    bool close (void);

  private:
    // make the copy constructor private
    Multicast (const Multicast&);
    // make the assignment operator private
    Multicast& operator = (const Multicast&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
