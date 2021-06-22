// ---------------------------------------------------------------------------
// - InputOutput.hpp                                                         -
// - standard object library - input-output stream class definition          -
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


#ifndef  AFNIX_INPUTOUTPUT_HPP
#define  AFNIX_INPUTOUTPUT_HPP

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The InputOutput class implements an input-output stream with a buffer
  /// which holds character during the processing of transit between the 
  /// stream to the input stream. The theory of operation goes as follow.
  /// The internal buffer is filled with characters with the help of the
  /// output stream. The characters are consumed from the buffer with the 
  /// help of the input stream (read method). If the buffer becomes empty
  /// the iseos returns true and the valid method false. The read method 
  /// will return the eos character. The InputOutput buffer can also be 
  /// initialized with a buffer which provides a nice mechanism to use a 
  /// buffer like an input stream. The i/o operations implemented by this
  /// class are non-blocking. As a consequence, it is not possible to suspend
  /// a thread with this class and have it awaken when some characters are
  /// available in the input stream.
  /// @author amaury darsch

  class InputOutput : public InputBuffer, public OutputStream {
  public:
    /// create a new input-output stream without data
    InputOutput (void);

    /// create a new input-output stream by resize flag
    /// @param rflg the buffer resize flag
    InputOutput (const bool rflg);

    /// create a new input-output stream with a string
    /// @param data the data for the buffer
    InputOutput (const String& data);

    /// create a new input-output stream with a buffer
    /// @param sbuf the initial input buffer
    InputOutput (const Buffer& sbuf);

    /// create a new input-output stream with a buffer and a size
    /// @param data the data for the buffer
    /// @param size the size of the data
    InputOutput (const char* data, const long size);

    /// @return the class name
    String repr (void) const override;

    /// @return the stream timeout
    long gettout (void) const override;

    /// set the stream timeout
    /// @param tout the stream time to set
    void settout (const long tout) override;

    /// @return true if we are at the eos
    bool iseos (void) const override;

    /// check if we can read a character
    bool valid (void) const override;

    /// @return a new character from the input stream
    char read (void) override;

    /// write one character on the output stream.
    /// @param value the character to write
    long write (const char value) override;

    /// write a character string to the output stream
    /// @param value the character string to write
    long write (const char* value) override;

    /// flush the buffer and set the stream with a new string
    /// @param data the string to set to this stream
    virtual void set (const String& data);

    /// @return the next character but do not remove it
    virtual char get (void) const;

  private:
    // make the copy constructor private
    InputOutput (const InputOutput&);
    // make the assignment operator private
    InputOutput& operator = (const InputOutput&);

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
