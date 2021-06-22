// ---------------------------------------------------------------------------
// - Hmac.hpp                                                                -
// - afnix:sec module - hashed mac class definition                          -
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

#ifndef  AFNIX_HMAC_HPP
#define  AFNIX_HMAC_HPP

#ifndef  AFNIX_MAC_HPP
#include "Mac.hpp"
#endif

#ifndef  AFNIX_HASHER_HPP
#include "Hasher.hpp"
#endif

namespace afnix {

  /// The Hmac class is a hashed message authentication code (mac) class
  /// that conforms to FIPS PUB 198. The class operates with a hasher object
  /// compute a mac. By default, the hasher object is SHA-1. A mac can be
  /// computed from a string, a buffer or an input stream in a way similar
  /// to the hasher object.
  /// @author amaury darsch

  class Hmac : public Mac {
  protected:
    /// the hasher object
    Hasher* p_hash;
    
  public:
    /// create default hmac by key
    /// @param mkey the hmac message key
    Hmac (const Key& mkey);

    /// create a hmac object by key and hasher
    /// @param mkey the hmac message key
    /// @param hash the hmac hasher
    Hmac (const Key& mkey, Hasher* hash);

    /// destroy this hmac
    ~Hmac (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this hmac
    void reset (void) override;

    /// @return the hmac length
    long gethlen (void) const override;

    /// @return the hmac result length
    long getrlen (void) const override;

    /// @return the hmac value by index
    t_byte getbyte (const long index) const override;

    /// @return the hmac value as a relatif
    Relatif gethval (void) const override;

    /// @return the hmac result as a relatif
    Relatif getrval (void) const override;

    /// @return the formatted hmac code
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
    
    /// finish processing the hmac
    void finish (void) override;

  private:
    // make the copy constructor private
    Hmac (const Hmac&);
    // make the assignment operator private
    Hmac& operator = (const Hmac&);

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
  };
}

#endif
