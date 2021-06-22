// ---------------------------------------------------------------------------
// - InputString.hpp                                                         -
// - standard object library - string stream class definition                -
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

#ifndef  AFNIX_INPUTSTRING_HPP
#define  AFNIX_INPUTSTRING_HPP

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

namespace afnix {

  /// The InputString class implements a simple string based mechanism for
  /// an input stream. The stream can be loaded at construction or by using
  /// the "set" method. The stream supports the standard Input class methods.
  /// @author amaury darsch

  class InputString : public InputBuffer {
  public:
    /// create a new string stream without data
    InputString (void);

    /// create a new string stream with a buffer
    /// @param data the data in this input stream
    InputString (const String& data);

    /// @return the class name
    String repr (void) const override;

    /// @return the stream timeout
    long gettout (void) const override;

    /// set the stream timeout
    /// @param tout the stream time to set
    void settout (const long tout) override;

    /// @return true if we are at the eos
    bool iseos (void) const override;

    /// @return true if we can read a character
    bool valid (void) const override;

    /// @return the new character on the input stream
    char read (void) override;

    /// flush the buffer and set the stream with a new string
    /// @param data the string to set to this stream
    virtual void set (const String& data);

    /// @return the next character but do not remove it
    virtual char get (void) const;
  
  private:
    // make the copy constructor private
    InputString (const InputString&);
    // make the assignment operator private
    InputString& operator = (const InputString&);

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
