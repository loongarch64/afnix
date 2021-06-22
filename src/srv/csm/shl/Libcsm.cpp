// ---------------------------------------------------------------------------
// - Libcsm.cpp                                                              -
// - afnix:csm service - declaration & implementation                        -
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
#include "Visa.hpp"
#include "Csmsrl.hxx"
#include "Libcsm.hpp"
#include "Domain.hpp"
#include "Notary.hpp"
#include "Tracker.hpp"
#include "Predcsm.hpp"
#include "CsmCalls.hpp"
#include "Function.hpp"
#include "Delegate.hpp"
#include "LocalZone.hpp"
#include "RealmZone.hpp"
#include "UserSpace.hpp"
#include "SessionSet.hpp"

namespace afnix {

  // initialize the afnix:csm service

  Object* init_afnix_csm (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;
    // bind the serial dispatcher
    if (Serial::isvdid (SRL_DEOD_DID) == false) {
      throw Exception ("interp-error", "invalid csm serial dispatcher");
    }
    // create the afnix:csm nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("csm");

    // bind all symbols in the afnix:csm nameset
    gset->symcst ("Act",              new Meta (Act::mknew));
    gset->symcst ("Blob",             new Meta (Blob::mknew));
    gset->symcst ("Bloc",             new Meta (Bloc::mknew));
    gset->symcst ("Visa",             new Meta (Visa::mknew));
    gset->symcst ("Part",             new Meta (Part::mknew));
    gset->symcst ("Whois",            new Meta (Whois::mknew));
    gset->symcst ("Whatis",           new Meta (Whatis::mknew));
    gset->symcst ("Domain",           new Meta (Domain::mknew));
    gset->symcst ("Notary",           new Meta (Notary::mknew));
    gset->symcst ("Carrier",          new Meta (Carrier::mknew));
    gset->symcst ("Tracker",          new Meta (Tracker::mknew));
    gset->symcst ("Session",          new Meta (Session::mknew)); 
    gset->symcst ("Identity",         new Meta (Identity::mknew));
    gset->symcst ("Delegate",         new Meta (Delegate::mknew));
    gset->symcst ("LocalZone",        new Meta (LocalZone::mknew));
    gset->symcst ("RealmZone",        new Meta (RealmZone::mknew));
    gset->symcst ("UserSpace",        new Meta (UserSpace::mknew));
    gset->symcst ("Authority",        new Meta (Authority::mknew));
    gset->symcst ("Principal",        new Meta (Principal::mknew));
    gset->symcst ("Collection",       new Meta (Collection::mknew));
    gset->symcst ("LocalSpace",       new Meta (LocalSpace::mknew));
    gset->symcst ("SessionSet",       new Meta (SessionSet::mknew));
    gset->symcst ("Credential",       new Meta (Credential::meval,
						Credential::mknew));

    // bind the predicates
    gset->symcst ("act-p",            new Function (csm_xactp));
    gset->symcst ("part-p",           new Function (csm_partp));
    gset->symcst ("blob-p",           new Function (csm_blobp));
    gset->symcst ("bloc-p",           new Function (csm_blocp));
    gset->symcst ("visa-p",           new Function (csm_visap));
    gset->symcst ("realm-p",          new Function (csm_relmp));
    gset->symcst ("whois-p",          new Function (csm_woisp));
    gset->symcst ("whatis-p",         new Function (csm_waisp));
    gset->symcst ("domain-p",         new Function (csm_domnp));
    gset->symcst ("notary-p",         new Function (csm_ntryp));
    gset->symcst ("carrier-p",        new Function (csm_cblbp));
    gset->symcst ("tracker-p",        new Function (csm_tckrp));
    gset->symcst ("session-p",        new Function (csm_sessp));
    gset->symcst ("identity-p",       new Function (csm_idtyp));
    gset->symcst ("delegate-p",       new Function (csm_dblbp));
    gset->symcst ("workzone-p",       new Function (csm_wzonp));
    gset->symcst ("workspace-p",      new Function (csm_wspcp));
    gset->symcst ("localzone-p",      new Function (csm_lzonp));
    gset->symcst ("realmzone-p",      new Function (csm_rzonp));
    gset->symcst ("userspace-p",      new Function (csm_uspcp));
    gset->symcst ("authority-p",      new Function (csm_authp));
    gset->symcst ("principal-p",      new Function (csm_pcplp));
    gset->symcst ("collection-p",     new Function (csm_collp));
    gset->symcst ("localspace-p",     new Function (csm_lspcp));
    gset->symcst ("credential-p",     new Function (csm_credp));
    gset->symcst ("session-set-p",    new Function (csm_ssetp));

    // bind other functions
    gset->symcst ("to-workspace-uri", new Function (csm_tozuri));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_csm (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_csm (interp, argv);
  }
}
