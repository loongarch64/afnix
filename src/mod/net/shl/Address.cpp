// ---------------------------------------------------------------------------
// - Address.cpp                                                             -
// - afnix:net module - address class implementation                         -
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
#include "Boolean.hpp"
#include "Address.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cnet.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the string address protocol
  static const String SAPF_XANY = "46";
  static const String SAPF_IPV4 = "4";
  static const String SAPF_IPV6 = "6";

  // map a string address protocol
  static t_iapf addr_toiapf (const String& sapf) {
    // check for null
    if (sapf.isnil () == true) return IAPF_XANY;
    // check for any
    if (sapf == SAPF_XANY) return IAPF_XANY;
    // check for version 4
    if (sapf == SAPF_IPV4) return IAPF_IPV4;
    // check for version 6
    if (sapf == SAPF_IPV6) return IAPF_IPV6;
    // invalid address protocol family
    throw Exception ("address-error", "invalid string protocol family", sapf);
  }
    
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // get the loopback name

  String Address::loopname (void) {
    String result = c_loopname ();
    return result;
  }

  // get the tcp service port by name

  t_word Address::tservice (const String& name) {
    char* data = nullptr;
    try {
      data = name.tochar ();
      t_word port = c_ipserv (data, true);
      delete [] data;
      if (port == 0) {
        throw Exception ("service-error", "cannot find tcp service", name);
      }
      return port;
    } catch (...) {
      delete [] data;
      throw;
    }
  }

  // get the tcp service port by name

  t_word Address::uservice (const String& name) {
    char* data = nullptr;
    try {
      data = name.tochar ();
      t_word port = c_ipserv (data, false);
      delete [] data;
      if (port == 0) {
        throw Exception ("service-error", "cannot find udp service", name);
      }
      return port;
    } catch (...) {
      delete [] data;
      throw;
    }
  }

  // check for a numerical address

  bool Address::isipn (const String& name, const String& sapf) {
    char* data = nullptr;
    try {
      data = name.tochar ();
      bool result = c_isipn (data, addr_toiapf (sapf));
      delete [] data;
      return result;
    } catch (...) {
      delete [] data;
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty address

  Address::Address (void) {
    p_addr = nullptr;
    d_size = 0;
    p_aals = nullptr;
  }

  // create a new address by name

  Address::Address (const String& name) {
    p_addr = nullptr;
    d_size = 0;
    p_aals = nullptr;
    resolve (name, false);
  }

  // create a new address by name and resolve lookup

  Address::Address (const String& name, const bool rflg) {
    p_addr = nullptr;
    d_size = 0;
    p_aals = nullptr;
    resolve (name, rflg);
  }
  
  // create a new address by name, resolve lookup and protocol family

  Address::Address (const String& name, const bool rflg, const String& sapf) {
    p_addr = nullptr;
    d_size = 0;
    p_aals = nullptr;
    resolve (name, rflg, sapf);
  }
  
  // create a new address by bytes

  Address::Address (const t_byte* addr) {
    p_addr = nullptr;
    d_size = 0;
    p_aals = nullptr;
    resolve (addr, false);
  }

  // create a new address by bytes and resolve flag

  Address::Address (const t_byte* addr, const bool rflg) {
    p_addr = nullptr;
    d_size = 0;
    p_aals = nullptr;
    resolve (addr, rflg);
  }

  // copy construct an address

  Address::Address (const Address& that) {
    that.rdlock ();
    try {
      // save canonical info
      d_name = that.d_name;
      d_cnam = that.d_cnam;
      p_addr = c_cpaddr (that.p_addr);
      // save aliases
      if (that.d_size > 0) {
	d_size = that.d_size;
	p_aals = new s_alias[d_size];
	for (long i = 0; i < d_size; i++) {
	  p_aals[i].d_anam = that.p_aals[i].d_anam;
	  p_aals[i].p_aadr = c_cpaddr (that.p_aals[i].p_aadr);
	}
      } else {
	d_size = 0;
	p_aals = nullptr;
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this address instance

  Address::~Address (void) {
    delete [] p_addr;
    delete [] p_aals;
  }

  // return the class name

  String Address::repr (void) const {
    return "Address";
  }

  // return a clone of this address

  Object* Address::clone (void) const {
    return new Address (*this);
  }

  // assign an address to this one

  Address& Address::operator = (const Address& that) {
    // check for equality
    if (this == &that) return *this;
    // lock and copy
    wrlock ();
    that.rdlock ();
    try {
      // delete old address
      delete [] p_addr;
      // save canonical info
      d_name = that.d_name;
      d_cnam = that.d_cnam;
      p_addr = c_cpaddr (that.p_addr);
      // save aliases
      if (that.d_size > 0) {
	d_size = that.d_size;
	p_aals = new s_alias[d_size];
	for (long i = 0; i < d_size; i++) {
	  p_aals[i].d_anam = that.p_aals[i].d_anam;
	  p_aals[i].p_aadr = c_cpaddr (that.p_aals[i].p_aadr);
	}
      } else {
	d_size = 0;
	p_aals = nullptr;
      }
      // unlock eveything
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }

  // prefix increase this address by one position

  Address& Address::operator ++ (void) {
    wrlock ();
    t_byte* addr = nullptr;
    // try to resolv the new address
    try {
      addr = c_nxaddr (p_addr);
      resolve (addr, true);
      delete [] addr;
      // here is the new address
      unlock ();
      return *this;
    } catch (...) {
      delete [] addr;
      unlock ();
      throw;
    }
  }

  // postfix increase this address by one position

  Address Address::operator ++ (int) {
    wrlock ();
    t_byte* addr = nullptr;
    try {
      Address result = *this;
      // get the next address
      addr = c_nxaddr (p_addr);
      // try to resolv the new address
      resolve (addr, true);
      delete [] addr;
      // here is the new address
      unlock ();
      return result;
    } catch (...) {
      delete [] addr;
      unlock ();
      throw;
    }
  }

  // compare two addresses

  bool Address::operator == (const Address& addr) const {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = c_eqaddr (p_addr, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // compare two addresses

  bool Address::operator != (const Address& addr) const {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = !c_eqaddr (p_addr, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // compare two addresses

  bool Address::operator < (const Address& addr) const {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = c_ltaddr (p_addr, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // compare two addresses

  bool Address::operator <= (const Address& addr) const {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = c_leaddr (p_addr, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // compare two addresses

  bool Address::operator > (const Address& addr) const {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = ! (*this <= addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }


  bool Address::operator >= (const Address& addr) const {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = ! (*this < addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // get the next available address but do not resolve

  void Address::next (void) {
    wrlock ();
    t_byte* addr = nullptr;
    try {
      // get the next address
      addr = c_nxaddr (p_addr);
      // resolve the address
      resolve (addr, false);
      delete [] addr;
      unlock ();
    } catch (...) {
      delete [] addr;
      throw;
    }
  }

  // resolve an existing address

  void Address::resolve (const bool rflg) {
    wrlock ();
    try {
      if ((rflg == true) && (p_addr != nullptr)) {
	char* cnam = c_getipn (p_addr, false);
	if (cnam != nullptr) {
	  d_cnam = cnam;
	  delete [] cnam;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the address by name and reverse lookup flag

  void Address::resolve (const String& name, const bool rflg) {
    wrlock ();
    try {
      resolve (name, rflg, "");
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the address by name and reverse lookup flag

  void Address::resolve (const String& name, const bool rflg,
			 const String& sapf) {
    wrlock ();
    char*     host = nullptr;
    s_ipaddr* addr = nullptr;
    try {
      // clean old address
      d_size = 0;
      delete [] p_addr; p_addr = nullptr;
      delete [] p_aals; p_aals = nullptr;
      // get the ip address info
      host = name.tochar ();
      addr = c_getipa (host, addr_toiapf (sapf));
      delete [] host; host = nullptr;
      // check for good address
      if ((addr == nullptr) || (addr->d_size == 0)) {
	throw Exception ("address-error", "cannot resolve address", name);
      }
      // save canonical info
      d_name = name;
      d_cnam = addr->p_name[0];
      p_addr = c_cpaddr (addr->p_addr[0]);
      // eventually do a reverse lookup
      resolve (rflg);
      // save aliases
      if (addr->d_size > 1) {
	d_size = addr->d_size - 1;
	p_aals = new s_alias[d_size];
	for (long i = 0; i < d_size; i++) {
	  p_aals[i].d_anam = addr->p_name[i+1];
	  p_aals[i].p_aadr = c_cpaddr (addr->p_addr[i+1]);
	}
      } else {
	d_size = 0;
	p_aals = nullptr;
      }
      delete addr;
      unlock ();
    } catch (...) {
      delete [] host;
      delete addr;
      unlock ();
      throw;
    }
  }

  // resolve an address by bytes and resolve flag

  void Address::resolve (const t_byte* addr, const bool rflg) {
    // do nothing if nil
    if (addr == nullptr) return;
    wrlock ();
    char* host = nullptr;
    try {
      // clean old address
      d_size = 0;
      delete [] p_addr; p_addr = nullptr;
      delete [] p_aals; p_aals = nullptr;
      // get the host name representation
      host   = c_getipn (addr, true);
      d_name = host;
      d_cnam = host;
      delete [] host; host = nullptr;
      // save the byte address form
      p_addr = c_cpaddr (addr);
      // eventually do a reverse lookup
      resolve (rflg);
      unlock ();
    } catch (...) {
      delete [] host;
      unlock ();
      throw;
    }
  }

  // return the address canonical name

  String Address::getcanon (void) const {
    rdlock ();
    try {
      String result = d_cnam;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the address representation

  String Address::getaddr (void) const {
    rdlock ();
    char* data = nullptr;
    try {
      data = c_getipn (p_addr, true);
      String result = data;
      delete [] data; data = nullptr;
      unlock ();
      return result;
    } catch (...) {
      delete [] data;
      unlock ();
      throw;
    }
  }

  // get an integer vector representation
  Vector* Address::getvmap (void) const {
    rdlock ();
    Vector* result = nullptr;
    try {
      if (p_addr == nullptr) {
	unlock ();
	return nullptr;
      }
      long vlen = (long) p_addr[0] + 1;
      result = new Vector;
      for (long i = 1; i < vlen; i++) result->add (new Integer (p_addr[i]));
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // return the alias size

  long Address::getsize (void) const {
    rdlock ();
    try {
      long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the address canonical name by index

  String Address::getcanon (const long index) const {
    rdlock ();
    try {
      if ((index < 0) || (index >= d_size)) {
	throw Exception ("index-error", "out-of-bound address alias index");
      }
      String result = p_aals[index].d_anam;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the address representation

  String Address::getaddr (const long index) const {
    rdlock ();
    char* data = nullptr;
    try {
      if ((index < 0) || (index >= d_size)) {
	throw Exception ("index-error", "out-of-bound address alias index");
      }
      data = c_getipn (p_aals[index].p_aadr, true);
      String result = data;
      delete [] data; data = nullptr;
      unlock ();
      return result;
    } catch (...) {
      delete [] data;
      unlock ();
      throw;
    }
  }

  // get an integer vector representation
  Vector* Address::getvmap (const long index) const {
    rdlock ();
    Vector* result = nullptr;
    try {
      if ((index < 0) || (index >= d_size)) {
	throw Exception ("index-error", "out-of-bound address alias index");
      }
      const t_byte* aadr = p_aals[index].p_aadr;
      if (aadr == nullptr) {
	unlock ();
	return nullptr;
      }
      long vlen = (long) aadr[0] + 1;
      result = new Vector;
      for (long i = 1; i < vlen; i++) {
	result->add (new Integer (aadr[i]));
      }
      unlock ();
      return result;
    } catch (...) {
      delete result;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 14;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OPP      = zone.intern ("++");
  static const long QUARK_EQL      = zone.intern ("==");
  static const long QUARK_NEQ      = zone.intern ("!=");
  static const long QUARK_LTH      = zone.intern ("<");
  static const long QUARK_GEQ      = zone.intern ("<=");
  static const long QUARK_GTH      = zone.intern ("<");
  static const long QUARK_LEQ      = zone.intern ("<=");
  static const long QUARK_NEXT     = zone.intern ("next");
  static const long QUARK_RESOLVE  = zone.intern ("resolve");
  static const long QUARK_GETNAME  = zone.intern ("get-name");
  static const long QUARK_GETADDR  = zone.intern ("get-address");
  static const long QUARK_GETVMAP  = zone.intern ("get-vector");
  static const long QUARK_GETSIZE  = zone.intern ("get-alias-size");
  static const long QUARK_GETANAM  = zone.intern ("get-alias-name");
  static const long QUARK_GETAADR  = zone.intern ("get-alias-address");
  static const long QUARK_GETAMAP  = zone.intern ("get-alias-vector");
  static const long QUARK_GETCANON = zone.intern ("get-canonical-name");

  // create a new object in a generic way
 
  Object* Address::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Address (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      bool   rflg = argv->getbool   (1);
      return new Address (name, rflg);
    }
    // check for 3 arguments
    if (argc == 3) {
      String name = argv->getstring (0);
      bool   rflg = argv->getbool   (1);
      String sapf = argv->getstring (2);
      return new Address (name, rflg, sapf);
    }
    throw Exception ("argument-error",
                     "too many argument with address constructor");
  }
 
  // return true if the given quark is defined

  bool Address::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Address::oper (t_oper type, Object* object) {
    Address* addr = dynamic_cast <Address*> (object);
    switch (type) {
    case Object::OPER_EQL:
      if (addr != nullptr) return new Boolean (*this == *addr);
      break;
    case Object::OPER_NEQ:
      if (addr != nullptr) return new Boolean (*this != *addr);
      break;
    case Object::OPER_LEQ:
      if (addr != nullptr) return new Boolean (*this <= *addr);
      break;
    case Object::OPER_LTH:
      if (addr != nullptr) return new Boolean (*this < *addr);
      break;
    case Object::OPER_GTH:
      if (addr != nullptr) return new Boolean (*this > *addr);
      break;
    case Object::OPER_GEQ:
      if (addr != nullptr) return new Boolean (*this >= *addr);
      break;
    default:
      throw Exception ("operator-error", "unsupported address operator");
    }
    throw Exception ("type-error", "invalid operand with address",
                     Object::repr (object));
  }

  // apply this object with a set of arguments and a quark
 
  Object* Address::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {

    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
 
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETNAME)  return new String  (d_name);
      if (quark == QUARK_GETCANON) return new String  (getcanon ());
      if (quark == QUARK_GETADDR)  return new String  (getaddr  ());
      if (quark == QUARK_GETSIZE)  return new Integer (getsize  ());
      if (quark == QUARK_GETVMAP)  return getvmap ();
      if (quark == QUARK_OPP) {
        ++(*this);
        zobj->post (this);
        return this;
      }
      if (quark == QUARK_NEXT) {
	next ();
	return nullptr;
      }
      if (quark == QUARK_RESOLVE) {
	resolve (true);
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_LTH) return oper (Object::OPER_LTH, argv->get (0));
      if (quark == QUARK_LEQ) return oper (Object::OPER_LEQ, argv->get (0));
      if (quark == QUARK_GTH) return oper (Object::OPER_GTH, argv->get (0));
      if (quark == QUARK_GEQ) return oper (Object::OPER_GEQ, argv->get (0));

      if (quark == QUARK_RESOLVE) {
	String name = argv->getstring (0);
	resolve (name, false);
	return nullptr;
      }
      if (quark == QUARK_GETANAM) {
	long index = argv->getlong (0);
	return new String (getcanon (index));
      }
      if (quark == QUARK_GETAADR) {
	long index = argv->getlong (0);
	return new String (getaddr (index));
      }
      if (quark == QUARK_GETAMAP) {
	long index = argv->getlong (0);
	return getvmap (index);
      }
    }
    if (argc == 2) {
      if (quark == QUARK_RESOLVE) {
	String name = argv->getstring (0);
	bool   rflg = argv->getbool   (1);
	resolve (name, rflg);
	return nullptr;
      }
    }
    if (argc == 3) {
      if (quark == QUARK_RESOLVE) {
	String name = argv->getstring (0);
	bool   rflg = argv->getbool   (1);
	String sapf = argv->getstring (2);
	resolve (name, rflg, sapf);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
