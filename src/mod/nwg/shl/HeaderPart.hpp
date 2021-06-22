// ---------------------------------------------------------------------------
// - HeaderPart.hpp                                                          -
// - afnix:nwg module - header part class definition                         -
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

#ifndef  AFNIX_HEADERPART_HPP
#define  AFNIX_HEADERPART_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The HeaderPart class is a base class for most of the http related
  /// transactions. It holds a paarmeter list which is fille either by
  /// parsing a stream, a buffer or setting it manually. The header part
  /// serves the single part, multi part and http protocolo objects.
  /// @author amaury darsch

  class HeaderPart : public virtual Object {
  protected:
    /// the header list
    Plist d_head;

  public:
    /// create a default header
    HeaderPart (void);

    /// copy construct this header part
    /// @param that the object to copy
    HeaderPart (const HeaderPart& that);

    /// assign a header part to this one
    /// @param that the object to assign
    HeaderPart& operator = (const HeaderPart& that);

    /// reset this protocol object
    virtual void reset (void);

    /// parse an input stream
    /// @param is the input stream to parse
    virtual void hparse (InputStream& is);

    /// parse a buffer
    /// @param buf the buffer to parse
    virtual void hparse (Buffer& buf);

    /// @return the header property list
    virtual Plist hplist (void) const;

    /// @return the header length
    virtual long hlength (void) const;
    
    /// @return true if a header property exists
    virtual bool hexists (const String& name) const;

    /// set a header property by name and litteral
    /// @param name the property name
    /// @param lval the property value
    virtual void hset (const String& name, const Literal& pval);

    /// @return a header property by index
    virtual Property* hget (const long index) const;

    /// @return a header property by name
    virtual Property* hfind (const String& name) const;

    /// @return a header property by name or throw an exception
    virtual Property* hlookup (const String& name) const;
    
    /// @return a header property value by name
    virtual String hmap (const String& name) const;

    /// @return a header property plist by name
    virtual Plist hmapl (const String& name) const;

    /// write the header to an output stream
    /// @param os the output stream
    virtual void hwrite (OutputStream& os) const;

    /// write the header to a buffer
    /// @param buf the target buffer
    virtual void hwrite (Buffer& buf) const;

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
