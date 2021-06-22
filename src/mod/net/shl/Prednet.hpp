// ---------------------------------------------------------------------------
// - Prednet.hpp                                                             -
// - afnix:net module - predicates declaration                               -
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

#ifndef  AFNIX_PREDNET_HPP
#define  AFNIX_PREDNET_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:sys
  /// standard module.
  /// @author amaury darsch

  /// the address object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_addressp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the socket option object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_sockoptp   (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the socket parameters predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_sockprmsp  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the socket object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_socketp    (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the datagram object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_datagramp  (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tcp socket object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_tcpsocketp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the udp socket object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_udpsocketp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tcp client object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_tcpclientp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the udp client object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_udpclientp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tcp server object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_tcpserverp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the udp server object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_udpserverp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the multicast object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* net_multicastp (Evaluable* zobj, Nameset* nset, Cons* args);
}


#endif
