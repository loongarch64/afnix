// ---------------------------------------------------------------------------
// - Libsys.cpp                                                              -
// - afnix:sys module - declaration & implementation                         -
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
#include "Date.hpp"
#include "Meter.hpp"
#include "Libsys.hpp"
#include "Options.hpp"
#include "Predsys.hpp"
#include "SysCalls.hpp"
#include "Function.hpp"

namespace afnix {

  // initialize the afnix:sys module

  Object* init_afnix_sys (Interp* interp, Vector* argv) {
    // make sure we are not called from something crazy
    if (interp == nullptr) return nullptr;

    // create the afnix:sys nameset
    Nameset* aset = interp->mknset ("afnix");
    Nameset* nset = aset->mknset   ("sys");

    // bind all classes in the afnix:sys nameset
    nset->symcst ("Time",               new Meta (Time::mknew));
    nset->symcst ("Date",               new Meta (Date::mknew));
    nset->symcst ("Meter",              new Meta (Meter::mknew));
    nset->symcst ("Options",            new Meta (Options::mknew));

    // bind all predicates in the afnix:sys nameset
    nset->symcst ("time-p",             new Function (sys_timep));
    nset->symcst ("date-p",             new Function (sys_datep));
    nset->symcst ("meter-p",            new Function (sys_pmtrp));
    nset->symcst ("options-p",          new Function (sys_optsp));

    // bind all sys calls in the afnix:sys nameset
    nset->symcst ("exit",               new Function (sys_exit));
    nset->symcst ("sleep",              new Function (sys_sleep));
    nset->symcst ("get-pid",            new Function (sys_getpid));
    nset->symcst ("get-env",            new Function (sys_getenv));
    nset->symcst ("get-option",         new Function (sys_getopt));
    nset->symcst ("get-unique-id",      new Function (sys_uniqid));
    nset->symcst ("get-host-fqdn",      new Function (sys_hostfqdn));
    nset->symcst ("get-domain-name",    new Function (sys_domainname));
    nset->symcst ("get-host-name",      new Function (sys_hostname));
    nset->symcst ("get-user-name",      new Function (sys_username));
    nset->symcst ("get-user-home",      new Function (sys_userhome));

    // not used but needed
    return nullptr;
  }
}

extern "C" {
  afnix::Object* dli_afnix_sys (afnix::Interp* interp, afnix::Vector* argv) {
    return init_afnix_sys (interp, argv);
  }
}
