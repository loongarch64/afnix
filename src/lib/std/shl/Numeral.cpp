// ---------------------------------------------------------------------------
// - Numeral.cpp                                                             -
// - standard object library - numeral class implementation                  -
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
#include "Real.hpp"
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Numeral.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"
#include "ccnv.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  static const long QUARK_SINT = String::intern ("INTEGER");
  static const long QUARK_SFLT = String::intern ("FLOAT");
  static const long QUARK_NMRL = String::intern ("Numeral");

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // map an item to a numeral type
  
  Numeral::t_numt Numeral::tonumt (const Item& item) {
    // check for an asn node item
    if (item.gettid () != QUARK_NMRL) {
      throw Exception ("item-error", "item is not a numeral item");
    }
    // map the item to the enumeration
    long quark = item.getquark ();
    if (quark == QUARK_SINT) return Numeral::NUMT_SINT;
    if (quark == QUARK_SFLT) return Numeral::NUMT_SFLT;
    throw Exception ("item-error", "cannot map item to numeral type");
  }

  // map a numeral type to an item

  Item* Numeral::toitem (const t_numt numt) {
    switch (numt) {
    case NUMT_SINT:
      return new Item (QUARK_NMRL, QUARK_SINT);
      break;
    case NUMT_SFLT:
      return new Item (QUARK_NMRL, QUARK_SFLT);
      break;
    }
    return nullptr;
  }

  // get the byte size of a numeral
  
  long Numeral::tobsiz (const Numeral::t_numt numt) {
    long result = 0L;
    switch (numt) {
    case Numeral::NUMT_SINT:
    case Numeral::NUMT_SFLT:
      result = 4L;
      break;
    }
    return result;
  }
  
  // compute the opposite of a numeral

  Numeral operator - (const Numeral& x) {
    x.rdlock ();
    try {
      Numeral result;
      switch (x.d_numt) {
      case Numeral::NUMT_SINT:
	result.d_numt = Numeral::NUMT_SINT;
	result.d_ival = -x.d_ival;
	break;
      case Numeral::NUMT_SFLT:
	result.d_numt = Numeral::NUMT_SFLT;
	result.d_fval = -x.d_fval;
	break;
      }
      x.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      throw;
    }
  }

  // add two numerals together

  Numeral operator + (const Numeral& x, const Numeral& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Numeral result;
      switch (x.d_numt) {
      case Numeral::NUMT_SINT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SINT;
	  result.d_ival = x.d_ival + y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = (float) x.d_ival + y.d_fval;
	  break;
	}
	break;
      case Numeral::NUMT_SFLT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_ival = x.d_fval + (float) y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = x.d_fval + y.d_fval;
	  break;
	}
	break;
      }
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // subtract two numerals

  Numeral operator - (const Numeral& x, const Numeral& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Numeral result;
      switch (x.d_numt) {
      case Numeral::NUMT_SINT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SINT;
	  result.d_ival = x.d_ival - y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = (float) x.d_ival - y.d_fval;
	  break;
	}
	break;
      case Numeral::NUMT_SFLT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_ival = x.d_fval - (float) y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = x.d_fval - y.d_fval;
	  break;
	}
	break;
      }
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // multiply two numerals

  Numeral operator * (const Numeral& x, const Numeral& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Numeral result;
      switch (x.d_numt) {
      case Numeral::NUMT_SINT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SINT;
	  result.d_ival = x.d_ival * y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = (float) x.d_ival * y.d_fval;
	  break;
	}
	break;
      case Numeral::NUMT_SFLT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_ival = x.d_fval * (float) y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = x.d_fval * y.d_fval;
	  break;
	}
	break;
      }
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // divide two numerals 

  Numeral operator / (const Numeral& x, const Numeral& y) {
    x.rdlock ();
    y.rdlock ();
    try {
      Numeral result;
      switch (x.d_numt) {
      case Numeral::NUMT_SINT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  if (y.d_ival == 0) {
	    throw Exception ("numeral-error", "division by zero");
	  }
	  result.d_numt = Numeral::NUMT_SINT;
	  result.d_ival = x.d_ival / y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = (float) x.d_ival / y.d_fval;
	  break;
	}
	break;
      case Numeral::NUMT_SFLT:
	switch (y.d_numt) {
	case Numeral::NUMT_SINT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_ival = x.d_fval / (float) y.d_ival;
	  break;
	case Numeral::NUMT_SFLT:
	  result.d_numt = Numeral::NUMT_SFLT;
	  result.d_fval = x.d_fval / y.d_fval;
	  break;
	}
	break;
      }
      x.unlock ();
      y.unlock ();
      return result;
    } catch (...) {
      x.unlock ();
      y.unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default numeral

  Numeral::Numeral (void) {
    d_numt = NUMT_SINT;
    d_ival = 0;
  }

  // create a numeral by type

  Numeral::Numeral (const t_numt numt) {
    d_numt = numt;
    switch (d_numt) {
    case NUMT_SINT:
      d_ival = 0;
      break;
    case NUMT_SFLT:
      d_fval = 0.0F;
      break;
    }	
  }

  // create a new numeral from a native value

  Numeral::Numeral (const t_long value) {
    d_numt = NUMT_SINT;
    d_ival = (int) value;
  }

  // create a new numeral from a native value

  Numeral::Numeral (const t_real value) {
    d_numt = NUMT_SFLT;
    d_fval = (float) value;
  }

  // create a new numeral from an integer

  Numeral::Numeral (const Integer& value) {
    d_numt = NUMT_SINT;
    d_ival = (int) (value.tolong ());
  }

  // create a new numeral from a real

  Numeral::Numeral (const Real& value) {
    d_numt = NUMT_SFLT;
    d_fval = (float) (value.toreal ());
  }

  // create a new numeral from a strign

  Numeral::Numeral (const String& value) {
    // check for an integer
    try {
      long ival = Utility::tolong (value);
      d_numt = NUMT_SINT;
      d_ival = (int) ival;
    } catch (...) {
      try {
	t_real rval = Utility::toreal (value);
	d_numt = NUMT_SFLT;
	d_fval = (float) rval;
      } catch (...) {
	throw Exception ("numeral-error", "invalid numeral value", value);
      }
    }
  }

  // copy constructor for this numeral

  Numeral::Numeral (const Numeral& that) {
    that.rdlock ();
    try {
      d_numt = that.d_numt;
      switch (d_numt) {
      case NUMT_SINT:
	d_ival = that.d_ival;
	break;
      case NUMT_SFLT:
	d_fval = that.d_fval;
	break;
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Numeral::repr (void) const {
    return "Numeral";
  }

  // return a clone of this object

  Object* Numeral::clone (void) const {
    return new Numeral (*this);
  }

  // clear this numeral

  void Numeral::clear (void) {
    wrlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	d_ival = 0;
	break;
      case NUMT_SFLT:
	d_fval = 0.0F;
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this numeral

  String Numeral::toliteral (void) const {
    rdlock ();
    try {
      String result = tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of this numeral

  String Numeral::tostring (void) const {
    rdlock ();
    try {
      String result;
      switch (d_numt) {
      case NUMT_SINT:
	result = Utility::tostring ((long) d_ival);
	break;
      case NUMT_SFLT:
	result = Utility::tostring ((t_real) d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format a styled string representation

  String Numeral::format (const Style& lstl) const {
    rdlock ();
    try {
      String result;
      switch (d_numt) {
      case NUMT_SINT:
	result = lstl.format ((long) d_ival);
	break;
      case NUMT_SFLT:
	result = lstl.format ((t_real) d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // return the numeral did

  t_word Numeral::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the numeral sid

  t_word Numeral::getsid (void) const {
    return SRL_NUMR_SID;
  }

  // serialize this numeral

  void Numeral::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the type
      os.write ((char) d_numt);
      // write by type
      if (d_numt == NUMT_SINT) {
	t_byte data[8];
	c_ohton (d_ival, data);
	os.write ((char*) data, 8);
      }
      if (d_numt == NUMT_SFLT) {
	t_byte data[8];
	c_rhton (d_fval, data);
	os.write ((char*) data, 8);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this numeral

  void Numeral::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the type
      d_numt = (t_numt) is.read ();
      // process by type
      if (d_numt == NUMT_SINT) {
	t_byte data[8];
	for (long i = 0; i < 8; i++) data[i] = (t_byte) is.read ();
	d_ival = c_ontoh (data);
      } else if (d_numt == NUMT_SFLT) {
	t_byte data[8];
	for (long i = 0; i < 8; i++) data[i] = (t_byte) is.read ();
	d_fval = c_ontor (data);
      } else {
	throw Exception ("numeral-error",
			 "invalid numeral type to deserialize");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set an numeral with a value

  Numeral& Numeral::operator = (const t_long value) {
    wrlock ();
    try {
      d_numt = NUMT_SINT;
      d_ival = (int) value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an numeral with a value

  Numeral& Numeral::operator = (const t_real value) {
    wrlock ();
    try {
      d_numt = NUMT_SFLT;
      d_fval = (float) value;
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a numeral to this one

  Numeral& Numeral::operator = (const Numeral& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_numt = that.d_numt;
      switch (d_numt) {
      case NUMT_SINT:
	d_ival = that.d_ival;
	break;
      case NUMT_SFLT:
	d_fval = that.d_fval;
	break;
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // add a numeral to this one

  Numeral& Numeral::operator += (const Numeral& x) {
    wrlock   ();
    x.rdlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SINT;
	  d_ival += x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval = (float) d_ival + x.d_fval;
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SFLT;
	  d_fval += (float) x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval += x.d_fval;
	  break;
	}
	break;
      }
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // subtract a numeral to this one

  Numeral& Numeral::operator -= (const Numeral& x) {
    wrlock   ();
    x.rdlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SINT;
	  d_ival -= x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval = (float) d_ival - x.d_fval;
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SFLT;
	  d_fval -= (float) x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval -= x.d_fval;
	  break;
	}
	break;
      }
      unlock ();
      x.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }
  
  // multiply a numeral with this one
  
  Numeral& Numeral::operator *= (const Numeral& x) {
    wrlock   ();
    x.rdlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SINT;
	  d_ival *= x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval = (float) d_ival * x.d_fval;
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SFLT;
	  d_fval *= (float) x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval *= x.d_fval;
	  break;
	}
	break;
      }
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }
  
  // multiply a numeral with this one
  
  Numeral& Numeral::operator /= (const Numeral& x) {
    wrlock   ();
    x.rdlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  if (x.d_ival == 0) {
	    throw Exception ("numeral-error", "division by zero");
	  }
	  d_numt = NUMT_SINT;
	  d_ival /= x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval = (float) d_ival / x.d_fval;
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  d_numt = NUMT_SFLT;
	  d_fval /= (float) x.d_ival;
	  break;
	case NUMT_SFLT:
	  d_numt = NUMT_SFLT;
	  d_fval /= x.d_fval;
	  break;
	}
	break;
      }
      unlock ();
      x.unlock   ();
      return *this;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }
  
  // prefix add one to the numeral

  Numeral& Numeral::operator ++ (void) {
    wrlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	++d_ival;
	break;
      case NUMT_SFLT:
	++d_fval;
	break;
      }
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // postfix add one to the numeral

  Numeral Numeral::operator ++ (int) {
    wrlock ();
    try {
      Numeral result = *this;
      switch (d_numt) {
      case NUMT_SINT:
	d_ival++;
	break;
      case NUMT_SFLT:
	d_fval++;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // prefix sub one to the numeral

  Numeral& Numeral::operator -- (void) {
    wrlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	--d_ival;
	break;
      case NUMT_SFLT:
	--d_fval;
	break;
      }
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // postfix sub one to the numeral

  Numeral Numeral::operator -- (int) {
    wrlock ();
    try {
      Numeral result = *this;
      switch (d_numt) {
      case NUMT_SINT:
	d_ival--;
	break;
      case NUMT_SFLT:
	d_fval--;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare an numeral with a native value
  
  bool Numeral::operator == (const t_long value) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival == (int) value);
	break;
      case NUMT_SFLT:
	result = (d_fval == (float) value);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare an numeral with a native value
  
  bool Numeral::operator != (const t_long value) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival != (int) value);
	break;
      case NUMT_SFLT:
	result = (d_fval != (float) value);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare a numeral with a numeral value
  
  bool Numeral::operator == (const t_real value) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival == (int) value);
	break;
      case NUMT_SFLT:
	result = (d_fval == (float) value);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare an numeral with a native value
  
  bool Numeral::operator != (const t_real value) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival != (int) value);
	break;
      case NUMT_SFLT:
	result = (d_fval != (float) value);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two numerals

  bool Numeral::operator == (const Numeral& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_ival == value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_ival == (int) value.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_fval == (float) value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_fval == value.d_fval);
	  break;
	}
	break;
      }
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // compare two numerals

  bool Numeral::operator != (const Numeral& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_ival != value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_ival != (int) value.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_fval != (float) value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_fval != value.d_fval);
	  break;
	}
	break;
      }
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // compare two numerals
  
  bool Numeral::operator < (const Numeral& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_ival < value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_ival < (int) value.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_fval < (float) value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_fval < value.d_fval);
	  break;
	}
	break;
      }
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // compare two numerals

  bool Numeral::operator <= (const Numeral& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_ival <= value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_ival <= (int) value.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_fval <= (float) value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_fval <= value.d_fval);
	  break;
	}
	break;
      }
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // compare two numerals

  bool Numeral::operator > (const Numeral& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_ival > value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_ival > (int) value.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_fval > (float) value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_fval > value.d_fval);
	  break;
	}
	break;
      }
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }

  // compare two numerals

  bool Numeral::operator >= (const Numeral& value) const {
    rdlock ();
    value.rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_ival >= value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_ival >= (int) value.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (value.d_numt) {
	case NUMT_SINT:
	  result = (d_fval >= (float) value.d_ival);
	  break;
	case NUMT_SFLT:
	  result = (d_fval >= value.d_fval);
	  break;
	}
	break;
      }
      unlock ();
      value.unlock ();
      return result;
    } catch (...) {
      unlock ();
      value.unlock ();
      throw;
    }
  }


  // return true if the number is 0

  bool Numeral::iszero (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival == 0);
	break;
      case NUMT_SFLT:
	result = (d_fval == 0.0F);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the number is odd

  bool Numeral::isodd (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = ((d_ival & 1) == 1);
	break;
      case NUMT_SFLT:
	result = false;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the number is even

  bool Numeral::iseven (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = ((d_ival & 1) == 0);
	break;
      case NUMT_SFLT:
	result = false;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare with a numeral by absolute precision

  bool Numeral::cmp (const t_long value) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival == (int) value);
	break;
      case NUMT_SFLT:
	result = Math::acmp (d_fval, (t_real) value);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // compare with a numeral by absolute precision

  bool Numeral::cmp (const t_real value) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = (d_ival == (int) value);
	break;
      case NUMT_SFLT:
	result = Math::acmp (d_fval, value);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the number is nan

  bool Numeral::isnan (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = false;
	break;
      case NUMT_SFLT:
	result = Math::isnan (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the number is infinite

  bool Numeral::isinf (void) const {
    rdlock ();
    try {
      bool result = false;
      switch (d_numt) {
      case NUMT_SINT:
	result = false;
	break;
      case NUMT_SFLT:
	result = Math::isinf (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the floor of this number

  Numeral Numeral::floor (void) const {
    rdlock ();
    try {
      Numeral result = *this;
      if (d_numt == NUMT_SFLT) result.d_fval = (float) Math::floor (d_fval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the ceiling of this number

  Numeral Numeral::ceiling (void) const {
    rdlock ();
    try {
      Numeral result = *this;
      if (d_numt == NUMT_SFLT) result.d_fval = (float) Math::ceiling (d_fval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the round of this number

  Numeral Numeral::round (void) const {
    rdlock ();
    try {
      Numeral result = *this;
      if (d_numt == NUMT_SFLT) result.d_fval = (float) Math::round (d_fval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the absolute value of this number
  
  Numeral Numeral::abs (void) const {
    rdlock ();
    try {
      Numeral result = *this;
      switch (d_numt) {
      case NUMT_SINT:
	if (result.d_ival < 0) result.d_ival = -result.d_ival;
	break;
      case NUMT_SFLT:
	if (result.d_fval < 0) result.d_fval = -result.d_fval;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the remainder of this number with its argument
  
  Numeral Numeral::mod (const Numeral& x) const {
    rdlock   ();
    x.rdlock ();
    try {
      Numeral result;
      switch (d_numt) {
      case NUMT_SINT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  result.d_numt = NUMT_SINT;
	  result.d_ival = d_ival % x.d_ival;
	  break;
	case NUMT_SFLT:
	  result.d_numt = NUMT_SFLT;
	  result.d_fval = Math::mod ((t_real)d_ival, x.d_fval);
	  break;
	}
	break;
      case NUMT_SFLT:
	switch (x.d_numt) {
	case NUMT_SINT:
	  result.d_numt = NUMT_SFLT;
	  result.d_fval = Math::mod (d_fval, (t_real) x.d_ival);
	  break;
	case NUMT_SFLT:
	  result.d_numt = NUMT_SFLT;
	  result.d_fval = Math::mod (d_fval, x.d_fval);
	  break;
	}
	break;
      }
      unlock   ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock   ();
      x.unlock ();
      throw;
    }
  }
  
  // return the square root of this numeral
  
  Numeral Numeral::sqrt (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::sqrt (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::sqrt (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the natural logarithm of this numeral
  
  Numeral Numeral::ln (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::ln (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::ln (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the exponential of this numeral
  
  Numeral Numeral::exp (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::exp (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::exp (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the power of this number with the argument
  
  Numeral Numeral::pow (const Numeral& x) const {
    rdlock   ();
    x.rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::pow (d_ival, x.toreal ());
	break;
      case NUMT_SFLT:
	result.d_fval = Math::pow (d_fval, x.toreal ());
	break;
      }
      unlock   ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }
  
  // return the sine of this number
  
  Numeral Numeral::sin (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::sin (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::sin (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the cosine of this number
  
  Numeral Numeral::cos (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::cos (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::cos (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the tangent of this number
  
  Numeral Numeral::tan (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::tan (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::tan (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the arc sine of this number
  
  Numeral Numeral::asin (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::asin (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::asin (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the arc cosine of this number
  
  Numeral Numeral::acos (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::acos (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::acos (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the arc tangent of this number
  
  Numeral Numeral::atan (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::atan (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::atan (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic sine of this number
  
  Numeral Numeral::sinh (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::sinh (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::sinh (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic cosine of this number
  
  Numeral Numeral::cosh (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::cosh (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::cosh (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hyperbolic tangent of this number

  Numeral Numeral::tanh (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::tanh (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::tanh (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic arc sine of this number

  Numeral Numeral::asinh (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::asinh (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::asinh (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the hyperbolic arc cosine of this number
  
  Numeral Numeral::acosh (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::acosh (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::acosh (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the hyperbolic arc tangent of this number
  
  Numeral Numeral::atanh (void) const {
    rdlock ();
    try {
      Numeral result;
      result.d_numt = NUMT_SFLT;
      switch (d_numt) {
      case NUMT_SINT:
	result.d_fval = Math::atanh (d_ival);
	break;
      case NUMT_SFLT:
	result.d_fval = Math::atanh (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the numeral type

  Numeral::t_numt Numeral::getnumt (void) const {
    rdlock ();
    try {
      t_numt result = d_numt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the numeral real value

  t_real Numeral::toreal (void) const {
    rdlock ();
    try {
      t_real result = 0.0;
      switch (d_numt) {
      case NUMT_SINT:
	result = (t_real) d_ival;
	break;
      case NUMT_SFLT:
	result = (t_real) d_fval;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an integer from this numeral
  
  t_long Numeral::tolong (void) const {
    rdlock ();
    try {
      t_long result = 0LL;
      switch (d_numt) {
      case NUMT_SINT:
	result = (t_long) d_ival;
	break;
      case NUMT_SFLT:
	result = (t_long) d_fval;
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size
  
  long Numeral::tosize (void) const {
    rdlock ();
    try {
      long result = 0L;
      switch (d_numt) {
      case NUMT_SINT:
	result = sizeof (d_ival);
	break;
      case NUMT_SFLT:
	result = sizeof (d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data
  
  t_byte* Numeral::tobyte (void) {
    wrlock ();
    try {
      t_byte* result = nullptr;
      switch (d_numt) {
      case NUMT_SINT:
	result = reinterpret_cast<t_byte*>(&d_ival);
	break;
      case NUMT_SFLT:
	result = reinterpret_cast<t_byte*>(&d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data
  
  const t_byte* Numeral::tobyte (void) const {
    rdlock ();
    try {
      const t_byte* result = nullptr;
      switch (d_numt) {
      case NUMT_SINT:
	result = reinterpret_cast<const t_byte*>(&d_ival);
	break;
      case NUMT_SFLT:
	result = reinterpret_cast<const t_byte*>(&d_fval);
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert a numeral by type

  Numeral::t_numt Numeral::convert (const t_numt numt) {
    wrlock ();
    try {
      switch (d_numt) {
      case NUMT_SINT:
	if (d_numt == NUMT_SFLT) d_fval = (float) d_ival;
	break;
      case NUMT_SFLT:
	if (d_numt == NUMT_SINT) d_ival = (int) d_fval;
	break;
      }
      t_numt result = d_numt;
      d_numt = numt;
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

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 44;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_OPP      = zone.intern ("++");
  static const long QUARK_OMM      = zone.intern ("--");
  static const long QUARK_ADD      = zone.intern ("+");
  static const long QUARK_SUB      = zone.intern ("-");
  static const long QUARK_MUL      = zone.intern ("*");
  static const long QUARK_DIV      = zone.intern ("/");
  static const long QUARK_EQL      = zone.intern ("==");
  static const long QUARK_NEQ      = zone.intern ("!=");
  static const long QUARK_LTH      = zone.intern ("<");
  static const long QUARK_LEQ      = zone.intern ("<=");
  static const long QUARK_GTH      = zone.intern (">");
  static const long QUARK_GEQ      = zone.intern (">=");
  static const long QUARK_AEQ      = zone.intern ("+=");
  static const long QUARK_SEQ      = zone.intern ("-=");
  static const long QUARK_MEQ      = zone.intern ("*=");
  static const long QUARK_DEQ      = zone.intern ("/=");
  static const long QUARK_QEQ      = zone.intern ("?=");
  static const long QUARK_ABS      = zone.intern ("abs");
  static const long QUARK_MOD      = zone.intern ("mod");
  static const long QUARK_POW      = zone.intern ("pow");  
  static const long QUARK_LN       = zone.intern ("ln");
  static const long QUARK_EXP      = zone.intern ("exp");
  static const long QUARK_SIN      = zone.intern ("sin");
  static const long QUARK_COS      = zone.intern ("cos");
  static const long QUARK_TAN      = zone.intern ("tan");
  static const long QUARK_SQRT     = zone.intern ("sqrt");
  static const long QUARK_NANP     = zone.intern ("nan-p");
  static const long QUARK_INFP     = zone.intern ("infinity-p");
  static const long QUARK_ASIN     = zone.intern ("asin");
  static const long QUARK_ACOS     = zone.intern ("acos");
  static const long QUARK_ATAN     = zone.intern ("atan");
  static const long QUARK_SINH     = zone.intern ("sinh");
  static const long QUARK_COSH     = zone.intern ("cosh");
  static const long QUARK_TANH     = zone.intern ("tanh");
  static const long QUARK_ASINH    = zone.intern ("asinh");
  static const long QUARK_ACOSH    = zone.intern ("acosh");
  static const long QUARK_ATANH    = zone.intern ("atanh");
  static const long QUARK_FLOOR    = zone.intern ("floor");
  static const long QUARK_ZEROP    = zone.intern ("zero-p");
  static const long QUARK_CONVERT  = zone.intern ("convert");
  static const long QUARK_TOREAL   = zone.intern ("to-real");
  static const long QUARK_TOINTG   = zone.intern ("to-integer");
  static const long QUARK_CEILING  = zone.intern ("ceiling");
  static const long QUARK_GETTYPE  = zone.intern ("get-type");

  // create a new numeral in a generic way

  Object* Numeral::mknew (Vector* argv) {
    if ((argv == nullptr) || (argv->length () == 0)) return new Numeral;
    if (argv->length () != 1) 
      throw Exception ("argument-error", 
		       "too many argument with numeral constructor");
    // try to map the numeral argument
    Object* obj = argv->get (0);
    if (obj == nullptr) return new Numeral;

    // try an integer object
    auto ival = dynamic_cast <Integer*> (obj);
    if (ival != nullptr) return new Numeral (ival->tolong ());

    // try a real object
    auto dval = dynamic_cast <Real*> (obj);
    if (dval != nullptr) return new Numeral (dval->toreal ());
    
    // try a numeral object
    auto nval = dynamic_cast <Numeral*> (obj);
    if (nval != nullptr) return new Numeral (*nval);

    // try an item object
    auto item = dynamic_cast <Item*> (obj);
    if (item != nullptr) return new Numeral (Numeral::tonumt(*item));

    // illegal object
    throw Exception ("type-error", "illegal object with numeral constructor",
		     obj->repr ());
  }

  // evaluate an object data member
  
  Object* Numeral::meval (Evaluable* zobj, Nameset* nset, const long quark) {
    if (quark == QUARK_SINT) return new Item (QUARK_NMRL, QUARK_SINT);
    if (quark == QUARK_SFLT) return new Item (QUARK_NMRL, QUARK_SFLT);
    throw Exception ("eval-error", "cannot evaluate member",
                     String::qmap (quark));
  }

  // return true if the given quark is defined

  bool Numeral::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Number::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this numeral with another object

  Object* Numeral::oper (t_oper type, Object* object) {
    auto iobj = dynamic_cast <Integer*> (object);
    auto nobj = dynamic_cast <Numeral*> (object);
    auto zobj = dynamic_cast <Real*> (object);
    switch (type) {
    case Object::OPER_ADD:
      if (iobj != nullptr) return new Numeral (*this + *iobj);
      if (nobj != nullptr) return new Numeral (*this + *nobj);
      if (zobj != nullptr) return new Numeral (*this + *zobj);
      break;
    case Object::OPER_SUB:
      if (iobj != nullptr) return new Numeral (*this - *iobj);
      if (nobj != nullptr) return new Numeral (*this - *nobj);
      if (zobj != nullptr) return new Numeral (*this - *zobj);
      break;
    case Object::OPER_MUL:
      if (iobj != nullptr) return new Numeral (*this * *iobj);
      if (nobj != nullptr) return new Numeral (*this * *nobj);
      if (zobj != nullptr) return new Numeral (*this * *zobj);
      break;
    case Object::OPER_DIV:
      if (iobj != nullptr) return new Numeral (*this / *iobj);
      if (nobj != nullptr) return new Numeral (*this / *nobj);
      if (zobj != nullptr) return new Numeral (*this / *zobj);
      break;
    case Object::OPER_UMN:
      return new Numeral (-(*this));
      break;
    case Object::OPER_EQL:
      if (iobj != nullptr) return new Boolean (*this == *iobj);
      if (nobj != nullptr) return new Boolean (*this == *nobj);
      if (zobj != nullptr) return new Boolean (*this == *zobj);
      break;
    case Object::OPER_QEQ:
      if (iobj != nullptr) return new Boolean (cmp(iobj->tolong()));
      if (nobj != nullptr) return new Boolean (cmp(nobj->toreal()));
      if (zobj != nullptr) return new Boolean (cmp(zobj->toreal()));
      break;
    case Object::OPER_NEQ:
      if (iobj != nullptr) return new Boolean (*this != *iobj);
      if (nobj != nullptr) return new Boolean (*this != *nobj);
      if (zobj != nullptr) return new Boolean (*this != *zobj);
      break;
    case Object::OPER_GEQ:
      if (iobj != nullptr) return new Boolean (*this >= *iobj);
      if (nobj != nullptr) return new Boolean (*this >= *nobj);
      if (zobj != nullptr) return new Boolean (*this >= *zobj);
      break;
    case Object::OPER_GTH:
      if (iobj != nullptr) return new Boolean (*this > *iobj);
      if (nobj != nullptr) return new Boolean (*this > *nobj);
      if (zobj != nullptr) return new Boolean (*this > *zobj);
      break;
    case Object::OPER_LEQ:
      if (iobj != nullptr) return new Boolean (*this <= *iobj);
      if (nobj != nullptr) return new Boolean (*this <= *nobj);
      if (zobj != nullptr) return new Boolean (*this <= *zobj);
      break;
    case Object::OPER_LTH:
      if (iobj != nullptr) return new Boolean (*this < *iobj);
      if (nobj != nullptr) return new Boolean (*this < *nobj);
      if (zobj != nullptr) return new Boolean (*this < *zobj);
      break;
    }
    throw Exception ("type-error", "invalid operand with numeral",
		     Object::repr (object));
  }

  // set an object to this numeral

  Object* Numeral::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      auto iobj = dynamic_cast <Integer*> (object);
      if (iobj != nullptr) {
	*this = *iobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      auto nobj = dynamic_cast <Numeral*> (object);
      if (nobj != nullptr) {
	*this = *nobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      auto dobj = dynamic_cast <Real*> (object);
      if (dobj != nullptr) {
	*this = *dobj;
	zobj->post (this);
	unlock ();
	return this;
      }
      throw Exception ("type-error", "invalid object with numeral vdef",
		       Object::repr (object));
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Numeral::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LN)       return new Numeral (ln      ());
      if (quark == QUARK_ABS)      return new Numeral (abs     ());
      if (quark == QUARK_EXP)      return new Numeral (exp     ());
      if (quark == QUARK_SIN)      return new Numeral (sin     ());
      if (quark == QUARK_COS)      return new Numeral (cos     ());
      if (quark == QUARK_TAN)      return new Numeral (tan     ());
      if (quark == QUARK_SINH)     return new Numeral (sinh    ());
      if (quark == QUARK_COSH)     return new Numeral (cosh    ());
      if (quark == QUARK_TANH)     return new Numeral (tanh    ());
      if (quark == QUARK_ASIN)     return new Numeral (asin    ());
      if (quark == QUARK_ACOS)     return new Numeral (acos    ());
      if (quark == QUARK_ATAN)     return new Numeral (atan    ());
      if (quark == QUARK_SQRT)     return new Numeral (sqrt    ());
      if (quark == QUARK_NANP)     return new Boolean (isnan   ());
      if (quark == QUARK_INFP)     return new Boolean (isinf   ());
      if (quark == QUARK_ASINH)    return new Numeral (asinh   ());
      if (quark == QUARK_ACOSH)    return new Numeral (acosh   ());
      if (quark == QUARK_ATANH)    return new Numeral (atanh   ());
      if (quark == QUARK_FLOOR)    return new Numeral (floor   ());
      if (quark == QUARK_CEILING)  return new Numeral (ceiling ());
      if (quark == QUARK_ZEROP)    return new Boolean (iszero  ());
      if (quark == QUARK_GETTYPE)  return toitem (getnumt ());
      if (quark == QUARK_TOINTG)   return new Integer (tolong ());
      if (quark == QUARK_TOREAL)   return new Integer (toreal ());

      if (quark == QUARK_OPP) {
	wrlock ();
	try {
	  ++(*this);
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_OMM) {
	wrlock ();
	try {
	  --(*this);
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }

    if (argc == 1) {
      if (quark == QUARK_ADD) return oper (Object::OPER_ADD, argv->get (0));
      if (quark == QUARK_SUB) return oper (Object::OPER_SUB, argv->get (0)); 
      if (quark == QUARK_MUL) return oper (Object::OPER_MUL, argv->get (0)); 
      if (quark == QUARK_DIV) return oper (Object::OPER_DIV, argv->get (0)); 
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0)); 
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0)); 
      if (quark == QUARK_LTH) return oper (Object::OPER_LTH, argv->get (0)); 
      if (quark == QUARK_LEQ) return oper (Object::OPER_LEQ, argv->get (0)); 
      if (quark == QUARK_GTH) return oper (Object::OPER_GTH, argv->get (0)); 
      if (quark == QUARK_GEQ) return oper (Object::OPER_GEQ, argv->get (0)); 

      if (quark == QUARK_MOD) {
	t_real val = argv->getrint (0);
	return new Numeral (mod (val));
      }
      if (quark == QUARK_AEQ) {
	wrlock ();
	try {
	  t_real val = argv->getrint (0);
	  *this += val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SEQ) {
	wrlock ();
	try {
	  t_real val = argv->getrint (0);
	  *this -= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MEQ) {
	wrlock ();
	try {
	  t_real val = argv->getrint (0);
	  *this *= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_DEQ) {
	wrlock ();
	try {
	  t_real val = argv->getrint (0);
	  *this /= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_QEQ) {
	t_real val = argv->getreal (0);
	return new Boolean (cmp (val));
      }
      if (quark == QUARK_POW) {
	t_real val = argv->getrint (0);
	return new Numeral (pow (val));
      }
      if (quark == QUARK_CONVERT) {
	Object* obj = argv->get (0);
	auto   item = dynamic_cast<Item*>(obj);
	if (item == nullptr) {
	  throw Exception ("type-error", "invalid object with numeral convert",
			   Object::repr (obj));
	}
	t_numt numt = convert (Numeral::tonumt(*item));
	return Numeral::toitem (numt);
      }
    }
    // call the number method
    return Number::apply (zobj, nset, quark, argv);
  }
}
