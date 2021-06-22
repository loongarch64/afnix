// ---------------------------------------------------------------------------
// - Libwax.cpp                                                              -
// - afnix:wax service - declaration & implementation                        -
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
#include "Libwax.hpp"
#include "Predwax.hpp"
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
#include "Function.hpp"
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
#include "XhtmlForm.hpp"
#include "XhtmlText.hpp"
#include "XhtmlThead.hpp"
#include "XhtmlTbody.hpp"
#include "XhtmlTfoot.hpp"
#include "XhtmlTable.hpp"
#include "XhtmlTitle.hpp"
#include "XhtmlStyle.hpp"
#include "XhtmlCanvas.hpp"
#include "XhtmlScript.hpp"
#include "XhtmlSubmit.hpp"
#include "XhtmlOption.hpp"
#include "XhtmlHidden.hpp"
#include "XhtmlButton.hpp"
#include "XhtmlCaption.hpp"

namespace afnix {

  // initialize the afnix:wax module

  Object* init_afnix_wax (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:wax nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("wax");

    // bind all symbols in the afnix:wax nameset
    gset->symcst ("XhtmlH1",         new Meta (XhtmlH1::mknew));
    gset->symcst ("XhtmlH2",         new Meta (XhtmlH2::mknew));
    gset->symcst ("XhtmlHr",         new Meta (XhtmlHr::mknew));
    gset->symcst ("XhtmlBr",         new Meta (XhtmlBr::mknew));
    gset->symcst ("XhtmlUl",         new Meta (XhtmlUl::mknew));
    gset->symcst ("XhtmlLi",         new Meta (XhtmlLi::mknew));
    gset->symcst ("XhtmlTh",         new Meta (XhtmlTh::mknew));
    gset->symcst ("XhtmlTd",         new Meta (XhtmlTd::mknew));
    gset->symcst ("XhtmlTr",         new Meta (XhtmlTr::mknew));
    gset->symcst ("XhtmlCol",        new Meta (XhtmlCol::mknew));
    gset->symcst ("XhtmlCgr",        new Meta (XhtmlCgr::mknew));
    gset->symcst ("XhtmlImg",        new Meta (XhtmlImg::mknew));
    gset->symcst ("XhtmlDiv",        new Meta (XhtmlDiv::mknew));
    gset->symcst ("XhtmlPre",        new Meta (XhtmlPre::mknew));
    gset->symcst ("XhtmlRef",        new Meta (XhtmlRef::mknew));
    gset->symcst ("XhtmlMime",       new Meta (XhtmlMime::mknew));
    gset->symcst ("XhtmlMeta",       new Meta (XhtmlMeta::mknew));
    gset->symcst ("XhtmlLink",       new Meta (XhtmlLink::mknew));
    gset->symcst ("XhtmlRoot",       new Meta (XhtmlRoot::mknew));
    gset->symcst ("XhtmlHtml",       new Meta (XhtmlHtml::mknew));
    gset->symcst ("XhtmlHead",       new Meta (XhtmlHead::mknew));
    gset->symcst ("XhtmlBody",       new Meta (XhtmlBody::mknew));
    gset->symcst ("XhtmlPara",       new Meta (XhtmlPara::mknew));
    gset->symcst ("XhtmlEmph",       new Meta (XhtmlEmph::mknew));
    gset->symcst ("XhtmlTxti",       new Meta (XhtmlTxti::mknew));
    gset->symcst ("XhtmlTxtb",       new Meta (XhtmlTxtb::mknew));
    gset->symcst ("XhtmlTxtt",       new Meta (XhtmlTxtt::mknew));
    gset->symcst ("XhtmlForm",       new Meta (XhtmlForm::mknew));
    gset->symcst ("XhtmlText",       new Meta (XhtmlText::mknew));
    gset->symcst ("XhtmlThead",      new Meta (XhtmlThead::mknew));
    gset->symcst ("XhtmlTbody",      new Meta (XhtmlTbody::mknew));
    gset->symcst ("XhtmlTfoot",      new Meta (XhtmlTfoot::mknew));
    gset->symcst ("XhtmlTable",      new Meta (XhtmlTable::mknew));
    gset->symcst ("XhtmlTitle",      new Meta (XhtmlTitle::mknew));
    gset->symcst ("XhtmlStyle",      new Meta (XhtmlStyle::mknew));
    gset->symcst ("XhtmlCanvas",     new Meta (XhtmlCanvas::mknew));
    gset->symcst ("XhtmlScript",     new Meta (XhtmlScript::mknew));
    gset->symcst ("XhtmlSubmit",     new Meta (XhtmlSubmit::mknew));
    gset->symcst ("XhtmlOption",     new Meta (XhtmlOption::mknew));
    gset->symcst ("XhtmlHidden",     new Meta (XhtmlHidden::mknew));
    gset->symcst ("XhtmlButton",     new Meta (XhtmlButton::mknew));
    gset->symcst ("XhtmlCaption",    new Meta (XhtmlCaption::mknew));

    gset->symcst ("XsmHtml",         new Meta (XsmHtml::mknew));
    gset->symcst ("XmlMime",         new Meta (XmlMime::mknew));
    gset->symcst ("JsonMime",        new Meta (JsonMime::mknew));

    // bind the predicates
    gset->symcst ("xhtml-i-p",       new Function (wax_txtip));
    gset->symcst ("xhtml-b-p",       new Function (wax_txtbp));
    gset->symcst ("xhtml-tt-p",      new Function (wax_txttp));
    gset->symcst ("xhtml-h1-p",      new Function (wax_h1p));
    gset->symcst ("xhtml-h2-p",      new Function (wax_h2p));
    gset->symcst ("xhtml-hr-p",      new Function (wax_hrp));
    gset->symcst ("xhtml-br-p",      new Function (wax_brp));
    gset->symcst ("xhtml-ul-p",      new Function (wax_ulp));
    gset->symcst ("xhtml-li-p",      new Function (wax_lip));
    gset->symcst ("xhtml-th-p",      new Function (wax_thp));
    gset->symcst ("xhtml-td-p",      new Function (wax_tdp));
    gset->symcst ("xhtml-tr-p",      new Function (wax_trp));
    gset->symcst ("xhtml-col-p",     new Function (wax_colp));
    gset->symcst ("xhtml-cgr-p",     new Function (wax_cgrp));
    gset->symcst ("xhtml-img-p",     new Function (wax_imgp));
    gset->symcst ("xhtml-div-p",     new Function (wax_divp));
    gset->symcst ("xhtml-pre-p",     new Function (wax_prep));
    gset->symcst ("xhtml-ref-p",     new Function (wax_refp));
    gset->symcst ("xhtml-mime-p",    new Function (wax_xhtmp));
    gset->symcst ("xhtml-meta-p",    new Function (wax_metap));
    gset->symcst ("xhtml-link-p",    new Function (wax_linkp));
    gset->symcst ("xhtml-root-p",    new Function (wax_rootp));
    gset->symcst ("xhtml-html-p",    new Function (wax_htmlp));
    gset->symcst ("xhtml-head-p",    new Function (wax_headp));
    gset->symcst ("xhtml-body-p",    new Function (wax_bodyp));
    gset->symcst ("xhtml-para-p",    new Function (wax_parap));
    gset->symcst ("xhtml-emph-p",    new Function (wax_emphp));
    gset->symcst ("xhtml-telem-p",   new Function (wax_telemp));
    gset->symcst ("xhtml-thead-p",   new Function (wax_theadp));
    gset->symcst ("xhtml-tbody-p",   new Function (wax_tbodyp));
    gset->symcst ("xhtml-tfoot-p",   new Function (wax_tfootp));
    gset->symcst ("xhtml-table-p",   new Function (wax_tablep));
    gset->symcst ("xhtml-title-p",   new Function (wax_titlep));
    gset->symcst ("xhtml-style-p",   new Function (wax_stylep));
    gset->symcst ("xhtml-canvas-p",  new Function (wax_canvasp));
    gset->symcst ("xhtml-script-p",  new Function (wax_scriptp));
    gset->symcst ("xhtml-option-p",  new Function (wax_optnp));
    gset->symcst ("xhtml-hidden-p",  new Function (wax_hidep));
    gset->symcst ("xhtml-button-p",  new Function (wax_butnp));
    gset->symcst ("xhtml-caption-p", new Function (wax_captp));

    gset->symcst ("xsm-html-p",      new Function (wax_xsmhp));
    gset->symcst ("xml-mime-p",      new Function (wax_xmlmp));
    gset->symcst ("json-mime-p",     new Function (wax_jsonp));
    
    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_wax (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_wax (interp, argv);
  }
}
