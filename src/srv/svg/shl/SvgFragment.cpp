// ---------------------------------------------------------------------------
// - SvgFragment.cpp                                                         -
// - afnix:svg service - svg fragment node class implementation              -
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

#include "Utility.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SvgFragment.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the svg node name
  static const String XML_TAG_NAME = "svg:svg";
  // the svg namespace attribute
  static const String XML_XNS_ATTR = "xmlns:svg";
  // the svg namespace value
  static const String XML_XNS_XVAL = "http://www.w3.org/2000/svg";
  // the svg version attribute
  static const String XML_VRS_ATTR = "version";
  // the svg version value
  static const String XML_VRS_XVAL = "1.1";
  // the svg embedded x position attribute
  static const String XML_EXP_ATTR = "x";
  // the svg embedded y position attribute
  static const String XML_EYP_ATTR = "y";
  // the svg width attribute
  static const String XML_WTH_ATTR = "width";
  // the svg height attribute
  static const String XML_HGT_ATTR = "height";
  // the svg viewbox attribute
  static const String XML_VBX_ATTR = "viewBox";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a svg fragment

  SvgFragment::SvgFragment (void) : XmlTag (XML_TAG_NAME) {
    setattr (XML_XNS_ATTR, XML_XNS_XVAL);
    setattr (XML_VRS_ATTR, XML_VRS_XVAL);
  }

  // create a svg fragment by width and height

  SvgFragment::SvgFragment (const long width, const long height) : 
    XmlTag (XML_TAG_NAME) {
    // set initial attributes
    setattr (XML_XNS_ATTR, XML_XNS_XVAL);
    setattr (XML_VRS_ATTR, XML_VRS_XVAL);
    // set the width and height
    setwth (width);
    sethgt (height);
    // set the view box
    setvbx (0,0, width, height);
  }

  // return the class name

  String SvgFragment::repr (void) const {
    return "SvgFragment";
  }

  // set the svg embedded x position attribute

  void SvgFragment::setx (const long x) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_EXP_ATTR, Utility::tostring (x));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg embedded y position attribute

  void SvgFragment::sety (const long y) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_EYP_ATTR, Utility::tostring (y));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg width attribute

  void SvgFragment::setwth (const long width) {
    // check for null value
    if (width == 0LL) return;
    // check for negative value
    if (width < 0LL) {
      throw Exception ("svg-error", "invalid negative svg width");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, Utility::tostring (width));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg width attribute

  void SvgFragment::setwth (const String& wth) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_WTH_ATTR, wth);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg height attribute

  void SvgFragment::sethgt (const long heigth) {
    // check for null value
    if (heigth == 0LL) return;
    // check for negative value
    if (heigth < 0LL) {
      throw Exception ("svg-error", "invalid negative svg height");
    }
    // lock and set
    wrlock ();
    try {
      setattr (XML_HGT_ATTR, Utility::tostring (heigth));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

 // set the svg height attribute

  void SvgFragment::sethgt (const String& hgt) {
    // lock and set
    wrlock ();
    try {
      setattr (XML_HGT_ATTR, hgt);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the svg viewbox attribute

  void SvgFragment::setvbx (const long tlx, const long tly, 
			    const long brx, const long bry) {
    // lock and set
    wrlock ();
    try {
      // prepare result string
      String vbx = Utility::tostring (tlx);
      vbx += ' ';
      vbx += Utility::tostring (tly);
      vbx += ' ';
      vbx += Utility::tostring (brx);
      vbx += ' ';
      vbx += Utility::tostring (bry);
      // set the attribute
      setattr (XML_VBX_ATTR, vbx);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
    
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETEXP = zone.intern ("set-x");
  static const long QUARK_SETEYP = zone.intern ("set-y");
  static const long QUARK_SETWTH = zone.intern ("set-width");
  static const long QUARK_SETHGT = zone.intern ("set-height");
  static const long QUARK_SETVBX = zone.intern ("set-view-box");

  // create a new object in a generic way

  Object* SvgFragment::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new SvgFragment;
    // check for 2 arguments
    if (argc == 2) {
      long width  = argv->getlong (0);
      long height = argv->getlong (1);
      return new SvgFragment (width, height);
    }
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg fragment node constructor");
  }

  // return true if the given quark is defined

  bool SvgFragment::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? XmlTag::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* SvgFragment::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETEXP) {
	long x = argv->getlong (0);
	setx (x);
	return nullptr;
      }
      if (quark == QUARK_SETEYP) {
	long y = argv->getlong (0);
	sety (y);
	return nullptr;
      }
      if (quark == QUARK_SETWTH) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long width = iobj->tolong ();
	  setwth (width);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  setwth (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with set-width",
			 Object::repr (obj));
      }
      if (quark == QUARK_SETHGT) {
	Object* obj = argv->get (0);
	// check for integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) {
	  long height = iobj->tolong ();
	  sethgt (height);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  sethgt (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with set-heigth",
			 Object::repr (obj));
      }
    }
    // dispatch 4 arguments
    if (argc == 4) {
      if (quark == QUARK_SETVBX) {
	long tlx = argv->getlong (0);
	long tly = argv->getlong (1);
	long brx = argv->getlong (2);
	long bry = argv->getlong (3);
	setvbx (tlx, tly, brx, bry);
	return nullptr;
      }
    }
    // call the xml tag method
    return XmlTag::apply (zobj, nset, quark, argv);
  }
}
