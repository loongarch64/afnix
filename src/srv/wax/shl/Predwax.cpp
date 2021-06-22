// ---------------------------------------------------------------------------
// - Predwax.cpp                                                             -
// - afnix:wax service - predicates implementation                           -
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
#include "Predwax.hpp"
#include "Boolean.hpp"
#include "XsmHtml.hpp"
#include "XhtmlH1.hpp"
#include "XhtmlH2.hpp"
#include "XhtmlHr.hpp"
#include "XhtmlBr.hpp"
#include "XhtmlUl.hpp"
#include "XhtmlTh.hpp"
#include "XhtmlTd.hpp"
#include "XhtmlTr.hpp"
#include "XhtmlCol.hpp"
#include "XhtmlCgr.hpp"
#include "XhtmlImg.hpp"
#include "XhtmlDiv.hpp"
#include "XhtmlPre.hpp"
#include "XhtmlRef.hpp"
#include "JsonMime.hpp"
#include "XhtmlMime.hpp"
#include "XhtmlMeta.hpp"
#include "XhtmlLink.hpp"
#include "XhtmlRoot.hpp"
#include "XhtmlPara.hpp"
#include "XhtmlEmph.hpp"
#include "XhtmlTxti.hpp"
#include "XhtmlTxtb.hpp"
#include "XhtmlTxtt.hpp"
#include "Exception.hpp"
#include "XhtmlForm.hpp"
#include "XhtmlText.hpp"
#include "XhtmlThead.hpp"
#include "XhtmlTbody.hpp"
#include "XhtmlTfoot.hpp"
#include "XhtmlTable.hpp"
#include "XhtmlTitle.hpp"
#include "XhtmlStyle.hpp"
#include "XhtmlCanvas.hpp"
#include "XhtmlSubmit.hpp"
#include "XhtmlScript.hpp"
#include "XhtmlOption.hpp"
#include "XhtmlHidden.hpp"
#include "XhtmlButton.hpp"
#include "XhtmlCaption.hpp"

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

  // xsmhp: xsm html object predicate

  Object* wax_xsmhp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xsm-html-p");
    bool result = (dynamic_cast <XsmHtml*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // jsonp: json mime object predicate
  
  Object* wax_jsonp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "json-mime-p");
    bool result = (dynamic_cast <JsonMime*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xmlmp: xml mime object predicate

  Object* wax_xmlmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xml-mime-p");
    bool result = (dynamic_cast <XmlMime*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // xhtmp: xhtml mime object predicate

  Object* wax_xhtmp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-mime-p");
    bool result = (dynamic_cast <XhtmlMime*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // rootp: xhtml root object predicate

  Object* wax_rootp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-root-p");
    bool result = (dynamic_cast <XhtmlRoot*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // htmlp: xhtml html object predicate

  Object* wax_htmlp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-html-p");
    bool result = (dynamic_cast <XhtmlHtml*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // headp: xhtml head object predicate

  Object* wax_headp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-head-p");
    bool result = (dynamic_cast <XhtmlHead*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // bodyp: xhtml body object predicate

  Object* wax_bodyp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-body-p");
    bool result = (dynamic_cast <XhtmlBody*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // thp: xhtml th object predicate

  Object* wax_thp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-th-p");
    bool result = (dynamic_cast <XhtmlTh*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tdp: xhtml td object predicate

  Object* wax_tdp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-td-p");
    bool result = (dynamic_cast <XhtmlTd*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // trp: xhtml tr object predicate

  Object* wax_trp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-tr-p");
    bool result = (dynamic_cast <XhtmlTr*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // colp: xhtml col object predicate

  Object* wax_colp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-col-p");
    bool result = (dynamic_cast <XhtmlCol*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // cgrp: xhtml cgr object predicate

  Object* wax_cgrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-cgr-p");
    bool result = (dynamic_cast <XhtmlCgr*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // captp: xhtml caption object predicate
  
  Object* wax_captp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-caption-p");
    bool result = (dynamic_cast <XhtmlCaption*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // telemp: xhtml table element object predicate

  Object* wax_telemp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-telem-p");
    bool result = (dynamic_cast <XhtmlTelem*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // theadp: xhtml table head object predicate

  Object* wax_theadp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-thead-p");
    bool result = (dynamic_cast <XhtmlThead*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tbodyp: xhtml table body object predicate

  Object* wax_tbodyp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-tbody-p");
    bool result = (dynamic_cast <XhtmlTbody*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tfootp: xhtml table foot object predicate

  Object* wax_tfootp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-ttoot-p");
    bool result = (dynamic_cast <XhtmlTfoot*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // tablep: xhtml table object predicate

  Object* wax_tablep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-table-p");
    bool result = (dynamic_cast <XhtmlTable*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // titlep: xhtml title object predicate

  Object* wax_titlep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-title-p");
    bool result = (dynamic_cast <XhtmlTitle*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // metap: xhtml meta object predicate

  Object* wax_metap (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-meta-p");
    bool result = (dynamic_cast <XhtmlMeta*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // linkp: xhtml link object predicate

  Object* wax_linkp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-link-p");
    bool result = (dynamic_cast <XhtmlLink*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // stylep: xhtml style object predicate

  Object* wax_stylep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-style-p");
    bool result = (dynamic_cast <XhtmlStyle*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // canvasp: xhtml canvas object predicate

  Object* wax_canvasp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-canvas-p");
    bool result = (dynamic_cast <XhtmlCanvas*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // scriptp: xhtml script object predicate

  Object* wax_scriptp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-script-p");
    bool result = (dynamic_cast <XhtmlScript*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // imgp: xhtml img object predicate

  Object* wax_imgp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-img-p");
    bool result = (dynamic_cast <XhtmlImg*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // divp: xhtml div object predicate

  Object* wax_divp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-div-p");
    bool result = (dynamic_cast <XhtmlDiv*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // prep: xhtml pre object predicate

  Object* wax_prep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-pre-p");
    bool result = (dynamic_cast <XhtmlPre*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // refp: xhtml ref object predicate

  Object* wax_refp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-ref-p");
    bool result = (dynamic_cast <XhtmlRef*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // parap: xhtml paragraph object predicate

  Object* wax_parap (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-para-p");
    bool result = (dynamic_cast <XhtmlPara*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // emphp: xhtml emphasize object predicate

  Object* wax_emphp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-emph-p");
    bool result = (dynamic_cast <XhtmlEmph*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // txtip: xhtml text italic object predicate

  Object* wax_txtip (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-i-p");
    bool result = (dynamic_cast <XhtmlTxti*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // txtbp: xhtml text bold object predicate

  Object* wax_txtbp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-b-p");
    bool result = (dynamic_cast <XhtmlTxtb*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // txttp: xhtml text teletype object predicate

  Object* wax_txttp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-tt-p");
    bool result = (dynamic_cast <XhtmlTxtt*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // h1p: xhtml h1 object predicate

  Object* wax_h1p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-h1-p");
    bool result = (dynamic_cast <XhtmlH1*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // h2p: xhtml h2 object predicate

  Object* wax_h2p (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-h2-p");
    bool result = (dynamic_cast <XhtmlH2*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
  
  // hrp: xhtml hr object predicate

  Object* wax_hrp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-hr-p");
    bool result = (dynamic_cast <XhtmlHr*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // brp: xhtml r object predicate

  Object* wax_brp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-br-p");
    bool result = (dynamic_cast <XhtmlBr*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // ulp: xhtml ul object predicate

  Object* wax_ulp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-ul-p");
    bool result = (dynamic_cast <XhtmlUl*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // lip: xhtml list item object predicate

  Object* wax_lip (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-li-p");
    bool result = (dynamic_cast <XhtmlLi*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // formp: xhtml form object predicate

  Object* wax_formp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-form-p");
    bool result = (dynamic_cast <XhtmlForm*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // formp: xhtml input text object predicate

  Object* wax_textp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-text-p");
    bool result = (dynamic_cast <XhtmlText*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // subtp: xhtml input submit object predicate

  Object* wax_subtp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-submit-p");
    bool result = (dynamic_cast <XhtmlSubmit*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // optnp: xhtml select option object predicate

  Object* wax_optnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-option-p");
    bool result = (dynamic_cast <XhtmlOption*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // hidep: xhtml input hidden object predicate

  Object* wax_hidep (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-hide-p");
    bool result = (dynamic_cast <XhtmlHidden*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }

  // butnp: xhtml button object predicate

  Object* wax_butnp (Evaluable* zobj, Nameset* nset, Cons* args) {
    Object* obj = get_obj (zobj, nset, args, "xhtml-button-p");
    bool result = (dynamic_cast <XhtmlButton*> (obj) == nullptr) ? false : true;
    Object::cref (obj);
    return new Boolean (result);
  }
}
