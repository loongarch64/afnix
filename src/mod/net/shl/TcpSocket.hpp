// ---------------------------------------------------------------------------
// - TcpSocket.hpp                                                           -
// - afnix:net module - tcp socket class definition                          -
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

#ifndef  AFNIX_TCPSOCKET_HPP
#define  AFNIX_TCPSOCKET_HPP

#ifndef  AFNIX_SOCKET_HPP
#include "Socket.hpp"
#endif

namespace afnix {

  /// The TcpSocket class is a base class for the tcp ip protocol. When a 
  /// tcp socket is created, the base socket methods can be called to
  /// perform its setup. The standard flow control i/o methods are
  /// implemented here. Note that a tcp server returns such socket after
  /// a call to accept.
  /// @author amaury darsch

  class TcpSocket : public Socket {    
  public:
    /// create a default tcp socket. 
    TcpSocket (void);

    /// create a tcp socket by id
    /// @param sid the tcp socket id
    TcpSocket (const int sid);

    /// create a tcp socket by flag
    /// @param cflg the create flag
    TcpSocket (const bool cflg);

    /// @return the class name
    String repr (void) const override;
    
    /// @return true if we are at the eos
    bool iseos (void) const override;
    
    /// check if we can read one character
    bool valid (void) const override;

    /// @return the next available character
    char read (void) override;

    /// copy the tcp socket into a buffer
    /// @param rbuf the reference buffer
    /// @param size the buffer size
    long copy (char* rbuf, const long size) override;

    /// write one character on the socket.
    /// @param value the character to write  
    long write (const char value) override;

    /// write a character string to the socket
    /// @param data the data to write
    long write (const char* data) override;

    /// write a character array to the socket
    /// @param rbuf the reference buffer to write
    /// @param size the number of character to write
    long write (const char* rbuf, const long size) override;

    /// @return the input stream channel
    InputStream* getis (void) override;

    /// @return the output stream channel
    OutputStream* getos (void) override;

    /// @retutn the socket protocol
    String getprotocol (void) const override;

    /// create a new default socket 
    virtual void create (void);

    /// create a new socket by address familly
    /// @param addr the reference address
    virtual void create (const Address& addr);

    /// set the socket keepalive parameters
    /// @param sid  the socket id
    /// @param kidl the keepalive idle
    /// @param kcnt the keepalive counter
    /// @param kitv the keepalive interval
    virtual bool setkalv (const long kidl, const long kcnt, const long kitv);

    /// reset the socket keepalive parameters
    /// @param sid  the socket id
    virtual void rstkalv (void);
    
  private:
    // make the copy construTctor private
    TcpSocket (const TcpSocket&);
    // make the assignment operator private
    TcpSocket& operator = (const TcpSocket&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
    
    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
