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

#ifndef  AFNIX_SOCKET_HPP
#define  AFNIX_SOCKET_HPP

#ifndef  AFNIX_SOCKPARAMS_HPP
#include "SockParams.hpp"
#endif

#ifndef  AFNIX_SOCKETSTREAM_HPP
#include "SocketStream.hpp"
#endif


namespace afnix {

  /// The Socket class is a base class for the socket server and socket
  /// client objects. A socket is composed of an address, a port number 
  /// and a stream descriptor. 
  /// @author amaury darsch

  class Socket : public SocketStream, public Sockopt {    
  protected:
    /// the socket descriptor
    int d_sid;

  public:
    /// create a default socket
    Socket (void);

    /// create a socket by id
    /// @param sid  the socket id
    Socket (const int sid);

    /// destroy this socket
    ~Socket (void);

    /// @return the class name
    String repr (void) const;

    /// close this socket
    bool close (void);

    /// @return the stream descriptor
    int getsid (void) const;

    /// @return true if we have an ipv6 socket
    bool isipv6 (void) const;

    /// @return true if we can broadcast messages
    bool isbcast (void) const;

    /// @return true if the socket is open
    bool isopen (void) const;

    /// @return the socket address
    Address* getsockaddr (void) const;

    /// @return the socket port
    t_word getsockport (void) const;

    /// @return the socket authority
    String getsockauth (void) const;

    /// @return the peer address
    Address* getpeeraddr (void) const;

    /// @return the peer port
    t_word getpeerport (void) const;

    /// @return the peer authority
    String getpeerauth (void) const;

    /// set a socket option by parameter
    /// @param prms the socket parameter
    bool setopt (const SockParams& prms);

    /// set a socket option
    /// @param opt the socket option
    /// @param val the option value
    bool setopt (const t_so opt, const bool val);

    /// set a socket option with a value
    /// @param opt the socket option
    /// @param val the value to set
    bool setopt (const t_so opt, const long val);

    /// get a socket option
    /// @param opt the socket option
    /// @param val the option value
    bool getbopt (const t_so opt) const;

    /// get a socket option with a value
    /// @param opt the socket option
    /// @param val the value to set
    long getlopt (const t_so opt) const;
    
    /// detach the stream descriptor
    virtual int detach (void);

    /// force the socket to close
    virtual bool shutdown (void);

    /// shutdown this socket. If the mode is false, further receive is not
    /// possible - if the mode is tue, further send is not possible
    /// @param mode the shutdown mode
    virtual bool shutdown (const bool mode);

    /// connect this socket by port and address
    /// @param port the port use
    /// @apram addr the address to use
    virtual bool connect (t_word port, const Address& addr);

    /// connect this socket by port to an aliases address
    /// @param port the port use
    /// @apram addr the address to use
    /// @apram alsf the alias address flag
    virtual bool connect (t_word port, const Address& addr, const bool alsf);

    /// bind this socket with a port
    /// @param port the port to bind
    virtual  bool bind (t_word port);

    /// bind this socket with a port and an address
    /// @param port the port to bind
    /// @param addr the addree to bind
    virtual bool bind (t_word port, const Address& addr);

  private:
    // make the copy constructor private
    Socket (const Socket&);
    // make the assignment operator private
    Socket& operator = (const Socket&);

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
