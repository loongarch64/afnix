// ---------------------------------------------------------------------------
// - UdpSocket.hpp                                                           -
// - afnix:net module - udp socket class definition                          -
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

#ifndef  AFNIX_UDPSOCKET_HPP
#define  AFNIX_UDPSOCKET_HPP

#ifndef  AFNIX_DATAGRAM_HPP
#include "Datagram.hpp"
#endif

namespace afnix {

  /// The UdpSocket class is a base class for the udp ip protocol. When a 
  /// udp socket is created, the base socket methods can be called to
  /// perform its setup. The standard i/o methods are implemented here. 
  /// The class implements also the accept method which can be used to get
  /// a datagram.
  /// @author amaury darsch

  class UdpSocket : public Socket {
  private:
    /// the receiving buffer
    char* p_buf;

  protected:
    /// the source address
    Address d_addr;
    /// the source port
    t_word  d_port;

  public:
    /// create a default udp socket
    UdpSocket (void);

    /// create a udp socket by id
    /// @param sid the socket id
    UdpSocket (const int sid);

    /// create a udp socket by flag
    /// @param cflg the create flag
    UdpSocket (const bool cflg);

    /// destroy this udp socket
    ~UdpSocket (void);

    /// @return the class name
    String repr (void) const;

    /// @return true if we can broadcast messages
    bool isbcast (void) const;

    /// join a multicast group by address
    /// @param addr the multicast address
    bool join (const Address& addr);

    /// drop from group by address
    /// @param addr the multicast address
    bool drop (const Address& addr);

    /// read one character from a udp message
    char read (void);

    /// read a buffer by size from a udp message
    /// @param size the number of character to read
    Buffer* read (const long size);

    /// write one character in a udp message
    /// @param value the character to send
    long write (const char value);

    /// write a buffer character in a udp message
    /// @param data the buffer to send
    long write (const char* data);

    /// write a character array in a udp message
    /// @param rbuf the buffer to write
    /// @param size the number of characters
    long write (const char* rbuf, const long size);

    /// return true if we are at the eos
    bool iseos (void) const;

    /// check if we can read a character
    bool valid (void) const;

    /// @return the input stream channel
    InputStream* getis (void);

    /// @return the output stream channel
    OutputStream* getos (void);
    
    /// @retutn the socket protocol
    String getprotocol (void) const;

    /// create a new socket by address familly
    /// @param addr the reference address
    virtual void create (const Address& addr);

    /// @return a datagram from this udp socket
    virtual Datagram* accept (void) const;

  private:
    // make the copy constructor private
    UdpSocket (const UdpSocket&);
    // make the assignment operator private
    UdpSocket& operator = (const UdpSocket&);

  public:
    /// create a new object in a generic way
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
