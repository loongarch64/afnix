// ---------------------------------------------------------------------------
// - cnet.cxx                                                                -
// - standard platform library - c network function implementation           -
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

#include "cnet.hpp"
#include "cstr.hpp"
#include "cthr.hpp"
#include "csys.hpp"
#include "cerr.hpp"
#include "cnet.hxx"

namespace afnix {
  // this function clean the memory by address and length
  static inline void mzero (void* p, t_size len) {
    if ((p == nullptr) || (len == 0)) return;
    char* ptr = (char*) p;
    for (t_size i = 0; i < len; i++) *ptr++ = nilc;
  }
}

#ifdef AFNIX_HAVE_CHAROPT
namespace afnix {
  // set a socket option
  static bool c_setsockopt (const int sid, const int level, const int optname,
			    const void* optval, unsigned int optlen) {
    int result = setsockopt (sid, level, optname, (char*) optval, optlen);
    return (result == -1) ? false : true;
  }
  // get a socket option
  static bool c_getsockopt (const int sid, const int level, const int optname,
			    void* optval, unsigned int& optlen) {
    int result = getsockopt (sid, level, optname, (char*) optval, &optlen);
    return (result == -1) ? false : true;
  }
}
#else
namespace afnix {
  // set a  socket option
  static bool c_setsockopt (const int sid, const int level, const int optname,
			    const void* optval, unsigned int optlen) {
    int result = setsockopt (sid, level, optname, optval, optlen);
    return (result == -1) ? false : true;
  }
  // get a  socket option
  static bool c_getsockopt (const int sid, const int level, const int optname,
			    void* optval, unsigned int& optlen) {
    int result = getsockopt (sid, level, optname, optval, (socklen_t*) &optlen);
    return (result == -1) ? false : true;
  }
}
#endif

#ifdef AFNIX_HAVE_IPV6
namespace afnix {
  // this function returns the address size based on the socket type
  static inline socklen_t get_addr_len (const int sid) {
    return c_isipv6 (sid) ? IP_ADDRLEN_6 : IP_ADDRLEN_4;
  }
  
  // initialize an ip address with a port and type
  static void init_addr (t_sockaddr& addr, t_word port, bool flag) {
    if (flag == true) {
      addr.addr6.sin6_family = AF_INET6;
      addr.addr6.sin6_port   = htons (port);
      for (long i = 0; i < 16; i++)
	addr.addr6.sin6_addr.s6_addr[i] = nilc;
    } else {
      addr.addr4.sin_family      = AF_INET;
      addr.addr4.sin_port        = htons (port);
      addr.addr4.sin_addr.s_addr = 0;
    }     
  }
  
  // fill an ip address from bytes
  static void byte_to_addr (t_sockaddr& addr, t_word port, const t_byte* src) {
    // check for ipv6
    if (src[0] == 16) {
      addr.addr6.sin6_family = AF_INET6;
      addr.addr6.sin6_port   = htons (port);
      for (long i = 0; i < 16; i++) 
	addr.addr6.sin6_addr.s6_addr[i] = src[i+1];
    }
    // check for ipv4
    if (src[0] == 4) {
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      for (long i = 0; i < 4; i++) value.d_bval[i] = src[i+1];
      addr.addr4.sin_family      = AF_INET;
      addr.addr4.sin_port        = htons (port);
      addr.addr4.sin_addr.s_addr = value.d_qval;
    }    
  }
  
  // map an ip address to bytes
  static t_byte* addr_to_byte (t_sockaddr& addr) {
    t_byte* result = nullptr;
    // check for ipv6
    if (addr.addr6.sin6_family == AF_INET6) {
      result    = new t_byte[17];
      result[0] = (t_byte) 16;
      for (long i = 0; i < 16; i++) 
	result[i+1] = addr.addr6.sin6_addr.s6_addr[i];
    }
    // check for ipv4
    if (addr.addr4.sin_family == AF_INET) {
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      value.d_qval = addr.addr4.sin_addr.s_addr;
      result    = new t_byte[5];
      result[0] = (t_byte) 4;
      for (long i = 0; i < 4; i++) result[i+1] = value.d_bval[i];
    }
    return result;
  }
  
  // get an address port
  static t_word get_addr_port (t_sockaddr& addr) {
    t_word result = 0;
    // check for ipv6
    if (addr.addr6.sin6_family == AF_INET6) {
      result = ntohs (addr.addr6.sin6_port);
    }
    // check for ipv4
    if (addr.addr4.sin_family == AF_INET) {
      result = ntohs (addr.addr4.sin_port);
    }
    return result;
  }
  
  // check for an ipv6 socket
  bool c_isipv6 (const int sid) {
    struct sockaddr address;
    socklen_t       addrlen = sizeof (address);
    mzero (&address, addrlen);
    // check for valid socket
    if (sid == 0) return false;
    // query socket address
    if (getsockname (sid,&address,&addrlen) != 0) return false;
    // query socket familly
    if (address.sa_family == AF_INET6) return true;
    return false;
  }
  
