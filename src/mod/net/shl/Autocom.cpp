// ---------------------------------------------------------------------------
// - Autocom.cpp                                                             -
// - afnix:net module - autocom class implementation                         -
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

#include "Vector.hpp"
#include "System.hpp"
#include "Autocom.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the autocom structure
  struct s_acom {
    // the autocom type
    enum t_acom
      {
       ACOM_PRMS, // socket params
       ACOM_HOST, // socket host
       ACOM_ADDR, // socket address
      } d_acom;
    // the socket parameters
    SockParams d_prms;
    /// the socket host
    String d_host;
    /// the socket address
    Address d_addr;
    /// the socket port
    t_word d_port;
    // create by parameters
    s_acom (const SockParams& prms) {
      d_acom = ACOM_PRMS;
      d_prms = prms;
    }
    // create by host and port
    s_acom (const String& host, const t_word port) {
      d_acom = ACOM_HOST;
      d_host = host;
      d_port = port;
    }
    // create by address and port
    s_acom (const Address& addr, const t_word port) {
      d_acom = ACOM_ADDR;
      d_addr = addr;
      d_port = port;
    }
    // connect by connectable
    bool connect (Connectable* cobj) {
      // check for valid connectable
      if (cobj == nullptr) return false;
      // check for socket parameters
      if (d_acom == ACOM_PRMS) return cobj->connect (d_prms);
      // check for socket host
      if (d_acom == ACOM_HOST) return cobj->connect (d_host, d_port);
      // check for socket address
      if (d_acom == ACOM_ADDR) return cobj->connect (d_addr, d_port);
      // unreachable
      return false;
    }
  };

  // the autocom timer function
  static inline long acom_time_mark (const long t) {
    if (t <= 0L)  return 1L;
    if (t <= 10)  return 1L;
    if (t <= 50)  return 2L;
    if (t <= 100) return 10L;
    if (t <= 300) return 30L;
    if (t <= 600) return 60L;
    return 600L;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default autocom

  Autocom::Autocom (void) {
    p_acom = nullptr;
    p_cobj = nullptr;
  }

  // create an autocom by parameters
  
  Autocom::Autocom (const SockParams& prms) {
    // preset objects
    p_acom = nullptr;
    p_cobj = nullptr;
    // bind by parameters
    if (bind (prms) == false) {
      throw Exception ("autocom-error", "cannot create autocom by parameters");
    }
  }

  // create an autocom by parameters and connectable
  
  Autocom::Autocom (const SockParams& prms, Connectable* cobj) {
    // preset objects
    p_acom = nullptr;
    p_cobj = nullptr;
    // bind by parameters
    if (bind (prms) == false) {
      throw Exception ("autocom-error", "cannot create autocom by parameters");
    }
    if (bind (cobj) == false) {
      throw Exception ("autocom-error", "cannot create autocom by parameters");
    }
  }

  // create an autocom by host and port
  
  Autocom::Autocom (const String& host, const t_word port) {
    // preset objects
    p_acom = nullptr;
    p_cobj = nullptr;
    // bind by parameters
    if (bind (host, port) == false) {
      throw Exception ("autocom-error", "cannot create autocom by host");
    }
  }

  // create an autocom by address and port
  
  Autocom::Autocom (const Address& addr, const t_word port) {
    // preset objects
    p_acom = nullptr;
    p_cobj = nullptr;
    // bind by parameters
    if (bind (addr, port) == false) {
      throw Exception ("autocom-error", "cannot create autocom by parameters");
    }
  }

  // create an autocom by host, port and connectable
  
  Autocom::Autocom (const String& host, const t_word port, Connectable* cobj) {
    // preset objects
    p_acom = nullptr;
    p_cobj = nullptr;
    // bind by parameters
    if (bind (host, port) == false) {
      throw Exception ("autocom-error", "cannot create autocom by host");
    }
    if (bind (cobj) == false) {
      throw Exception ("autocom-error", "cannot create autocom by host");
    }
  }

  // create an autocom by address, port and connectable
  
  Autocom::Autocom (const Address& addr, const t_word port, Connectable* cobj) {
    // preset objects
    p_acom = nullptr;
    p_cobj = nullptr;
    // bind by parameters
    if (bind (addr, port) == false) {
      throw Exception ("autocom-error", "cannot create autocom by parameters");
    }
    if (bind (cobj) == false) {
      throw Exception ("autocom-error", "cannot create autocom by parameters");
    }
  }
  
  // destroy this autocom

  Autocom::~Autocom (void) {
    delete p_acom;
    Object::dref (p_cobj);
  }

  // return the class name
  
  String Autocom::repr (void) const {
    return "Autocom";
  }

  // run the autocom by a task
  
  void Autocom::run (void) noexcept {
    wrlock ();
    try {
      long tcnt = 0L;
      bool done = false;
      while (done == false) {
	try {
	  if (p_acom != nullptr) done = p_acom->connect (p_cobj);
	} catch (...) {
	  done = false;
	}
	if (done == false) {
	  tcnt += acom_time_mark (tcnt);
	  System::sleep (tcnt * 1000);
	} 
      }
      unlock ();
    } catch (...) {
      unlock ();
    }
  }

  // get the autocom result object

  Object* Autocom::toresult (void) const {
    return nullptr;
  }
  
  // bind the autocom by connectable

  bool Autocom::bind (Connectable* cobj) {
    wrlock ();
    try {
      bool result = false;
      if ((p_cobj == nullptr) && (cobj != nullptr)) {
	Object::iref (p_cobj = cobj);
	result = true;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    } 
  }
  
  // bind the autocom by parameters
  
  bool Autocom::bind (const SockParams& prms) {
    wrlock ();
    try {
      bool result = false;
      if (p_acom == nullptr) {
	p_acom = new s_acom(prms);
	result = true;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // bind the autocom by host and port

  bool Autocom::bind (const String& host, const t_word port) {
    wrlock ();
    try {
      bool result = false;
      if (p_acom == nullptr) {
	p_acom = new s_acom(host, port);
	result = true;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind the autocom by address and port

  bool Autocom::bind (const Address& addr, const t_word port) {
    wrlock ();
    try {
      bool result = false;
      if (p_acom == nullptr) {
	p_acom = new s_acom(addr, port);
	result = true;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
