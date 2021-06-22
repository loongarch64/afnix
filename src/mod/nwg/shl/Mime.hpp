// ---------------------------------------------------------------------------
// - Mime.hpp                                                                -
// - afnix:nwg module - mime base class definition                           -
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

#ifndef  AFNIX_MIME_HPP
#define  AFNIX_MIME_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTEAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The Mime class is an abstract  class used for the definition of mime
  /// document. The class defines the getmime method that returns the
  /// document mime type.
  /// @author amaury darsch

  class Mime : public virtual Object {
  public:
    /// check if a mime extension exists by name
    /// @param mext the mime extension to check
    static bool ismext (const String& mext);

    /// check if a mime extension exists by value
    /// @param mval the mime value to check
    static bool ismval (const String& mval);

    /// check for a valid xml mime type
    /// @param mval the mime value to check
    static bool isxmlm (const String& mval);

    /// convert a mime object to an extension
    /// @param mime the mime object to convert
    static String tomext (const String& mime);

    /// convert a mime extension to an object
    /// @param mext the mime extension to convert
    static String tomime (const String& mext);

    /// convert a mime extension to an object
    /// @param mext the mime extension to convert
    /// @param dflg the default mime flag
    static String tomime (const String& mext, const bool dflg);

    /// @return the default mime value
    static String getmdef (void);

  protected:
    /// the mime value
    String d_mime;

  public:
    /// create a default mime object
    Mime (void);

    /// create a mime object by type
    /// @param type the mime type to set
    Mime (const String& mime);

    /// copy construct a mime object
    /// @param that the object to copy
    Mime (const Mime& that);

    /// assign a mime object to this one
    /// @param that the object to assign
    Mime& operator = (const Mime& that);

    /// @return the class name
    String repr (void) const;

    /// @return the document mime type
    virtual String getmime (void) const;

    /// write a document into a buffer
    /// @param buf the buffer to write
    virtual void write (Buffer& buf) const =0;

    /// write a document into an output stream
    /// @param os the output stream to write
    virtual void write (OutputStream& os) const =0;

  public:
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