  // set socket option
  bool c_ipsetopt (const int sid, const t_sopt opt, const bool val) {
    // check for a valid socket
    if (sid == -1) return false;
    struct linger  lval;
    int bval      = val ? 1 : 0;
    lval.l_onoff  = bval;
    lval.l_linger = 0;
    // dispatch ip independant options
    switch (opt) {
    case SOPT_RADR:
      return c_setsockopt (sid, SOL_SOCKET, SO_REUSEADDR, &bval, sizeof (bval));
      break;
    case SOPT_RPRT:
      return c_setsockopt (sid, SOL_SOCKET, SO_REUSEPORT, &bval, sizeof (bval));
      break;
    case SOPT_DNTR:
      return c_setsockopt (sid, SOL_SOCKET, SO_DONTROUTE, &bval, sizeof (bval));
      break;
    case SOPT_KLIV:
      return c_setsockopt (sid, SOL_SOCKET, SO_KEEPALIVE, &bval, sizeof (bval));
      break;
    case SOPT_LIGT:
      return c_setsockopt (sid, SOL_SOCKET, SO_LINGER,    &lval, sizeof (lval));
      break;
    case SOPT_NDLY:
      return c_setsockopt (sid, SOL_TCP,    TCP_NODELAY,  &bval, sizeof (bval));
      break;
    default:
      break;
    }
    // dispatch between ipv6 and ipv4
    if (c_isipv6 (sid) == true) {
      switch (opt) {
      case SOPT_MLBK:
	return c_setsockopt (sid, SOL_IPV6, IPV6_MULTICAST_LOOP, &bval, 
			     sizeof (bval));
	break;
      default:
	break;
      }
    } else {
      switch (opt) {
      case SOPT_BCST:
	return c_setsockopt (sid, SOL_SOCKET, SO_BROADCAST,&bval,
			     sizeof (bval));
	break;
      case SOPT_MLBK:
	return c_setsockopt (sid, SOL_IP, IP_MULTICAST_LOOP, &bval, 
			     sizeof (bval));
	break;
      default:
	break;
      }
    }
    return false;
  }

