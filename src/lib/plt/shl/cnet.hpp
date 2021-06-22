// ---------------------------------------------------------------------------
// - cnet.hpp                                                                -
// - standard platform library - c network native function definition        -
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

#ifndef  AFNIX_CNET_HPP
#define  AFNIX_CNET_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the functions that manipulate network connection.
  /// The interface is designed to be platform independant but is largely
  /// inspired by the TCP/IP socket model. The address structure is designed
  /// to work with both IPV4 and IPV6 model.
  /// @author amaury darsch

  /// the shutdown mode
  enum t_shut {
    SHUT_BOTH, // shutdown both directions
    SHUT_RECV, // shutdown reception
    SHUT_SEND  // shutdown emission
  };

  /// the supported socket options
  enum t_sopt {
    SOPT_RADR, // reuse address
    SOPT_RPRT, // reuse address
    SOPT_BCST, // enable packet broadcast
    SOPT_DNTR, // bypass routing table
    SOPT_KLIV, // set connection test
    SOPT_LIGT, // set linger time
    SOPT_RSIZ, // receive buffer size
    SOPT_SSIZ, // send buffer size
    SOPT_SHOP, // socket hop limit
    SOPT_MLBK, // multicast use loopback
    SOPT_MHOP, // multicast hop limit
    SOPT_MSIZ, // tcp maximum segment size
    SOPT_NDLY  // disable naggle algorithm
  };

  /// the address family
  enum t_iapf {
    IAPF_XANY, // any family
    IAPF_IPV4, // IP4 address
    IAPF_IPV6  // IP6 address
  };
  
  /// the ip address structure
  struct s_ipaddr {
    /// the vector size
    long  d_size;
    /// the vector name
    char**   p_name;
    /// the vector address
    t_byte** p_addr;
    // initialize the structure
    s_ipaddr (const long size) {
      d_size = size;
      p_name = new char*  [size];
      p_addr = new t_byte*[size];
    }
    // destroy this structure
    ~s_ipaddr (void) {
      for (long i = 0; i < d_size; i++) {
	delete [] p_name[i];
	delete [] p_addr[i];
      }
      delete [] p_name;
      delete [] p_addr;
    }
  };

  /// set a socket option
  /// @param sid the socket id
  /// @param opt the option to set
  /// @param val the option flag
  bool c_ipsetopt (const int sid, const t_sopt sopt, const bool val);

  /// get a socket option
  /// @param sid the socket id
  /// @param opt the option to set
  /// @param val the option flag
  bool c_ipgetopt (const int sid, const t_sopt sopt, bool& val);

  /// set a socket option by value
  /// @param int sid the socket id
  /// @param opt the option to set
  /// @param val the option value
  bool c_ipsetopt (const int sid, const t_sopt opt, const long val);

  /// get a socket option by value
  /// @param sid the socket id
  /// @param opt the option to set
  /// @param val the option value
  bool c_ipgetopt (const int sid, const t_sopt opt, long& val);

  /// set the socket keepalive parameters
  /// @param sid  the socket id
  /// @param kidl the keepalive idle
  /// @param kcnt the keepalive counter
  /// @param kitv the keepalive interval
  bool c_setkalv (const int sid,const int kidl,const int kcnt,const int kitv);

  /// reset the socket keepalive parameters
  /// @param sid  the socket id
  void c_rstkalv (const int sid);
  
  /// shutdown a socket connection
  /// @param sid the socket id
  /// @param how the shutdown mode
  bool c_ipshut (const int sid, const t_shut how);

  /// @return the loopback name
  const char* c_loopname (void);

  /// get an ip address record by name
  /// @param name the host name to resolve
  /// @param iapf the address protocol family
  s_ipaddr* c_getipa (const char* name, const t_iapf iapf);

  /// check for a numeric ip address
  /// @param name the host name to resolve
  /// @param iapf the address protocol family
  bool c_isipn (const char* name, const t_iapf iapf);
  
  /// get a host name by address (reverse lookup)
  /// @param addr the address to reverse
  /// @param nflg the numerical flag
  char* c_getipn (const t_byte* addr, const bool nflg);

  /// copy an address by  bytes
  /// @param addr the address to copy
  /// @return a byte array copy of the address
  t_byte* c_cpaddr (const t_byte* addr);

  /// get the next address in byte form
  /// @param addr the address to increase
  t_byte* c_nxaddr (const t_byte* addr);

  /// @return true if two addresses are equal
  /// @param addr1 the first address to check
  /// @param addr2 the second address to check
  bool c_eqaddr (const t_byte* addr1, const t_byte* addr2);

  /// @return true if an address is less than another one
  /// @param addr1 the first address to check
  /// @param addr2 the second address to check
  bool c_ltaddr (const t_byte* addr1, const t_byte* addr2);

  /// @return true if an address is less equal another one
  /// @param addr1 the first address to check
  /// @param addr2 the second address to check
  bool c_leaddr (const t_byte* addr1, const t_byte* addr2);

  /// get a service by name and flag
  /// @param name the service name to find
  /// @param flag check for udp if false, or tcp if true
  t_word c_ipserv (const char* name, bool flag);

  /// check for an ipv6 socket
  /// @param sid the socket id to check
  bool c_isipv6 (const int sid);

  /// get the socket address of a socket
  /// @param sid the socket id
  t_byte* c_ipsockaddr (const int sid);

  /// get the peer address of a socket
  /// @param sid the socket id
  t_byte* c_ippeeraddr (const int sid);

  /// get the socket port of a socket
  /// @param sid the socket id
  t_word c_ipsockport (const int sid);

  /// get the peer port of a socket
  /// @param sid the socket id
  t_word c_ippeerport (const int sid);

  /// create a tcp socket by protocol family
  /// @para, iapf the protocol family
  int c_ipsocktcp (const t_iapf iapf);

  /// create a tcp socket by address family
  /// @param addr the address reference
  int c_ipsocktcp (const t_byte* addr);

  /// create a udp socket by protocol family
  /// @param iapf the protocol family
  int c_ipsockudp (const t_iapf iapf);

  /// create a udp socket by address family
  /// @param addr the address reference
  int c_ipsockudp (const t_byte* addr);

  /// connect a socket with a server
  /// @param sid the client socket id
  /// @param port the port to connect
  /// @param dest the server address
  bool c_ipconnect (const int sid, t_word port, t_byte* dest);

  /// join a multicast group
  /// @param sid the socket id
  /// @param dest the multicast group address
  bool c_ipjoin (const int sid, t_byte* dest);

  /// drop a multicast group
  /// @param sid the socket id
  /// @param dest the multicast group address
  bool c_ipdrop (const int sid, t_byte* dest);

  /// send data to a socket
  /// @param sid the client socket
  /// @param buf the buffer of data to send
  /// @param size the buffer size
  long c_ipsend (const int sid, const char* buf, long size);

  /// send a datagram by address and port
  /// @param sid  the client socket
  /// @param port the destination port
  /// @param dest the destination address
  /// @param buf  the buffer of data to send
  /// @param size the buffer size
  long c_ipsendto (const int sid, t_word port, t_byte* dest, const char* buf, 
		   const long size);

  /// receive data from a socket
  /// @param sid the client socket
  /// @param buf the buffer to receive
  /// @param size the buffer size
  long c_iprecv (const int sid, char* buf, long size);

  /// receive a datagram on a port and update address
  /// @param sid  the client socket
  /// @param port the received port
  /// @param dest the received address
  /// @param buf  the buffer of data to receive
  /// @param size the buffer size
  long c_iprecvfr (const int sid, t_word& port, t_byte* dest, char* buf, 
		   const long size);

  /// bind and listen for a server socket
  /// @param sid the server socket
  /// @param port the port to bind
  /// @param backlog the backlog of incoming connection
  bool c_ipbind (const int sid, t_word port);

  /// bind and listen for a server socket at a certain address
  /// @param sid the server socket
  /// @param port the port to bind
  /// @param dest the server address to use
  bool c_ipbind (const int sid, t_word port, t_byte* dest);

  /// listen for a server socket
  /// @param sid the server socket
  /// @param backlog the backlog of incoming connection
  bool c_iplisten (const int sid, const long backlog);

  /// accept a connection for a server socket
  /// sid the socket id
  int c_ipaccept (const int sid);
}

#endif
