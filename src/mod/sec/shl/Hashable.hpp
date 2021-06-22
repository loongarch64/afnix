// ---------------------------------------------------------------------------
// - Hashable.hpp                                                            -
// - afnix:sec module - hashable base class definition                       -
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

#ifndef  AFNIX_HASHABLE_HPP
#define  AFNIX_HASHABLE_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The Hashable class is a base class for the hasher and hmac objects.
  /// The class provides the common methods to feed a hashable object as
  /// well as the accessors to the result.
  /// @author amaury darsch

  class Hashable : public Nameable {
  protected:
    /// the hashable name
    String d_name;
    
  public:
    /// create a hashable object by name
    /// @param name the hashable name
    Hashable (const String& name);

    /// @return the hashable name
    String getname (void) const override;
    
    /// reset this hashable
    virtual void reset (void) =0;

    /// @return the hashable length
    virtual long gethlen (void) const =0;

    /// @return the hashable result length
    virtual long getrlen (void) const =0;
    
    /// @return the byte value by index
    virtual t_byte getbyte (const long index) const =0;

    /// @return the hash value as a relatif
    virtual Relatif gethval (void) const =0;

    /// @return the hash result as a relatif
    virtual Relatif getrval (void) const =0;

    /// @return the formatted hashable string
    virtual String format (void) const =0;

    /// process a message by data
    /// @param data the data to process
    /// @param size the data size
    virtual void process (const t_byte* data, const long size) =0;
 
    /// process a message with a buffer
    /// @param buf the buffer to process
    virtual void process (Buffer& buf) =0;

    /// process a message with an input stream
    /// @param is the input stream to process
    virtual void process (InputStream& is) =0;

    /// finish processing the hashable
    virtual void finish (void) =0;

    /// derive a message from a string
    /// @param s the string to process
    virtual String derive (const String& msg);

    /// compute a message from a string
    /// @param msg the string message to process
    virtual String compute (const String& msg);

    /// compute a message from a buffer
    /// @param buf the buffer to process
    virtual String compute (Buffer& buf); 

    /// compute a message from an input stream
    /// @param is the input stream
    virtual String compute (InputStream& is); 

    /// push the hash value into a buffer
    /// @param buf the buffer to fill
    virtual long pushb (Buffer& buf);

    /// hash and push the hash into a buffer
    /// @param obuf the buffer to fill
    /// @param ibuf the buffer to hash
    virtual long pushb (Buffer& obuf, Buffer& ibuf);
    
  private:
    // make the copy constructor private
    Hashable (const Hashable&) =delete;
    // make the assignment operator private
    Hashable& operator = (const Hashable&) =delete;

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