  // get socket option
  bool c_ipgetopt (const int sid, const t_sopt opt, bool& val) {
    // check for a valid socket
    if (sid == -1) return false;
    int bval = 0;
    unsigned int blen = sizeof (bval);
    struct linger lval;
    lval.l_onoff  = 0;
    lval.l_linger = 0;
    // dispatch ip independant options
    bool result = false;
    switch (opt) {
    case SOPT_RADR:
      result =  c_getsockopt (sid, SOL_SOCKET, SO_REUSEADDR, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_RPRT:
      result =  c_getsockopt (sid, SOL_SOCKET, SO_REUSEPORT, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_DNTR:
      result = c_getsockopt (sid, SOL_SOCKET, SO_DONTROUTE, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_KLIV:
      result = c_getsockopt (sid, SOL_SOCKET, SO_KEEPALIVE, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_LIGT:
      blen = sizeof (lval);
      result = c_getsockopt (sid, SOL_SOCKET, SO_LINGER, &lval, blen);
      bval = (blen == 0) ? 0 : lval.l_onoff;
      break;
    case SOPT_NDLY:
      result = c_getsockopt (sid, SOL_TCP, TCP_NODELAY, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    default:
      break;
    }
    // dispatch between ipv6 and ipv4
    if (c_isipv6 (sid) == true) {
      switch (opt) {
      case SOPT_MLBK:
	result = c_getsockopt (sid, SOL_IPV6, IPV6_MULTICAST_LOOP,&bval,blen); 
	if (blen == 0) bval = 0;
	break;
      default:
	break;
      }
    } else {
      switch (opt) {
      case SOPT_BCST:
	result = c_getsockopt (sid, SOL_SOCKET, SO_BROADCAST, &bval, blen);
	if (blen == 0) bval = 0;
	break;
      case SOPT_MLBK:
	result = c_getsockopt (sid, SOL_IP, IP_MULTICAST_LOOP, &bval, blen);
	if (blen == 0) bval = 0;
	break;
      default:
	break;
      }
    }
    val = (bval == 0) ? false : true;
    return result;
  }
  
  // set socket option
  bool c_ipsetopt (const int sid, const t_sopt opt, const long val) {
    // check for a valid socket
    if (sid == -1) return false;
    struct linger  lval;
    int ival      = val;
    lval.l_onoff  = 1;
    lval.l_linger = ival;
    // dispatch ip independant options
    switch (opt) {
    case SOPT_LIGT:
      return c_setsockopt (sid, SOL_SOCKET, SO_LINGER,   &lval, sizeof (lval));
      break;
    case SOPT_RSIZ:
      return c_setsockopt (sid, SOL_SOCKET, SO_RCVBUF,   &ival, sizeof (ival));
      break;
    case SOPT_SSIZ:
      return c_setsockopt (sid, SOL_SOCKET, SO_SNDBUF,   &ival, sizeof (ival));
      break;
    case SOPT_MSIZ:
      return c_setsockopt (sid, SOL_TCP,    TCP_MAXSEG,  &ival, sizeof (ival));
      break;
    default:
      break;
    }
    // dispatch between ipv6 and ipv4
    if (c_isipv6 (sid) == true) {
      switch (opt) {
      case SOPT_SHOP:
	return c_setsockopt (sid, SOL_IPV6, IPV6_UNICAST_HOPS, &ival, 
			     sizeof (ival));
	break;
      case SOPT_MHOP:
	return c_setsockopt (sid, SOL_IPV6, IPV6_MULTICAST_HOPS, &ival, 
			     sizeof (ival));
	break;
      default:
	break;
      }
    } else {
      switch (opt) {
      case SOPT_SHOP:
	return c_setsockopt (sid, SOL_IP, IP_TTL, &ival, sizeof (ival));
	break;
      case SOPT_MHOP:
	return c_setsockopt (sid, SOL_IP, IP_MULTICAST_TTL, &ival, 
			     sizeof (ival));
	break;
      default:
	break;
      }
    }
    return false;
  }

  // get socket option
  bool c_ipgetopt (const int sid, const t_sopt opt, long& val) {
    // check for a valid socket
    if (sid == -1) return false;
    int ival = 0;
    unsigned int ilen = sizeof (ival);
    struct linger lval;
    lval.l_onoff  = 0;
    lval.l_linger = 0;
    // dispatch ip independant options
    bool result = false;
    switch (opt) {
    case SOPT_LIGT:
      ilen = sizeof (lval);
      result = c_getsockopt (sid, SOL_SOCKET, SO_LINGER, &lval, ilen);
      ival = (ilen == 0) ? 0 : lval.l_linger;
      break;
    case SOPT_RSIZ:
      result = c_getsockopt (sid, SOL_SOCKET, SO_RCVBUF, &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    case SOPT_SSIZ:
      result = c_getsockopt (sid, SOL_SOCKET, SO_SNDBUF, &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    case SOPT_MSIZ:
      result = c_getsockopt (sid, SOL_TCP, TCP_MAXSEG,   &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    default:
      break;
    }
    // dispatch between ipv6 and ipv4
    if (c_isipv6 (sid) == true) {
      switch (opt) {
      case SOPT_SHOP:
	result = c_getsockopt (sid, SOL_IPV6, IPV6_UNICAST_HOPS, &ival, ilen);
	if (ilen == 0) ival = 0;
	break;
      case SOPT_MHOP:
	result = c_getsockopt (sid, SOL_IPV6, IPV6_MULTICAST_HOPS,&ival,ilen); 
	if (ilen == 0) ival = 0;
	break;
      default:
	break;
      }
    } else {
      switch (opt) {
      case SOPT_SHOP:
	result = c_getsockopt (sid, SOL_IP, IP_TTL, &ival, ilen);
	if (ilen == 0) ival = 0;
	break;
      case SOPT_MHOP:
	return c_getsockopt (sid, SOL_IP, IP_MULTICAST_TTL, &ival, ilen);
	if (ilen == 0) ival = 0;
	break;
      default:
	break;
      }
    }
    val = ival;
    return result;
  }
}
#else
namespace afnix {
  // this function returns the address size based on the socket type
  static inline socklen_t get_addr_len (const int sid) {
    return IP_ADDRLEN_4;
  }
  // initialize an ip address with a port and type
  static void init_addr (t_sockaddr& addr, t_word port, bool) {
    addr.addr4.sin_family      = AF_INET;
    addr.addr4.sin_port        = htons (port);
    addr.addr4.sin_addr.s_addr = 0;
  }     
  // fill an ip address from bytes
  static void byte_to_addr (t_sockaddr& addr, t_word port, const t_byte* src) {
    // check for ipv4
    if (src[0] == 4) {
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      for (long i = 0; i < 4; i++) value.d_bval[i] = src[i+1];
      addr.addr4.sin_family      = AF_INET;
      addr.addr4.sin_port        = htons (port);
      addr.addr4.sin_addr.s_addr = value.d_qval;
    }    
  }
  // map an ip address to bytes
  static t_byte* addr_to_byte (t_sockaddr& addr) {
    t_byte* result = nullptr;
    if (addr.addr4.sin_family == AF_INET) {
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      value.d_qval = addr.addr4.sin_addr.s_addr;
      result    = new t_byte[5];
      result[0] = (t_byte) 4;
      for (long i = 0; i < 4; i++) result[i+1] = value.d_bval[i];
    }
    return result;
  }
  // get an address port
  static t_word get_addr_port (t_sockaddr& addr) {
    t_word result = 0;
    // check for ipv4
    if (addr.addr4.sin_family == AF_INET) {
      result = ntohs (addr.addr4.sin_port);
    }
    return result;
  }

  // check for an ipv6 socket - always return false here
  bool c_isipv6 (const int) {
    return false;
  }
  
  // set socket option
  bool c_ipsetopt (const int sid, const t_sopt opt, const bool val) {
    // check for a valid socket
    if (sid == -1) return false;
    struct linger  lval;
    int bval      = val ? 1 : 0;
    lval.l_onoff  = bval;
    lval.l_linger = 0;
    // dispatch ip independant options
    switch (opt) {
    case SOPT_RADR:
      return c_setsockopt (sid, SOL_SOCKET, SO_REUSEADDR, &bval, sizeof (bval));
      break;
    case SOPT_RPRT:
      return c_setsockopt (sid, SOL_SOCKET, SO_REUSEPORT, &bval, sizeof (bval));
      break;
    case SOPT_BCST:
      return c_setsockopt (sid, SOL_SOCKET, SO_BROADCAST, &bval, sizeof (bval));
      break;
    case SOPT_DNTR:
      return c_setsockopt (sid, SOL_SOCKET, SO_DONTROUTE, &bval, sizeof (bval));
      break;
    case SOPT_KLIV:
      return c_setsockopt (sid, SOL_SOCKET, SO_KEEPALIVE, &bval, sizeof (bval));
      break;
    case SOPT_LIGT:
      return c_setsockopt (sid, SOL_SOCKET, SO_LINGER,    &lval, sizeof (lval));
      break;
    case SOPT_NDLY:
      return c_setsockopt (sid, SOL_TCP,    TCP_NODELAY,  &bval, sizeof (bval));
      break;
    case SOPT_MLBK:
      return c_setsockopt (sid, SOL_IP, IP_MULTICAST_LOOP,&bval, sizeof (bval));
      break;
    default:
      break;
    }
    return false;
  }

  // get socket option
  bool c_ipgetopt (const int sid, const t_sopt opt, bool& val) {
    // check for a valid socket
    if (sid == -1) return false;
    int bval = 0;
    unsigned int blen = sizeof (bval);
    struct linger lval;
    lval.l_onoff  = 0;
    lval.l_linger = 0;
    // dispatch ip independant options
    bool result = false;
    switch (opt) {
    case SOPT_RADR:
      result =  c_getsockopt (sid, SOL_SOCKET, SO_REUSEADDR, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_RPRT:
      result =  c_getsockopt (sid, SOL_SOCKET, SO_REUSEPORT, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_BCST:
      result = c_getsockopt (sid, SOL_SOCKET, SO_BROADCAST, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_DNTR:
      result = c_getsockopt (sid, SOL_SOCKET, SO_DONTROUTE, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_KLIV:
      result = c_getsockopt (sid, SOL_SOCKET, SO_KEEPALIVE, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_LIGT:
      blen = sizeof (lval);
      result = c_getsockopt (sid, SOL_SOCKET, SO_LINGER, &lval, blen);
      bval = (blen == 0) ? 0 : lval.l_onoff;
      break;
    case SOPT_NDLY:
      result = c_getsockopt (sid, SOL_TCP, TCP_NODELAY, &bval, blen);
      if (blen == 0) bval = 0;
      break;
    case SOPT_MLBK:
      result = c_getsockopt (sid, SOL_IP, IP_MULTICAST_LOOP, &bval, blen);
      if (blen == 0) bval = 0;
    default:
      break;
    }
    val = (bval == 0) ? false : true;
    return result;
  }
  
  // set socket option
  bool c_ipsetopt (const int sid, const t_sopt opt, const long val) {
    // check for a valid socket
    if (sid == -1) return false;
    struct linger  lval;
    int ival      = val;
    lval.l_onoff  = 1;
    lval.l_linger = ival;
    // dispatch ip independant options
    switch (opt) {
    case SOPT_LIGT:
      return c_setsockopt (sid, SOL_SOCKET, SO_LINGER,   &lval, sizeof (lval));
      break;
    case SOPT_RSIZ:
      return c_setsockopt (sid, SOL_SOCKET, SO_RCVBUF,   &ival, sizeof (ival));
      break;
    case SOPT_SSIZ:
      return c_setsockopt (sid, SOL_SOCKET, SO_SNDBUF,   &ival, sizeof (ival));
      break;
    case SOPT_MSIZ:
      return c_setsockopt (sid, SOL_TCP,    TCP_MAXSEG,  &ival, sizeof (ival));
      break;
    case SOPT_SHOP:
      return c_setsockopt (sid, SOL_IP,     IP_TTL,      &ival, sizeof (ival));
      break;
    case SOPT_MHOP:
      return c_setsockopt (sid, SOL_IP, IP_MULTICAST_TTL,&ival, sizeof (ival));
      break;
    default:
      break;
    }
    return false;
  }
  
  // get socket option
  bool c_ipgetopt (const int sid, const t_sopt opt, long& val) {
    // check for a valid socket
    if (sid == -1) return false;
    int ival = 0;
    unsigned int ilen = sizeof (ival);
    struct linger lval;
    lval.l_onoff  = 0;
    lval.l_linger = 0;
    // dispatch ip independant options
    bool result = false;
    switch (opt) {
    case SOPT_LIGT:
      ilen = sizeof (lval);
      result = c_getsockopt (sid, SOL_SOCKET, SO_LINGER, &lval, ilen);
      ival = (ilen == 0) ? 0 : lval.l_linger;
      break;
    case SOPT_RSIZ:
      result = c_getsockopt (sid, SOL_SOCKET, SO_RCVBUF, &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    case SOPT_SSIZ:
      result = c_getsockopt (sid, SOL_SOCKET, SO_SNDBUF, &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    case SOPT_MSIZ:
      result = c_getsockopt (sid, SOL_TCP, TCP_MAXSEG,   &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    case SOPT_SHOP:
      result = c_getsockopt (sid, SOL_IP, IP_TTL, &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    case SOPT_MHOP:
      return c_getsockopt (sid, SOL_IP, IP_MULTICAST_TTL, &ival, ilen);
      if (ilen == 0) ival = 0;
      break;
    default:
      break;
    }
    val = ival;
    return result;
  }
}
#endif

#ifdef AFNIX_HAVE_IPV6_MCAST
namespace afnix {
  // do multicast operation
  static bool c_ipmcast (const int sid, t_byte* dest, bool drop) {
    if ((sid == -1) || (dest == nullptr)) return false;
    // check for ipv6
    if (dest[0] == 16) {
      struct ipv6_mreq mreq;
      mreq.ipv6mr_interface = 0;
      for (long i = 0; i < 16; i++) mreq.ipv6mr_multiaddr.s6_addr[i] = dest[i];
      int opt = drop ? IPV6_DROP_MEMBERSHIP : IPV6_ADD_MEMBERSHIP;
      return c_setsockopt (sid, SOL_IPV6, opt, &mreq, sizeof (mreq));
    }
    // check for ipv4
    if (dest[0] == 4) {
      struct ip_mreq mreq;
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      for (long i = 0; i < 4; i++) value.d_bval[i] = dest[i+1];
      mreq.imr_multiaddr.s_addr = value.d_qval; 
      mreq.imr_interface.s_addr = 0;
      int opt = drop ? IP_DROP_MEMBERSHIP : IP_ADD_MEMBERSHIP;
      return c_setsockopt (sid, SOL_IP, opt, &mreq, sizeof (mreq));
    }
    return false;
  }
}
#else
namespace afnix {
  // do a multicast operation
  static bool c_ipmcast (const int sid, t_byte* dest, bool drop) {
    if ((sid == -1) || (dest == nullptr)) return false;
    // check for ipv4
    if (dest[0] == 4) {
      struct ip_mreq mreq;
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      for (long i = 0; i < 4; i++) value.d_bval[i] = dest[i+1];
      mreq.imr_multiaddr.s_addr = value.d_qval; 
      mreq.imr_interface.s_addr = 0;
      int opt = drop ? IP_DROP_MEMBERSHIP : IP_ADD_MEMBERSHIP;
      return c_setsockopt (sid, SOL_IP, opt, &mreq, sizeof (mreq));
    }
    return false;
  }
}
#endif

#ifdef AFNIX_HAVE_ADDRINFO
namespace afnix {
  // this function create an ip address information structure
  // from an addrinfo structure
  static s_ipaddr* new_ipaddr (struct addrinfo* info) {
    // check for record
    if (info == NULL) return nullptr;
    // compute address info size
    long size = 0;
    struct addrinfo* addr = nullptr;
    for (addr = info; addr != NULL; addr = addr->ai_next) size += 1;
    // create the result structure
    s_ipaddr* ipaddr = new s_ipaddr (size);
    // loop into the result
    addr = info;
    for (long i = 0; i < size; i++) {
      ipaddr->p_name[i] = c_strdup (addr->ai_canonname);
      ipaddr->p_addr[i] = addr_to_byte (*((t_sockaddr*) addr->ai_addr));
      addr = addr->ai_next;
    }
    return ipaddr;
  }

  // get the address information by name
  s_ipaddr* c_getipa (const char* name, const t_iapf iapf) {
    // name check first
    if (c_strlen (name) == 0) return nullptr;
    // initialize the hint structure
    struct addrinfo hint;
    mzero (&hint, sizeof (hint));
    hint.ai_flags = AI_CANONNAME;
    switch (iapf) {
    case IAPF_XANY:
      hint.ai_family = PF_UNSPEC;
      break;
    case IAPF_IPV4:
      hint.ai_family = AF_INET;
      break;
    case IAPF_IPV6:
      hint.ai_family = AF_INET6;
      break;
    };
    // get the address info
    struct addrinfo* info = NULL;
    if (getaddrinfo (name, NULL, &hint, &info) != 0) return nullptr;
    // create the ip address structure
    s_ipaddr* ipaddr = new_ipaddr (info);
    // free data structure and return
    freeaddrinfo (info);
    return ipaddr;
  }
}
#else
namespace afnix {
  // static mutex creation function
  static void* cnet_hmtx_create (void);
  // mutex or network services
  static void* cnet_hmtx = cnet_hmtx_create ();
  // this function destroy the mutex at exit
  static void cnet_hmtx_destroy (void) {
    c_mtxdestroy (cnet_hmtx);
  }
  // this function initialize a mutex statically
  static void* cnet_hmtx_create (void) {
    void* result = c_mtxcreate ();
    c_atexit (cnet_hmtx_destroy);
    return result;
  }
  // get the address information by name using the old
  // gethostbyname function call
  s_ipaddr* c_getipa (const char* name, const t_iapf iapf) {
    // name check first
    if (c_strlen (name) == 0) return nullptr;
    // check protocol family for this obsolete call
    if (iapf == IAPF_IPV6) return nullptr;
    // lock the mutex
    c_mtxlock (cnet_hmtx);
    // call the resolver and get address
    struct hostent* hostrec = gethostbyname (name);
    if (hostrec == nullptr) {
      c_mtxunlock (cnet_hmtx);
      return nullptr;
    }
    // create an ip address structure with one entry
    s_ipaddr* ipaddr = new s_ipaddr (1);
    // extract name and address fields
    ipaddr->p_name[0] = c_strdup (hostrec->h_name);
    ipaddr->p_addr[0] = new t_byte[hostrec->h_length+1];
    t_byte* addr = ipaddr->p_addr[0];
    // update address field (in network order)
    if (hostrec->h_length == 4)  addr[0] = (t_byte) 4;
    if (hostrec->h_length == 16) addr[0] = (t_byte) 16;
    for (long i = 0; i < hostrec->h_length; i++) {
      addr[i+1] = hostrec->h_addr_list[0][i];
    }
    c_mtxunlock (cnet_hmtx);
    return ipaddr;
  }
}
#endif

#ifdef AFNIX_HAVE_NAMEINFO
namespace afnix {
  // do a reverse lookup on an address
  char* c_getipn (const t_byte* addr, const bool nflg) {
    // initial check as usual
    if (addr == nullptr) return nullptr;
    // initialize the address
    t_sockaddr address;
    socklen_t  addrlen = sizeof (address);
    mzero (&address, addrlen);
    byte_to_addr (address, 0, addr);
    // get the host name or its numerical form
    char hbuf[NI_MAXHOST];
    if (nflg == false) {
      if (getnameinfo ((struct sockaddr*) &address, addrlen, hbuf, sizeof(hbuf),
		       NULL, 0, NI_NAMEREQD) != 0) return nullptr;
    } else {
      if (getnameinfo ((struct sockaddr*) &address, addrlen, hbuf, sizeof(hbuf),
		       NULL, 0, NI_NUMERICHOST) != 0) return nullptr;
    }
    // copy the result
    return c_strdup (hbuf);
  }
}
# else
namespace afnix {
  // reverse lookup is not possible here
  char* c_getipn (const t_byte* addr, const bool nflg) {
    // initial check as usual
    if ((addr == nullptr) || (nflg == false)) return nullptr;
    // initialize the address
    t_sockaddr address;
    socklen_t  addrlen = sizeof (address);
    mzero (&address, addrlen);
    byte_to_addr (address, 0, addr);
    char hbuf[1024];
    // check for ipv6
    if (addr[0] == 16) {
      if (inet_ntop (AF_INET6, &addr.addr6.sin6_addr,
		     hbuf, sizeof(hbuf)) == NULL) return nullptr;
    }
    // check for ipv4
    if (addr[0] == 4) {
      if (inet_ntop (AF_INET, &addr.addr4.sin_addr,
		     hbuf, sizeof(hbuf)) == NULL) return nullptr;
    }
    // copy the result
    return c_strdup (hbuf);
  }
}
#endif

#ifdef AFNIX_HAVE_KEEPALIVE
namespace afnix {
  // set the socket keepalive parameters
  bool c_setkalv (const int sid,const int kidl,const int kcnt,const int kitv) {
    // check arguments first
    if ((sid < 0) || (kidl < 0) || (kcnt <= 0) || (kitv < 0)) return false;
    // enable the keepalive sopcket option
    int kval = 1;
    if (c_setsockopt (sid, SOL_SOCKET, SO_KEEPALIVE,
		      &kval, sizeof(kval)) == false) return false;
    // set the keepidle
    if (c_setsockopt (sid, SOL_TCP, TCP_KEEPIDLE,
		      &kidl, sizeof(kidl)) == false) {
      c_rstkalv (sid);
      return false;
    }
    // set the keepcnt
    if (c_setsockopt (sid, SOL_TCP, TCP_KEEPCNT,
		      &kcnt, sizeof(kcnt)) == false) {
      c_rstkalv (sid);
      return false;
    }
    // set the keepintvl
    if (c_setsockopt (sid, SOL_TCP, TCP_KEEPINTVL,
		      &kitv, sizeof(kitv)) == false) {
      c_rstkalv (sid);
      return false;
    }
    return true;
  }

  // reset the keepalive parameters
  void c_rstkalv (const int sid) {
    // check for valid parameters
    if (sid < 0) return;
    // reset keepalive
    int kval = 0;
    c_setsockopt (sid, SOL_SOCKET, SO_KEEPALIVE, &kval, sizeof(kval));
    // reset the keepidle
    int kidl = 7200;
    c_setsockopt (sid, SOL_TCP, TCP_KEEPIDLE, &kidl, sizeof(kidl));
    // reset the keepcnt
    int kcnt = 9;
    c_setsockopt (sid, SOL_TCP, TCP_KEEPCNT, &kcnt, sizeof(kcnt));
    // reset the keepintvl
    int kitv = 75;
    c_setsockopt (sid, SOL_TCP, TCP_KEEPINTVL, &kitv, sizeof(kitv));
  }
}
#else
namespace afnix {
  // set the socket keepalive parameters
  bool c_setkalv (const int, const int, const int, const int) {
    return false;
  }

  // reset the keepalive parameters
  void c_rstkalv (const int sid) {
    // reset keepalive
    int kval = 0;
    c_setsockopt (sid, SOL_SOCKET, SO_KEEPALIVE, &kval, sizeof(kval));
  }
}
#endif

namespace afnix {
  // static mutex creation function
  static void* cnet_smtx_create (void);
  // mutex or network services
  static void* cnet_smtx = cnet_smtx_create ();
  // this function destroy the mutex at exit
  static void cnet_smtx_destroy (void) {
    c_mtxdestroy (cnet_smtx);
  }
  // this function initialize a mutex statically
  static void* cnet_smtx_create (void) {
    void* result = c_mtxcreate ();
    c_atexit (cnet_smtx_destroy);
    return result;
  }

  // return the loopback name

  const char* c_loopname (void) {
    return AFNIX_LOOPBACK_NAME;
  }

  // check for a numeric ip address

  bool c_isipn (const char* name, const t_iapf iapf) {
    // check for valid name
    if (name == nullptr) return false;
    // check for anything
    if (iapf == IAPF_XANY) {
      if (c_isipn (name, IAPF_IPV4) == true) return true;
      if (c_isipn (name, IAPF_IPV6) == true) return true;
      return false;
    }
    // check for version 4
    if (iapf == IAPF_IPV4) {
      struct in_addr addr;
      if (inet_pton (AF_INET, name, &addr) == 1) return true;
      return false;
    }
    // check for version 6
    if (iapf == IAPF_IPV6) {
      struct in6_addr addr;
      if (inet_pton (AF_INET6, name, &addr) == 1) return true;
      return false;
    }
    return false;
  }

  // make a copy of an address

  t_byte* c_cpaddr (const t_byte* addr) {
    // check for nil address
    if (addr == nullptr) return nullptr;
    // check for length
    long len = (long) addr[0] + 1;
     t_byte* result = new t_byte[len];
     for (long i = 0; i < len; i++) result[i] = addr[i];
     return result;
  }

  // compute the next byte address

  t_byte* c_nxaddr (const t_byte* addr) {
    // get a copy of that address
    t_byte* result = c_cpaddr (addr);
    // do nothing if nil
    if (result == nullptr) return nullptr;
    // get the length and loop
    bool cry = false;
    long len = (long) addr[0];
    for (long i = len; i > 0; i--) {
      // check at max (carry propagate)
      if (result[i] == 0xff) {
	result[i] = 0x00;
	continue;
      }
      // increase value
      result[i]++;
      // check at max carry
      if ((result[i] == 0xff) && (cry == true)) {
	result[i] = 0x00;
	continue;
      }
      if (cry == true) {
	result[i]++;
	cry = false;
      }
      break;
    }
    return result;
  }

  // compare two addresses for equality

  bool c_eqaddr (const t_byte* addr1, const t_byte* addr2) {
    // nil check first
    if ((addr1 == nullptr) || (addr2 == nullptr)) return false;
    // here both address are not nil - check the size
    long len1 = (long) addr1[0];
    long len2 = (long) addr2[0];
    if (len1 != len2) return false;
    // here we have both size - compare byte by byte
    for (long i = 1; i <= len1; i++) {
      if (addr1[i] != addr2[i]) return false;
    }
    return true;
  }

  // check if an address is less than another one

  bool c_ltaddr (const t_byte* addr1, const t_byte* addr2) {
    // nil check first
    if ((addr1 == nullptr) || (addr2 == nullptr)) return false;
    // here both address are not nil - check the size
    long len1 = (long) addr1[0];
    long len2 = (long) addr2[0];
    if (len1 != len2) return false;
    // here we have both size - compare byte by byte
    for (long i = 1; i <= len1; i++) {
      if (addr1[i] > addr2[i]) return false;
      if (addr1[i] < addr2[i]) return true;
    }
    return false;
  }

  // check if an address is less equal than another one

  bool c_leaddr (const t_byte* addr1, const t_byte* addr2) {
    // nil check first
    if ((addr1 == nullptr) || (addr2 == nullptr)) return false;
    // here both address are not nil - check the size
    long len1 = (long) addr1[0];
    long len2 = (long) addr2[0];
    if (len1 != len2) return false;
    // here we have both size - compare byte by byte
    for (long i = 1; i <= len1; i++) {
      if (addr1[i] > addr2[i]) return false;
      if (addr1[i] < addr2[i]) return true;
    }
    return true;
  }

  // get a service by name and flag

  t_word c_ipserv (const char* name, bool flag) {
    // check for service
    if (name == nullptr) return 0;
    // lock the mutex
    c_mtxlock (cnet_smtx);
    // get the service
    const char* proto  = flag ? "tcp" : "udp";
    struct servent* he = getservbyname (name, proto);
    if (he == nullptr) {
      c_mtxunlock (cnet_smtx);
      return nilw;
    }
    t_word result = ntohs (he->s_port);
    c_mtxunlock (cnet_smtx);
    return result;
  }

  // get the socket address

  t_byte* c_ipsockaddr (const int sid) {
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // check for valid socket
    if (sid < 0) return nullptr;
    // query socket address
    if (getsockname (sid, (struct sockaddr*) &address, &addrlen) != 0) 
      return nullptr;
    return addr_to_byte (address);
  }

  // get the peer address

  t_byte* c_ippeeraddr (const int sid) {
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // check for valid socket
    if (sid < 0) return nullptr;
    // query socket address
    if (getpeername (sid,(struct sockaddr*) &address,&addrlen) != 0) 
      return nullptr;
    return addr_to_byte (address);
  }

  // get the socket port

  t_word c_ipsockport (const int sid) {
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // check for valid socket
    if (sid < 0) return AFNIX_ERR_IARG;
    // query socket address
    if (getsockname (sid,(struct sockaddr*) &address, &addrlen) != 0) 
	return AFNIX_ERR_IARG;
    return get_addr_port (address);
  }

  // get the peer port

  t_word c_ippeerport (const int sid) {
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // check for valid socket
    if (sid < 0) return AFNIX_ERR_IARG;
    // query peer address
    if (getpeername (sid,(struct sockaddr*) &address, &addrlen) != 0) 
	return AFNIX_ERR_IARG;
    return get_addr_port (address);
  }

  // connect a socket with a server

  bool c_ipconnect (const int sid, t_word port, t_byte* dest) {
    // fill in the address
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    byte_to_addr (address, port, dest);
    // do the connection
    if (connect (sid, (struct sockaddr*) &address, addrlen) == 0) return true;
    return false;
  }

  // bind a socket with a port only

  bool c_ipbind (const int sid, t_word port) {
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // check for a valid socket
    if (sid < 0) return false;
    // initialize the address
    init_addr (address, port, c_isipv6 (sid));
    // bind this socket
    if (bind (sid, (struct sockaddr*) &address, addrlen) == -1) return false;
    return true;
  }

  // bind a socket with a port and address

  bool c_ipbind (const int sid, t_word port, t_byte* dest) {
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // check for a valid socket
    if (sid < 0) return false;
    // initialize the address
    byte_to_addr (address, port, dest);
    // bind this socket
    if (bind (sid, (struct sockaddr*) &address, addrlen) == -1) return false;
    return true;
  }

  // send a datagram on a connected socket

  long c_ipsend (const int sid, const char* buf, long size) {
    if (sid < 0) return AFNIX_ERR_IARG;
    long status = send (sid, buf, size, 0);
    return (status == -1) ? c_errmap (errno) : status;
  }

  // receive a datagram from a connected socket

  long c_iprecv (const int sid, char* buf, long size) {
    if (sid < 0) return AFNIX_ERR_IARG;
    long status = recv (sid, buf, size, 0);
    return (status == -1) ? c_errmap (errno) : status;
  }

  // send a datagram by address and port

  long c_ipsendto (const int sid, t_word port, t_byte* dest, 
		   const char* buf, const long size) {
    // initialize the address
    t_sockaddr address;
    socklen_t  addrlen = get_addr_len (sid);
    mzero (&address, addrlen);
    // convert the address
    byte_to_addr (address, port, dest);
    // send the datagram
    long result = 0;
    if ((buf == nullptr) || (size == 0)) {
      result = sendto (sid,NULL, 0, 0, (struct sockaddr*) &address, addrlen);
    } else {
      result = sendto (sid,buf,size,0, (struct sockaddr*) &address, addrlen);
    }
    return (result == -1) ? c_errmap (result) : result;
  }

  // listen for a socket

  bool c_iplisten (const int sid, const long backlog) {
    return (listen (sid, backlog) == -1) ? false : true;
  }

  // accept a connection on a server socket

  int c_ipaccept (const int sid) {
    // check for a valid socket
    if (sid < 0) return AFNIX_ERR_IARG;
    long status = accept (sid, NULL, 0);
    return (status == -1) ? c_errmap (errno) : status;
  }

  // join a multicast group

  bool c_ipjoin (const int sid, t_byte* dest) {
    return c_ipmcast (sid, dest, false);
  }

  // drop from a multicast group

  bool c_ipdrop (const int sid, t_byte* dest) {
    return c_ipmcast (sid, dest, true);
  }

  // shutdown a socket

  bool c_ipshut (const int sid, const t_shut how) {
    bool status = false;
    if (sid == -1) return false;
    switch (sid) {
    case SHUT_BOTH:
      status = (shutdown (sid, 2) == 0) ? true : false;
      break;
    case SHUT_RECV:
      status = (shutdown (sid, 0) == 0) ? true : false;
      break;
    case SHUT_SEND:
      status = (shutdown (sid, 1) == 0) ? true : false;
      break;
    }
    return status;
  }
}

#ifdef AFNIX_HAVE_IPV6
namespace afnix {
  // create a tcp socket by protocol family
  int c_ipsocktcp (const t_iapf iapf) {
    // get the loopback address
    s_ipaddr* ipaddr = c_getipa (c_loopname (), iapf);
    if (ipaddr == nullptr) return AFNIX_ERR_ADDR;
    t_byte* addr = ipaddr->d_size == 0 ? nullptr : ipaddr->p_addr[0];
    // create a tcp socket ipv4 or ipv6
    int sid = AFNIX_ERR_PRIV;
    if (addr[0] == 16)
      sid = socket (PF_INET6, SOCK_STREAM, IPPROTO_TCP);
    else
      sid = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    delete ipaddr;
    return (sid == -1) ? c_errmap (errno) : sid;
  }

  // create a tcp socket by address family
  int c_ipsocktcp (const t_byte* addr) {
    // check the address
    if (addr == nullptr) return c_ipsocktcp (IAPF_XANY);
    // create a tcp socket ipv4 or ipv6
    int sid = AFNIX_ERR_PRIV;
    if (addr[0] == 16)
      sid = socket (PF_INET6, SOCK_STREAM, IPPROTO_TCP);
    else
      sid = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    return (sid == -1) ? c_errmap (errno) : sid;
  }
}
#else
namespace afnix {
  // create a tcp socket
  int c_ipsocktcp (const t_iapf iapf) {
    // check for invalif protocol
    if (iapf == IAPF_INET6) return AFNIX_ERR_ADDR;
    // create a tcp socket ipv4 only
    int sid = socket (PF_INET,SOCK_STREAM, IPPROTO_TCP);
    return (sid == -1) ? c_errmap (errno) : sid;
  }

  // create a tcp socket by address
  int c_ipsocktcp (const t_byte* addr) {
    // check the address
    if (addr == nullptr) return c_ipsocktcp ();
    // create a tcp socket ipv4 or ipv6
    int sid = AFNIX_ERR_PRIV;
    if (addr[0] == 16)
      return sid;
    else
      sid = socket (PF_INET, SOCK_STREAM, IPPROTO_TCP);
    return (sid == -1) ? c_errmap (errno) : sid;
  }
}
#endif

#ifdef AFNIX_HAVE_IPV6
namespace afnix {
  // create a udp socket
  int c_ipsockudp (const t_iapf iapf) {
    // get the loopback address
    s_ipaddr* ipaddr = c_getipa (c_loopname (), iapf);
    if (ipaddr == nullptr) return AFNIX_ERR_ADDR;
    t_byte* addr = ipaddr->d_size == 0 ? nullptr : ipaddr->p_addr[0];
    // create a udp socket ipv4 or ipv6
    int sid = AFNIX_ERR_PRIV;
    if (addr[0] == 16)
      sid = socket (PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    else
      sid = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    delete ipaddr;
    return (sid == -1) ? c_errmap (errno) : sid;
  }

  // create a udp socket by address family
  int c_ipsockudp (const t_byte* addr) {
    // check the address
    if (addr == nullptr) return c_ipsockudp (IAPF_XANY);
    // create a udp socket ipv4 or ipv6
    int sid = AFNIX_ERR_PRIV;
    if (addr[0] == 16)
      sid = socket (PF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    else
      sid = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return (sid == -1) ? c_errmap (errno) : sid;
  }
}
#else
namespace afnix {
  // create a udp socket
  int c_ipsockudp (const t_iapf iapf) {
    // check for a valif protocol
    if (iapf == IAPF_IPV6) return AFNIX_ERR_ADDR;
    // create a udp socket ipv4 only
    int sid = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return (sid == -1) ? c_errmap (errno) : sid;
  }

  // create a udp socket by address family
  int c_ipsockudp (const t_byte* addr) {
    // check the address
    if (addr == nullptr) return c_ipsockudp ();
    // create a udp socket ipv4 or ipv6
    int sid = AFNIX_ERR_PRIV;
    if (addr[0] == 16)
      return sid;
    else
      sid = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return (sid == -1) ? c_errmap (errno) : sid;
  }
}
#endif

#ifdef AFNIX_HAVE_IPV6
namespace afnix {
  // receive a datagram by address and port
  long c_iprecvfr (const int sid, t_word& port, t_byte* dest, 
		   char* buf, const long size) {
    struct sockaddr* address;
    socklen_t        addrlen;
    // check for ipv6 or ipv4
    if (dest[0] == 16) {
      struct sockaddr_in6 addr;
      addr.sin6_family = AF_INET6;
      addr.sin6_port   = htons (port);
      for (long i = 0; i < 16; i++) addr.sin6_addr.s6_addr[i] = nilc;
      address = (struct sockaddr*) &addr;
      addrlen = IP_ADDRLEN_6;
    } else {
      struct sockaddr_in addr;
      addr.sin_family      = AF_INET;
      addr.sin_port        = htons (port);
      addr.sin_addr.s_addr = 0;
      address = (struct sockaddr*) &addr;
      addrlen = IP_ADDRLEN_6;
    }
    // receive the datagram
    long result = recvfrom (sid, buf, size, 0, address, &addrlen);
    if (result == -1) return c_errmap (result);
    // extract now the received addredss
    if (dest[0] == 16) {
      struct sockaddr_in6* addr = (struct sockaddr_in6*) address;
      port = ntohs (addr->sin6_port);
      for (long i = 0; i < 16; i++) dest[i+1] = addr->sin6_addr.s6_addr[i];
    } else {
      struct sockaddr_in* addr = (struct sockaddr_in*) address;
      port = ntohs (addr->sin_port);
      using t_addr = union {
	t_quad d_qval;
	t_byte d_bval[4];
      };
      t_addr value;
      value.d_qval = addr->sin_addr.s_addr;
      for (long i = 0; i < 4; i++) dest[i+1] = value.d_bval[i];
    }
    // return result
    return result;
  }
}
#else
namespace afnix {
  // receive a datagram by address and port
  long c_iprecvfr (const int sid, t_word& port, t_byte* dest, 
		   char* buf, const long size) {
    struct sockaddr* address;
    socklen_t        addrlen;
    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons (port);
    addr.sin_addr.s_addr = 0;
    address = (struct sockaddr*) &addr;
    addrlen = IP_ADDRLEN_4;
    // receive the datagram
    long result = recvfrom (sid, buf, size, 0, address, &addrlen);
    if (result == -1) return c_errmap (result);
    // extract now the received addredss
    port = ntohs (addr.sin_port);
    using t_addr = union {
      t_quad d_qval;
      t_byte d_bval[4];
    };
    t_addr value;
    value.d_qval = addr.sin_addr.s_addr;
    for (long i = 0; i < 4; i++) dest[i+1] = value.d_bval[i];
    // return result
    return result;
  }
}
#endif
