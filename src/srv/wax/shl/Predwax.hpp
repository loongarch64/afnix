// ---------------------------------------------------------------------------
// - Predwax.hpp                                                             -
// - afnix:wax service - predicates declaration                              -
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

#ifndef  AFNIX_PREDWAX_HPP
#define  AFNIX_PREDWAX_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix
  /// wax application management (wax) service.
  /// @author amaury darsch

  /// the xsm html object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_xsmhp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the json writer object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_jsonp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml mime object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_xmlmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml mime object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_xhtmp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml root object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_rootp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml html object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_htmlp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml head object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_headp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml body object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_bodyp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml th object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_thp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml td object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_tdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml tr object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_trp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml col object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_colp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml cgr object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_cgrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml caption object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_captp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml table element object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_telemp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml table head object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_theadp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml table body object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_tbodyp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml table foot object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_tfootp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml table object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_tablep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml title object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_titlep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml meta object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_metap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml link object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_linkp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml style object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_stylep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml canvas object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_canvasp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml script object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_scriptp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml img object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_imgp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml div object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_divp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml pre object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_prep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml ref object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_refp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml para object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_parap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml emph object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_emphp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml text italic object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_txtip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml text bold object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_txtbp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml text teletype object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_txttp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml h1 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_h1p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml h2 object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_h2p (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml hr object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_hrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml br object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_brp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml ul object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_ulp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml list item object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_lip (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml form object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_formp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml input text object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_textp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml input submit object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_subtp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml select option object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_optnp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml input hidden object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_hidep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xhtml button object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* wax_butnp (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
