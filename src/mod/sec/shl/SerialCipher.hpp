// ---------------------------------------------------------------------------
// - SerialCipher.hpp                                                        -
// - afnix:sec module - serial cipher class definition                       -
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

#ifndef  AFNIX_SERIALCIPHER_HPP
#define  AFNIX_SERIALCIPHER_HPP

#ifndef  AFNIX_CIPHER_HPP
#include "Cipher.hpp"
#endif

namespace afnix {

  /// The SerialCipher class is an abstract class that is used to implement 
  /// a byte serial cipher. In this type of cipher, the encryption and
  /// decryption operation is done on a byte basis. A serial cipher is
  /// similar to a block cipher but does not generally provides a padding
  /// scheme since there is no concept of block. However, to ease the
  /// processing, a serial block size is associated with the class which
  /// can be used to control the stream or the buffer processing on a block
  /// basis as long as the serial block size is not null.
  /// @author amaury darsch

  class SerialCipher : public Cipher {
  protected:
    /// the serial block size
    long d_sbsz;

  public:
    /// create a cipher by name
    /// @param name the cipher name
    SerialCipher (const String& name);

    /// reset this cipher
    void reset (void) override;
    
    /// clear this cipher
    void clear (void) override;
    
    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;

    /// set the serial block size
    /// @param sbsz the serial block size
    virtual void setsbsz (const long sbsz);

    /// @return the serial block size
    virtual long getsbsz (void) const;

  protected:
    /// encode a byte into another one
    /// @param bi the input byte
    virtual t_byte encode (const t_byte bi) =0;
    
    /// decode a byte into another one
    /// @param bi the input byte
    virtual t_byte decode (const t_byte bi) =0;

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

    /// decode an input stream into an output stream
    /// @param os the output stream to write
    /// @param is the input stream to read
    /// @return the number of processed bytes
    long decode (OutputStream& os, InputStream& is) override;

    /// preset the stream processing
    long preset (void ) override;

    /// finish the stream processing
    long finish (void ) override;
    
  private:
    // make the copy constructor private
    SerialCipher (const SerialCipher&) =delete;
    // make the assignment operator private
    SerialCipher& operator = (const SerialCipher&) =delete;

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
