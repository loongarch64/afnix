// ---------------------------------------------------------------------------
// - InputFile.hpp                                                           -
// - standard object library - input file class definition                   -
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

#ifndef  AFNIX_INPUTFILE_HPP
#define  AFNIX_INPUTFILE_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

#ifndef  AFNIX_INPUTTIMEOUT_HPP
#include "InputTimeout.hpp"
#endif


namespace afnix {

  /// The InputFile class is derived from the Input base base class and 
  /// provide a facility for reading file. The file is open at construction
  /// and closed at destruction or after a specific call to the close method.
  /// Sequential access is provided with the lseek method.
  /// @author amaury darsch

  class InputFile :public InputBuffer, public InputTimeout, public Nameable {
  private:
    /// the file name
    String d_name;
    /// the stream id
    int    d_sid;

  public:
    /// create a new input file by name
    /// @param name the file name
    InputFile (const String& name);

    /// create a new input file by name and encoding mode
    /// @param name the file name
    /// @param emod the encodig mode
    InputFile (const String& name, const String& emod);

    /// close and destroy this input stream
    ~InputFile (void);

    /// @return the class name
    String repr (void) const override;

    /// @return the stream descriptor
    int getsid (void) const override;

    /// @return true if we are at the eos
    bool iseos (void) const override;

    /// @return true if we can read a character
    bool valid (void) const override;

    /// @return the next available character
    char read (void) override;

    /// copy an input stream into a buffer
    /// @param rbuf the reference buffer
    /// @param size the buffer size
    long copy (char* rbuf, const long size) override;

    /// @return the file name for this file stream
    String getname (void) const override;

    /// close this input file and mark the eos marker
    bool close (void) override;

    /// place the file marker at a certain position
    /// @param pos the position to go
    virtual void lseek (const t_long pos);

    /// @return the input file size
    virtual t_long length (void) const;

    /// @return the file modification time
    virtual t_long mtime (void) const;

  private:
    // make the copy constructor private
    InputFile (const InputFile&);
    // make the assignment operator private
    InputFile& operator = (const InputFile&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
