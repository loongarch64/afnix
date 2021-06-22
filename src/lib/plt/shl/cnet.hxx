// ---------------------------------------------------------------------------
// - cnet.hxx                                                                -
// - standard platform library - c network function platform definitions     -
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
// - author (c) 1999-2021 amaury   darsch                                    -
// - author (c) 1999-2021 pino     toscano                     hurd platform -
// ---------------------------------------------------------------------------

// check for unknown platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_UNKNOWN)
#error "unknown platform type"
#endif

// check for unknown processor
#if (AFNIX_PLATFORM_PROCID == AFNIX_PROCTYPE_UNKNOWN)
#error "unknown processor type"
#endif

// linux platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_LINUX)
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_KEEPALIVE
#define  AFNIX_HAVE_IPV6_MCAST
#define  AFNIX_LOOPBACK_NAME "localhost"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_IPV6_MCAST
#define  AFNIX_LOOPBACK_NAME "localhost"
#endif

// solaris platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_SOLARIS)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
// solaris 2.5.1 uses old setsockopt and does not define socklen_t
#if (AFNIX_PLATFORM_MAJOR == 5) && (AFNIX_PLATFORM_MINOR < 7)
using socklen_t = int;
#define AFNIX_HAVE_CHAROPT
#endif
// ipv6 is only on solaris 8
#if (AFNIX_PLATFORM_MAJOR == 5) && (AFNIX_PLATFORM_MINOR >= 8)
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_IPV6_MCAST
#include <netinet/ip6.h>
#endif
#define  AFNIX_LOOPBACK_NAME "localhost"
#endif

// freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_FREEBSD)
#define  AFNIX_LOOPBACK_NAME "localhost"
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_KEEPALIVE
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_IPV6_MCAST
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
// define ipv6 multicast socket option
#ifndef IPV6_ADD_MEMBERSHIP
#define IPV6_ADD_MEMBERSHIP  IPV6_JOIN_GROUP
#endif
#ifndef IPV6_DROP_MEMBERSHIP
#define IPV6_DROP_MEMBERSHIP IPV6_LEAVE_GROUP
#endif
#define  AFNIX_LOOPBACK_NAME "localhost"
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_IPV6_MCAST
#endif

// darwin platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_DARWIN)
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
// define ipv6 multicast socket option
#ifndef IPV6_ADD_MEMBERSHIP
#define IPV6_ADD_MEMBERSHIP  IPV6_JOIN_GROUP
#endif
#ifndef IPV6_DROP_MEMBERSHIP
#define IPV6_DROP_MEMBERSHIP IPV6_LEAVE_GROUP
#endif
#define  AFNIX_LOOPBACK_NAME "localhost"
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_IPV6_MCAST
#endif

// gnu/freebsd platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNUKBSD)
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_KEEPALIVE
#define  AFNIX_HAVE_IPV6_MCAST
#define  AFNIX_LOOPBACK_NAME "localhost"
#endif

// gnu platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_GNU)
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_KEEPALIVE
#define  AFNIX_HAVE_IPV6_MCAST
#define  AFNIX_LOOPBACK_NAME "localhost"
#endif

// android platform
#if (AFNIX_PLATFORM_PLATID == AFNIX_PLATFORM_ANDROID)
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#define  AFNIX_HAVE_IPV6
#define  AFNIX_HAVE_ADDRINFO
#define  AFNIX_HAVE_NAMEINFO
#define  AFNIX_HAVE_IPV6_MCAST
#define  AFNIX_LOOPBACK_NAME "localhost"
#endif

// the global socket address space is defined depending on ipv6 or not.
// some implementations are sensitive to the address length
#ifdef  AFNIX_HAVE_IPV6
// the socket option level
#ifndef SOL_IPV6
#define SOL_IPV6 IPPROTO_IPV6
#endif
// the address length
const socklen_t IP_ADDRLEN_6 = sizeof (struct sockaddr_in6);
const socklen_t IP_ADDRLEN_4 = sizeof (struct sockaddr_in);
// the global address space
using t_sockaddr = union {
  struct sockaddr_in  addr4;
  struct sockaddr_in6 addr6;
};
#else
const socklen_t IP_ADDRLEN_4 = sizeof (struct sockaddr_in);
using t_sockaddr = union {
  struct sockaddr_in addr4;
};
#endif

// adjust socket options constant
#ifndef SOL_TCP
#define SOL_TCP  IPPROTO_TCP
#endif

#ifndef SOL_IP
#define SOL_IP   IPPROTO_IP
#endif
