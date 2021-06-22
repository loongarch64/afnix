// ---------------------------------------------------------------------------
// - BlockBuffer.hpp                                                         -
// - standard object library - block buffer class definition                 -
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

#ifndef  AFNIX_BLOCKBUFFER_HPP
#define  AFNIX_BLOCKBUFFER_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The BlockBuffer class is a special buffer class designed to hold bytes
  /// in a bound or unbound way. In the bound mode, the buffer size is know
  /// and the buffer cannot be resized. In the unbound mode, the buffer size
  /// is unknown and the buffer can be resized as needed. The block buffer
  /// is designed to be loaded by various means, including data, buffer or
  /// stream. Additionaly, the block buffer can be used to write into another
  /// buffer or a stream by block. By default the read and write block size
  /// is the system block size and the default mode is the bound mode, which
  /// can be changed by setting the buffer resize flag.
  /// @author amaury darsch

  class BlockBuffer : public Buffer {
  protected:
    /// the buffer read count
    t_octa d_rcnt;
    /// the buffer write count
    t_long d_wcnt;

  public:
    /// create a default block buffer
    BlockBuffer (void);

    /// create a block buffer by size
    /// @param size the block buffer size
    BlockBuffer (const long size);

    /// @return the class name
    String repr (void) const override;

    /// reset this block buffer
    void reset (void) override;

    /// add a character to this block buffer
    /// @param value the character to add
    long add (const char value) override;

    /// @return the next available character
    char read (void) override;

    /// pushback a character to this block buffer
    /// @param value the character to pushback
    long pushback (const char value) override;

    /// @return the block buffer read count
    virtual t_long getrcnt (void) const;

    /// @return the block buffer write count
    virtual t_long getwcnt (void) const;

    /// copy a character array in the block buffer
    /// @param data the character buffer to copy
    /// @param size the buffer size
    virtual long copy (const char* data, const long size);

    /// add a string to this block buffer
    /// @param s the string to add to this buffer
    virtual long copy (const String& s);

    /// copy a buffer into this block buffer
    /// @param buffer the buffer to copy
    virtual long copy (Buffer& buffer);

    /// copy an input stream into this block buffer
    /// @param is the input stream to copy
    virtual long copy (InputStream& is);

    /// copy this block buffer into and output stream
    /// @param os the destination output stream
    virtual long copy (OutputStream& os);

    /// copy an input stream into an output stream
    /// @param os the destination output stream
    /// @param is the input stream to copy
    virtual long copy (OutputStream& os, InputStream& is);

  private:
    // make the copy constructor private
    BlockBuffer (const BlockBuffer&);
    // make the assignment operator private
    BlockBuffer& operator = (const BlockBuffer&);

  public:
    /// create a new object in a generic object
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
