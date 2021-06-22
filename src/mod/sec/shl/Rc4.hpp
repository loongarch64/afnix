// ---------------------------------------------------------------------------
// - Rc4.hpp                                                                 -
// - afnix:sec module - RC4 block cipher class definition                    -
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

#ifndef  AFNIX_RC4_HPP
#define  AFNIX_RC4_HPP

#ifndef  AFNIX_SERIALCIPHER_HPP
#include "SerialCipher.hpp"
#endif

namespace afnix {

  /// The Rc4 class is a serial cipher class that implements the RC4
  /// encryption algorithm. This is an original implementation that
  /// conforms to ARC4 published specification. The cipher operates with
  /// a serial key which can be at least 256 bytes.
  /// @author amaury darsch

  class Rc4 : public SerialCipher {
  private:
    /// the derived s-box
    t_byte* p_sbox;
    /// the sbox indexes
    long d_sidx[2];

  public:
    /// create a new cipher by key
    /// @param key the cipher key
    Rc4 (const Key& key);

    /// create a new cipher by key and flag
    /// @param key the cipher key
    /// @param rflg the reverse flag
    Rc4 (const Key& key, const bool rflg);

    /// destroy this cipher
    ~Rc4 (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this cipher
    void reset (void) override;

    /// clear this cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
  protected:    
    /// encode a byte into another one
    /// @param bi the input byte
    t_byte encode (const t_byte bi) override;
    
    /// decode a byte into another one
    /// @param bi the input byte
    t_byte decode (const t_byte bi) override;

  private:
    // make the copy constructor private
    Rc4 (const Rc4&) =delete;
    // make the assignment operator private
    Rc4& operator = (const Rc4&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
