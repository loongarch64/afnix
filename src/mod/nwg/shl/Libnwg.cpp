// ---------------------------------------------------------------------------
// - Libnwg.cpp                                                              -
// - afnix:nwg module - declaration & implementation                         -
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
#include "Uuid.hpp"
#include "Json.hpp"
#include "Nwgsrl.hxx"
#include "Libnwg.hpp"
#include "Basexx.hpp"
#include "Prednwg.hpp"
#include "UriPath.hpp"
#include "UriQuery.hpp"
#include "NwgCalls.hpp"
#include "Function.hpp"
#include "Hyperlink.hpp"
#include "HttpStream.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

namespace afnix {

  // initialize the afnix:nwg module

  Object* init_afnix_nwg (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid nwg serial dispatcher");
    }
    // create the afnix:nwg nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("nwg");

    // bind all symbols in the afnix:nwg nameset
    gset->symcst ("Uri",                new Meta (Uri::mknew));
    gset->symcst ("Uuid",               new Meta (Uuid::mknew));
    gset->symcst ("Json",               new Meta (Json::mknew));
    gset->symcst ("Cookie",             new Meta (Cookie::mknew));
    gset->symcst ("Basexx",             new Meta (Basexx::meval,
						  Basexx::mknew));
    gset->symcst ("UriPath",            new Meta (UriPath::mknew));
    gset->symcst ("UriQuery",           new Meta (UriQuery::mknew));
    gset->symcst ("Hyperlink",          new Meta (Hyperlink::mknew));
    gset->symcst ("CookieJar",          new Meta (CookieJar::mknew));
    gset->symcst ("HttpStream",         new Meta (HttpStream::mknew));
    gset->symcst ("HttpRequest",        new Meta (HttpRequest::mknew));
    gset->symcst ("HttpResponse",       new Meta (HttpResponse::mknew));

    // bind the predicates
    gset->symcst ("uri-p",              new Function (nwg_urip));
    gset->symcst ("mime-p",             new Function (nwg_mimep));
    gset->symcst ("uuid-p",             new Function (nwg_uuidp));
    gset->symcst ("json-p",             new Function (nwg_jsonp));
    gset->symcst ("cookie-p",           new Function (nwg_cookp));
    gset->symcst ("basexx-p",           new Function (nwg_bsxxp));
    gset->symcst ("uri-path-p",         new Function (nwg_uripp));
    gset->symcst ("uri-query-p",        new Function (nwg_uriqp));
    gset->symcst ("hyperlink-p",        new Function (nwg_hlnkp));
    gset->symcst ("cookie-jar-p",       new Function (nwg_cjarp));
    gset->symcst ("mime-xml-p",         new Function (nwg_xmlmp));
    gset->symcst ("mime-value-p",       new Function (nwg_mvalp));
    gset->symcst ("http-proto-p",       new Function (nwg_protop));
    gset->symcst ("http-stream-p",      new Function (nwg_hstrmp));
    gset->symcst ("http-request-p",     new Function (nwg_hrqstp));
    gset->symcst ("http-response-p",    new Function (nwg_hrespp));
    gset->symcst ("mime-extension-p",   new Function (nwg_mextp));
    
    // bind other functions
    gset->symcst ("uri-encode",         new Function (nwg_uencd));
    gset->symcst ("percent-encode",     new Function (nwg_pencd));
    gset->symcst ("component-encode",   new Function (nwg_cencd));
    gset->symcst ("percent-decode",     new Function (nwg_pdecd));
    gset->symcst ("path-uri-name",      new Function (nwg_pthunm));
    gset->symcst ("string-uri-p",       new Function (nwg_surip));
    gset->symcst ("to-string-uri",      new Function (nwg_tosuri));
    gset->symcst ("system-uri-name",    new Function (nwg_sysunm));
    gset->symcst ("extension-to-mime",  new Function (nwg_tomime));
    gset->symcst ("mime-to-extension",  new Function (nwg_tomext));
    gset->symcst ("normalize-uri-name", new Function (nwg_nrmunm));
    gset->symcst ("normalize-uri-host", new Function (nwg_nrmhost));
    gset->symcst ("normalize-uri-port", new Function (nwg_nrmport));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_nwg (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_nwg (interp, argv);
  }
}
