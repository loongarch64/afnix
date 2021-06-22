// ---------------------------------------------------------------------------
// - PublicCipher.hpp                                                        -
// - afnix:sec module - public cipher class definition                       -
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

#ifndef  AFNIX_PUBLICCIPHER_HPP
#define  AFNIX_PUBLICCIPHER_HPP

#ifndef  AFNIX_CIPHER_HPP
#include "Cipher.hpp"
#endif

namespace afnix {

  /// The PublicCipher class is an abstract class that is used to implement 
  /// an asymmetric cipher. An asymmetric cipher or public key cipher is
  /// designed to operate with a public key and a secret key. Depending
  /// on the use model, the public key might be used to crypt the data, and
  /// the secret key to decrypt. The basic assumption around a public cipher
  /// is that the secret key cannot be derived from the public key.
  /// @author amaury darsch

  class PublicCipher : public Cipher {
  protected:
    /// the message block size
    long d_mbsz;
    /// the crypted block size
    long d_cbsz;

  public:
    /// create a public cipher by name
    /// @param name the cipher name
    PublicCipher (const String& name);

    /// set the public cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;

    /// @return the message block size
    virtual long getmbsz (void) const;

    /// @return the crypted block size
    virtual long getcbsz (void) const;

  protected:    
    /// encode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    /// @param cc the input count
    virtual long encode (t_byte* bo, const t_byte* bi, const long cc) =0;
    
    /// decode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    /// @param cc the input count
    virtual long decode (t_byte* bo, const t_byte* bi, const long cc) =0;

    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, Buffer& ib) override;

    /// encode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, InputStream& ib) override;

    /// encode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long encode (OutputStream& os, InputStream& is) override;

    /// decode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long decode (Buffer& ob, Buffer& ib) override;

    /// decode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long decode (Buffer& ob, InputStream& ib) override;

    /// decode an input buffer into an output buffer
    /// decode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long decode (OutputStream& os, InputStream& is) override;

    /// preset the stream processing
    long preset (void) override;

    /// finish the stream processing
    long finish (void) override;

  private:
    // make the copy constructor private
    PublicCipher (const PublicCipher&) =delete;
    // make the assignment operator private
    PublicCipher& operator = (const PublicCipher&) =delete;

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
