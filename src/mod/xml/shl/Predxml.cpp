// ---------------------------------------------------------------------------
// - Predxml.cpp                                                             -
// - afnix:xml module - predicates implementation                            -
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
#include "XmlGe.hpp"
#include "XmlPe.hpp"
#include "XmlTag.hpp"
#include "XmlRoot.hpp"
#include "XmlText.hpp"
#include "XmlData.hpp"
#include "XmlCref.hpp"
#include "XmlEref.hpp"
#include "Predxml.hpp"
#include "Boolean.hpp"
#include "XneTree.hpp"
#include "XneCond.hpp"
#include "XsmReader.hpp"
#include "XmlReader.hpp"
#include "XmlTexter.hpp"
#include "Exception.hpp"
#include "XmlComment.hpp"
#include "XmlDoctype.hpp"
#include "XmlElement.hpp"
#include "XmlAttlist.hpp"
#include "XmlSection.hpp"
#include "XmlDocument.hpp"
#include "XsmDocument.hpp"

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

  // docp: xml document object predicate

  Object* xml_docp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "document-p");
    bool result = (dynamic_cast <XmlDocument*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tagp: xml tag node object predicate

  Object* xml_tagp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "tag-p");
    bool result = (dynamic_cast <XmlTag*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // nodep: xml node object predicate

  Object* xml_nodep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "node-p");
    bool result = (dynamic_cast <XmlNode*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rootp: xml root node object predicate

  Object* xml_rootp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "root-p");
    bool result = (dynamic_cast <XmlRoot*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pindp: xml processing instruction node object predicate

  Object* xml_pindp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pi-p");
    bool result = (dynamic_cast <XmlPi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // declp: xml declaration node node object predicate

  Object* xml_declp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "decl-p");
    bool result = (dynamic_cast <XmlDecl*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // doctp: xml document type node node object predicate

  Object* xml_doctp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "doctype-p");
    bool result = (dynamic_cast <XmlDoctype*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }


  // comtp: xml comment node object predicate

  Object* xml_comtp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "comment-p");
    bool result = (dynamic_cast <XmlComment*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // textp: xml text node object predicate

  Object* xml_textp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "text-p");
    bool result = (dynamic_cast <XmlText*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // datap: xml cdata node object predicate

  Object* xml_datap  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "data-p");
    bool result = (dynamic_cast <XmlData*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xrefp: xml reference node object predicate

  Object* xml_xrefp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "reference-p");
    bool result = (dynamic_cast <XmlRef*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // crefp: xml cref node object predicate

  Object* xml_crefp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "cref-p");
    bool result = (dynamic_cast <XmlCref*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // erefp: xml eref node object predicate

  Object* xml_erefp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "eref-p");
    bool result = (dynamic_cast <XmlEref*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // entp: xml entity node object predicate

  Object* xml_entp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "entity-p");
    bool result = (dynamic_cast <XmlEntity*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // gentp: xml general entity node object predicate

  Object* xml_gentp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "ge-p");
    bool result = (dynamic_cast <XmlGe*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // pentp: xml parameter entity node object predicate

  Object* xml_pentp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "pe-p");
    bool result = (dynamic_cast <XmlPe*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // elemp: xml element node object predicate

  Object* xml_elemp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "element-p");
    bool result = (dynamic_cast <XmlElement*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // attlp: xml attribute list node object predicate

  Object* xml_attlp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "attlist-p");
    bool result = (dynamic_cast <XmlAttlist*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // sectp: xml section node object predicate

  Object* xml_sectp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "section-p");
    bool result = (dynamic_cast <XmlSection*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // readp: xml reader object predicate

  Object* xml_readp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "reader-p");
    bool result = (dynamic_cast <XmlReader*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // txtrp: xml texter object predicate

  Object* xml_txtrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "texter-p");
    bool result = (dynamic_cast <XmlTexter*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // treep: xne tree object predicate

  Object* xml_treep  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xne-tree-p");
    bool result = (dynamic_cast <XneTree*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // condp: xne condition object predicate

  Object* xml_condp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xne-cond-p");
    bool result = (dynamic_cast <XneCond*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xsmnp: xsm node object predicate

  Object* xml_xsmnp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xsm-node-p");
    bool result = (dynamic_cast <XsmNode*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xsmnp: xsm reader object predicate

  Object* xml_xsmrp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xsm-reader-p");
    bool result = (dynamic_cast <XsmReader*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xsmdp: xsm document object predicate

  Object* xml_xsmdp  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xsm-document-p");
    bool result = (dynamic_cast <XsmDocument*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xsoip: xso info object predicate

  Object* xml_xsoip  (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xso-info-p");
    bool result = (dynamic_cast <XsoInfo*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
