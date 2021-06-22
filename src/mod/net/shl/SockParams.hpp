// ---------------------------------------------------------------------------
// - SockParams.hpp                                                          gg-
// - afnix:net module - socket parameters class definition                   -
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

#ifndef  AFNIX_SOCKPARAMS_HPP
#define  AFNIX_SOCKPARAMS_HPP

#ifndef  AFNIX_SOCKOPT_HPP
#include "Sockopt.hpp"
#endif

#ifndef  AFNIX_ADDRESS_HPP
#include "Address.hpp"
#endif

namespace afnix {

  /// The SockParams class is the the socket parameters class. The class
  /// defines the socket options as well as other parameters which can be
  /// to create a socket.
  /// @author amaury darsch

  class SockParams : public Sockopt {
  private:
    /// the host address
    Address d_addr;
    /// the host port
    t_word d_port;
    /// enable address reuse
    bool d_radr;
    /// enable port reuse
    bool d_rprt;
    /// enable broadcast
    bool d_bcst;
    /// bypass routing table
    bool d_dntr;
    /// connection keepalive
    bool d_kliv;
    /// connection linger time
    long d_ligt;
    /// receive buffer size
    long d_rsiz;
    /// send buffer size
    long d_ssiz;
    /// socket hop limit
    long d_shop;
    /// multicast use loopback
    bool d_mlbk;
    /// multicast hop limit
    long d_mhop;
    /// tcp maximum segment size
    long d_msiz;
    /// disable naggle algorithm
    bool d_ndly;
    /// the server backlog
    long d_blog;

  public:
    /// create a default parameters
    SockParams (void);

    /// create a socket parameter by host and port
    /// @param host the host name
    /// @param port the port to bind
    SockParams (const String& host, const t_word port);

    /// create a socket parameter by host and port and familly
    /// @param host the host name
    /// @param port the port to bind
    /// @param sapf the address family
    SockParams (const String& host, const t_word port, const String& sapf);

    /// create a socket parameter by address and port
    /// @param addr the host address
    /// @param port the port to bind
    SockParams (const Address& addr, const t_word port);
    
    /// copy construct this object
    /// @param that the object to copy
    SockParams (const SockParams& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign an object to this one
    /// @param that the address to assign
    SockParams& operator = (const SockParams& that);

    /// set a boolean socket option
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
      
    /// set the address parameter
    /// @param addr the address to set
    virtual void setaddr (const Address& addr);

    /// set the host parameter
    /// @param host the host to set
    virtual void sethost (const String& host);

    /// @return the address parameter
    virtual Address getaddr (void) const;

    /// set the port parameter
    /// @param port the port to set
    virtual void setport (const t_word port);

    /// @return the port parameter
    virtual t_word getport (void) const;

    /// set the backlog parameter
    /// @param blog the backlog to set
    virtual void setblog (const long blog);
    
    /// @return the backlog parameter
    virtual long getblog (void) const;
    
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
