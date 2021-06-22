// ---------------------------------------------------------------------------
// - OutputString.hpp                                                        -
// - standard object library - output string stream class definition         -
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

#ifndef  AFNIX_OUTPUTSTRING_HPP
#define  AFNIX_OUTPUTSTRING_HPP

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

#ifndef  AFNIX_STRBUF_HPP
#include "Strbuf.hpp"
#endif

namespace afnix {

  /// The OutputString class offers a facility to treat a string buffer like
  /// a string. Each call to the write method accumulates characters in the 
  /// string buffer. When the tostring method is called, the accumulated string
  /// is returned. The OutputString class is very usefull to perform stream
  /// based testing.
  /// @author amaury darsch

  class OutputString: public OutputStream {
  private:
    /// the string buffer
    Strbuf d_sbuf;

  public:
    /// create a new string stream. 
    OutputString (void);

    /// create a new string stream with a value
    /// @param value the string initial value
    OutputString (const String& value);

    /// @return the class name
    String repr (void) const;

    /// flush this string buffer
    void flush (void);

    /// @return the length of this output string buffer
    long length (void) const;

    /// write one character on the string stream.
    /// @param value the character to write
    long write (const char value);

    /// write a character string to the output stream
    /// @param data the data to write  
    long write (const char* data);

    /// @return a string representation of this buffer
    String tostring (void) const;

  private:
    // make the copy constructor private
    OutputString (const OutputString&);
    // make the assignment operator private
    OutputString& operator = (const OutputString&);
    
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
