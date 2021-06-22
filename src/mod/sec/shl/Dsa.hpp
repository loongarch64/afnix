// ---------------------------------------------------------------------------
// - Dsa.hpp                                                                 -
// - afnix:sec module - dsa/dsa signature class definition                   -
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

#ifndef  AFNIX_DSA_HPP
#define  AFNIX_DSA_HPP

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

#ifndef  AFNIX_SIGNER_HPP
#include "Signer.hpp"
#endif

namespace afnix {

  /// The Dsa class is an original implementation of the Digital Signature
  /// Standard (DSS) as published in FIPS PUB 186-4. This class implements
  /// the Digital Signature Algorithm (DSA) with an approved key length of
  /// 1024, 2048 and 3072 bits with a 160, 224 and 256 bits hash function
  /// which is part of the SHA family.
  /// @author amaury darsch

  class Dsa : public Signer {
  private:
    /// the approved hasher
    Hasher* p_hash;
    /// the fixed message secret
    Relatif d_sk;

  public:
    /// create a default dsa signature
    Dsa (void);

    /// create a dsa signature by key
    /// @param key the signature key
    Dsa (const Key& key);

    /// create a dsa signature by key
    /// @param key the signature key
    /// @param k   the secret number k
    Dsa (const Key& key, const Relatif& k);

    /// create a dsa signature by signature
    /// @param sign the signature
    Dsa (const Signature& sign);
    
    /// destroy this signature
    ~Dsa (void);

    /// @return the class name
    String repr (void) const;

    /// reset this signature
    void reset (void);

    /// set the signature key
    /// @param key the signature key to set
    void setkey (const Key& key);

  protected:
    /// process a message by data
    /// @param data the data to process
    /// @param size the data size
    void process (const t_byte* data, const long size);
 
    /// process a message with a buffer
    /// @param buf the buffer to process
    void process (Buffer& buf);

    /// process a message with an input stream
    /// @param is the input stream to process
    void process (InputStream& is);

    /// finish the signature processing
    Signature finish (void);

    /// validate the signature processing
    /// @param sgn the signature to validate
    bool validate (const Signature& sgn);
    
  private:
    // make the copy constructor private
    Dsa (const Dsa&);
    // make the assignment operator private
    Dsa& operator = (const Dsa&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
