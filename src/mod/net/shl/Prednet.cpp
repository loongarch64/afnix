// ---------------------------------------------------------------------------
// - Prednet.cpp                                                             -
// - afnix:net module - predicates implementation                            -
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

#include "Cons.hpp"
#include "Prednet.hpp"
#include "Address.hpp"
#include "Boolean.hpp"
#include "TcpClient.hpp"
#include "UdpClient.hpp"
#include "TcpServer.hpp"
#include "UdpServer.hpp"
#include "Multicast.hpp"
#include "Exception.hpp"

namespace afnix {

  // this procedure checks that we have one argument only and returns
  // the evaluated object
  static inline Object* get_obj (Evaluable* zobj, Nameset* nset, Cons* args,
                                 const String& pname) {
    Object* car = nullptr;
    if ((args == nullptr) || (args->length () != 1))
      throw Exception ("argument-error", "illegal arguments with predicate",
                       pname);
    car = args->getcar ();
    return (car == nullptr) ? nullptr : car->eval (zobj,nset);
  }

  // addressp: address object predicate

  Object* net_addressp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "address-p");
    bool result =  (dynamic_cast <Address*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // sockoptp: socket option object predicate

  Object* net_sockoptp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sockopt-p");
    bool result =  (dynamic_cast <Sockopt*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sockprmsp: socket parameters object predicate

  Object* net_sockprmsp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "sock-params-p");
    bool result =  (dynamic_cast <SockParams*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // socketp: socket object predicate

  Object* net_socketp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "socket-p");
    bool result =  (dynamic_cast <Socket*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // datagramp: datagram object predicate

  Object* net_datagramp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "datagram-p");
    bool result =  (dynamic_cast <Datagram*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tcpsocketp: tcp socket object predicate

  Object* net_tcpsocketp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tcp-socket-p");
    bool result =  (dynamic_cast <TcpSocket*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // udpsocketp: udp socket object predicate

  Object* net_udpsocketp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "udp-socket-p");
    bool result =  (dynamic_cast <UdpSocket*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tcpclientp: tcp client object predicate

  Object* net_tcpclientp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tcp-client-p");
    bool result =  (dynamic_cast <TcpClient*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // udpclientp: udp client object predicate

  Object* net_udpclientp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "udp-client-p");
    bool result =  (dynamic_cast <UdpClient*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tcpserverp: tcp server object predicate

  Object* net_tcpserverp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tcp-server-p");
    bool result =  (dynamic_cast <TcpServer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // udpserverp: udp server object predicate

  Object* net_udpserverp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "udp-server-p");
    bool result =  (dynamic_cast <UdpServer*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // multicastp: multicast object predicate

  Object* net_multicastp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "multicast-p");
    bool result =  (dynamic_cast <Multicast*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

}
