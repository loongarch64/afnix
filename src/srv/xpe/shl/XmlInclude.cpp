// ---------------------------------------------------------------------------
// - XmlInclude.cpp                                                          -
// - afnix:xpe module - xml include feature class implementation             -
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

#include "Uri.hpp"
#include "XmlTag.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "XmlInclude.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the xml include name and info
  static const String XML_FTRS_NAME = "xi";
  static const String XML_FTRS_INFO = "xml include 1.0";
  // the xml include processor level
  static const long   XML_FTRS_PLVL = 10;

  // the xi:include tag to process
  static const String XML_XTAG_NAME = "xi:include";
  // the xi:include attributes
  static const String XML_HREF_ATTR = "href";
  static const String XML_TYPE_ATTR = "parse";
  static const String XML_XPTR_ATTR = "xpointer";
  static const String XML_EMOD_ATTR = "encoding";
  static const String XML_ACCP_ATTR = "accept";
  static const String XML_ACCL_ATTR = "accept-language";
  
  // the valid parse attribute value
  static const String XML_TYPE_XVAL = "xml";
  static const String XML_TYPE_TVAL = "text";

  // forward content processing declaration
  static XmlContent* process_xcnt (const XmlContent* xcnt);

  // the xml include structure
  struct s_xi {    
    // the include reference
    String d_href;
    // the parse type
    String d_type;
    // the xpointer reference
    String d_xptr;
    // the encoding mode
    String d_emod;
    // the hhtp accept
    String d_accp;
    // the http accept language
    String d_accl;
    // the reference node
    XmlTag* p_tag;
    // create a xi structure by node
    s_xi (XmlTag* tag) {
      // initialize content
      d_href = "";
      d_type = XML_TYPE_XVAL;
      d_xptr = "";
      d_emod = "";
      d_emod = "";
      d_accp = "";
      d_accl = "";
      p_tag  = nullptr;
      // set the node
      settag (tag);
    }
    // destroy this structure
    ~s_xi (void) {
      Object::dref (p_tag);
    }
    // set the xi tag
    void settag (XmlTag* tag) {
      // check for nil type
      if (tag == nullptr) return;
      if (tag->isname (XML_XTAG_NAME) == false) {
	throw Exception ("internal-error", "invalid tag for include",
			 tag->getname ());
      }
      // save the node
      Object::iref (tag);
      Object::dref (p_tag);
      p_tag = tag;
      // update from attributes
      if (tag->isattr (XML_HREF_ATTR) == true) {
	d_href = tag->getpval (XML_HREF_ATTR);
      }
      if (tag->isattr (XML_TYPE_ATTR) == true) {
	d_type = tag->getpval (XML_TYPE_ATTR);
	if ((d_type != XML_TYPE_XVAL) && (d_type != XML_TYPE_TVAL)) {
	  throw Exception ("xi-error", "invalid parse attribute value",
			   d_type);
	}
      }
      if (tag->isattr (XML_XPTR_ATTR) == true) {
	d_xptr = tag->getpval (XML_XPTR_ATTR);
	if ((d_type != XML_TYPE_XVAL) && (d_xptr.isnil () == false)) {
	  throw Exception ("xi-error", 
			   "invlid xpointer attribute with non xml type",
			   d_type);
	}
      }
      if (tag->isattr (XML_EMOD_ATTR) == true) {
	d_emod = tag->getpval (XML_EMOD_ATTR);
      }
      if (tag->isattr (XML_ACCP_ATTR) == true) {
	d_accp = tag->getpval (XML_ACCP_ATTR);
      }
      if (tag->isattr (XML_ACCL_ATTR) == true) {
	d_accl = tag->getpval (XML_ACCL_ATTR);
      }
    }
    // process the tag with a root node and normalized uri
    void process (const String& nuri) {
      // check first for circular reference
      if ((d_href.isnil () == true) && (d_type == XML_TYPE_XVAL) &&
	  (d_xptr.isnil () == true)) {
	throw Exception ("xi-error", "self xml document inclusion", nuri);
      }
      // create a new uri from the reference
      Uri  uri = nuri ;
      Uri huri = uri.gethref (d_href);
      // initialize the content to load
      XmlContent* bcnt = nullptr;
      // check for xml type
      if (d_type == XML_TYPE_XVAL) {
	bcnt = new XmlContent (d_href, huri.getname ());
      }
      // check for text type
      if (d_type == XML_TYPE_TVAL) {
	bcnt = new XmlContent (d_href, huri.getname (), d_emod);
      }
      // process this content
      XmlContent* xcnt = nullptr;
      try {
	xcnt = process_xcnt (bcnt);
	delete bcnt;
      } catch (...) {
	delete bcnt;
	throw;
      }
      // merge the content
      try {
	if (xcnt == nullptr) return;
	// get the root node and clean the declaration
	XmlRoot* root = xcnt->getroot ();
	if (root != nullptr) root = root->bdup ();
	// get the parent node find the node index and merge
	XmlNode* pnod = p_tag->getparent ();
	if (pnod != nullptr) {
	  long nidx = pnod->getnidx (p_tag);
	  pnod->merge (root, nidx);
	}
	root->release (); delete root;
	delete xcnt;
      } catch (...) {
	delete xcnt;
	throw;
      }
    }
  };

  // process a node with a document name
  static void process_node (XmlNode* node, const String nuri) {
    // check for nil node
    if (node == nullptr) return;
    // process the include node
    long clen = node->lenchild ();
    for (long i = 0; i < clen; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      if (cnod->isname (XML_XTAG_NAME) ==  true) {
	// get the tag to process
	XmlTag* tag = dynamic_cast <XmlTag*> (cnod);
	if (tag == nullptr) continue;
	// create a xi structure
	s_xi xi (tag);
	// process the node by name
	xi.process (nuri);
	// restart the count
	clen = node->lenchild ();
	i = 0;
	// done with this node
	continue;
      }
    }
    // process children if any
    clen = node->lenchild ();
    for (long i = 0; i < clen; i++) {
      XmlNode* cnod = node->getchild (i);
      if (cnod == nullptr) continue;
      if (cnod->nilchild () == false) process_node (cnod, nuri);
    }
  }

  // process the xi:include node in a tree
  static XmlContent* process_xcnt (const XmlContent* xcnt) {
    // check for nil
    if (xcnt == nullptr) return nullptr;
    // create a working content
    XmlContent* result = new XmlContent (*xcnt);
    // get the root node
    XmlRoot* root= result->getroot ();
    // do nothing if nil
    if (root == nullptr) return result;
    // get the document name
    String nuri = result->getnuri ();
    // process the node
    process_node (root, nuri);
    // here is the result
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default xml include feature

  XmlInclude::XmlInclude (void) : XmlFeature (XML_FTRS_NAME, XML_FTRS_INFO) {
    d_plvl = XML_FTRS_PLVL;
  }

  // create a xml include feature with a processing level

  XmlInclude::XmlInclude (const long plvl) : 
    XmlFeature (XML_FTRS_NAME, XML_FTRS_INFO) {
    d_plvl = plvl;
  }

  // return the class name

  String XmlInclude::repr (void) const {
    return "XmlInclude";
  }

  // process a xml content and generate a new content

  XmlContent* XmlInclude::process (const XmlContent* xcnt) {
    rdlock ();
    try {
      // get a copy of the tree
      XmlContent* result = process_xcnt (xcnt);
      // here it is
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

  Object* XmlInclude::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new XmlInclude;
    // check for 1 argument
    if (argc == 1) {
      long plvl = argv->getlong (0);
      return new XmlInclude (plvl);
    }
    // wrong arguments
    throw Exception ("argument-error",
                     "too many arguments with xml include constructor");
  }
}
