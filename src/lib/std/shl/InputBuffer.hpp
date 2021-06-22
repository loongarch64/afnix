// ---------------------------------------------------------------------------
// - InputBuffer.hpp                                                         -
// - standard object library - input stream buffer class definition          -
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

#ifndef  AFNIX_INPUTBUFFER_HPP
#define  AFNIX_INPUTBUFFER_HPP

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The InputBuffer class is an abstract class which implements the buffer
  /// portion of the input stream class. The read and valid method are not yet
  /// implemented, as well as the timeout managenemt methods, thus leaving 
  /// room for specific implementations.
  /// @author amaury darsch

  class InputBuffer : public virtual InputStream {
  protected:
    /// the stream buffer
    mutable Buffer d_sbuf;

  public:
    /// create a default input stream
    InputBuffer (void);

    /// copy an input stream into a buffer
    /// @param rbuf the reference buffer
    /// @param size the buffer size
    long copy (char* rbuf, const long size) override;

    /// set the stream encoding mode
    /// @param emod the stream encoding mode to set
    void setemod (const Encoding::t_emod emod) override;

    /// flush the input stream buffer
    void flush (void) override;

    /// pushback a character in the input buffer
    /// @param value the character to push back
    long pushback (const char value) override;

    /// pushback a unicode character in the input buffer
    /// @param value the character to push back
    long pushback (const t_quad value) override;

    /// pushback a character string in the input buffer
    /// @param s the string to pushback
    long pushback (const char* s) override;
    
    /// pushback a buffer by size in the input buffer
    /// @param s    the string to pushback
    /// @param size the string size
    long pushback (const char* s, const long size) override;

    /// consume a stream by reading and pushing back
    long consume (void) override;

    /// @return the size of the input buffer
    long buflen (void) const override;

    /// @return a copy if the input buffer
    Buffer toibuf (void) const override;
    
    /// @return a string representation of the input buffer
    String tostring (void) const override;

    /// @return the buffer content as an octet string
    String format (void) const override;
  };
}

#endif
