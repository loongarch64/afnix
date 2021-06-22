// ---------------------------------------------------------------------------
// - InputStream.hpp                                                         -
// - standard object library - input stream base class definition            -
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

#ifndef  AFNIX_INPUTSTREAM_HPP
#define  AFNIX_INPUTSTREAM_HPP

#ifndef  AFNIX_STREAM_HPP
#include "Stream.hpp"
#endif

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

namespace afnix {

  /// The InputStream class is an abstract stream class that provides the
  /// interface of an input stream. The primary method associated with such
  /// class is the character read method as well as the valid predicate. The
  /// class is designed to provide a unified mechanism that is further
  /// extended by the various implementation such like the input file
  /// stream or the input string stream.
  /// @author amaury darsch

  class InputStream : public Stream {
  public:
    /// @return the stream timeout
    virtual long gettout (void) const =0;

    /// set the stream timeout
    /// @param tout the stream time to set
    virtual void settout (const long tout) =0;

    /// flush the input stream buffer
    virtual void flush (void) =0;

    /// @return true if we are at the eos
    virtual bool iseos (void) const =0;

    /// @return true if we can read a character
    virtual bool valid (void) const =0;

    /// @return the next available character
    virtual char read (void) =0;

    /// @return the next available byte
    virtual t_byte readb (void);

    /// @return the next available word
    virtual t_word readw (const bool hflg);

    /// @return the next available quad
    virtual t_quad readq (const bool hflg);

    /// @return the next available octa
    virtual t_octa reado (const bool hflg);

    /// copy an input stream into a buffer
    /// @param rbuf the reference buffer
    /// @param size the buffer size
    virtual long copy (char* rbuf, const long size);

    /// pushback a character in the input buffer.
    /// @param value the character to push back
    virtual long pushback (const char value) =0;

    /// pushback a unicode character in the input buffer.
    /// @param value the character to push back
    virtual long pushback (const t_quad value) =0;

    /// pushback a c-string in the input buffer
    /// @param s the string to pushback
    virtual long pushback (const char* s) =0;

    /// pushback a c-string by size in the input buffer
    /// @param s    the string to pushback
    /// @param size the string size
    virtual long pushback (const char* s, const long size) =0;

    /// consume the input stream into the input buffer
    virtual long consume (void) =0;

    /// @return the size of the input buffer
    virtual long buflen (void) const =0;

    /// @return a copy if the input buffer
    virtual Buffer toibuf (void) const =0;
    
    /// @return a string representation of the input buffer
    virtual String tostring (void) const =0;

    /// @return the content as an octet string
    virtual String format (void) const;
    
    /// flush the input stream until a character is found
    /// @param value the blocking character
    /// @return true if the character was found
    virtual bool flush (const char value);

    /// flush the input stream until a character is found
    /// @param value the blocking character
    /// @return true if the character was found
    virtual bool flush (const t_quad value);

    /// @return a buffer of character
    /// @param size the number of character to read
    virtual Buffer* read (const long size);

    /// @return the next available line
    virtual String readln (void);

    /// @return the next quad byte
    virtual t_quad getb (void);

    /// @return the next quad unicode character
    virtual t_quad getc (void);

    /// @return the next encoded unicode character
    virtual t_quad getu (void);

    /// pushback a string on this input stream
    virtual long pushback (const String& s);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
