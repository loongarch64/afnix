// ---------------------------------------------------------------------------
// - Bitset.hpp                                                              -
// - standard object library - bitset class definition                       -
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

#ifndef  AFNIX_BITSET_HPP
#define  AFNIX_BITSET_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

namespace afnix {

  /// The Bitset class is a resizable bit set object which can be used
  /// to mark a particular bit in a defined field. Standard boolean methods
  /// are provided to manipulate the bits. Note that the bitset can only
  /// be resized through the use of the "add" method.
  /// @author amaury darsch

  class Bitset : public virtual Object {
  public:
    /// generate a random bitset
    /// @param bits the number of bits
    static Bitset random (const long bits);

  private:
    /// the bitset size
    long    d_size;
    /// the byte size
    long    d_bsiz;
    /// the array of bytes
    t_byte* p_byte;

  public:
    /// create an empty bitset
    Bitset (void);

    /// create a bitset by size size
    /// @param size the bitset size
    Bitset (const long size);

    /// create a bitset by string
    /// @param sval the string value
    Bitset (const String& sval);

    /// create a bitset with a buffer
    /// @param buf the buffer to copy
    /// @param baf the byte ascending flag
    Bitset (const Buffer& buf, const bool baf);

    /// copy construct this bitset
    /// @param that the bitset to copy 
    Bitset (const Bitset& that);

    /// copy move this bitset
    /// @param that the bitset to copy 
    Bitset (Bitset&& that) noexcept;

    /// destroy this bitset
    ~Bitset (void);

    /// assign a bitset to this one
    /// @param that the bitset to assign
    Bitset& operator = (const Bitset& that);

    /// move a bitset to this one
    /// @param that the bitset to assign
    Bitset& operator = (Bitset&& that) noexcept;

    /// @return true if two bitset are equals
    bool operator == (const Bitset& that) const;

    /// add a bitset to this one
    /// @param bset the bitset to add
    Bitset operator + (const Bitset& bset) const;

    /// @return the class name
    String repr (void) const override;

    /// @return a clone of this object
    Object* clone (void) const override;

    /// reset this bitset
    void reset (void);

    /// @return the bitset size
    long length (void) const;

    /// @return the bitset byte size
    long bsize (void) const;

    /// @return true if the bit position is marked
    bool ismark (const long pos) const;

    /// clear a bit at a certain position
    /// @param pos the bit position
    void clear (const long pos);

    /// mark a bit at a certain position
    /// @param pos the bit position
    void mark (const long pos);

    /// mark a bit at a certain position by value
    /// @param pos the bit position
    /// @param bval the boolean value to set
    void mark (const long pos, const bool bval);

    /// add a bit at a certain position by value
    /// @param pos the bit position
    /// @param bval the boolean value to set
    void add (const long pos, const bool bval);

    /// set a bitset by octa value
    /// @param oval the octa value to set
    void set (const t_octa oval);

    /// set a bitset by string value
    /// @param sval the bit string to parse
    void set (const String& sval);

    /// map the bitset to a byte by position
    /// @param pos the starting bit position
    t_byte tobyte (const long pos) const;

    /// @return the clamping bit index by value
    long getcbi (const bool bval) const;

    /// clamp the bitset by value
    /// @param bval the clamping bit value
    void clamp (const bool bval);

    /// extact a sub bitset by size
    /// @param size the sub bitset size
    Bitset subset (const long size) const;

    /// extact a sub bitset by size and position
    /// @param size the sub bitset size
    /// @param spos the start position
    Bitset subset (const long size, const long spos) const;

    /// convert a bitset to a relatif
    Relatif torelatif (void) const;

    /// map this bitset to an anonymous data structure
    /// @param data the data structure to map
    /// @param size the data size to map
    long tomap (void* data, const long size) const;

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
    /// @param args  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv) override;
  };
}

#endif
