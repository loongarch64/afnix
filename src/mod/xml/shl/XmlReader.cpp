// ---------------------------------------------------------------------------
// - XmlReader.cpp                                                           -
// - afnix:xml module - xml reader class implementation                      -
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

#include "XmlPe.hpp"
#include "XmlGe.hpp"
#include "XmlTag.hpp"
#include "XmlEnd.hpp"
#include "XmlRoot.hpp"
#include "XmlText.hpp"
#include "XmlData.hpp"
#include "XmlCref.hpp"
#include "XmlEref.hpp"
#include "Unicode.hpp"
#include "Nameable.hpp"
#include "Evaluable.hpp"
#include "XsoStream.hpp"
#include "XmlBuffer.hpp"
#include "XmlReader.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlAttlist.hpp"
#include "XmlComment.hpp"
#include "XmlDoctype.hpp"
#include "XmlElement.hpp"
#include "XmlSection.hpp"
#include "InputString.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the standard entity marker
  static const t_quad XML_SREF_MC = 0x7FFFFFFFUL;
  // the xml special characters
  static const t_quad XML_CHAR_LT = 0x0000003CUL; // <
  static const t_quad XML_CHAR_GT = 0x0000003EUL; // >
  static const t_quad XML_CHAR_AM = 0x00000026UL; // &
  static const t_quad XML_CHAR_EP = 0x00000021UL; // !
  static const t_quad XML_CHAR_QM = 0x0000003FUL; // ?
  static const t_quad XML_CHAR_SL = 0x0000002FUL; // /
  static const t_quad XML_CHAR_MN = 0x0000002DUL; // -
  static const t_quad XML_CHAR_SC = 0x0000003BUL; // ;
  static const t_quad XML_CHAR_LB = 0x0000005BUL; // [
  static const t_quad XML_CHAR_RB = 0x0000005DUL; // ]
  static const t_quad XML_CHAR_PC = 0x00000025UL; // %

  // the xml version attribute
  static const String XML_ATTR_XVID = "version";
  // the xml encoding attribute
  static const String XML_ATTR_EMOD = "encoding";
  // the xml standalone attribute
  static const String XML_ATTR_STND = "standalone";

  // return true if the string is a yes/no value
  static inline bool is_yes_no (const String& sval) {
    if (sval == "no")  return true;
    if (sval == "yes") return true;
    return false;
  }
  
  // this function parse a subset declaration
  static XmlNode* parse_xml_subs (XmlBuffer& xbuf) {
    // double check as usual
    if (xbuf.issubs () == false) return nullptr;
    // get the subset string
    String subs = xbuf.getsubs ();
    // get the buffer version
    String xvid = xbuf.getxvid ();
    // create a new reader
    XmlReader xmlr (xvid);
    // parse the string
    xmlr.parse (subs);
    // get the root node and 
    XmlNode* result = xmlr.getroot ();
    // protect the node and return
    Object::iref (result);
    return result;
  }

  // this function parse a xml text node
  static XmlNode* parse_xml_text (XmlBuffer& xbuf) {
    // create a text node by string
    XmlNode* node = new XmlText (xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function parse a xml cdata node
  static XmlNode* parse_xml_data (XmlBuffer& xbuf) {
   // create a text node by string
    XmlNode* node = new XmlData (xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function parse a xml comment node
  static XmlNode* parse_xml_comt (XmlBuffer& xbuf) {
    // create a comment node by string
    XmlNode* node = new XmlComment (xbuf.tostring ());
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function parse a xml entity reference node
  static XmlNode* parse_xml_xref (XmlBuffer& xbuf) {
    // rebuild the entity string
    String text = xbuf.tostring ();
    String xref = "&";
    xref += text;
    xref += ";";
    // check for a character reference
    XsoBuffer xsob = xref;
    // the xml node result
    XmlNode* node = nullptr;
    // check for a character reference
    if (xsob.iscref() == true) {
      // get the character value
      t_quad c = xsob.tocref();
      // create the character node
      node = new XmlCref (c);
    } else {
      // create an entity node
      node = new XmlEref (text);
    }
    // set the node line number
    node->setlnum (xbuf.getlnum ());
    // clear the buffer
    xbuf.reset ();
    // here is the xref node
    return node;
  }

  // this function parse a xml tag
  static XmlNode* parse_xml_tag (XmlBuffer& xbuf, const bool eflg) {
    // get the tag name
    String name = xbuf.getname ();
    // create the result tag
    XmlTag* tag = new XmlTag (name, eflg);
    // set the node line number
    tag->setlnum (xbuf.getlnum ());
    // add the tag attributes
    while (xbuf.isnext () == true) {
      Property attr = xbuf.getattr ();
      tag->addattr (new Property (attr));
    }
    // here is the tag
    return tag;
  }

  // this function parse a xml end tag
  static XmlNode* parse_xml_etag (XmlBuffer& xbuf) {
    // get the tag name
    String name = xbuf.getname ();
    // check that the buffer is empty
    if (xbuf.empty () == false) {
      throw Exception ("xml-error", "trailing character with end tag", name);
    }
    XmlNode* node = new XmlEnd (name);
    // set the node line number
    node->setlnum (xbuf.getlnum());
    // here is the node
    return node;
  }

  // this function parse xml declaration
  static XmlNode* parse_xml_decl (XmlBuffer& xbuf) {
    // get the version in the form of an attribute
    Property pvid = xbuf.getattr ();
    // check the attribute
    if (pvid.getname () != XML_ATTR_XVID) {
      throw Exception ("xml-error", "missing version in xml declaration");
    }
    String xvid = pvid.getpval ();
    if (xbuf.isnext () == false) {
      // create the declaration node
      XmlNode* node = new XmlDecl (xvid);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // get optional encoding or standalone
    Property prop = xbuf.getattr ();
    if (prop.getname () == XML_ATTR_STND) {
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", "trailing data in xml declaration");
      }
      String stnd = prop.getpval ();
      // create the declaration node
      XmlNode* node = new XmlDecl (xvid, "", stnd);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // here it can only be the encoding
    if (prop.getname () !=  XML_ATTR_EMOD) {
      throw Exception ("xml-error", "invalid attribute in xml declaration",
		       prop.getname ());
    }
    String emod = prop.getpval ();
    if (xbuf.isnext () == false) {
      // create the declaration node
      XmlNode* node = new XmlDecl (xvid, emod);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // get the standalone property
    prop = xbuf.getattr ();
    if (prop.getname () !=  XML_ATTR_STND) {
      throw Exception ("xml-error", "invalid attribute in xml declaration",
		       prop.getname ());
    }
    if (xbuf.isnext () == true) {
      throw Exception ("xml-error", "trailing data in xml declaration");
    }
    String stnd = prop.getpval ();
    if (is_yes_no (stnd) == false) {
      throw Exception ("xml-error", "invalid standalone attribute value",
		       stnd);
    }
    // create the declaration node
    XmlNode* node = new XmlDecl (xvid, emod, stnd);
    //set the node line number
    node->setlnum (xbuf.getlnum());
    // here is the node
    return node;
  }

  // this function parse a processing instruction
  static XmlNode* parse_xml_pi (XmlBuffer& xbuf) {
    // get the node name
    String xval = xbuf.getname  ();
    // check for xml declaration
    if ((xval == "XML") || (xval == "xml")) return parse_xml_decl (xbuf);
    // return a regular processing instruction
    String info = xbuf.tostring ();
    // create the pi node
    XmlNode* node = new XmlPi (xval, info);
    //set the node line number
    node->setlnum (xbuf.getlnum());
    // here is the node
    return node;
  }

  // this function parse a doctype element
  static XmlNode* parse_xml_doct (XmlBuffer& xbuf) {
    // get the document type name
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing document type name");
    }
    String xval = xbuf.getname ();
    if (xbuf.isnext () == false) {
      // create the document type node
      XmlNode* node =  new XmlDoctype (xval);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // check for a subset declaration
    if (xbuf.issubs () == true) {
      // create the document type node
      XmlDoctype* doct = new XmlDoctype (xval);
      //set the node line number
      doct->setlnum (xbuf.getlnum());
      // get the subset declaration
      XmlNode* node = parse_xml_subs (xbuf);
      // check if we have some garbage
      if (xbuf.isnext () == true) {
	Object::cref (doct);
	Object::dref (node);
	throw Exception ("xml-error", 
			 "trailing characters after subset declaration",
			 xbuf.tostring ());
      }
      // update the node and clean
      doct->setnode (node);
      Object::tref (node);
      return doct;
    }
    // check for an external id
    String eidt = xbuf.getname ();
    // the external id must be either PUBLIC or SYSTEM
    if (eidt == "SYSTEM") {
      // get the system id literal
      String sysl = xbuf.getqstr ();
      // create the document type node
      XmlDoctype* doct = new XmlDoctype (xval, sysl);
      //set the node line number
      doct->setlnum (xbuf.getlnum());
      // check for a subset declaration
      if (xbuf.issubs () == true) {
	// get the subset declaration
	XmlNode* node = parse_xml_subs (xbuf);
	// check if we have some garbage
	if (xbuf.isnext () == true) {
	  Object::cref (doct);
	  Object::dref (node);
	  throw Exception ("xml-error", 
			   "trailing characters after subset declaration",
			   xbuf.tostring ());
	}
	// update the node and clean
	doct->setnode (node);
	Object::tref (node);
      }
      // check for garbage
      if (xbuf.isnext () == true) {
	Object::cref (doct);
	throw Exception ("xml-error", "trailing characters with external id",
			 xbuf.tostring ());
      }
      // here is the node
      return doct;
    }
    if (eidt == "PUBLIC") {
      // get the public id literal
      String publ = xbuf.getqstr ();
      // get the system id literal
      String sysl = xbuf.getqstr ();
      // create the document type node
      XmlDoctype* doct = new XmlDoctype (xval, publ, sysl);
      //set the node line number
      doct->setlnum (xbuf.getlnum());
      // check for a subset declaration
      if (xbuf.issubs () == true) {
	// get the subset declaration
	XmlNode* node = parse_xml_subs (xbuf);
	// check if we have some garbage
	if (xbuf.isnext () == true) {
	  Object::cref (doct);
	  Object::dref (node);
	  throw Exception ("xml-error", 
			   "trailing characters after subset declaration",
			   xbuf.tostring ());
	}
	// update the node and clean
	doct->setnode (node);
	Object::tref (node);
      }
      // check for garbage
      if (xbuf.isnext () == true) {
	Object::cref (doct);
	throw Exception ("xml-error", "trailing characters with external id",
			 xbuf.tostring ());
      }
      // here is the node
      return doct;
    }
    throw Exception ("xml-error", "invalid external id definition", eidt);
  }

  // this function parse a section node
  static XmlNode* parse_xml_sect (XmlBuffer& xbuf) {
    // get the section name
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing section name");
    }
    String xval = xbuf.getpnam ();
    // check for the section node
    if (xbuf.issubs () == false) {
      throw Exception ("xml-error", "missing section node definition");
    }
    // create the section node
    XmlSection* sect = new XmlSection (xval);
    //set the node line number
    sect->setlnum (xbuf.getlnum());
    // get the subset declaration
    XmlNode* node = parse_xml_subs (xbuf);
    // check if we have some garbage
    if (xbuf.isnext () == true) {
      Object::cref (sect);
      Object::dref (node);
      throw Exception ("xml-error", 
		       "trailing characters after subset declaration",
		       xbuf.tostring ());
    }
    // update the node and clean
    sect->setnode (node);
    Object::tref (node);
    return sect;
  }

  // this function parse a parameter entity element
  static XmlNode* parse_xml_pent (XmlBuffer& xbuf) {
    // check that we have a parameter entity
    if (xbuf.getu () != XML_CHAR_PC) {
      throw Exception ("internal-error", 
		       "trying to parse a parameter entity");
    }
    // get the entity name
    String name = xbuf.getname ();
    // here we have a general entity - check for a value
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing entity value");
    }
    if (xbuf.isqstr () == true) {
      String xval = xbuf.getqstr ();
      // check for garbage
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", 
			 "garbage characters in parameter entity declaration");
      }
      XmlNode* node = new XmlPe (name, xval);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // get a regular name and check for system or public
    String eidt = xbuf.getname ();
    // check for system
    if (eidt == "SYSTEM") {
      // get the system id literal
      String sysl = xbuf.getqstr ();
      // check for garbage
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", 
			 "garbage characters in parameter entity declaration");
      }
      // build the result node
      XmlNode* node = new XmlPe (name, "", sysl);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // check for public
    if (eidt == "PUBLIC") {
      // get the public id literal
      String publ = xbuf.getqstr ();
      // get the system id literal
      String sysl = xbuf.getqstr ();
      // check for garbage
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", 
			 "garbage characters in parameter entity declaration");
      }
      // build the result node
      XmlNode* node = new XmlPe (name, publ, sysl);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    throw Exception ("xml-error", "invalid parameter entity definition", eidt);
  }

  // this function parse an entity element
  static XmlNode* parse_xml_gent (XmlBuffer& xbuf) {
    // get the entity name
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing entity name");
    }
    // check for a parameter entity
    if (xbuf.get () == XML_CHAR_PC) return parse_xml_pent (xbuf);
    // get the entity name
    String name = xbuf.getname ();
    // here we have a general entity - check for a value
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing entity value");
    }
    if (xbuf.isqstr () == true) {
      String xval = xbuf.getqstr ();
      // check for garbage
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", 
			 "garbage characters in entity declaration");
      }
      XmlNode* node = new XmlGe (name, xval);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // get a regular name and check for system or public
    String eidt = xbuf.getname ();
    // check for system
    if (eidt == "SYSTEM") {
      // get the system id literal
      String sysl = xbuf.getqstr ();
      // check for extra definition
      if (xbuf.isnext () == false) {
	XmlNode* node = new XmlGe (name, "", sysl);
	//set the node line number
	node->setlnum (xbuf.getlnum());
	// here is the node
	return node;
      }
      // get the entity data
      String xtra = xbuf.getname ();
      if (xtra != "NDATA") {
	throw Exception ("xml-error", "garbage datain entity declaration",
			 xtra);
      }
      if (xbuf.isnext () == false) {
	throw Exception ("xml-error", 
			 "missing ndata value in entity declaration");
      }
      String data = xbuf.getname ();
      // check for garbage
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", 
			 "garbage characters in entity declaration");
      }
      // build the result node
      XmlNode* node = new XmlGe (name, "", sysl, data);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    // check for public
    if (eidt == "PUBLIC") {
      // get the public id literal
      String publ = xbuf.getqstr ();
      // get the system id literal
      String sysl = xbuf.getqstr ();
      // check for extra definition
      if (xbuf.isnext () == false) {
	XmlNode* node = new XmlGe (name, publ, sysl);
	//set the node line number
	node->setlnum (xbuf.getlnum());
	// here is the node
	return node;
      }
      // get the entity data
      String xtra = xbuf.getname ();
      if (xtra != "NDATA") {
	throw Exception ("xml-error", "garbage datain entity declaration",
			 xtra);
      }
      if (xbuf.isnext () == false) {
	throw Exception ("xml-error", 
			 "missing ndata value in entity declaration");
      }
      String data = xbuf.getname ();
      // check for garbage
      if (xbuf.isnext () == true) {
	throw Exception ("xml-error", 
			 "garbage characters in entity declaration");
      }
      // build the result node
      XmlNode* node = new XmlGe (name, publ, sysl, data);
      //set the node line number
      node->setlnum (xbuf.getlnum());
      // here is the node
      return node;
    }
    throw Exception ("xml-error", "invalid general entity definition", eidt);
  }

  // this function parse a xml element
  static XmlNode* parse_xml_elem (XmlBuffer& xbuf) {
    // get the entity name
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing element name");
    }
    String name = xbuf.getpnam ();
    // strip the buffer and get the value
    xbuf.strip ();
    String xval = xbuf.tostring ();
    // build the result node
    XmlNode* node = new XmlElement (name, xval);
    //set the node line number
    node->setlnum (xbuf.getlnum());
    // clear the buffer
    xbuf.reset ();
    // here is the node
    return node;
  }

  // this function parse a xml attribute list
  static XmlNode* parse_xml_attl (XmlBuffer& xbuf) {
    // get the attribute element name
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing attribute element name");
    }
    String name = xbuf.getpnam ();
    // get the attribute name
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing attribute name");
    }
    String attr = xbuf.getpnam ();
    // create the attribute node
    XmlAttlist* node = new XmlAttlist (name, attr);
    node->setlnum (xbuf.getlnum());
    if (xbuf.isnext () == false) return node;
    // get the attribute type    
    String type = xbuf.getenam ();
    // check for enumeration
    if (xbuf.isenum (type) == true) {
      Strvec xenm = xbuf.getxenm (type);
      node->settype (xenm, false);
    } else if (type == "NOTATION") {
      if (xbuf.isnext () == false) {
	throw Exception ("xml-error", "missing notation attribute type");
      }
      String xval = xbuf.getenam ();
      Strvec xenm = xbuf.getxenm (xval);
      node->settype (xenm, true);
    } else {
      node->settype (type);
    }
    // get the attribute default
    if (xbuf.isnext () == false) {
      throw Exception ("xml-error", "missing attribute default");
    }
    String xdef = xbuf.getxdef ();
    if (xdef == "#FIXED") {
      // get the attribute default
      if (xbuf.isnext () == false) {
	throw Exception ("xml-error", "missing attribute fixed default");
      }
      String xval = xbuf.getqstr ();
      node->setfixd (xval);
    } else {
      node->setxdef (xdef);
    }
    if (xbuf.empty () == false) {
      throw Exception ("xml-error", "trailing character in attribute list",
		       xbuf.tostring ());
    } 
    // here is the node
    return node;
  }
  
  // this function parse a reserved tag
  static XmlNode* parse_xml_rtag (XmlBuffer& xbuf) {
    // get the reserve tag name
    String name = xbuf.getname ();
    // dispatch according to type
    if (name == "ENTITY")  return parse_xml_gent (xbuf);
    if (name == "DOCTYPE") return parse_xml_doct (xbuf);
    if (name == "ELEMENT") return parse_xml_elem (xbuf);
    if (name == "ATTLIST") return parse_xml_attl (xbuf);
    // invalid reserved tag name
    throw Exception ("xml-error", "invalid reserved tag name", name);
  }

  // this function parse a xml node
  static XmlNode* parse_xml_node (XsoStream& xis, XmlBuffer& xbuf) {
    // state: s_helo
    // get the first character for dispatch
  s_helo:
    t_quad c = xis.getu ();
    // process normally
    switch (c) {
    case XML_CHAR_LT:
      xbuf.begin (xis.getlnum ());
      goto s_stag;
    case XML_CHAR_AM:
      xbuf.begin (xis.getlnum ());
      goto s_xref;
    case nilq:
    case eosq:
      return nullptr;
    default:
      c &= XML_SREF_MC;
      if (xbuf.isspc (c) == true) {
	if (xbuf.isskip (c) == false) {
	  xbuf.add (c);
	  xbuf.setlnum (xis.getlnum ());
	}
	goto s_helo;
      }
      xbuf.add (c);
      xbuf.setlnum (xis.getlnum ());
      goto s_text;
    }

    // state: s_text
    // read some text and accumulate
  s_text:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_AM:
    case XML_CHAR_LT:
    case eosq:
      xis.pushback (c);
      return parse_xml_text (xbuf);
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_text;
    }

    // state: s_stag
    // start a tag or any xml stuff
  s_stag:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_EP:
      goto s_rtag;
    case XML_CHAR_QM:
      goto s_ptag;
    case XML_CHAR_SL:
      goto s_etag;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_ntag
    // read a normal tag
  s_ntag:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_SL:
      goto s_entg;
    case XML_CHAR_GT:
      return parse_xml_tag (xbuf, false);
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_entg
    // read an empty normal tag
  s_entg:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_GT:
      return parse_xml_tag (xbuf, true);
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;      
      xbuf.add (XML_CHAR_SL);
      xbuf.add (c);
      goto s_ntag;
    }

    // state: s_etag
    // read an end tag
  s_etag:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_GT:
      return parse_xml_etag (xbuf);
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;      
      xbuf.add (c);
      goto s_etag;
    }

    // state: s_rtag
    // start a reserved tag
  s_rtag:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_MN:
      goto s_bgcm;
    case XML_CHAR_LB:
      goto s_bgcs;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;   
      xbuf.add (c);
      goto s_rdrt;
    }

    // state: s_rdrt
    // read a reserved tag
  s_rdrt:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_GT:
      return parse_xml_rtag (xbuf);
    case XML_CHAR_LB:
      xbuf.add (c);
      goto s_rsub;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;   
      xbuf.add (c);
      goto s_rdrt;
    }

    // state: s_rsub
    // read a subset node in a reserved tag
  s_rsub:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_RB:
      xbuf.add (c);
      goto s_rdrt;
    case XML_CHAR_LB:
      goto s_error;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_rsub;
    }

    // state: s_bgcm
    // begin a comment node
  s_bgcm:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_MN:
      goto s_ctag;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_rtag;
    }

    // state: s_ctag
    // read a comment node
  s_ctag:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_MN:
      goto s_encm;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_ctag;
    }

    // state: s_encm
    // start the end comment node
  s_encm:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_MN:
      goto s_fncm;
    case eosq:
      goto s_error;
    default:
      c &= XML_SREF_MC;
      xbuf.add (XML_CHAR_MN);
      xbuf.add (c);
      goto s_ctag;
    }

    // state: s_fncm
    // finish the end comment node
  s_fncm:
    c = xis.getu ();
    // process normally
    switch (c) {
    case XML_CHAR_GT:
      return parse_xml_comt (xbuf);
    case eosq:
      throw Exception ("xml-error", "unterminated comment node");
    default:
      throw Exception ("xml-error", "invalid -- sequence in comment node");
    }

    // state: s_bgcs
    // start a cdata or section node
  s_bgcs:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_LB:
      if (xbuf.tostring () == "CDATA") {
	xbuf.begin (xis.getlnum ());
	goto s_rdcd;
      }
      xbuf.add (c);
      goto s_rdcs;
    case eosq:
      throw Exception ("xml-error", "unterminated node", xbuf.tostring ());
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_bgcs;
    }

    // state: s_rdcd
    // read cdata characters
  s_rdcd:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_RB:
      goto s_encd;
    case eosq:
      throw Exception ("xml-error", "unterminated cdata node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_rdcd;
    }

    // state: s_encd
    // start end cdata node
  s_encd:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_RB:
      goto s_fncd;
    case eosq:
      throw Exception ("xml-error", "unterminated cdata node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (XML_CHAR_RB);
      xbuf.add (c);
      goto s_rdcd;
    }

    // state: s_fncd
    // finish end cdata node
  s_fncd:
    c = xis.getu ();
    // process normally
    switch (c) {
    case XML_CHAR_GT:
      return parse_xml_data (xbuf);
    case eosq:
      throw Exception ("xml-error", "unterminated cdata node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (XML_CHAR_RB);
      xbuf.add (XML_CHAR_RB);
      xbuf.add (c);
      goto s_rdcd;
    }

    // state: s_rdcs
    // read section character
  s_rdcs:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_RB:
      goto s_encs;
    case eosq:
      throw Exception ("xml-error", "unterminated section node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_rdcs;
    }

    // state: s_encs
    // start end section node
  s_encs:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_RB:
      goto s_fncs;
    case eosq:
      throw Exception ("xml-error", "unterminated section node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (XML_CHAR_RB);
      xbuf.add (c);
      goto s_rdcs;
    }

    // state: s_fncs
    // finish end section node
  s_fncs:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_GT:
      xbuf.add (XML_CHAR_RB);
      return parse_xml_sect (xbuf);
    case eosq:
      throw Exception ("xml-error", "unterminated section node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (XML_CHAR_RB);
      xbuf.add (XML_CHAR_RB);
      xbuf.add (c);
      goto s_rdcs;
    }

    // state: s_ptag
    // processing instruction tag
  s_ptag:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_QM:
      goto s_etpi;
    case eosq:
      throw Exception ("xml-error", "unterminated processing node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_ptag;
    }

    // state: s_etpi
    // end processing instruction tag
  s_etpi:
    c = xis.getu ();
    switch (c) {
    case XML_CHAR_GT:
      return parse_xml_pi (xbuf);
    case eosq:
      throw Exception ("xml-error", "unterminated processing node");
    default:
      c &= XML_SREF_MC;
      xbuf.add (c);
      goto s_ptag;
    }

    // state: s_xref
    // process an entity/character reference node
  s_xref:
    c = xis.getu ();
    // process normally
    switch (c) {
    case XML_CHAR_SC:
      return parse_xml_xref (xbuf);
    case XML_CHAR_LT:
    case eosq:
      xbuf.pushback (XML_CHAR_AM);
      xis.pushback  (c);
      return parse_xml_text (xbuf);
    default:
      c &= XML_SREF_MC;
      if (xbuf.isspc (c) == true) {
	xbuf.pushback (XML_CHAR_AM);
	xis.pushback  (c);
	return parse_xml_text (xbuf);
      }
      xbuf.add (c);
      goto s_xref;
    }

    // state: s_error
    // return a syntax error
  s_error:
    throw Exception ("xml-error", "syntax error while reading node");
  }

  // this function parse a xml tree - be carefull in this code as
  // the top stack node is always popped and sometime pushed-back
  static void parse_xml_tree (XsoStream& xis, XmlRoot* root) {
    // do nothing without a root node
    if (root == nullptr) return;
    // create a xml buffer
    XmlBuffer xbuf;
    // create a vector stack and push the root node
    Vector stk;
    stk.add (root);
    try {
      // loop as long as we have a valid stream
      while (xis.valid () == true) { 
	// get the next available node
	XmlNode* node = parse_xml_node (xis, xbuf);
	if (node== nullptr) break;
	// pop the current stack node
	XmlNode* pnod = dynamic_cast <XmlNode*> (stk.rml ());
	if (pnod == nullptr) {
	  throw Exception ("xml-error", "empty stack found during parsing");
	}
	// check if the node is an end tag
	XmlEnd* etag = dynamic_cast <XmlEnd*> (node);
	if (etag != nullptr) {
	  String name = etag->getname ();
	  if (pnod->isname (name) == false) {
	    throw Exception ("xml-error", "end tag name mismatch", name);
	  }
	  Object::cref (etag);
	  continue;
	}
	// check if the node is a declaration node - which might
	// change the version as well as the encoding
	XmlDecl* decl = dynamic_cast <XmlDecl*> (node);
	if (decl != nullptr) {
	  xis.setemod  (decl->getemod ());
	  xbuf.setxmlv (decl->getxvid ());
	}
	// check if the node is an empty node
	if (node->geteflg () == true) {
	  pnod->addchild (node);
	  stk.add (pnod);
	  continue;
	}
	// here the node is attached as a child node and pushed
	pnod->addchild (node);
	stk.add (pnod);
	stk.add (node);
      }
    } catch (Exception& e) {
      e.updlnum (xbuf.getlnum ());
      throw e;
    } catch (...) {
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty reader

  XmlReader::XmlReader (void) {
    d_xvid = XmlSystem::getxvid ();
    p_root = nullptr;
    reset ();
  }

  // create a xml reader with a version

  XmlReader::XmlReader (const String& xvid) {
    if (XmlSystem::isxvid (xvid) == false) {
      throw Exception ("xml-error", "invalid xml version", xvid);
    }
    d_xvid = xvid;
    p_root = nullptr;
    reset ();
  }
  
  // destroy this reader

  XmlReader::~XmlReader (void) {
    reset ();
    Object::dref (p_root);
  }

  // return the document class name

  String XmlReader::repr (void) const {
    return "XmlReader";
  }

  // reset this reader

  void XmlReader::reset (void) {
    wrlock ();
    try {
      Object::dref (p_root);
      p_root = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the root node

  XmlRoot* XmlReader::getroot (void) const {
    rdlock ();
    XmlRoot* result = p_root;
    unlock ();
    return result;
  }

  // get the next available node

  XmlNode* XmlReader::getnode (InputStream* is) {
    // check the input stream
    if (is == nullptr) return nullptr;
    // create a xml stream
    XsoStream xis (is);
    // create a xml buffer
    XmlBuffer xbuf;
    // lock and parse
    wrlock ();
    try {
      // parse the xml tree
      XmlNode* result = parse_xml_node (xis, xbuf);
      unlock ();
      return result;
    } catch (Exception& e) {
      e.updlnum (xis.getlnum ());
      reset  ();
      unlock ();
      throw e;
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // get the next available node

  XmlNode* XmlReader::getnode (const String& value) {
    // create an input stream
    InputStream* is = new InputString (value);
    // lock and parse
    wrlock ();
    try {
      XmlNode* result = getnode (is);
      delete is;
      unlock ();
      return result;
    } catch (...) {
      delete is;
      unlock ();
      throw;
    }
  }

  // parse the input stream

  void XmlReader::parse (InputStream* is) {
    // check the input stream
    if (is == nullptr) return;
    // create a xml stream
    XsoStream xis (is);
    // lock and parse
    wrlock ();
    try {
      //check if we have a root node
      if (p_root == nullptr) Object::iref (p_root = new XmlRoot);
      // parse the xml tree
      parse_xml_tree (xis, p_root);
      unlock ();
    } catch (Exception& e) {
      // set the line number
      e.updlnum (xis.getlnum ());
      // check for a name
      Nameable* nobj = dynamic_cast <Nameable*> (is);
      if (nobj != nullptr) e.setname (nobj->getname ());
      // reset and clean
      reset  ();
      unlock ();
      throw e;
    } catch (...) {
      reset  ();
      unlock ();
      throw;
    }
  }

  // parse a string 

  void XmlReader::parse (const String& value) {
    // create an input stream
    InputStream* is = new InputString (value);
    // lock and parse
    wrlock ();
    try {
      parse  (is);
      delete is;
      unlock ();
    } catch (...) {
      delete is;
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_PARSE   = zone.intern ("parse");
  static const long QUARK_GETROOT = zone.intern ("get-root");
  static const long QUARK_GETNODE = zone.intern ("get-node");

  // create a new object in a generic way

  Object* XmlReader::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default reader object
    if (argc == 0) return new XmlReader;
    // argument error
    throw Exception ("argument-error",
                     "too many argument with xml reader constructor");
  }

  // return true if the given quark is defined

  bool XmlReader::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* XmlReader::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_GETROOT) {
	rdlock ();
	Object* result = getroot ();
	zobj->post (result);
	unlock ();
	return result;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_PARSE) {
	Object* obj = argv->get (0);
	if (obj == nullptr) return nullptr;
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) {
	  parse (is);
	  return nullptr;
	}
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) {
	  parse (*sobj);
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object with parse",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETNODE) {
	Object* obj = argv->get (0);
	if (obj == nullptr) return nullptr;
	// check for an input stream
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is != nullptr) return getnode (is);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return getnode (*sobj);
	throw Exception ("type-error", "invalid object with get-node",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
