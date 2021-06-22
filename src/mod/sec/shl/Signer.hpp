// ---------------------------------------------------------------------------
// - Signer.hpp                                                              -
// - afnix:sec module - base message signature class definition              -
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

#ifndef  AFNIX_SIGNER_HPP
#define  AFNIX_SIGNER_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

#ifndef  AFNIX_SIGNABLE_HPP
#include "Signable.hpp"
#endif

#ifndef  AFNIX_SIGNATURE_HPP
#include "Signature.hpp"
#endif

namespace afnix {

  /// The Signer class is a base class that is used to build a message
  /// signature. The signature result is stored in a special signature
  /// object which is algorithm dependant.
  /// @author amaury darsch

  class Signer : public Nameable {
  protected:
    /// the signature name
    String  d_name;
    /// the signature key
    Key     d_skey;

  public:
    /// create a signature object by name
    /// @param name the signature name
    Signer (const String& name);

    /// create a signature object by name and key
    /// @param name the signature name
    /// @param key  the signature key
    Signer (const String& name, const Key& key);

    /// @return the class name
    String repr (void) const;

    /// @return the signature name
    String getname (void) const;

    /// reset this signature
    virtual void reset (void) =0;

    /// set the sgnature key
    /// @param key the key to set
    virtual void setkey (const Key& key);

    /// @return the signature key
    virtual Key getkey (void) const;

    /// derive a message signature from an octet string
    /// @param s the string to process
    virtual Signature derive (const String& msg);

    /// compute a message signature from a string
    /// @param msg the string message to process
    virtual Signature compute (const String& msg);

    /// compute a message signature from a buffer
    /// @param buf the buffer to process
    virtual Signature compute (Buffer& buf); 

    /// compute a message signature from an input stream
    /// @param is the input stream
    virtual Signature compute (InputStream& is); 

    /// compute the signature of a signable object
    /// @param sobj the signable object
    virtual Signature compute (Signable* sobj);
    
    /// verify a message signature from a string
    /// @param sgn the signature to verify
    /// @param msg the the string message
    virtual bool verify (const Signature& sgn, const String& msg);

    /// verify a message signature from a buffer
    /// @param sgn the signature to verify
    /// @param buf the buffer to process
    virtual bool verify (const Signature& sgn, Buffer& buf); 

    /// verify a message signature from an input stream
    /// @param sgn the signature to verify
    /// @param is the input stream
    virtual bool verify (const Signature& sgn, InputStream& is);
    
    /// verify a message signature from a seril object
    /// @param sgn  the signature to verify
    /// @param sojb the signable object
    virtual bool verify (const Signature& sgn, const Signable* sobj);
    
  protected:
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

    /// finish the signature processing
    virtual Signature finish (void) =0;

    /// validate the signature processing
    /// @param sgn the signature to validate
    virtual bool validate (const Signature& sgn) =0;

  private:
    // make the copy constructor private
    Signer (const Signer&);
    // make the assignment operator private
    Signer& operator = (const Signer&);

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
