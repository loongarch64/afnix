// ---------------------------------------------------------------------------
// - Predcsm.hpp                                                             -
// - afnix:csm service - predicates declaration                              -
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

#ifndef  AFNIX_PREDCSM_HPP
#define  AFNIX_PREDCSM_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:csm
  /// standard service.
  /// @author amaury darsch

  /// the part object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_partp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the blob object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_blobp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the bloc object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_blocp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the carrier blob object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_cblbp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the delegate blob object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_dblbp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the tracker blob object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_tckrp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the collection object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_collp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the domain object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_domnp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the whois object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_woisp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the whatis object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_waisp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the session object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_sessp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the session set object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_ssetp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the workzone object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_wzonp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the workspace object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_wspcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the localzone object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_lzonp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the localspace object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_lspcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the userspace object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_uspcp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the realm object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_relmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the realm zone object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_rzonp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the identity object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_idtyp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the authority object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_authp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the principal object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_pcplp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the act object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_xactp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the visa object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_visap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the credential object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_credp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the notary object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* csm_ntryp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
