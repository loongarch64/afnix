// ---------------------------------------------------------------------------
// - XmlDocument.hpp                                                         -
// - afnix:xml module - xml document class definition                        -
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

#ifndef  AFNIX_XMLDOCUMENT_HPP
#define  AFNIX_XMLDOCUMENT_HPP

#ifndef  AFNIX_XMLROOT_HPP
#include "XmlRoot.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The XmlDocument class is the root document class that maintains a
  /// xml document along with its associated tree and other useful
  /// information. Generally the class is constructed with a file name or
  /// a name and a buffer or an input stream that is used for parsing the
  /// input data. The document can also be designed by constructing manually
  /// the document tree. In that case, the document name must be set.
  /// @author amaury darsch

  class XmlDocument : public Nameable {
  protected:
    /// the document name
    String   d_name;
    /// the root node
    XmlRoot* p_root;

  public:
    /// create a default document
    XmlDocument (void);

    /// create an document by name
    /// @param name the document file name
    XmlDocument (const String& name);

    /// create an document by name and buffer
    /// @param name the document name
    /// @param sbuf the buffer to parse
    XmlDocument (const String& name, const Buffer& sbuf);

    /// create an document by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    XmlDocument (const String& name, InputStream* is);

    /// create an document by name and root node
    /// @param name the document name
    /// @param root the document root node
    XmlDocument (const String& name, XmlRoot* root);

    /// copy construct this document
    /// @param that the document to copy
    XmlDocument (const XmlDocument& that);

    /// destroy this document
    ~XmlDocument (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a xml document to this one
    /// @param that the document to assign
    XmlDocument& operator = (const XmlDocument&);

    /// @return the document name
    String getname (void) const;

    /// set the document name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// set the document root by name
    /// @param name the document name
    virtual void setroot (const String& name);

    /// Set the document root by name and buffer
    /// @param name the document name
    /// @param sbuf the buffer to parse
    virtual void setroot (const String& name, const Buffer& sbuf);

    /// Set the document root by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    virtual void setroot (const String& name, InputStream* is);

    /// set the document root by name and xml root node
    /// @param name the document name
    /// @param root the xml root node
    virtual void setroot (const String& name, XmlRoot* root);

    /// set the document root text by name
    /// @param name the document name
    virtual void setrtxt (const String& name);

    /// set the document root text by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    virtual void setrtxt (const String& name, InputStream* is);

    /// get the root node or create a new one
    /// @param dflg the declaration flag
    virtual XmlRoot* newroot (const bool dflg);

    /// @return the document root node
    virtual XmlRoot* getroot (void) const;

    /// @return the document root node without the declaration
    virtual XmlRoot* getbody (void) const;

  public:
    /// create an object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
