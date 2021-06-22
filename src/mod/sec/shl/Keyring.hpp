// ---------------------------------------------------------------------------
// - Keyring.hpp                                                             -
// - afnix:sec module - keyring class definition                             -
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

#ifndef  AFNIX_KEYRING_HPP
#define  AFNIX_KEYRING_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_CRYPTO_HPP
#include "Crypto.hpp"
#endif

namespace afnix {

  /// The Keyring class is a container of keys which can be attached to
  /// a crypted buffer (of file). When the keyring is loaded, an input
  /// stream is decoded and the associated buffer is unserialized as a vector
  /// of keys. Naturally, when the keyring is saved (as opposed to serialized)
  /// the vector of keys is serialized and then crypted.
  /// @author amaury darsch

  class Keyring : public Taggable {
  protected:
    /// the cipher type
    Crypto::t_cifr d_cifr;
    /// the hasher type
    Crypto::t_hash d_hash;
    /// the key size
    long d_cksz;
    /// the unlocked vector
    mutable Vector* p_keys; 
    /// the locked buffer
    mutable Buffer* p_lbuf;
    
  public:
    /// create a default keyring
    Keyring (void);

    /// create a keyring by name
    /// @param name the keyring name
    Keyring (const String& name);

    /// create a keyring by name and info
    /// @param name the keyring name
    /// @param info the keyring info
    Keyring (const String& name, const String& info);

    /// copy construct this keyring
    /// @param that the key keyring to copy
    Keyring (const Keyring& that);

    // destroy this keyring
    ~Keyring (void);
    
    /// assign a keyring to this one
    /// @param that the keyring to assign
    Keyring& operator = (const Keyring& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this key
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this key
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// create a keyring key by credential
    /// @param cred the key credential
    virtual Key tockey (const String& cred) const;

    /// @return true if the keyring is open
    virtual bool isopen (void) const;
    
    /// open the keyring by credential
    /// @param cred the credential string
    virtual void open (const String& cred);

    /// open the keyring by credential key
    /// @param ckey the credential key
    virtual void open (const Key& ckey);
    
    /// close the keyring by credential
    /// @param cred the credential string
    virtual void close (const String& cred);

    /// close the keyring by credential key
    /// @param ckey the credential key
    virtual void close (const Key& ckey);
    
    /// check if a key exists
    /// @param name the key name to check
    virtual bool exists (const String& name) const;
    
    /// add a key to the keyring
    /// @param key the key to add
    virtual void add (const Key& key);

    /// @return a key by name
    virtual Key get (const String& name) const;

    /// @return the vector of keys
    virtual Vector* tokeys (void);

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
