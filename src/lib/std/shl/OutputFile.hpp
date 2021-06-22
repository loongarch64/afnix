// ---------------------------------------------------------------------------
// - OutputFile.hpp                                                          -
// - standard object library - output file class definition                  -
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

#ifndef  AFNIX_OUTPUTFILE_HPP
#define  AFNIX_OUTPUTFILE_HPP

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The OutputFile class is a class to access file for writing. The
  /// class is constructed from the file name. If the file does not exist, it
  /// created. If the file exist, it is overwritten. All write method are
  /// available with this class, including the one defined in the base class.
  /// @author amaury darsch

  class OutputFile : public OutputStream, public Nameable {
  private:
    /// the file name
    String d_name;
    /// the stream id
    int d_sid;

  public:
    /// create a new output stream by name
    /// @param name the file name
    OutputFile (const String& name);

    /// create a new output stream by name and encoding mode
    /// @param name the file name
    /// @param emod the encoding mode
    OutputFile (const String& name, const String& emod);

    /// create a new output stream by name and flags.
    /// @param name the file name
    /// @param tflg the truncate flag
    /// @param aflg the append flag
    OutputFile (const String& name, const bool tflg, const bool aflg);
    
    /// close and destroy this file output stream
    ~OutputFile (void);

    /// @return the class name
    String repr (void) const;

    /// @return the stream descriptor
    int getsid (void) const;

    /// close this output file
    bool close (void);

    /// write one character on the output stream.
    /// @param value the character to write  
    long write (const char value);

    /// write a character string to the output stream
    /// @param data the data to write
    long write (const char* data);

    /// write a character array to the output stream
    /// @param rbuf the reference buffer to write
    /// @param size the number of character to write
    long write (const char* rbuf, const long size);

    /// @return the file name associated with this stream
    String getname (void) const;

  private:
    // make the copy constructor private
    OutputFile (const OutputFile&);
    // make the assignment operator private
    OutputFile& operator = (const OutputFile&);

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
