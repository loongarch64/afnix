// ---------------------------------------------------------------------------
// - Kdf.hpp                                                                 -
// - afnix:sec module - base key derivation function class definition        -
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

#ifndef  AFNIX_KDF_HPP
#define  AFNIX_KDF_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The Kdf class is an abstract class used to model key derivation
  /// function. The class provides only a byte buffer which can be accessed
  /// by index. Ile the key derivation functions land, there are numerous
  /// standards, such like PKCS 2.1, IEEE P1363-2000, ISO/IEC 18033-2.
  /// All of these standards have sometimes conflicting definitions.
  /// @author amaury darsch

  class Kdf : public Nameable {
  protected:
    /// the kdf name
    String  d_name;
    /// key buffer size
    long    d_kbsz;
    /// derived key buffer
    t_byte* p_kbuf;

  public:
    /// create a kdf object by name and size
    /// @param name the kdf name
    /// @param kbsz the key buffer size
    Kdf (const String& name, const long kbsz);

    /// destroy this kdf object
    ~Kdf (void);

    /// @return the class name
    String repr (void) const;

    /// @return the kdf name
    String getname (void) const;

    /// reset this kdf object
    virtual void reset (void);

    /// @return the key buffer size
    virtual long getkbsz (void) const;

    /// get the key byte by index
    /// @param index the byte index
    virtual t_byte getbyte (const long index) const;

    /// @return the key as a buffer
    virtual Buffer getkbuf (void) const;
    
    /// @return a string representation of the key
    virtual String format (void) const;

    /// derive a key from a string
    /// @param s the string to process
    virtual String derive (const String& s);

    /// derive a key from an octet string by size
    /// @param ostr the octet string to process
    /// @param size the octet string size
    virtual void derive (const t_byte* ostr, const long size) =0;

    /// compute a key from a message
    /// @param emsg the message to process
    virtual String compute (const String& mesg);
    
  private:
    // make the copy constructor private
    Kdf (const Kdf&);
    // make the assignment operator private
    Kdf& operator = (const Kdf&);

  public:
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
