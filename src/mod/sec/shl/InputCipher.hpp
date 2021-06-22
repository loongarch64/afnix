// ---------------------------------------------------------------------------
// - InputCipher.hpp                                                         -
// - afnix:sec module - input stream cipher class definition                 -
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

#ifndef  AFNIX_INPUTCIPHER_HPP
#define  AFNIX_INPUTCIPHER_HPP

#ifndef  AFNIX_CIPHER_HPP
#include "Cipher.hpp"
#endif

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

namespace afnix {

  /// The InputCipher implements an input stream interface that can
  /// stream out an input stream from a cipher. In other word, an input
  /// stream is read and encoded or decoded as long as the input stream
  /// read characters. If the cipher is nil, the input cipher simply
  /// read the input stream and is therefore transparent.
  /// @author amaury darsch

  class InputCipher : public InputBuffer {
  private:
    /// the stream cipher
    Cipher* p_sc;
    /// the input stream
    InputStream* p_is;

  public:
    /// create a default input cipher
    InputCipher (void);

    /// create an input cipher by cipher
    /// @param sc the stream cipher
    InputCipher (Cipher* sc);

    /// create an input cipher by stream
    /// @param is the input stream to bind
    InputCipher (InputStream* is);

    /// create an input cipher by stream
    /// @param is the input stream to bind
    /// @param sc the stream cipher
    InputCipher (InputStream* is, Cipher* sc);

    /// destroy this input cipher
    ~InputCipher (void);

    /// @return the class name
    String repr (void) const;

    /// reset this input cipher
    void reset (void);

    /// @return the stream timeout
    long gettout (void) const;

    /// set the stream timeout
    /// @param tout the stream time to set
    void settout (const long tout);
    
    /// @return true if we are at the eos
    bool iseos (void) const;

    /// @return true if we can read a character
    bool valid (void) const;

    /// @return the next available character
    char read (void);

    /// set the input stream
    /// @param is the input stream to set
    void setis (InputStream* is);

    /// set the stream cipher
    /// @param sc the stream cipher to set
    void setsc (Cipher* sc);

  private:
    // make the copy constructor private
    InputCipher (const InputCipher&);
    // make the assignment operator private
    InputCipher& operator = (const InputCipher&);

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
