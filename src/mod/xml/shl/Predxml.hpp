// ---------------------------------------------------------------------------
// - Predxml.hpp                                                             -
// - afnix:xml module - predicates declaration                               -
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

#ifndef  AFNIX_PREDXML_HPP
#define  AFNIX_PREDXML_HPP

#ifndef  AFNIX_OBJECT_HPP
#include "Object.hpp"
#endif
 
namespace afnix {

  /// this file contains the predicates associated with the afnix:xml
  /// standard module.
  /// @author amaury darsch

  /// the xml document object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_docp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml tag node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_tagp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_nodep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml root node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_rootp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml processing instruction node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_pindp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml declaration node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_declp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml document type node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_doctp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml comment node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_comtp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml text node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_textp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml cdata node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_datap (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml reference node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_xrefp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml cref node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_crefp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml eref node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_erefp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml ent node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_entp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml gent node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_gentp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml pent node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_pentp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml element node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_elemp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml attribute node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_attlp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml section node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_sectp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml reader object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_readp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xml texter object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_txtrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xne condition object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_condp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xne tree object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_treep (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xsm node object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_xsmnp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xsm reader object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_xsmrp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xsm document object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_xsmdp (Evaluable* zobj, Nameset* nset, Cons* args);

  /// the xso info object predicate
  /// @param zobj the current evaluable
  /// @param nset the current nameset
  /// @param args the arguments list
  Object* xml_xsoip (Evaluable* zobj, Nameset* nset, Cons* args);
}

#endif
