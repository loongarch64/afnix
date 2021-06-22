// ---------------------------------------------------------------------------
// - Prednwg.hpp                                                             -
// - afnix:nwg module - predicates declaration                               -
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

#ifndef  AFNIX_PREDNWG_HPP
#define  AFNIX_PREDNWG_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:nwg
  /// standard module.
  /// @author amaury darsch

  /// the uri object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_urip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the uri path object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_uripp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the uri query object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_uriqp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the mime document object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_mimep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the hyperlink object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_hlnkp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the uuid object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_uuidp (Evaluable* zobj, Nameset* nset, Cons* args);
  
  /// the http proto object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_protop (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the http request object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_hrqstp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the http response object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_hrespp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the http stream object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_hstrmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cookie object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_cookp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the cookie jar object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_cjarp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the basexx codec object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_bsxxp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the json object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* nwg_jsonp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
