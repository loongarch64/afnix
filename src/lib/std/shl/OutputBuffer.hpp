// ---------------------------------------------------------------------------
// - OutputBuffer.hpp                                                        -
// - standard object library - output buffer stream class definition         -
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

#ifndef  AFNIX_OUTPUTBUFFER_HPP
#define  AFNIX_OUTPUTBUFFER_HPP

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The OutputBuffer class offers a facility to treat a buffer like a stream.
  /// Each call to the write method accumulates characters in the buffer. When
  /// the tostring method is called, the accumulated string is returned. The
  /// format method can also be called as a mean to get an octet string
  /// representation of the buffer. The OutputBuffer class is very usefull
  /// to perform stream based testing with non printable bytes.
  /// @author amaury darsch

  class OutputBuffer : public OutputStream {
  private:
    /// the stream buffer
    Buffer d_sbuf;

  public:
    /// create a new output buffer
    OutputBuffer (void);

    /// create a new output buffer by value
    /// @param value the initial string value
    OutputBuffer (const String& value);

    /// @return the class name
    String repr (void) const;

    /// set the stream encoding mode
    /// @param emod the stream encoding mode to set
    void setemod (const Encoding::t_emod emod);

    /// flush this output buffer
    void flush (void);

    /// @return the length of this output buffer
    long length (void) const;

    /// write one character on the output buffer.
    /// @param value the character to write
    long write (const char value);

    /// write a character string to the output buffer
    /// @param data the data to write  
    long write (const char* data);

    /// @return a copy of the stream buffer
    Buffer tobuffer (void) const;
    
    /// @return a string representation of this output buffer
    String tostring (void) const;

    /// @return an octet string representation of this output buffer
    String format (void) const;

    /// send the output buffer content to an output stream
    /// @param os the output stream to write
    void send (OutputStream* os);

  private:
    // make the copy constructor private
    OutputBuffer (const OutputBuffer&);
    // make the assignment operator private
    OutputBuffer& operator = (const OutputBuffer&);
    
  public:
    /// create a new object in a generic way
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
