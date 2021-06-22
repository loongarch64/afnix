// ---------------------------------------------------------------------------
// - Strbuf.hpp                                                              -
// - standard object library - string buffer class definition                -
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

#ifndef  AFNIX_STRBUF_HPP
#define  AFNIX_STRBUF_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {
  /// The Strbuf class is a string buffer which can be used to accumulate
  /// unicode characters. The class is similar to the buffer class except
  /// that it operates with quad representation. The internal representation
  /// is also different from the string class as the buffer class stores
  /// the characters in a 2 dimensional array. The horizontal dimension is
  /// used by the characters without combining class, while the vertical
  /// dimension is used for diacritics. With this approach, the horizontal
  /// dimension is the string representation length. Nevertheless, the length
  /// method returns as usual the total number of chararacters while the 
  /// ncclen returns the non combining length, which is the horizontal length
  /// which is also the representation length.
  /// @author amaury darsch

  class Strbuf : public virtual Object {
  private:
    /// the buffer allocated size
    long d_size;
    /// the buffer length
    long d_length;
    /// the character buffer
    t_quad** p_buffer;

  public:
    /// create a new buffer class.
    Strbuf (void);

    /// create a new buffer class with a default size.
    /// @param size the buffer default size
    Strbuf (const long size);
    
    /// create a new buffer and initialize it with a c string
    /// @param value the c string to initialize
    Strbuf (const char* value);

    /// create a new buffer and initialize it with a string
    /// @param value the string to initialize
    Strbuf (const String& value);

    /// destroy this buffer
    ~Strbuf (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this buffer
    virtual void reset (void);

    /// @return the length of this buffer
    virtual long length (void) const;

    /// @return the non combining length of this buffer
    virtual long ncclen (void) const;

    /// resize this buffer
    /// @param size the new buffer size
    virtual void resize (const long size);

    /// add a character in this buffer
    /// @param value the character to add in this buffer
    virtual void add (const char value);

    /// add a unicode character in this buffer
    /// @param value the character to add in this buffer
    virtual void add (const t_quad value);

    /// add a unicode character in this buffer at a position
    /// @param value the character to add in this buffer
    /// @param pos   the buffer position
    virtual void add (const t_quad value, const long pos);

    /// add a character buffer in this buffer
    /// @param s the buffer to add
    virtual void add (const char* s);

    /// add a character buffer in this buffer by size
    /// @param s    the buffer to add
    /// @param size the buffer size
    virtual void add (const char* s, const long size);

    /// add a unicode buffer in this buffer
    /// @param s the buffer to add
    virtual void add (const t_quad* s);

    /// add a unicode buffer in this buffer by size
    /// @param s    the buffer to add
    /// @param size the buffer size
    virtual void add (const t_quad* s, const long size);

    /// add a string to this buffer
    /// @param s the string to add to this buffer
    virtual void add (const String& s);

    /// put a unicode character in this buffer at a position
    /// @param value the character to put in this buffer
    /// @param pos   the buffer position
    virtual void put (const t_quad value, const long pos);

    /// delete a character at a certain position
    /// @param pos the character position
    virtual void chdel (const long pos);

    /// @return the buffer contents as a string
    virtual String tostring (void) const;

    /// @return the buffer contents as a string from position to end
    virtual String substr (const long pos) const;

  private:
    // make the copy constructor private
    Strbuf (const Strbuf&) =delete;
    // make the assignment operator private
    Strbuf& operator =  (const Strbuf&) =delete;
  };
}

#endif
