// ---------------------------------------------------------------------------
// - Authority.hxx                                                           -
// - afnix:csm module - xml common authority reader/writer definition        -
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

#ifndef  AFNIX_AUTHORITY_HXX
#define  AFNIX_AUTHORITY_HXX

#include "XmlTag.hpp"
#include "UriStream.hpp"
#include "TcpClient.hpp"
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
  // the authority node name
  static const String XML_PTN_NAME  = "authority";
  // the credential node name
  static const String XML_CRD_NAME  = "credential";
  // the description comment
  static const String XML_PTN_COMT  = "authority description";
  // the xmlns attribute
  static const String XML_XXNS_ATTR = "xmlns:afnix";
  // the xmlns attribute value
  static const String XML_XXNS_XVAL = "http://www.afnix.org/csm";
  // the rid attribute
  static const String XML_XRID_ATTR = "rid";
  // the name attribute
  static const String XML_NAME_ATTR = "name";
  // the info attribute
  static const String XML_INFO_ATTR = "info";
  // the type attribute
  static const String XML_TYPE_ATTR = "type";
  // the value attribute
  static const String XML_CVAL_ATTR = "value";

  // the credential type value
  static const String XML_TVAL_NONE = "none";
  static const String XML_TVAL_PASS = "pass";
  
  // -------------------------------------------------------------------------
  // - reader section                                                        -
  // -------------------------------------------------------------------------

  // this procedure converts a string to a credential type
  static Credential::t_cred xrd_tocred (const String& tval) {
    if (tval == XML_TVAL_NONE) return Credential::CRED_NONE;
    if (tval == XML_TVAL_PASS) return Credential::CRED_PASS;
    throw Exception ("csm-error", "invalid credential type attribute", tval);
  }
  
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

  // this procedure creates an authority from a csm tag
  static Authority xrd_get_ptn (XmlNode* csm) {
    // check for valid tag
    if (csm == nullptr) return Authority();
    // get the authority node
    XmlTag* tag = xrd_get_tag (csm, XML_PTN_NAME);
    // get name and authority
    if (tag->isattr (XML_NAME_ATTR) == false) {
      throw Exception ("csm-error", "missing authority name in node");
    }
    String name = tag->getpval (XML_NAME_ATTR);
    Authority result(name);
    // check for info attribute
    if (tag->isattr (XML_INFO_ATTR) == true) {
      String info = tag->getpval (XML_INFO_ATTR);
      result.setinfo (info);
    }
    // check for rid attribute
    if (tag->isattr (XML_XRID_ATTR) == true) {
      String xrid = tag->getpval (XML_XRID_ATTR);
      result.setrid (xrid);
    }
    // get the authority credential
    Credential cred;
    for (long k = 0L; k < tag->lenchild (); k++) {
      XmlTag* crd = dynamic_cast <XmlTag*> (tag->getchild (k));
      if (crd == nullptr) {
	throw Exception ("csm-error", "invalid authority child xml node");
      }
      if (crd->isname (XML_CRD_NAME) == false) continue;
      if (crd->isattr(XML_TYPE_ATTR) == false) {
	throw Exception ("csm-error", "missing credential type attribute");
      }
      String tval =  crd->getpval (XML_TYPE_ATTR);
      Credential::t_cred type = xrd_tocred (tval);
      if (crd->isattr(XML_CVAL_ATTR) == false) {
	throw Exception ("csm-error", "missing credential value attribute");
      }
      String cval = crd->getpval (XML_CVAL_ATTR);
      cred.add (type, cval, true);
    }
    result.setcred (cred);
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
  
  class AuthorityReader : public XmlDocument {
  private:
    /// the csm node
    XmlNode* p_csm;
    
  public:
    // create a reader by uri
    AuthorityReader (const Uri& uri) {
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
    AuthorityReader (const String& suri) {
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
    AuthorityReader (const String& name, InputStream* is) {
      setroot (name, is);
      p_csm = xrd_get_csm (*this);
    };
    
    // get the class name
    String repr (void) const {
      return "AuthorityReader";
    };
    
    /// get the authority object
    Authority xmlto (void) const {
      rdlock ();
      try {
	// get the authority
	Authority result = xrd_get_ptn (p_csm);
	unlock ();
	return result;
      } catch (...) {
	unlock ();
	throw;
      }
    };
    
  private:
    // make the copy constructor private
    AuthorityReader (const AuthorityReader&) =delete;
    // make the assignment operator private
    AuthorityReader& operator = (const AuthorityReader&) =delete;
  };

  // -------------------------------------------------------------------------
  // - writer section                                                        -
  // -------------------------------------------------------------------------

  // this procedure converts a credential type to a string
  static String xwr_tostring (const Credential::t_cred cred) {
    String result;
    switch (cred) {
    case Credential::CRED_NONE:
      result = XML_TVAL_NONE;
      break;
    case Credential::CRED_PASS:
      result = XML_TVAL_PASS;
      break;
    }
    return result;
  }
  
  // this procedure creates an authority node
  static XmlNode* xwr_get_ptn (const Authority& auth) {
    // create an authority tag
    XmlTag* tag = new XmlTag (XML_PTN_NAME);
    // get the name and info
    String name = auth.getname ();
    String info = auth.getinfo ();
    String xrid = auth.getrid  ();
    if (name.isnil () == false) tag->setattr (XML_NAME_ATTR, name);
    if (info.isnil () == false) tag->setattr (XML_INFO_ATTR, info);
    if (xrid.isnil () == false) tag->setattr (XML_XRID_ATTR, xrid);
    // add the credential tags
    Credential* cred = auth.getcred ();
    if (cred != nullptr) {
      for (long k = 0L; k < cred->length (); k++) {
	XmlTag* crd = new XmlTag (XML_CRD_NAME);
	crd->setattr (XML_TYPE_ATTR, xwr_tostring (cred->gettype (k)));
	crd->setattr (XML_CVAL_ATTR, cred->getcval (k));
	tag->addchild (crd);
      }
    }
    // here it is
    return tag;
  }

  // this procedure creates an csm node by authority
  static XmlNode* xwr_get_csm (const Authority& auth) {
    // create a qualified acn node
    String tnam = XML_XNS_NAME; tnam += ':'; tnam += XML_CSM_NAME;
    // create a tag by qualified name
    XmlTag* tag = new XmlTag (tnam);
    // set the xmlns attribute
    tag->setattr (XML_XXNS_ATTR, XML_XXNS_XVAL);
    // add the authority node
    tag->addchild (new XmlComment (XML_PTN_COMT));
    tag->addchild (xwr_get_ptn (auth));
    // here it is
    return tag;
  }

  class AuthorityWriter : public XmlDocument {
  private:
    OutputStream* p_os;
    
  public:
    // create a writer by uri
    AuthorityWriter (const Authority& auth, const Uri& uri) {
      Object::iref (p_os = UriStream().ostream (uri));
      toxml (auth);
    };

    // create a writer by string uri
    AuthorityWriter (const Authority& auth, const String& suri) {
      Object::iref (p_os = UriStream().ostream (suri));
      toxml (auth);
    };

    // create a writer by output stream
    AuthorityWriter (const Authority& auth, OutputStream* os) {
      Object::iref (p_os = os);
      toxml (auth);
    };

    // destroy this write
    ~AuthorityWriter (void) {
      if (p_os != nullptr) p_os->close ();
      Object::dref (p_os);
    }
    
    // get the class name
    String repr (void) const {
      return "AuthorityWriter";
    };
  
    // authority to xml
    void toxml (const Authority& auth) {
      wrlock ();
      try {
	// get the csm tag
	XmlNode* node = xwr_get_csm (auth);
	// get the xml content root node
	XmlRoot* root = newroot (true);
	// add the acn node
	root->addchild (node);
	// done
	unlock ();
      } catch (...) {
	unlock ();
	throw;
      }
    };

    // write the authority
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
    AuthorityWriter (const AuthorityWriter&) =delete;
    // make the assignment operator private
    AuthorityWriter& operator = (const AuthorityWriter&) =delete;
  };
}

#endif
