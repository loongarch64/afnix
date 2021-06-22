// ---------------------------------------------------------------------------
// - Gcm.hpp                                                                 -
// - afnix:sec module - galois counter mode cipher class definition          -
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

#ifndef  AFNIX_GCM_HPP
#define  AFNIX_GCM_HPP

#ifndef  AFNIX_AEADCIPHER_HPP
#include "AeadCipher.hpp"
#endif

namespace afnix {

  /// The Gcm class is a specialized class which implements the Galois Counter
  /// Mode for an approved block cipher as specified by NIST 800-38D. The class
  /// is designed to be use in a standalone mode or with the general framework
  /// of the cipher object. For this class, the Galois field is directly
  /// created with the appropriate polynomial.
  /// @author amaury darsch

  class Gcm : public AeadCipher {
  private:
    /// the extended iv
    Buffer d_eiv;
    /// the block cipher
    Cipher* p_cifr;
    /// the gcm structure
    struct s_gcm* p_xgcm;
    
  public:
    /// create a default gcm
    Gcm (void);

    /// create a gcm with a cipher
    /// @param cifr the cipher to set
    Gcm (Cipher* cifr);

    /// create a gcm with a cipher and flag
    /// @param cifr the cipher to set
    /// @param rflg the reverse flag
    Gcm (Cipher* cifr, const bool rflg);

    /// destroy this gcm
    ~Gcm (void);
    
    /// @return the class name
    String repr (void) const override;

    /// reset the gcm
    void reset (void) override;

    /// clear the gcm
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

    /// add an explicit initial vector
    /// @param evs the explicit iv buffer
    void addiv (const String& evs) override;

    /// add an explicit initial vector
    /// @param eiv the explicit iv buffer
    void addiv (const Buffer& eiv) override;
    
    /// set the authentication data by octet string
    /// @param auth the authentication data string
    void setauth (const String& auth) override;

    /// set the authentication data by buffer
    /// @param kiv the authentication data buffer
    void setauth (const Buffer& auth) override;

    /// set the gcm cipher
    /// @param cifr the cipher to set
    virtual void setcifr (Cipher* cifr);

  protected:
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
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
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
