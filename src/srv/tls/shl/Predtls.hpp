// ---------------------------------------------------------------------------
// - Predtls.hpp                                                             -
// - afnix:tls service - predicates declaration                              -
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

#ifndef  AFNIX_PREDTLS_HPP
#define  AFNIX_PREDTLS_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix
  /// tls service.
  /// @author amaury darsch
 
  /// the tls block object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_blkp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls header object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_hdrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls record object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_recp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls message object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_msgp (Evaluable* zobj, Nameset* nset, Cons* args);
 
  /// the tls handshake message object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_shkp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls state object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_stap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls parameters object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_prmp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the tls connect object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_conp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls socket object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_sokp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls server object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_srvp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls client object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_cltp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls server hello object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_shlop (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls client hello object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_chlop (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls suite object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_suitep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tls certificate list object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_certsp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the X509 algorithm object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_xalgop (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pkcs object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_pkcsp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the pkcs1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_pkcs1p (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the X509 certificate object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_x509p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the pem object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* tls_pemp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
