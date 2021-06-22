// ---------------------------------------------------------------------------
// - InputMapped.hpp                                                         -
// - standard object library - mapped input stream class definition          -
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

#ifndef  AFNIX_INPUTMAPPED_HPP
#define  AFNIX_INPUTMAPPED_HPP

#ifndef  AFNIX_BITSET_HPP
#include "Bitset.hpp"
#endif

#ifndef  AFNIX_INPUTBUFFER_HPP
#include "InputBuffer.hpp"
#endif

#ifndef  AFNIX_INPUTTIMEOUT_HPP
#include "InputTimeout.hpp"
#endif

namespace afnix {

  /// The InputMapped class is an input stream class that provides the
  /// facility for reading a mapped input stream. The input stream
  /// is mapped at construction given a file name, a size and a file offset.
  /// An anonymous mapped input stream can also be designed with a buffer
  /// object. Finally, without any information an always valid null input
  /// stream is constructed.
  /// @author amaury darsch

  class InputMapped : public InputBuffer, public InputTimeout {
  private:
    /// the mapped buffer
    char*  p_mbuf;
    /// the anonymous flag
    bool   d_anon;
    /// the buffer size
    t_long d_size;
    /// the buffer marker
    t_long d_mark;

  public:
    /// create a null input stream
    InputMapped (void);

    /// create a new mapped input stream by name
    /// @param name the input file name to map
    InputMapped (const String& name);

    /// create an anonymous mapped input stream
    /// @param buf the buffer to map
    InputMapped (const Buffer& buf);

    /// create an anonymous mapped input stream
    /// @param bset the bitset to map
    InputMapped (const Bitset& bset);

    /// create a new mapped input stream by name, size and offset
    /// @param name the input file name to map
    /// @param size the buffer size to map
    /// @param boff the buffer offset
    InputMapped (const String& name, const t_long size, const t_long boff);

    /// destroy this mapped input stream
    ~InputMapped (void);

    /// @return the class name
    String repr (void) const override;

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

    /// place the marker at a certain position
    /// @param pos the position to go
    virtual void lseek (const t_long pos);

    /// @return the mapped input stream size
    virtual t_long length (void) const;

    /// @return a buffer of this mapped stream
    virtual Buffer* tobuffer (void) const;
    
  private:
    // make the copy constructor private
    InputMapped (const InputMapped&);
    // make the assignment operator private
    InputMapped& operator = (const InputMapped&);

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
