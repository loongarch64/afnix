// ---------------------------------------------------------------------------
// - XmlContent.hpp                                                          -
// - afnix:xpe module - xml content class definition                         -
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

#ifndef  AFNIX_XMLCONTENT_HPP
#define  AFNIX_XMLCONTENT_HPP

#ifndef  AFNIX_URIPATH_HPP
#include "UriPath.hpp"
#endif

#ifndef  AFNIX_XMLDOCUMENT_HPP
#include "XmlDocument.hpp"
#endif

namespace afnix {

  /// The XmlContent class is an extension of the xml document class that 
  /// operates at the uri level. If the uri is a local file the xml document
  /// is created from an input file stream. If the uri is an url, the 
  /// content is fetched automatically. The class constructors permit to
  /// separate the content name from the document name and also to specify 
  /// the content encoding.
  /// @author amaury darsch

  class XmlContent : public XmlDocument {
  private:
    /// the normalized uri
    String d_nuri;
    /// the optional uri path
    UriPath* p_puri;

  public:
    /// create a xml content for writing
    XmlContent (void);

    /// create a xml content by uri name
    /// @param name the uri name to fetch
    XmlContent (const String& name);

    /// create a xml content by name and input stream
    /// @param name the content name 
    /// @param is   the document input stream 
    XmlContent (const String& name, InputStream* is);

    /// create a xml content by uri name and path
    /// @param name the uri name 
    /// @param puri the uri path
    XmlContent (const String& name, const UriPath& puri);

    /// create a xml content by name and document name
    /// @param name the content name 
    /// @param docn the document name to fetch
    XmlContent (const String& name, const String& docn);

    /// create a xml content by name, document name and encoding mode
    /// @param name the content name 
    /// @param docn the document name to fetch
    /// @param emod the document encoding mode
    XmlContent (const String& name, const String& docn, const String& emod);

    /// copy construct this content
    /// @param that the content to copy
    XmlContent (const XmlContent& that);

    /// destroy this object
    ~XmlContent (void);

    /// @return the normalized uri
    String getnuri (void) const;

    /// @return the document name
    String getdocn (void) const;

    /// write a xml content to a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf);

    /// write a xml content by uri name
    /// @param name the uri name to write
    void write (const String& name);

    /// write a xml content by name and ouput stream
    /// @param name the content name 
    /// @param os   the document output stream 
    void write (const String& name, OutputStream* os);

    /// write a xml content by uri name and path
    /// @param name the uri name 
    /// @param puri the uri path
    void write (const String& name, const UriPath& puri);

  private:
    // make the assignment operator private
    XmlContent& operator = (const XmlContent&);

  public:
    /// create a new object in a generic object
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
