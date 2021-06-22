// ---------------------------------------------------------------------------
// - Libxml.cpp                                                              -
// - afnix:xml module - declaration & implementation                         -
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
#include "XmlGe.hpp"
#include "XmlPe.hpp"
#include "Libxml.hpp"
#include "XmlTag.hpp"
#include "XmlRoot.hpp"
#include "XmlText.hpp"
#include "XmlData.hpp"
#include "XmlCref.hpp"
#include "XmlEref.hpp"
#include "Predxml.hpp"
#include "XneTree.hpp"
#include "XneCond.hpp"
#include "Function.hpp"
#include "XmlCalls.hpp"
#include "XsmReader.hpp"
#include "XmlReader.hpp"
#include "XmlTexter.hpp"
#include "XmlComment.hpp"
#include "XmlDoctype.hpp"
#include "XmlElement.hpp"
#include "XmlAttlist.hpp"
#include "XmlSection.hpp"
#include "XmlDocument.hpp"
#include "XsmDocument.hpp"

namespace afnix {

  // initialize the afnix:xml module

  Object* init_afnix_xml (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:xml nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("xml");

    // bind all symbols in the afnix:xml nameset
    gset->symcst ("XmlPi",          new Meta (XmlPi::mknew));
    gset->symcst ("XmlGe",          new Meta (XmlGe::mknew));
    gset->symcst ("XmlPe",          new Meta (XmlPe::mknew));
    gset->symcst ("XmlTag" ,        new Meta (XmlTag::mknew));
    gset->symcst ("XmlRoot",        new Meta (XmlRoot::mknew));
    gset->symcst ("XmlText",        new Meta (XmlText::mknew));
    gset->symcst ("XmlData",        new Meta (XmlData::mknew));
    gset->symcst ("XmlCref",        new Meta (XmlCref::mknew));
    gset->symcst ("XmlEref",        new Meta (XmlEref::mknew));
    gset->symcst ("XmlDecl",        new Meta (XmlDecl::mknew));
    gset->symcst ("XmlReader",      new Meta (XmlReader::mknew));
    gset->symcst ("XmlTexter",      new Meta (XmlTexter::mknew));
    gset->symcst ("XmlComment",     new Meta (XmlComment::mknew));
    gset->symcst ("XmlDoctype",     new Meta (XmlDoctype::mknew));
    gset->symcst ("XmlElement",     new Meta (XmlElement::mknew));
    gset->symcst ("XmlAttlist",     new Meta (XmlAttlist::mknew));
    gset->symcst ("XmlSection",     new Meta (XmlSection::mknew));
    gset->symcst ("XmlDocument",    new Meta (XmlDocument::mknew));

    gset->symcst ("Xne",            new Meta (Xne::meval));
    gset->symcst ("XneTree",        new Meta (XneTree::mknew));
    gset->symcst ("XneCond",        new Meta (XneCond::mknew));

    gset->symcst ("XsmNode",        new Meta (XsmNode::meval, XsmNode::mknew));
    gset->symcst ("XsmReader",      new Meta (XsmReader::mknew));
    gset->symcst ("XsmDocument",    new Meta (XsmDocument::mknew));

    gset->symcst ("XsoInfo",        new Meta (XsoInfo::mknew));

    // bind the predicates
    gset->symcst ("pi-p",           new Function (xml_pindp));
    gset->symcst ("ge-p",           new Function (xml_gentp));
    gset->symcst ("pe-p",           new Function (xml_pentp));
    gset->symcst ("tag-p",          new Function (xml_tagp));
    gset->symcst ("node-p",         new Function (xml_nodep));
    gset->symcst ("root-p",         new Function (xml_rootp));
    gset->symcst ("text-p",         new Function (xml_textp));
    gset->symcst ("data-p",         new Function (xml_datap));
    gset->symcst ("cref-p",         new Function (xml_crefp));
    gset->symcst ("eref-p",         new Function (xml_erefp));
    gset->symcst ("decl-p",         new Function (xml_declp));
    gset->symcst ("reader-p",       new Function (xml_readp));
    gset->symcst ("texter-p",       new Function (xml_txtrp));
    gset->symcst ("entity-p",       new Function (xml_entp));
    gset->symcst ("comment-p",      new Function (xml_comtp));
    gset->symcst ("doctype-p",      new Function (xml_doctp));
    gset->symcst ("element-p",      new Function (xml_elemp));
    gset->symcst ("attlist-p",      new Function (xml_attlp));
    gset->symcst ("section-p",      new Function (xml_sectp));
    gset->symcst ("document-p",     new Function (xml_docp));
    gset->symcst ("reference-p",    new Function (xml_xrefp));

    gset->symcst ("xne-tree-p",     new Function (xml_treep));
    gset->symcst ("xne-cond-p",     new Function (xml_condp));

    gset->symcst ("xsm-node-p",     new Function (xml_xsmnp));
    gset->symcst ("xsm-reader-p",   new Function (xml_xsmrp));
    gset->symcst ("xsm-document-p", new Function (xml_xsmdp));

    gset->symcst ("xso-info-p",     new Function (xml_xsoip));
    
    // bind other functions
    gset->symcst ("to-normal",      new Function (xml_tonorm));
    gset->symcst ("pre-normal",     new Function (xml_prenorm));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_xml (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_xml (interp, argv);
  }
}
