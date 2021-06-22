// ---------------------------------------------------------------------------
// - XhtmlMime.hpp                                                           -
// - afnix:wax service - xhtml mime class definition                         -
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

#ifndef  AFNIX_XHTMLMIME_HPP
#define  AFNIX_XHTMLMIME_HPP

#ifndef  AFNIX_XMLMIME_HPP
#include "XmlMime.hpp"
#endif

namespace afnix {

  /// The XhtmlMime class is generic xhtml mime document class. The class is 
  /// used to construct a mime version of a xml document which can be
  /// obtained from a file name, or an input stream. By default, the mime type
  /// 'application/xhtml+xml' which can be changed at construction. This occurs
  /// most likely when processing xhtml text from a stream.
  /// @author amaury darsch

  class XhtmlMime : public XmlMime {
  public:
    /// create a default xml mime
    XhtmlMime (void);

    /// create a xhtml mime document by name
    /// @param name the file name to use
    XhtmlMime (const String& name);
   
    /// create a xhtml mime by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    XhtmlMime (const String& name, InputStream* is);

    /// @return the class name
    String repr (void) const;

  private:
    // make the copy constructor private
    XhtmlMime (const XhtmlMime&);
    // make the assignment operator private
    XhtmlMime& operator = (const XhtmlMime&);

  public:
    /// create a new object in a generic object
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
