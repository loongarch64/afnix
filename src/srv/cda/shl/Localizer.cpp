// ---------------------------------------------------------------------------
// - Localizer.cpp                                                           -
// - afnix:cda service - localizer interface implementation                  -
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

#include "Math.hpp"
#include "Item.hpp"
#include "Real.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Localizer.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - interface section                                                     -
  // -------------------------------------------------------------------------

  // create a default localizer

  Localizer::Localizer (void) {
    d_lmth = LMTH_AFWD;
  }
  
  // copy move this localizer

  Localizer::Localizer (Localizer&& that) noexcept :
    // copy base object
    Object (static_cast<Object&&>(that)) {
    that.wrlock ();
    try {
      // copy locally
      d_lmth = that.d_lmth; that.d_lmth = LMTH_AFWD;
      that.unlock ();
    } catch (...) {
      d_lmth = LMTH_AFWD;
      that.unlock ();
    } 
  }
  
  // move this localizer into this one

  Localizer& Localizer::operator = (Localizer&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // move base object
      Object::operator = (static_cast<Object&&>(that));
      // move locally
      d_lmth = that.d_lmth; that.d_lmth = LMTH_AFWD;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_lmth = LMTH_AFWD;
      unlock ();
      that.unlock ();
    } 
    return *this;
  }

  // set the localization method

  void Localizer::setlmth (const t_lmth lmth) {
    wrlock ();
    try {
      d_lmth = lmth;
      reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the localization method

  Localizer::t_lmth Localizer::getlmth (void) const {
    rdlock ();
    try {
      t_lmth result = d_lmth;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a localized position

  bool Localizer::validate (const t_real pval) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // locate by boolean argument

  t_real Localizer::locate (const bool pval) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // locate by integer argument

  t_real Localizer::locate (const long pval) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // locate by real argument

  t_real Localizer::locate (const t_real pval) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the localized boolean by position

  bool Localizer::getbool (const t_real pos) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the localized integer by position

  long Localizer::getlong (const t_real pos) const {
    rdlock ();
    try {
      long result = 0L;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the localized real by position

  t_real Localizer::getreal (const t_real pos) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the object eval quarks
  static const long QUARK_LOCL = String::intern ("Localizer");
  static const long QUARK_AFWD = String::intern ("ABSOLUTE-FORWARD");
  static const long QUARK_ABWD = String::intern ("ABSOLUTE-BACKWARD");
  static const long QUARK_RFWD = String::intern ("RELATIVE-FORWARD");
  static const long QUARK_RBWD = String::intern ("RELATIVE-BACKWARD");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_LOCATE   = zone.intern ("locate");
  static const long QUARK_GETREAL  = zone.intern ("get-real");
  static const long QUARK_GETBOOL  = zone.intern ("get-boolean");
  static const long QUARK_GETLONG  = zone.intern ("get-integer");
  static const long QUARK_SETLMTH  = zone.intern ("set-localization-method");
  static const long QUARK_GETLMTH  = zone.intern ("get-localization-method");
  static const long QUARK_VALIDATE = zone.intern ("validate");

  // map an item to a localization method
  static inline Localizer::t_lmth item_to_lmth (const Item& item) {
    // check for a key item
    if (item.gettid () != QUARK_LOCL)
      throw Exception ("item-error", "item is not a localizer item");
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_AFWD) return Localizer::LMTH_AFWD;
    if (quark == QUARK_ABWD) return Localizer::LMTH_ABWD;
    if (quark == QUARK_RFWD) return Localizer::LMTH_RFWD;
    if (quark == QUARK_RBWD) return Localizer::LMTH_RBWD;
    throw Exception ("item-error", "cannot map item to localizer method");
  }

  // map a localization method to an item
  static inline Item* lmth_to_item (const Localizer::t_lmth lmth) {
    Item* result = nullptr;
    switch (lmth) {
    case Localizer::LMTH_AFWD:
      result = new Item (QUARK_LOCL, QUARK_AFWD);
      break;
    case Localizer::LMTH_ABWD:
      result = new Item (QUARK_LOCL, QUARK_ABWD);
      break;
    case Localizer::LMTH_RFWD:
      result = new Item (QUARK_LOCL, QUARK_RFWD);
      break;
    case Localizer::LMTH_RBWD:
      result = new Item (QUARK_LOCL, QUARK_RBWD);
      break;
    }
    return result;
  }
  
  // evaluate an object data member

  Object* Localizer::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_AFWD)
      return new Item (QUARK_LOCL, QUARK_AFWD);
    if (quark == QUARK_ABWD)
      return new Item (QUARK_LOCL, QUARK_ABWD);
    if (quark == QUARK_RFWD)
      return new Item (QUARK_LOCL, QUARK_RFWD);
    if (quark == QUARK_RBWD)
      return new Item (QUARK_LOCL, QUARK_RBWD);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Localizer::isquark (const long quark, const bool hflg) const {
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

  // apply this object with a set of arguments and a quark

  Object* Localizer::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0L) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_GETLMTH) {
	return lmth_to_item (getlmth ());
      }
    }    
    // dispatch 1 argument
    if (argc == 1L) {
      if (quark == QUARK_LOCATE) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new Real (locate (bobj->tobool ()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new Real (locate ((long) iobj->tolong ()));
	// check for a real
	Real* lobj = dynamic_cast <Real*> (obj);
	if (lobj != nullptr) return new Real (locate (lobj->toreal ()));
	// invalid object
	throw Exception ("type-error", "invalid object for locate",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETREAL) {
	t_real pos = argv->getreal(0);
	return new Real (getreal (pos));
      }
      if (quark == QUARK_GETBOOL) {
	t_real pos = argv->getreal(0);
	return new Boolean (getbool (pos));
      }
      if (quark == QUARK_GETLONG) {
	t_real pos = argv->getreal(0);
	return new Integer (getlong (pos));
      }
      if (quark == QUARK_SETLMTH) {
	// check for an item type
        Object* obj = argv->get (0);
        Item*  iobj = dynamic_cast <Item*> (obj);
        if (iobj == nullptr) {
          throw Exception ("type-error", 
                           "invalid object with set-localization-method",
                           Object::repr (obj));
        }
        setlmth (item_to_lmth (*iobj));
	return nullptr;
      }
      if (quark == QUARK_VALIDATE) {
	t_real pval  = argv->getreal(0);
	return new Boolean (validate (pval));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
