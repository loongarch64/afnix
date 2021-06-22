// ---------------------------------------------------------------------------
// - Libitu.cpp                                                              -
// - afnix:itu module - declaration & implementation                         -
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
#include "Libitu.hpp"
#include "AsnEoc.hpp"
#include "AsnGtm.hpp"
#include "AsnUtc.hpp"
#include "AsnIas.hpp"
#include "AsnOid.hpp"
#include "AsnSet.hpp"
#include "AsnNull.hpp"
#include "AsnRoid.hpp"
#include "AsnBits.hpp"
#include "AsnNums.hpp"
#include "AsnPrts.hpp"
#include "AsnBmps.hpp"
#include "AsnUnvs.hpp"
#include "AsnUtfs.hpp"
#include "Preditu.hpp"
#include "Function.hpp"
#include "AsnBuffer.hpp"
#include "AsnRandom.hpp"
#include "AsnOctets.hpp"
#include "AsnBoolean.hpp"
#include "AsnInteger.hpp"
#include "AsnSequence.hpp"

namespace afnix {

  // initialize the afnix:itu module

  Object* init_afnix_itu (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:itu nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* gset = aset->mknset   ("itu");

    // bind all symbols in the afnix:itu nameset
    gset->symcst ("AsnEoc",            new Meta (AsnEoc::mknew));
    gset->symcst ("AsnGtm",            new Meta (AsnGtm::mknew));
    gset->symcst ("AsnUtc",            new Meta (AsnUtc::mknew));
    gset->symcst ("AsnIas",            new Meta (AsnIas::mknew));
    gset->symcst ("AsnOid",            new Meta (AsnOid::mknew));
    gset->symcst ("AsnSet",            new Meta (AsnSet::mknew));
    gset->symcst ("AsnNode",           new Meta (AsnNode::meval));
    gset->symcst ("AsnNull",           new Meta (AsnNull::mknew));
    gset->symcst ("AsnNums",           new Meta (AsnNums::mknew));
    gset->symcst ("AsnPrts",           new Meta (AsnPrts::mknew));
    gset->symcst ("AsnBmps",           new Meta (AsnBmps::mknew));
    gset->symcst ("AsnUnvs",           new Meta (AsnUnvs::mknew));
    gset->symcst ("AsnUtfs",           new Meta (AsnUtfs::mknew));
    gset->symcst ("AsnRoid",           new Meta (AsnRoid::mknew));
    gset->symcst ("AsnBits",           new Meta (AsnBits::mknew));
    gset->symcst ("AsnBuffer",         new Meta (AsnBuffer::mknew));
    gset->symcst ("AsnOctets",         new Meta (AsnOctets::mknew));
    gset->symcst ("AsnBoolean",        new Meta (AsnBoolean::mknew));
    gset->symcst ("AsnInteger",        new Meta (AsnInteger::mknew));
    gset->symcst ("AsnInteger",        new Meta (AsnInteger::mknew));
    gset->symcst ("AsnSequence",       new Meta (AsnSequence::mknew));

    // bind the asn predicates
    gset->symcst ("asn-eoc-p",         new Function (itu_asneocp));
    gset->symcst ("asn-gtm-p",         new Function (itu_asngtmp));
    gset->symcst ("asn-utc-p",         new Function (itu_asnutcp));
    gset->symcst ("asn-ias-p",         new Function (itu_asniasp));
    gset->symcst ("asn-oid-p",         new Function (itu_asnoidp));
    gset->symcst ("asn-set-p",         new Function (itu_asnsetp));
    gset->symcst ("asn-node-p",        new Function (itu_asnnodep));
    gset->symcst ("asn-null-p",        new Function (itu_asnnullp));
    gset->symcst ("asn-nums-p",        new Function (itu_asnnumsp));
    gset->symcst ("asn-prts-p",        new Function (itu_asnprtsp));
    gset->symcst ("asn-bmps-p",        new Function (itu_asnbmpsp));
    gset->symcst ("asn-unvs-p",        new Function (itu_asnunvsp));
    gset->symcst ("asn-utfs-p",        new Function (itu_asnutfsp));
    gset->symcst ("asn-roid-p",        new Function (itu_asnroidp));
    gset->symcst ("asn-bits-p",        new Function (itu_asnbitsp));
    gset->symcst ("asn-buffer-p",      new Function (itu_asnbufp));
    gset->symcst ("asn-octets-p",      new Function (itu_asnoctsp));
    gset->symcst ("asn-boolean-p",     new Function (itu_asnboolp));
    gset->symcst ("asn-integer-p",     new Function (itu_asnintgp));
    gset->symcst ("asn-sequence-p",    new Function (itu_asnseqp));

    // bind the itu objects
    gset->symcst ("Oid",               new Meta (Oid::mknew));

    // bind the itu predicates
    gset->symcst ("oid-p",             new Function (itu_oidp));

    // bind the special functions
    gset->symcst ("asn-random-bits",   new Function (itu_asnbrnd));
    gset->symcst ("asn-random-octets", new Function (itu_asnornd));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_itu (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_itu (interp, argv);
  }
}
