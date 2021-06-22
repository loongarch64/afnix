// ---------------------------------------------------------------------------
// - ModeCipher.hpp                                                         -
// - afnix:sec module - block cipher class definition                        -
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

#ifndef  AFNIX_MODECIPHER_HPP
#define  AFNIX_MODECIPHER_HPP

#ifndef  AFNIX_BLOCKCIPHER_HPP
#include "BlockCipher.hpp"
#endif

namespace afnix {

  /// The ModeCipher class is an abstract class that is used to implement 
  /// a symmetric block cipher.  The class provides the mechanism
  /// to pad a cipher block. The default padding mode is the NIST 800-38A.
  /// the class also provides the support for the block mode. By default, 
  /// the class operates with the electronic codebook (ECB) mode . 
  /// The ECB mode simply copy a block. The cipher block chaining (CBC) mode
  /// is a common mode that operates by xoring a block with the previous one.
  /// In order to start, an initial block called the initial vector (IV) must 
  /// be set. Variations around the CBC exists in the form of the cipher
  /// feedback block (CFB) mode and output feedback block (OFB) mode.
  /// For a block cipher, a block size controls the cipher operations.
  /// @author amaury darsch

  class ModeCipher : public BlockCipher {
  public:
    /// cipher padding mode
    enum t_pmod
      {
       PMOD_NONE, // no padding
       PMOD_BITM, // bit padding
       PMOD_X923, // ANSI X.923
       PMOD_N800  // NIST 800-38A, PKCS#7, RFC 2130 padding mode
      };

    /// the cipher block mode
    enum t_cmod
      {
       CMOD_ECBM, // electronic codebook mode
       CMOD_CBCM, // cipher block chaining mode
       CMOD_EBCM, // explicit block chaining mode
       CMOD_CFBM, // cipher feedback block mode
       CMOD_OFBM, // output feedback block mode
      };

  protected:
    /// the cipher padding mode
    t_pmod d_pmod;
    /// the cipher block mode
    t_cmod  d_cmod;
    /// the last block
    t_byte* p_bl;
    /// the iv size
    long d_ivsz;
    /// the iv data
    t_byte* p_iv;
    /// the initial block flag
    bool d_iblk;

  public:
    /// create a cipher by name and size
    /// @param name the cipher name
    /// @param cbsz the cipher block size
    ModeCipher (const String& name, const long cbsz);

    /// destroy this mode cipher
    ~ModeCipher (void);

    /// reset this mode cipher
    void reset (void) override;

    /// clear this mode cipher
    void clear (void) override;

    /// set the cipher key
    /// @param key the key to set
    void setkey (const Key& key) override;
    
    /// normalize a data size
    /// @param size the size to normalize
    t_long waist (const t_long size) const override;

    /// set the initial vector by octet string
    /// @param ivs the initial vector string
    void setiv (const String& ivs) override;

    /// set the initial vector by buffer
    /// @param kiv the initial vector buffer
    void setiv (const Buffer& kiv) override;

    /// @return the initial vector as an octet string
    String getiv (void) const override;
    
    /// set the cipher padding mode
    /// @param pmod the padding mode to set
    virtual void setpmod (const t_pmod pmod);

    /// @return the cipher padding mode
    virtual t_pmod getpmod (void) const;

    /// set the cipher block mode
    /// @param cmod the block mode to set
    virtual void setcmod (const t_cmod cmod);

    /// @return the cipher block mode
    virtual t_cmod getcmod (void) const;

  protected:
    /// encode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    virtual void encode (t_byte* bo, const t_byte* bi) =0;
    
    /// decode a block buffer into another one
    /// @param bo the output buffer
    /// @param bi the input buffer
    virtual void decode (t_byte* bo, const t_byte* bi) =0;
    
    /// encode an input buffer into an output buffer
    /// @param ob the output buffer to write
    /// @param ib the input  buffer to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, Buffer& ib) override;

    /// encode an input stream into an output buffer
    /// @param ob the output buffer to write
    /// @param is the input  stream to read
    /// @return the number of processed bytes
    long encode (Buffer& ob, InputStream& is) override;

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
    long decode (Buffer& ob, InputStream& is) override;

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
    ModeCipher (const ModeCipher&) =delete;
    // make the assignment operator private
    ModeCipher& operator = (const ModeCipher&) =delete;

  public:
    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
