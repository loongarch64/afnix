// ---------------------------------------------------------------------------
// - Stmdl.cpp                                                               -
// - afnix:sps module - streamable data localizer class implementation       -
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
#include "Stmdl.hpp"
#include "Vector.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default localizer

  Stmdl::Stmdl (void) {
    p_strm = nullptr;
    reset ();
  }

  // create a localizer by streamable

  Stmdl::Stmdl (Streamable* strm) {
    Object::iref (p_strm = strm);
    reset ();
  }
  
  // delete this localizer

  Stmdl::~Stmdl (void) {
    Object::dref (p_strm);
  }

  // return the object name

  String Stmdl::repr (void) const {
    return "Stmdl";
  }

  // reset the localizer

  void Stmdl::reset (void) {
    wrlock ();
    try {
      d_rpos = 0.0;
      if (p_strm != nullptr) {
	p_strm->reset ();
	p_strm->begin ();
	d_locd = p_strm->getreal ();
	p_strm->end ();
	d_loca = p_strm->getreal ();
	switch (d_lmth) {
	case LMTH_AFWD:
	case LMTH_RFWD:
	  p_strm->begin();
	  d_rpos = d_locd;
	  break;
	case LMTH_ABWD:
	case LMTH_RBWD:
	  p_strm->end ();
	  d_rpos = d_loca;
	  break;
	}	
      }	
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // validate a localized position
  
  bool Stmdl::validate (const t_real pval) const {
    rdlock ();
    try {
      bool result = Localizer::validate (pval);
      if (p_strm != nullptr) {
	switch (d_lmth) {
	case LMTH_AFWD:
	case LMTH_ABWD:
	  result = (pval >= d_locd) && (pval <= d_loca);
	  break;
	case LMTH_RFWD:
	  result = ((d_rpos + pval) >= d_locd) && ((d_rpos + pval) <= d_loca);
	  break;
	case LMTH_RBWD:
	  result = ((d_rpos - pval) >= d_locd) && ((d_rpos - pval) <= d_loca);
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // locate by boolean argument
  
  t_real Stmdl::locate (const bool pval) const {
    rdlock ();
    try {
      t_real result = Localizer::locate (pval);
      if (p_strm != nullptr) {
	switch (d_lmth) {
	case LMTH_AFWD:
	case LMTH_RFWD:
	  result = p_strm->forward (pval);
	  break;
	case LMTH_ABWD:
	case LMTH_RBWD:
	  result = p_strm->backward (pval);
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // locate by integer argument
  
  t_real Stmdl::locate (const long pval) const {
    rdlock ();
    try {
      t_real result = Localizer::locate (pval);
      if (p_strm != nullptr) {
	switch (d_lmth) {
	case LMTH_AFWD:
	  result = p_strm->forward (pval);
	  break;
	case LMTH_RFWD:
	  result = p_strm->forward ((long) (d_rpos + pval));
	  break;
	case LMTH_ABWD:
	  result = p_strm->backward (pval);
	  break;
	case LMTH_RBWD:
	  result = p_strm->backward ((long) (d_rpos - pval));
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // locate by real argument
  
  t_real Stmdl::locate (const t_real pval) const {
    rdlock ();
    try {
      t_real result = Localizer::locate (pval);
      if (p_strm != nullptr) {
	switch (d_lmth) {
	case LMTH_AFWD:
	  result = p_strm->forward (pval);
	  break;
	case LMTH_RFWD:
	  result = p_strm->forward (d_rpos + pval);
	  break;
	case LMTH_ABWD:
	  result = p_strm->backward (pval);
	  break;
	case LMTH_RBWD:
	  result = p_strm->backward (d_rpos - pval);
	  break;
	}
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the localized boolean by position
  
  bool Stmdl::getbool (const t_real pos) const {
    rdlock ();
    try {
      bool result = (p_strm == nullptr)
	? Localizer::getbool (pos)
	: p_strm->getbool (pos);
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the localized integer by position
  
  long Stmdl::getlong (const t_real pos) const {
    rdlock ();
    try {
      long result = (p_strm == nullptr)
	? Localizer::getlong (pos)
	: p_strm->getlong (pos);
      unlock ();
      return result;      
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the localized real by position
  
  t_real Stmdl::getreal (const t_real pos) const {
    rdlock ();
    try {
      t_real result = (p_strm == nullptr)
	? Localizer::getreal (pos)
	: p_strm->getreal (pos);
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

  // create a new object in a generic way

  Object* Stmdl::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Stmdl;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Streamable* strm = dynamic_cast <Streamable*> (obj);
      if (strm != nullptr) return new Stmdl (strm);
      throw Exception ("type-error",
		       "invalid argument with streamable localizer",
		       Object::repr (obj));
    }
    throw Exception ("argument-error",
		     "too many arguments with streamable localizer");
  }
}
