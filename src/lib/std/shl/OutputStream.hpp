// ---------------------------------------------------------------------------
// - OutputStream.hpp                                                        -
// - standard object library - output stream base class definition           -
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

#ifndef AFNIX_OUTPUTSTREAM_HPP
#define AFNIX_OUTPUTSTREAM_HPP

#ifndef AFNIX_STREAM_HPP
#include "Stream.hpp"
#endif

#ifndef AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef AFNIX_EXCEPTION_HPP
#include "Exception.hpp"
#endif

namespace afnix {

  /// The OutputStream class is an abstract class which provides a write 
  /// method for character in a generic way. There are several ways to create
  /// a stream, including standard output, file based or socket stream.
  /// Various derived classes are provided to create an output stream.
  /// @author amaury darsch

  class OutputStream : public Stream {
  public:
    /// @return true if the output stream is a tty
    virtual bool istty (void) const;

    /// write a quad byte
    /// @param value the unicode value to write
    virtual long putb (const t_quad value);

    /// write a quad unicode character
    /// @param value the unicode value to write
    virtual long putc (const t_quad value);

    /// write one character on the output stream.
    /// @param value the character to write
    virtual long write (const char value) =0;

    /// write a character string to the output stream
    /// @param value the character string to write
    virtual long write (const char* value) =0;

    /// write a unicode character according to the stream mode
    /// @param value the unicode value to write
    virtual long write (const t_quad value);

    /// write a unicode string according to the stream mode
    /// @param value the unicode value to write
    virtual long write (const t_quad* value);

    /// write a character array to the output stream
    /// @param rbuf the buffer to write
    /// @param size the number of characters
    virtual long write (const char* rbuf, const long size);

    /// write an unicode character array to the output stream
    /// @param value the character string to write
    /// @param size  the number of character to write
    virtual long write (const t_quad* value, const long size);

    /// write a string to the output stream
    /// @param value the character string to write
    virtual long write (const String& value);

    /// write a buffer to the output stream
    /// @param buf the buffer to write
    virtual long write (const Buffer& buf);

    /// write a string as an error string
    /// @param value the character string to write
    virtual void error (const String& value);

    /// write a panic message as an error string
    /// @param value the string to write
    virtual void panic (const String& value);

    /// write a newline to this output stream
    virtual void newline (void);

    /// write a newline to this output stream with a carriage return
    /// @param crlf the carriage return flag
    virtual void newline (const bool crlf);

    /// write a character string followed by a new line
    /// @param line the line to write
    virtual void writeln (const String& line);

    /// write a character string followed by a newline and a crriage return
    /// @param line the line to write
    /// @param crlf the carriage return flag
    virtual void writeln (const String& line, const bool crlf);

    /// write a character string followed by a new line in error mode.
    /// @param line the line to write
    virtual void errorln (const String& line);

    /// write a character string followed by a new line in error mode.
    /// @param line the line to write
    /// @param crlf the carriage return flag
    virtual void errorln (const String& line, const bool crlf);

    /// write an exception string on this output stream.
    /// @param e the exception to write
    virtual void errorln (const Exception& e);

    /// write a boolean on the output stream.
    /// @param value the integer value to write
    OutputStream& operator << (const bool value);

    /// write a character on the output stream.
    /// @param c the character to write
    OutputStream& operator << (const char c);

    /// write an integer on the output stream.
    /// @param value the integer value to write
    OutputStream& operator << (const long value);

    /// write an integer on the output stream.
    /// @param value the integer value to write
    OutputStream& operator << (const t_long value);

    /// write a real on the output stream.
    /// @param value the real value to write
    OutputStream& operator << (const t_real value);

    /// write a string on the output stream.
    /// @param s the string to write
    /// @return the stream
    OutputStream& operator << (const char* s);

    /// write a string on the output stream.
    /// @param s the string to write
    /// @return the stream
    OutputStream& operator << (const String& s);

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;
    
    /// apply this output with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the argument vector to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
