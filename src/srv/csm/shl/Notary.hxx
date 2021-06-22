// ----------------------------------------------------------------------------
// - Notary.hxx                                                               -
// - afnix:csm module - xml common notary reader definition                   -
// ----------------------------------------------------------------------------
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

#ifndef  AFNIX_NOTARY_HXX
#define  AFNIX_NOTARY_HXX

#include "XmlTag.hpp"
#include "UriStream.hpp"
#include "TcpClient.hpp"
#include "RealmZone.hpp"
#include "XmlComment.hpp"
#include "XmlDocument.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - common section                                                        -
  // -------------------------------------------------------------------------

  // the afnix xml namespace
  static const String XML_XNS_NAME  = "afnix";
  // the csm node name
  static const String XML_CSM_NAME  = "csm";
  // the notary node name
  static const String XML_NTY_NAME  = "notary";
  // the credential node name
  static const String XML_RLM_NAME  = "realm";
  // the description comment
  static const String XML_NTY_COMT  = "notary description";
  // the xmlns attribute
  static const String XML_XXNS_ATTR = "xmlns:afnix";
  // the xmlns attribute value
  static const String XML_XXNS_XVAL = "http://www.afnix.org/csm";
  // the name attribute
  static const String XML_NAME_ATTR = "name";
  // the info attribute
  static const String XML_INFO_ATTR = "info";
  // the type attribute
  static const String XML_TYPE_ATTR = "type";
  // the root attribute
  static const String XML_ROOT_ATTR = "root";

  // the realm type value
  static const String XML_TVAL_ZONE = "zone";
  
  // -------------------------------------------------------------------------
  // - reader section                                                        -
  // -------------------------------------------------------------------------
  
  // this procedure finds a node by name in the evrix namespace
  static inline XmlNode* xrd_get_node (XmlNode* root, const String& name) {
    if (root == nullptr) return nullptr;
    // try to find the node by name
    XmlNode* result = root->getchild (name);
    if (result != nullptr) return result;
    // try with the namespace
    String ns = XML_XNS_NAME; ns += ':'; ns += name;
    return root->getchild (ns);
  }

  // this procedure finds a tag by name in the evrix namespace
  static inline XmlTag* xrd_get_tag (XmlNode* root, const String& name) {
    // get the node by name
    XmlNode* node = xrd_get_node (root, name);
    // check for nil
    if (node == nullptr) return nullptr;
    // check for a tag
    XmlTag* tag = dynamic_cast <XmlTag*> (node);
    if (tag != nullptr) return tag;
    // invalid node
    throw Exception ("csm-error", "invalid xml node in csm structure",
                     Object::repr (node));
  }

  // this procedure creates a realm from a notary tag
  static Realm* xrd_get_rlm (XmlNode* nty) {
    // check for valid tag
    if (nty == nullptr) return nullptr;
    // get the realm node
    XmlTag* tag = xrd_get_tag (nty, XML_RLM_NAME);
    // check type attribute
    if (tag->isattr (XML_TYPE_ATTR) == false) {
      throw Exception ("csm-error", "missing realm type in node");
    }
    String type = tag->getpval (XML_TYPE_ATTR);
    // check for a zone
    if (type == XML_TVAL_ZONE) {
      // check name attribute
      if (tag->isattr (XML_NAME_ATTR) == false) {
	throw Exception ("csm-error", "missing realm name in node");
      }
      // check info attribute
      if (tag->isattr (XML_INFO_ATTR) == false) {
	throw Exception ("csm-error", "missing realm info in node");
      }
      // check root attribute
      if (tag->isattr (XML_ROOT_ATTR) == false) {
	throw Exception ("csm-error", "missing realm root in node");
      }
      // collect realm information
      String name = tag->getpval (XML_NAME_ATTR);
      String info = tag->getpval (XML_INFO_ATTR);
      String root = tag->getpval (XML_ROOT_ATTR);
      // create the realm
      return new RealmZone (root, name, info);
    }
    throw Exception ("csm-error", "invalid realm type in node", type);
  }

  // this procedure creates a notary from a csm tag
  static Notary xrd_get_nty (XmlNode* csm) {
    // check for valid tag
    if (csm == nullptr) return Notary();
    // get the notary node
    XmlTag* tag = xrd_get_tag (csm, XML_NTY_NAME);
    // get name and notary
    if (tag->isattr (XML_NAME_ATTR) == false) {
      throw Exception ("csm-error", "missing notary name in node");
    }
    String name = tag->getpval (XML_NAME_ATTR);
    Notary result(name);
    // check for info attribute
    if (tag->isattr (XML_INFO_ATTR) == true) {
      String info = tag->getpval (XML_INFO_ATTR);
      result.setinfo (info);
    }
    // set the notary realm
    Realm* rlm = xrd_get_rlm (tag);
    result.setrelm (rlm);
    return result;
  }

  // this procedure gets the csm node from the xml document
  static XmlTag* xrd_get_csm (const XmlDocument& xdoc) {
    // get the root node
    XmlRoot* root = xdoc.getroot ();
    if (root == nullptr) {
      throw Exception ("csm-error", "nil root node in xml document");
    }
    // get the csm node
    XmlNode* node = xrd_get_node (root, XML_CSM_NAME);
    XmlTag*   tag = dynamic_cast <XmlTag*> (node);
    if (tag == nullptr) {
      throw Exception ("csm-error", "cannot find csm node in document");
    }
    // check the namespace if any
    if (tag->isattr (XML_XXNS_ATTR) == true) {
      String pval = tag->getpval (XML_XXNS_ATTR);
      if (pval != XML_XXNS_XVAL) {
        throw Exception ("csm-error", "invalid xml namespace value", pval);
      }
    }
    // here it is
    return tag;
  }
  
  class NotaryReader : public XmlDocument {
  private:
    /// the csm node
    XmlNode* p_csm;
    
  public:
    // create a reader by uri
    NotaryReader (const Uri& uri) {
      // create a uri stream with a tcp client
      auto iosm = [] (const String& host, const long port) -> Object* {
	return new TcpClient (host, port);
      };
      UriStream uris (iosm);
      // get the input stream
      InputStream* is = uris.istream (uri);
      try {
	Object::iref (is);
	setroot (uri.gethnam(), is);
	p_csm = xrd_get_csm (*this);
	Object::dref (is);
      } catch (...) {
	Object::dref (is);
      }
    };
    
    // create a reader by string uri
    NotaryReader (const String& suri) {
      // create a uri stream with a tcp client
      auto iosm = [] (const String& host, const long port) -> Object* {
	return new TcpClient (host, port);
      };
      UriStream uris (iosm);
      // get the input stream
      InputStream* is = uris.istream (suri);
      try {
	Object::iref (is);
	setroot (suri, is);
	p_csm = xrd_get_csm (*this);
	Object::dref (is);
      } catch (...) {
	Object::dref (is);
      }
    };
    // create a reader by name and input stream
    NotaryReader (const String& name, InputStream* is) {
      setroot (name, is);
      p_csm = xrd_get_csm (*this);
    };
    
    // get the class name
    String repr (void) const {
      return "NotaryReader";
    };
    
    // xml to notary object
    Notary xmlto (void) const {
      rdlock ();
      try {
	// get the notary
	Notary result = xrd_get_nty (p_csm);
	unlock ();
	return result;
      } catch (...) {
	unlock ();
	throw;
      }
    };
    
  private:
    // make the copy constructor private
    NotaryReader (const NotaryReader&) =delete;
    // make the assignment operator private
    NotaryReader& operator = (const NotaryReader&) =delete;
  };

  // -------------------------------------------------------------------------
  // - writer section                                                        -
  // -------------------------------------------------------------------------

  // this procedure create a realm node
  static XmlNode* xwr_get_rlm (Realm* rlm) {
    // check for valid realm
    if (rlm == nullptr) return nullptr;
    // check for a realm zone
    RealmZone* rzn = dynamic_cast<RealmZone*> (rlm);
    if (rzn != nullptr) {
      // collect realm informations
      String name = rzn->getname ();
      String info = rzn->getinfo ();
      String root = rzn->getroot ();
      // create a realm tag
      XmlTag* tag = new XmlTag (XML_RLM_NAME);
      // set the tag attributes
      tag->setattr (XML_TYPE_ATTR, XML_TVAL_ZONE);
      if (name.isnil () == false) tag->setattr (XML_NAME_ATTR, name);
      if (info.isnil () == false) tag->setattr (XML_INFO_ATTR, info);
      if (root.isnil () == false) tag->setattr (XML_ROOT_ATTR, root);
      return tag;
    }
    throw Exception ("csm-error", "invalid realm type", Object::repr (rlm));
  }
    
  // this procedure creates an notary node
  static XmlNode* xwr_get_nty (const Notary& nty) {
    // create an notary tag
    XmlTag* tag = new XmlTag (XML_NTY_NAME);
    // get the name and info
    String name = nty.getname ();
    String info = nty.getinfo ();
    if (name.isnil () == false) tag->setattr (XML_NAME_ATTR, name);
    if (info.isnil () == false) tag->setattr (XML_INFO_ATTR, info);
    // get the realm tag
    tag->addchild (xwr_get_rlm (nty.getrelm ()));
    // here it is
    return tag;
  }

  // this procedure creates an csm node by notary
  static XmlNode* xwr_get_csm (const Notary& act) {
    // create a qualified acn node
    String tnam = XML_XNS_NAME; tnam += ':'; tnam += XML_CSM_NAME;
    // create a tag by qualified name
    XmlTag* tag = new XmlTag (tnam);
    // set the xmlns attribute
    tag->setattr (XML_XXNS_ATTR, XML_XXNS_XVAL);
    // add the notary node
    tag->addchild (new XmlComment (XML_NTY_COMT));
    tag->addchild (xwr_get_nty (act));
    // here it is
    return tag;
  }

  class NotaryWriter : public XmlDocument {
  private:
    OutputStream* p_os;
    
  public:
    // create a writer by uri
    NotaryWriter (const Notary& ntry, const Uri& uri) {
      Object::iref (p_os = UriStream().ostream (uri));
      toxml (ntry);
    };

    // create a writer by string uri
    NotaryWriter (const Notary& ntry, const String& suri) {
      Object::iref (p_os = UriStream().ostream (suri));
      toxml (ntry);
    };

    // create a writer by output stream
    NotaryWriter (const Notary& ntry, OutputStream* os) {
      Object::iref (p_os = os);
      toxml (ntry);
    };

    // destroy this write
    ~NotaryWriter (void) {
      if (p_os != nullptr) p_os->close ();
      Object::dref (p_os);
    }
    
    // get the class name
    String repr (void) const {
      return "NotaryWriter";
    };
  
    // notary to xml
    void toxml (const Notary& ntry) {
      wrlock ();
      try {
	// get the csm tag
	XmlNode* node = xwr_get_csm (ntry);
	// get the xml content root node
	XmlRoot* root = newroot (true);
	// add the notary node
	root->addchild (node);
	// done
	unlock ();
      } catch (...) {
	unlock ();
	throw;
      }
    };

    // write the notary
    void write (void) {
      wrlock ();
      try {
	if (p_os == nullptr) {
	  unlock ();
	  return ;
	}
	XmlRoot* root = newroot (true);
	try {
	  if (root != nullptr) root->write (*p_os);
	  Object::dref (root);
	} catch (...) {
	  Object::dref (root);
	  throw;
	}
      } catch (...) {
	unlock ();
	throw;
      }
    }

  private:
    // make the copy constructor private
    NotaryWriter (const NotaryWriter&) =delete;
    // make the assignment operator private
    NotaryWriter& operator = (const NotaryWriter&) =delete;
  };
}

#endif
