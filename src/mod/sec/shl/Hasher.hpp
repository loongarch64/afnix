// ---------------------------------------------------------------------------
// - Hasher.hpp                                                              -
// - afnix:sec module - base message hasher class definition                 -
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

#ifndef  AFNIX_HASHER_HPP
#define  AFNIX_HASHER_HPP

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

#ifndef  AFNIX_HASHABLE_HPP
#include "Hashable.hpp"
#endif

#ifndef  AFNIX_BLOCKBUFFER_HPP
#include "BlockBuffer.hpp"
#endif

namespace afnix {

  /// The Hasher class is a base class that is used to build a message
  /// hash. The hash result is stored in an array of bytes and can be
  /// retreived byte by byte or as a formatted octet string.
  /// @author amaury darsch

  class Hasher : public BlockBuffer, public Hashable {
  protected:
    /// the hash size
    long    d_hlen;
    /// the result length
    long    d_rlen;
    /// the hash result
    t_byte* p_hash;

  public:
    /// create a hasher object by name and size
    /// @param name the hasher name
    /// @param size the block size
    /// @param hlen the hash length
    Hasher (const String& name, const long size, const long hlen);

    /// create a hasher object by name, size and result length
    /// @param name the hasher name
    /// @param size the block size
    /// @param hlen the hash length
    /// @param rlen the result length
    Hasher (const String& name, const long size, const long hlen, 
	    const long rlen);

    /// destroy the hasher
    ~Hasher (void);

    /// reset this hasher
    void reset (void) override;

    /// @return the hasher length
    long gethlen (void) const override;

    /// @return the hasher result length
    long getrlen (void) const override;

    /// @return the hash value by index
    t_byte getbyte (const long index) const override;

    /// @return the hash value as a relatif
    Relatif gethval (void) const override;

    /// @return the hash result as a relatif
    Relatif getrval (void) const override;

    /// @return the formatted message hash
    String format (void) const override;

    /// process a message by data
    /// @param data the data to process
    /// @param size the data size
    void process (const t_byte* data, const long size) override;
 
    /// process a message with a buffer
    /// @param buf the buffer to process
    void process (Buffer& buf) override;

    /// process a message with an input stream
    /// @param is the input stream to process
    void process (InputStream& is) override;

    /// check if a string is a valid hash
    /// @param s the string to check
    virtual bool ishash (const String& s) const;

  protected:
    /// update the hasher state with the buffer data
    virtual bool update (void) =0;

  private:
    // make the copy constructor private
    Hasher (const Hasher&);
    // make the assignment operator private
    Hasher& operator = (const Hasher&);

  public:
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
