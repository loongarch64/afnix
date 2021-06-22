// ---------------------------------------------------------------------------
// - Libtls.cpp                                                              -
// - afnix:tls service - declaration & implementation                        -
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

#include "Pem.hpp"
#include "Meta.hpp"
#include "X509.hpp"
#include "Pkcs1.hpp"
#include "Libtls.hpp"
#include "Predtls.hpp"
#include "Function.hpp"
#include "TlsSuite.hpp"
#include "TlsCerts.hpp"
#include "TlsShake.hpp"
#include "TlsRecord.hpp"
#include "TlsServer.hpp"
#include "TlsClient.hpp"
#include "TlsShello.hpp"
#include "TlsChello.hpp"
#include "TlsMessage.hpp"
#include "TlsConnect.hpp"

namespace afnix {

  // initialize the afnix:tls service

  Object* init_afnix_tls (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:tls nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("tls");

    // bind all symbols in the afnix:tls nameset
    gset->symcst ("Pki",                new Meta (Pki::meval));
    gset->symcst ("Pem",                new Meta (Pem::meval, Pem::mknew));
    gset->symcst ("X509",               new Meta (X509::mknew));
    gset->symcst ("Pkcs1",              new Meta (Pkcs1::mknew));
    gset->symcst ("Xalgo",              new Meta (Xalgo::mknew)); 
    gset->symcst ("TlsShake",           new Meta (TlsShake::mknew));
    gset->symcst ("TlsSuite",           new Meta (TlsSuite::mknew));
    gset->symcst ("TlsCerts",           new Meta (TlsCerts::mknew));
    gset->symcst ("TlsState",           new Meta (TlsState::mknew));
    gset->symcst ("TlsParams",          new Meta (TlsParams::mknew));
    gset->symcst ("TlsRecord",          new Meta (TlsRecord::mknew));
    gset->symcst ("TlsServer",          new Meta (TlsServer::mknew));
    gset->symcst ("TlsClient",          new Meta (TlsClient::mknew));
    gset->symcst ("TlsShello",          new Meta (TlsShello::mknew));
    gset->symcst ("TlsChello",          new Meta (TlsChello::mknew));
    gset->symcst ("TlsSocket",          new Meta (TlsSocket::mknew));
    gset->symcst ("TlsConnect",         new Meta (TlsConnect::mknew));
    gset->symcst ("TlsMessage",         new Meta (TlsMessage::mknew));

    // bind the predicates
    gset->symcst ("pem-p",              new Function (tls_pemp));
    gset->symcst ("x509-p",             new Function (tls_x509p));
    gset->symcst ("pkcs-p",             new Function (tls_pkcsp));
    gset->symcst ("pkcs-1-p",           new Function (tls_pkcs1p));
    gset->symcst ("xalgo-p",            new Function (tls_xalgop));
    gset->symcst ("suite-p",            new Function (tls_suitep));
    gset->symcst ("block-p",            new Function (tls_blkp));
    gset->symcst ("state-p",            new Function (tls_stap));
    gset->symcst ("params-p",           new Function (tls_prmp));
    gset->symcst ("header-p",           new Function (tls_hdrp));
    gset->symcst ("record-p",           new Function (tls_recp));
    gset->symcst ("socket-p",           new Function (tls_sokp));
    gset->symcst ("server-p",           new Function (tls_srvp));
    gset->symcst ("client-p",           new Function (tls_cltp));
    gset->symcst ("connect-p",          new Function (tls_conp));
    gset->symcst ("message-p",          new Function (tls_msgp));
    gset->symcst ("handshake-p",        new Function (tls_shkp));
    gset->symcst ("server-hello-p",     new Function (tls_shlop));
    gset->symcst ("client-hello-p",     new Function (tls_chlop));
    gset->symcst ("certificate-list-p", new Function (tls_certsp));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_tls (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_tls (interp, argv);
  }
}
