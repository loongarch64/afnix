// ---------------------------------------------------------------------------
// - Libnet.cpp                                                              -
// - afnix:net module - declaration and implementation                       -
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

#include "Meta.hpp"
#include "Libnet.hpp"
#include "Prednet.hpp"
#include "NetCalls.hpp"
#include "Function.hpp"
#include "TcpClient.hpp"
#include "UdpClient.hpp"
#include "Multicast.hpp"
#include "TcpServer.hpp"
#include "UdpServer.hpp"

namespace afnix {

  // initialize the afnix:net module

  Object* init_afnix_net (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    
    // create the afnix:net nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* nset = aset->mknset   ("net");
    
    // bind all classes in the afnix:net nameset
    nset->symcst ("Address",         new Meta (Address::mknew));
    nset->symcst ("Sockopt",         new Meta (Sockopt::meval));
    nset->symcst ("TcpSocket",       new Meta (TcpSocket::mknew));
    nset->symcst ("TcpServer",       new Meta (TcpServer::mknew));
    nset->symcst ("TcpClient",       new Meta (TcpClient::mknew));
    nset->symcst ("UdpSocket",       new Meta (UdpSocket::mknew));
    nset->symcst ("UdpServer",       new Meta (UdpServer::mknew));
    nset->symcst ("UdpClient",       new Meta (UdpClient::mknew));
    nset->symcst ("Multicast",       new Meta (Multicast::mknew));
    nset->symcst ("SockParams",      new Meta (SockParams::mknew));
    
    // bind all predicates in the afnix:net nameset
    nset->symcst ("socket-p",        new Function (net_socketp));
    nset->symcst ("sockopt-p",       new Function (net_sockoptp));
    nset->symcst ("address-p",       new Function (net_addressp));
    nset->symcst ("datagram-p",      new Function (net_datagramp));
    nset->symcst ("tcp-socket-p",    new Function (net_tcpsocketp));
    nset->symcst ("udp-socket-p",    new Function (net_udpsocketp));
    nset->symcst ("tcp-client-p",    new Function (net_tcpclientp));
    nset->symcst ("udp-client-p",    new Function (net_udpclientp));
    nset->symcst ("tcp-server-p",    new Function (net_tcpserverp));
    nset->symcst ("udp-server-p",    new Function (net_udpserverp));
    nset->symcst ("multicast-p",     new Function (net_multicastp));
    nset->symcst ("sock-params-p",   new Function (net_sockprmsp));
    
    // bind the network system calls
    nset->symcst ("get-loopback",    new Function (net_getloopback));
    nset->symcst ("get-tcp-service", new Function (net_gettcpserv));
    nset->symcst ("get-udp-service", new Function (net_getudpserv));
    nset->symcst ("isipn-p",         new Function (net_isipn));
    
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_net (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_net (interp, argv);
  }
}
