// ---------------------------------------------------------------------------
// - Style.cpp                                                               -
// - standard object library - style class  implementation                   -
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

#include "Item.hpp"
#include "Style.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default ellipsis
  static const String STL_ELPS_T = "....";
  
  // the text weight byte code
  static const t_byte STL_WGHT_N = 0x00U;
  static const t_byte STL_WGHT_B = 0x01U;
  static const t_byte STL_WGHT_I = 0x02U;
  // the text alignment byte code
  static const t_byte STL_ALGN_L = 0x00U;
  static const t_byte STL_ALGN_C = 0x01U;
  static const t_byte STL_ALGN_R = 0x02U;

  // this procedure converts a weight to a byte code
  static t_byte stl_tobyte (const Style::t_wght wght) {
    t_byte result = 0x00U;
    switch (wght) {
    case Style::WGHT_N:
      result = STL_WGHT_N;
      break;
    case Style::WGHT_B:
      result = STL_WGHT_B;
      break;
    case Style::WGHT_I:
      result = STL_WGHT_I;
      break;
    }
    return result;
  }

  // this procedure convert a byte code to a weight
  static Style::t_wght stl_towght (const t_byte code) {
    switch (code) {
    case STL_WGHT_N:
      return Style::WGHT_N;
      break;
    case STL_WGHT_B:
      return Style::WGHT_B;
      break;
    case STL_WGHT_I:
      return Style::WGHT_I;
      break;
    default:
      break;
    }
    throw Exception ("style-error", "invalid text style serialization code");
  }

  // this procedure converts an alignment to a byte code
  static t_byte stl_tobyte (const Style::t_algn algn) {
    t_byte result = 0x00U;
    switch (algn) {
    case Style::ALGN_L:
      result = STL_ALGN_L;
      break;
    case Style::ALGN_C:
      result = STL_ALGN_C;
      break;
    case Style::ALGN_R:
      result = STL_ALGN_R;
      break;
    }
    return result;
  }

  // this procedure convert a byte code to an alignment
  static Style::t_algn stl_toalgn (const t_byte code) {
    switch (code) {
    case STL_ALGN_L:
      return Style::ALGN_L;
      break;
    case STL_ALGN_C:
      return Style::ALGN_C;
      break;
    case STL_ALGN_R:
      return Style::ALGN_R;
      break;
    default:
      break;
    }
    throw Exception ("style-error", "invalid text position serialization code");
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default style
  
  Style::Style (void) {
    reset ();
  }

  // copy construct this style

  Style::Style (const Style& that) {
    that.rdlock ();
    try { 
      d_wght = that.d_wght;
      d_algn = that.d_algn;
      d_tsiz = that.d_tsiz;
      d_fill = that.d_fill;
      d_psiz = that.d_psiz;
      d_sflg = that.d_sflg;
      d_tflg = that.d_tflg;
      d_lsiz = that.d_lsiz;
      d_rsiz = that.d_rsiz;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Style::repr (void) const {
    return "Style";
  }

  // return a clone of this object

  Object* Style::clone (void) const {
    return new Style (*this);
  }

  // return the style did

  t_word Style::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the style sid

  t_word Style::getsid (void) const {
    return SRL_STYL_SID;
  }

  // serialize this style

  void Style::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the text style
      os.write ((char) stl_tobyte (d_wght));
      os.write ((char) stl_tobyte (d_algn));
      Serial::wrlong (d_tsiz, os);
      Serial::wrchar (d_fill, os);
      // write the number formating
      Serial::wrlong (d_psiz, os);
      Serial::wrbool (d_sflg, os);
      // write the truncate formating
      Serial::wrbool (d_tflg, os);
      Serial::wrlong (d_lsiz, os);
      Serial::wrlong (d_rsiz, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this style

  void Style::rdstream (InputStream& is) {
    wrlock ();
    try {
      // read the test style
      d_wght = stl_towght ((t_byte) is.read ());
      d_algn = stl_toalgn ((t_byte) is.read ());
      d_tsiz = Serial::rdlong (is);
      d_fill = Serial::rdchar (is);
      // get the number formating
      d_psiz = Serial::rdlong (is);
      d_sflg = Serial::rdbool (is);
      // get the truncate formating
      d_tflg = Serial::rdbool (is);
      d_lsiz = Serial::rdlong (is);
      d_rsiz = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    } 
  }

  // assign a style to this one

  Style& Style::operator = (const Style& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_wght = that.d_wght;
      d_algn = that.d_algn;
      d_tsiz = that.d_tsiz;
      d_fill = that.d_fill;
      d_psiz = that.d_psiz;
      d_sflg = that.d_sflg;
      d_tflg = that.d_tflg;
      d_lsiz = that.d_lsiz;
      d_rsiz = that.d_rsiz;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this style

  void Style::reset (void) {
    wrlock ();
    try {
      d_wght = WGHT_N;
      d_algn = ALGN_L;
      d_tsiz = 0L;
      d_fill = ' ';
      d_psiz = 0L;
      d_sflg = false;
      d_tflg = false;
      d_lsiz = 0L;
      d_rsiz = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the text weight

  void Style::setwght (const t_wght wght) {
    wrlock ();
    try {
      d_wght = wght;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the text weight

  Style::t_wght Style::getwght (void) const {
    rdlock ();
    try {
      t_wght result = d_wght;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the text alignment

  void Style::setalgn (const t_algn algn) {
    wrlock ();
    try {
      d_algn = algn;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the text alignment

  Style::t_algn Style::getalgn (void) const {
    rdlock ();
    try {
      t_algn result = d_algn;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the text size

  void Style::settsiz (const long tsiz) {
    wrlock ();
    try {
      if (tsiz < 0L) {
	throw Exception ("style-error", "invalid negative text size");
      }
      d_tsiz = tsiz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the text size

  long Style::gettsiz (void) const {
    rdlock ();
    try {
      long result = d_tsiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the filling character

  void Style::setfill (const t_quad fill) {
    wrlock ();
    try {
      d_fill = fill;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the filling character

  t_quad Style::getfill (void) const {
    rdlock ();
    try {
      t_quad result = d_fill;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the number precision

  void Style::setpsiz (const long psiz) {
    wrlock ();
    try {
      if (psiz < 0L) {
	throw Exception ("style-error", "invalid negative number precision");
      }
      d_psiz = psiz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number precision

  long Style::getpsiz (void) const {
    rdlock ();
    try {
      long result = d_psiz;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the scientific flag

  void Style::setsflg (const bool sflg) {
    wrlock ();
    try {
      d_sflg = sflg;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the scientific flag

  bool Style::getsflg (void) const {
    rdlock ();
    try {
      bool result = d_sflg;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the truncate mode

  void Style::settrunc (const long lsiz,const long rsiz) {
    wrlock ();
    try {
      d_lsiz = (lsiz <= 0L) ? 0L : lsiz;
      d_rsiz = (rsiz <= 0L) ? 0L : rsiz;
      d_tflg = (d_lsiz + d_rsiz + STL_ELPS_T.length() == 0) ? false : true;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // convert an integer number into a string - no style

  String Style::format (const long ival) const {
    rdlock ();
    try {
      String result = Utility::tostring (ival);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert an integer number into a string - no style

  String Style::format (const t_long ival) const {
    rdlock ();
    try {
      String result = Utility::tostring (ival);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert a real number into a string - no style

  String Style::format (const t_real rval) const {
    rdlock ();
    try {
      String result = Utility::tostring (rval, d_psiz, d_sflg);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format a string with the text style

  String Style::format (const String& sval, const long size) const {
    rdlock ();
    try {
      // format the result string using non combining length
      String result;
      long slen = sval.ncclen ();
      // check if we process the size or not
      if (d_tsiz == 0L) {
	if (slen < size) {
	  if (d_algn == ALGN_R) {
	    result = sval.lfill (d_fill, size);
	  } else {
	    result = sval.rfill (d_fill, size);
	  }
	} else {
	  result = sval;
	}	     
      } else {
	if (slen == d_tsiz) result = sval;
	if (slen < d_tsiz) {
	  if (d_algn == ALGN_R) {
	    result = sval.lfill (d_fill, d_tsiz);
	  } else {
	    result = sval.rfill (d_fill, d_tsiz);
	  }
	}
	if (slen > d_tsiz) {
	  if (d_algn == ALGN_R) {
	    result = sval.lsubstr (d_tsiz);
	  } else {
	    result = sval.rsubstr (slen - d_tsiz);
	  }
	}
      }
      // check if we truncate the result
      if (d_tflg == true) {
	result = Utility::truncate (result, d_lsiz, d_rsiz, STL_ELPS_T);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // truncate a string by style

  String Style::truncate (const String& sval) const {
    rdlock ();
    try {
      String result = sval;
      if (d_tflg == true) {
	if (d_tsiz == 0L) {
	  result = Utility::truncate (sval, d_lsiz, d_rsiz, STL_ELPS_T);
	} else {
	  long slen = sval.ncclen ();
	  if (slen > d_tsiz) {
	    result = Utility::truncate (sval, d_lsiz, d_rsiz, STL_ELPS_T);
	  }
	}
      }
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
  static const long QUARK_WGHTN = String::intern ("WEIGHT-NORMAL");
  static const long QUARK_WGHTB = String::intern ("WEIGHT-BOLD");
  static const long QUARK_WGHTI = String::intern ("WEIGHT-ITALIC");
  static const long QUARK_ALGNL = String::intern ("ALIGN-LEFT");
  static const long QUARK_ALGNC = String::intern ("ALIGN-CENTER");
  static const long QUARK_ALGNR = String::intern ("ALIGN-RIGHT");
  static const long QUARK_STYLE = String::intern ("Style");

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_TRUNC   = zone.intern ("truncate");
  static const long QUARK_SETTRNC = zone.intern ("set-truncate");
  static const long QUARK_SETTSIZ = zone.intern ("set-text-size");
  static const long QUARK_GETTSIZ = zone.intern ("get-text-size");
  static const long QUARK_SETFILL = zone.intern ("set-text-fill");
  static const long QUARK_GETFILL = zone.intern ("get-text-fill");
  static const long QUARK_SETWGHT = zone.intern ("set-text-weight");
  static const long QUARK_GETWGHT = zone.intern ("get-text-weight");
  static const long QUARK_SETALGN = zone.intern ("set-text-alignment");
  static const long QUARK_GETALGN = zone.intern ("get-text-alignment");
  static const long QUARK_SETPSIZ = zone.intern ("set-number-precision");
  static const long QUARK_GETPSIZ = zone.intern ("get-number-precision");
  static const long QUARK_SETSFLG = zone.intern ("set-scientific-notation");
  static const long QUARK_GETSFLG = zone.intern ("get-scientific-notation");

  // map an item to a weight type
  static inline Style::t_wght item_to_wght (const Item& item) {
    // check for a stream item 
    if (item.gettid () != QUARK_STYLE) {
      throw Exception ("item-error", "item is not a style item");
    }
    // map the item to the enumeration
    long quark = item.getquark (); 
    if (quark == QUARK_WGHTN) return Style::WGHT_N;
    if (quark == QUARK_WGHTB) return Style::WGHT_B;
    if (quark == QUARK_WGHTI) return Style::WGHT_I;
    throw Exception ("item-error", "cannot map item to weight type");
  }

  // map a weight type to an item
  static inline Item* wght_to_item (const Style::t_wght wght) {
    Item* result  = nullptr;
    switch (wght) {
    case Style::WGHT_N:
      result = new Item (QUARK_STYLE, QUARK_WGHTN);
      break;
    case Style::WGHT_B:
      result = new Item (QUARK_STYLE, QUARK_WGHTB);
      break;
    case Style::WGHT_I:
      result = new Item (QUARK_STYLE, QUARK_WGHTI);
      break;
    }
    return result;
  }

  // map an item to an alignment type
  static inline Style::t_algn item_to_algn (const Item& item) {
    // check for a stream item 
    if (item.gettid () != QUARK_STYLE) {
      throw Exception ("item-error", "item is not a style item");
    }
    // map the item to the enumeration
    long quark = item.getquark (); 
    if (quark == QUARK_ALGNL) return Style::ALGN_L;
    if (quark == QUARK_ALGNC) return Style::ALGN_C;
    if (quark == QUARK_ALGNR) return Style::ALGN_R;
    throw Exception ("item-error", "cannot map item to alignment type");
  }

  // map an alignment type to an item
  static inline Item* algn_to_item (const Style::t_algn algn) {
    Item* result  = nullptr;
    switch (algn) {
    case Style::ALGN_L:
      result = new Item (QUARK_STYLE, QUARK_ALGNL);
      break;
    case Style::ALGN_C:
      result = new Item (QUARK_STYLE, QUARK_ALGNC);
      break;
    case Style::ALGN_R:
      result = new Item (QUARK_STYLE, QUARK_ALGNR);
      break;
    }
    return result;
  }

  // create a new object in a generic way

  Object* Style::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Style;
    // invalid arguments
    throw Exception ("argument-error", "too many arguments with style");
  }

  // evaluate a quark statically

  Object* Style::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_WGHTN)
      return new Item (QUARK_STYLE, QUARK_WGHTN);
    if (quark == QUARK_WGHTB)
      return new Item (QUARK_STYLE, QUARK_WGHTB);
    if (quark == QUARK_WGHTI)
      return new Item (QUARK_STYLE, QUARK_WGHTI);
    if (quark == QUARK_ALGNL)
      return new Item (QUARK_STYLE, QUARK_ALGNL);
    if (quark == QUARK_ALGNC)
      return new Item (QUARK_STYLE, QUARK_ALGNC);
    if (quark == QUARK_ALGNR)
      return new Item (QUARK_STYLE, QUARK_ALGNR);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Style::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Style::apply (Evaluable* zobj, Nameset* nset, const long quark,
			Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETWGHT) return wght_to_item  (getwght ());
      if (quark == QUARK_GETALGN) return algn_to_item  (getalgn ());
      if (quark == QUARK_GETTSIZ) return new Integer   (gettsiz ());
      if (quark == QUARK_GETFILL) return new Character (getfill ());
      if (quark == QUARK_GETPSIZ) return new Integer   (getpsiz ());
      if (quark == QUARK_GETSFLG) return new Boolean   (getsflg ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETWGHT) {
        Object* obj = argv->get (0);
        Item*  iobj = dynamic_cast <Item*> (obj);
        if (iobj == nullptr) {
          throw Exception ("type-error", "invalid object for item type",
                           Object::repr (obj));
        }
        setwght (item_to_wght (*iobj));
        return nullptr;
      }
      if (quark == QUARK_SETALGN) {
        Object* obj = argv->get (0);
        Item*  iobj = dynamic_cast <Item*> (obj);
        if (iobj == nullptr) {
          throw Exception ("type-error", "invalid object for item type",
                           Object::repr (obj));
        }
        setalgn (item_to_algn (*iobj));
        return nullptr;
      }
      if (quark == QUARK_SETTSIZ) {
	long tsiz = argv->getlong (0);
	settsiz (tsiz);
	return nullptr;
      }
      if (quark == QUARK_SETFILL) {
	t_quad fill = argv->getchar (0);
	setfill (fill);
	return nullptr;
      }
      if (quark == QUARK_SETPSIZ) {
	long psiz = argv->getlong (0);
	setpsiz (psiz);
	return nullptr;
      }
      if (quark == QUARK_SETSFLG) {
	bool sflg = argv->getbool (0);
	setsflg (sflg);
	return nullptr;
      }
      if (quark == QUARK_TRUNC) {
	String sval = argv->getstring (0);
	return new String (truncate (sval));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETTRNC) {
	long   lsiz = argv->getlong (0);
	long   rsiz = argv->getlong (1);
	settrunc (lsiz, rsiz);
	return nullptr;
      }
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
