// ---------------------------------------------------------------------------
// - Pbkdf2.hpp                                                              -
// - afnix:sec module - pkcs key derivation (PBKDF2) class definition        -
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

#ifndef  AFNIX_PBKDF2_HPP
#define  AFNIX_PBKDF2_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Pbkdf2 class is a hashed key derivation function class that
  /// implements the PKCS5 V2.1 key derivation function which is itself
  /// derived from the KDF2 derivation function. There is almost no
  /// difference except the number of iteration and the method used to
  /// process the password. The heart of the derivation process is a hmac
  /// those key is the password. an optional salt can be processed as well.
  /// Note that this implementation is compatible with Openssl. Openssl uses
  /// a SHA256-HMAC with 10000 iterations. The default number of iterations
  /// in this implementation is 1.
  /// @author amaury darsch

  class Pbkdf2 : public Object {
  protected:
    /// the salt length
    long d_slen;
    /// the salt data
    t_byte* p_salt;
    /// the key derivate size
    long d_kbsz;
    /// the iteration number
    long d_inum;
    /// the hasher object
    Hasher* p_hobj;
    
  public:
    /// create a default key derivation function
    Pbkdf2 (void);
    
    /// create a kdf2 object by size
    /// @param kbsz the key buffer size
    Pbkdf2 (const long kbsz);

    /// create a kdf2 object by size and iteration number
    /// @param kbsz the key buffer size
    /// @param inum the iteration number
    Pbkdf2 (const long kbsz, const long inum);

    /// destroy this key derivatio function
    ~Pbkdf2 (void);

    /// @return the class name
    String repr (void) const;

    /// bind a hashable
    /// @param hobj the hasher to bind
    virtual void bind (Hasher* hobj);

    /// generate a random salt by size
    /// @param size the salt size
    virtual void newsalt (const long size);

    /// set the salt by string
    /// @param salt the salt string
    virtual void setsalt (const String& salt);

    /// set the key derivation salt by octet string
    /// @param ostr the octet string to process
    /// @param size the octet string size
    virtual void setsalt (const t_byte* ostr, const long size);

    /// @return the salt as a buffer
    virtual Buffer getsalt (void) const;
    
    /// set the key derivation size
    /// @param kbsz the key derivation size
    virtual void setkbsz (const long kbsz);
    
    /// set the iteration number
    /// @param inum the iteration number
    virtual void setinum (const long inum);
    
    /// derive a key by string
    /// @param ostr the octet string to process
    virtual Buffer derive (const String& ostr) const;
 
    /// derive a key from an octet string by size
    /// @param ostr the octet string to process
    /// @param size the octet string size
    virtual Buffer derive (const t_byte* ostr, const long size) const;
 
  private:
    // make the copy constructor private
    Pbkdf2 (const Pbkdf2&) = delete;
    // make the assignment operator private
    Pbkdf2& operator = (const Pbkdf2&) = delete;

  public:
    /// cerate a new object in a generic way
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
