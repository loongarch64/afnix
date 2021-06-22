// ---------------------------------------------------------------------------
// - Datagram.hpp                                                            -
// - afnix:net module - datagram class definition                            -
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

#ifndef  AFNIX_DATAGRAM_HPP
#define  AFNIX_DATAGRAM_HPP

#ifndef  AFNIX_SOCKET_HPP
#include "Socket.hpp"
#endif

namespace afnix {

  /// The Datagram class is a special class used to store a datagram received
  /// by a udp socket. The datagram holds the read buffer, the server address
  /// and port and the datagram source address and port (or peer).
  /// @author amaury darsch

  class Datagram : public Socket {
  private:
    /// the datagram address
    t_byte* p_daddr;
    /// the datagram port
    t_word  d_dport;

  public:
    /// create a datagram object. 
    /// @param sid   the udp  socket
    /// @param dport the datagram port
    /// @param daddr the datagram address
    /// @param the datagram buffer
    /// @param the datagram size
    Datagram (const int sid, t_word dport, const t_byte* daddr,
	      const char* buf, const long size);

    /// destroy this datagram
    ~Datagram (void);

    /// @return the class name
    String repr (void) const;

    /// close this datagram
    bool close (void);

    /// @return the next available character
    char read (void);

    /// write one character on the source datagram.
    /// @param value the character to write  
    long write (const char value);

    /// write a character string to the source datagram
    /// @param data the data to write
    long write (const char* data);

    /// write a character array in a udp message
    /// @param rbuf the buffer to write
    /// @param size the number of characters
    long write (const char* rbuf, const long size);

    /// @return true if we are at the eos
    bool iseos (void) const;
    
    /// check if we can read some character
    bool valid (void) const;

    /// @return the peer address
    Address* getpeeraddr (void) const;

    /// @return the peer port
    t_word getpeerport (void) const;

    /// @retutn the socket protocol
    String getprotocol (void) const;

  private:
    // make the copy constructor private
    Datagram (const Datagram&);
    // make the assignment operator private
    Datagram& operator = (const Datagram&);
  };
}

#endif
