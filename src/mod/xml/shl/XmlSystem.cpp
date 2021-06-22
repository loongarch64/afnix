// ---------------------------------------------------------------------------
// - XmlSystem.cpp                                                           -
// - afnix:xml module - xml system class implementation                      -
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
#include "Unicode.hpp"
#include "XsoBuffer.hpp"
#include "XmlSystem.hpp"
#include "Exception.hpp"

#include "array.tcc"
#include "transient.tcc"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default xml version id
  static const String XML_DEFAULT_XVID = "1.0";
  // the default xml ncoding mode
  static const String XML_DEFAULT_EMOD = "UTF-8";

  // the xml special characters
  static const t_quad XML_CHAR_DQ = 0x00000022; // "
  static const t_quad XML_CHAR_AM = 0x00000026; // &
  static const t_quad XML_CHAR_SQ = 0x00000027; // '
  static const t_quad XML_CHAR_LT = 0x0000003C; // <
  static const t_quad XML_CHAR_GT = 0x0000003E; // >

  // the xml entity character reference
  static const String XML_TENT_LT = "&lt;";
  static const String XML_TENT_GT = "&gt;";
  static const String XML_TENT_AM = "&amp;";
  static const String XML_TENT_DQ = "&quot;";
  static const String XML_TENT_SQ = "&apos;";

  // the xml value escape sequences
  static const String XML_VENT_LT = "&#x3C;";
  static const String XML_VENT_GT = "&#x3E;";
  static const String XML_VENT_AM = "&#x26;";
  static const String XML_VENT_DQ = "&#x22;";
  static const String XML_VENT_SQ = "&#x27;";

  // this procedure add a string to quad array
  static inline void xml_blok_add (t_array<t_quad>& blok, const String& sval) {
    long slen = sval.length ();
    for (long k = 0L; k < slen; k++) blok.push (sval[k]);
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return the default xml system version string

  String XmlSystem::getxvid (void) {
    return XML_DEFAULT_XVID;
  }

  // return the default xml system version id 

  XmlSystem::t_xmlv XmlSystem::getxmlv (void) {
    return XmlSystem::toxmlv (XmlSystem::getxvid ());
  }

  // get the default xml system encoding mode

  String XmlSystem::getemod (void) {
    return XML_DEFAULT_EMOD;
  }

  // return true if the vid is valid

  bool XmlSystem::isxvid (const String& xvid) {
    if (xvid == "1.0") return true;
    if (xvid == "1.1") return true;
    return false;
  }

  // convert a string into a version

  XmlSystem::t_xmlv XmlSystem::toxmlv (const String& xvid) {
    if (xvid == "1.0") return XmlSystem::XML_1_0;
    if (xvid == "1.1") return XmlSystem::XML_1_1;
    throw Exception ("xml-error", "unsupported xml version", xvid);
  }

  // convert a version to a string

  String XmlSystem::toxvid (const t_xmlv xmlv) {
    String result;
    switch (xmlv) {
    case XML_1_0:
      result = "1.0";
      break;
    case XML_1_1:
      result = "1.1";
      break;
    }
    return result;
  }

  // check for a valid standard entity reference

  bool XmlSystem::issref (const String& xref) {
    // check for standard reference
    if (xref == XML_TENT_LT) return true;
    if (xref == XML_TENT_GT) return true;
    if (xref == XML_TENT_AM) return true;
    if (xref == XML_TENT_DQ) return true;
    if (xref == XML_TENT_SQ) return true;
    // check for value reference
    if (xref == XML_VENT_GT) return true;
    if (xref == XML_VENT_AM) return true;
    if (xref == XML_VENT_DQ) return true;
    if (xref == XML_VENT_SQ) return true;
    return false;
  }
  
  // convert a standard reference to a character form
  
  t_quad XmlSystem::tosref (const String& xref) {
    // check for standard reference
    if (xref == XML_TENT_LT) return XML_CHAR_LT;
    if (xref == XML_TENT_GT) return XML_CHAR_GT;
    if (xref == XML_TENT_AM) return XML_CHAR_AM;
    if (xref == XML_TENT_DQ) return XML_CHAR_DQ;
    if (xref == XML_TENT_SQ) return XML_CHAR_SQ;
    // check for value reference
    if (xref == XML_VENT_LT) return XML_CHAR_LT;
    if (xref == XML_VENT_GT) return XML_CHAR_GT;
    if (xref == XML_VENT_AM) return XML_CHAR_AM;
    if (xref == XML_VENT_DQ) return XML_CHAR_DQ;
    if (xref == XML_VENT_SQ) return XML_CHAR_SQ;
    throw Exception ("xml-error", "cannot convert character entity reference",
		     xref);
  }
  
  // convert a character to a character entity

  String XmlSystem::tocent (const t_quad c) {
    String result = "&#x";
    result += Utility::tohexa (c);
    result += ';';
    return result;
  }

  // convert a character to an escape entity

  String XmlSystem::tocesc (const t_quad c) {
    // check the standard escape
    if (c == XML_CHAR_LT) return XML_TENT_LT;
    if (c == XML_CHAR_GT) return XML_TENT_GT;
    if (c == XML_CHAR_AM) return XML_TENT_AM;
    if (c == XML_CHAR_DQ) return XML_TENT_DQ;
    if (c == XML_CHAR_SQ) return XML_TENT_SQ;
    // default to character entity
    return tocent (c);
  }

  // escape a text string with the predefined entities

  String XmlSystem::totesc (const String& text) {
    // prepare block result
    t_array<t_quad> blok;
    // get data block and iterate
    long size = text.length (); t_transient<t_quad> data = text.toquad ();
    for (long i = 0; i < size; i++) {
      t_quad c = data[i];
      switch (c) {
      case XML_CHAR_LT:
	xml_blok_add (blok, XML_TENT_LT);
	break;
      case XML_CHAR_GT:
	xml_blok_add (blok, XML_TENT_GT);
	break;
      case XML_CHAR_AM:
	xml_blok_add (blok, XML_TENT_AM);
	break;
      case XML_CHAR_DQ:
	xml_blok_add (blok, XML_TENT_DQ);
	break;
      case XML_CHAR_SQ:
	xml_blok_add (blok, XML_TENT_SQ);
	break;
      default:
	blok.push (c);
	break;
      }
    }
    blok.push (nilq);
    // prepare result
    String result (blok.getdata ());
    return result;
  }

  // escape a value string with the predefined entities

  String XmlSystem::tovesc (const String& text) {
    // prepare block result
    t_array<t_quad> blok;
    // get data block and iterate
    long size = text.length (); t_transient<t_quad> data = text.toquad ();
    for (long i = 0; i < size; i++) {
      t_quad c = data[i];
      if (c == XML_CHAR_LT) {
	xml_blok_add (blok, XML_VENT_LT);
	continue;
      }
      if (c == XML_CHAR_GT) {
	xml_blok_add (blok, XML_VENT_GT);
	continue;
      }
      if (c == XML_CHAR_AM) {
	xml_blok_add (blok, XML_VENT_AM);
	continue;
      }
      if (c == XML_CHAR_DQ) {
	xml_blok_add (blok, XML_VENT_DQ);
	continue;
      }
      if (c == XML_CHAR_SQ) {
	xml_blok_add (blok, XML_VENT_SQ);
	continue;
      }
      blok.push (c);
    }
    blok.push (nilq);
    // prepare result
    String result (blok.getdata ());
    return result;
  }

  // escape completly a text string with unicode characters

  String XmlSystem::touesc (const String& text) {
    // prepare block result
    t_array<t_quad> blok;
    // get data block and iterate
    long size = text.length (); t_transient<t_quad> data = text.toquad ();
    for (long i = 0; i < size; i++) {
      t_quad c = data[i];
      switch (c) {
      case XML_CHAR_LT:
	xml_blok_add (blok, XML_TENT_LT);
	break;
      case XML_CHAR_GT:
	xml_blok_add (blok, XML_TENT_GT);
	break;
      case XML_CHAR_AM:
	xml_blok_add (blok, XML_TENT_AM);
	break;
      case XML_CHAR_DQ:
	xml_blok_add (blok, XML_TENT_DQ);
	break;
      case XML_CHAR_SQ:
	xml_blok_add (blok, XML_TENT_SQ);
	break;
      default:
	if (Unicode::isascii (c) == false) {
	  xml_blok_add (blok, XmlSystem::tocent (c));
	  break;
	}
	blok.push (c);
	break;
      }
    }
    blok.push (nilq);
    // prepare result
    String result (blok.getdata ());
    return result;
  }

  // prenormalize a text by keeping the eol

  String XmlSystem::prenorm (const String& text) {
    // do nothing with empty text
    if (text.isnil () == true) return text;
    // create a working buffer
    XsoBuffer xbuf;
    // loop in the text
    long  size = text.length (); t_transient<t_quad> data = text.toquad ();
    for (long i = 0; i < size; i++) {
      t_quad c = data[i];
      if ((c == 0x00000020) || (c == 0x00000009) ||
	  (c == 0x0000000D) || (c == 0x0000000A)) {
	// do not add at the beginning
	if (xbuf.empty () == true) continue;
	// always remove the cr character
	if (c == 0x0000000d) continue;
	// keep only one eol
	if (xbuf.islast (eolq) == true) continue;
      }
      xbuf.add (c);
    }
    // prepare result
    String result = xbuf.tostring ();
    return result.strip ();
  }

  // normalize a text by fixing the spacing charcaters

  String XmlSystem::tonorm (const String& text) {
    // do nothing with empty text
    if (text.isnil () == true) return text;
    // create a working buffer
    XsoBuffer xbuf;
    // loop in the text
    long size = text.length (); t_transient<t_quad> data = text.toquad ();
    for (long i = 0; i < size; i++) {
      t_quad c = data[i];
      if ((c == 0x00000020) || (c == 0x00000009) ||
	  (c == 0x0000000D) || (c == 0x0000000A)) {
	if (xbuf.islast (blkq) == true) continue;
	xbuf.add (blkq);
      } else {
	xbuf.add (c);
      }
    }
    return xbuf.tostring ();
  }

  // convert a property into an attribute string

  String XmlSystem::toattr (const Property& prop) {
    // get the attribute name
    String result = prop.getname ();
    if (result.isnil ()) return result;
    // get the attribute value
    String pval = prop.getpval ();
    result += "=";
    result += '"';
    result += XmlSystem::tovesc (pval.tostring ());
    result += '"';
    return result;
  }

  // convert a property list into an attribute string

  String XmlSystem::toattr (const Plist& plst) {
    // get the plist length
    long plen = plst.length ();
    // format result
    String result;
    for (long i = 0; i < plen; i++) {
      Property* prop = plst.get (i);
      if (prop == nullptr) continue;
      String attr = toattr (*prop);
      if (attr.isnil () == true) continue;
      result += attr;
      if (i < plen - 1) result += ' ';
    }
    return result;
  }
}
