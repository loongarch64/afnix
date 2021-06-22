// ---------------------------------------------------------------------------
// - Libtxt.cpp                                                              -
// - afnix:txt module - declaration & implementation                         -
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

#include "Trie.hpp"
#include "Meta.hpp"
#include "Worder.hpp"
#include "Libtxt.hpp"
#include "Predtxt.hpp"
#include "Scanner.hpp"
#include "Lexicon.hpp"
#include "TxtCalls.hpp"
#include "Literate.hpp"
#include "Function.hpp"

namespace afnix {

  // initialize the afnix:txt module

  Object* init_afnix_txt (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    
    // create the afnix:sys nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* nset = aset->mknset   ("txt");
    
    // bind all classes in the afnix:txt nameset 
    nset->symcst ("Trie",           new Meta (Trie::mknew));
    nset->symcst ("Worder",         new Meta (Worder::mknew));
    nset->symcst ("Lexeme",         new Meta (Lexeme::mknew));
    nset->symcst ("Pattern",        new Meta (Pattern::meval, Pattern::mknew));
    nset->symcst ("Scanner",        new Meta (Scanner::mknew));
    nset->symcst ("Lexicon",        new Meta (Lexicon::mknew));
    nset->symcst ("Literate",       new Meta (Literate::mknew));

    // bind all predicates in the afnix:txt nameset
    nset->symcst ("trie-p",         new Function (txt_triep));
    nset->symcst ("worder-p",       new Function (txt_wordp));
    nset->symcst ("lexeme-p",       new Function (txt_lexp));
    nset->symcst ("pattern-p",      new Function (txt_patp));
    nset->symcst ("scanner-p",      new Function (txt_scanp));
    nset->symcst ("lexicon-p",      new Function (txt_tlexp));
    nset->symcst ("literate-p",     new Function (txt_tlitp));

    // bind other functions
    nset->symcst ("indent",         new Function (txt_indent));
    nset->symcst ("sort-ascent",    new Function (txt_qslth));
    nset->symcst ("sort-descent",   new Function (txt_qsgth));
    nset->symcst ("sort-lexical",   new Function (txt_qslex));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_txt (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_txt (interp, argv);
  }
}
