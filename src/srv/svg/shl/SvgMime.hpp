// ---------------------------------------------------------------------------
// - SvgMime.hpp                                                             -
// - afnix:svg service - svg mime class definition                           -
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

#ifndef  AFNIX_SVGMIME_HPP
#define  AFNIX_SVGMIME_HPP

#ifndef  AFNIX_MIME_HPP
#include "Mime.hpp"
#endif

#ifndef  AFNIX_XMLDOCUMENT_HPP
#include "XmlDocument.hpp"
#endif

namespace afnix {

  /// The SvgMime class is generic svg mime document class. The class is 
  /// used to construct a mime version of a xml document which can be
  /// obtained from a file name, or an input stream. By default, the mime type
  /// 'image/svg+xml'.
  /// @author amaury darsch

  class SvgMime : public XmlDocument, public Mime {
  public:
    /// create a default svg mime
    SvgMime (void);

    /// create a svg mime document by name
    /// @param name the file name to use
    SvgMime (const String& name);

    /// create a svg mime by name and stream
    /// @param name the document name
    /// @param is   the input stream to parse
    SvgMime (const String& name, InputStream* is);

    /// @return the class name
    String repr (void) const;

    /// write a node into a buffer
    /// @param buf the buffer to write
    void write (Buffer& buf) const;
    
    /// write a node into an output stream
    /// @param os the output stream to write
    void write (OutputStream& os) const;

  private:
    // make the copy constructor private
    SvgMime (const SvgMime&);
    // make the assignment operator private
    SvgMime& operator = (const SvgMime&);

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
