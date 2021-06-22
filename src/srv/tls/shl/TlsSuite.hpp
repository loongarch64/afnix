// ---------------------------------------------------------------------------
// - TlsSuite.hpp                                                            -
// - afnix:tls service - tls cipher suite class definition                   -
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

#ifndef  AFNIX_TLSSUITE_HPP
#define  AFNIX_TLSSUITE_HPP

#ifndef  AFNIX_HMAC_HPP
#include "Hmac.hpp"
#endif

#ifndef  AFNIX_CIPHER_HPP
#include "Cipher.hpp"
#endif

#ifndef  AFNIX_TLSINFOS_HPP
#include "TlsInfos.hpp"
#endif

namespace afnix {

  /// The TlsSuite class is the tls supported cipher suite class definition.
  /// The class is designed to store the cipher defnitions as well as the
  /// cipher suite which appears during the handshake process.
  /// @author amaury darsch

  class TlsSuite : public TlsInfos {
  public:
    /// the key exchange suite
    enum t_exch {
      TLS_EXCH_NIL,
      TLS_EXCH_RSA
    };
    /// the cipher suite
    enum t_cifr {
      TLS_CIFR_NIL,
      TLS_CIFR_RC4,
      TLS_CIFR_AES
    };
    /// the cipher block mode
    enum t_cmod {
      TLS_CMOD_NIL,
      TLS_CMOD_CBC,
      TLS_CMOD_EBC,
      TLS_CMOD_GCM
    };
    /// the hash suite
    enum t_hash {
      TLS_HASH_NIL,
      TLS_HASH_MD5,
      TLS_HASH_SHA,
      TLS_HASH_SHA256,
      TLS_HASH_SHA384,
    };
    /// the cipher info structure
    struct s_cnfo {
      /// the cipher name
      const char* p_name;
      /// the tls code
      t_word d_code;
      /// the minimum tls version
      t_word d_vmin;
      /// the maximum tls version
      t_word d_vmax;
      /// the key exchange
      t_exch d_exch;
      /// the cipher algorithm
      t_cifr d_cifr;
      /// the cipher block mode
      t_cmod d_cmod;
      /// the cipher key size
      long   d_ksiz;
      /// the cipher block size
      long   d_bsiz;
      /// the cipher iv size
      long   d_vsiz;
      /// the hash algorithm
      t_hash d_hash;
      /// the hash size
      long   d_hsiz;
      /// the hmac flag
      bool   d_hmac;
      /// the certificate flag
      bool   d_cert;
    };

  private:
    /// the cipher length
    long d_clen;
    /// the cipher list
    s_cnfo* p_clst;
    /// the suite length
    long    d_slen;
    /// the suite list
    t_word* p_slst;

  public:
    /// create a default suite
    TlsSuite (void);

    /// create a suite by version code
    /// @param vcod the version code
    TlsSuite (const t_word vcod);
    
    /// create a suite by version
    /// @param vmaj the major version
    /// @param vmin the minor version
    TlsSuite (const t_byte vmaj, const t_byte vmin);
    
    /// destroy this suite
    ~TlsSuite (void);

    /// @return the class name
    String repr (void) const;

    /// reset this suite
    void reset (void);

    /// @return the suite info as a plist
    Plist getinfo (void) const;

    /// add a new cipher by code
    /// @param code the cipher code
    virtual void add (const t_word code);

    /// add a new cipher by tls code
    /// @param ucod the upper byte
    /// @param lcod the lower byte
    virtual void add (const t_byte ucod, const t_byte lcod);

    /// @return the suite length
    virtual long length (void) const;

    /// get a cifr code by suite index
    /// @param sidx the suite index
    virtual t_word getcifr (const long sidx) const;
    
    /// check if a cipher is valid by code
    /// @param code the the cipher code
    virtual bool valid (const t_word code) const;

    /// check if a cipher is valid by tls code
    /// @param ucod the upper byte
    /// @param lcod the lower byte
    virtual bool valid (const t_byte ucod, const t_byte lcod) const;

    /// locate the first valid cipher code
    virtual t_word locate (void) const;

    /// update the cipher info list by version code
    /// @param vcod the version code
    virtual void update (const t_word vcod);

    /// update the cipher info list by version
    /// @param vmaj the major version
    /// @param vmin the minor version
    virtual void update (const t_byte vmaj, const t_byte vmin);

    /// create the tls suite from the cipher list
    virtual void mksuit (void);
    
    /// get a cipher info by code
    /// @param code the the cipher code
    virtual s_cnfo getcnfo (const t_word code) const;

    /// get a cipher info by tls code
    /// @param ucod the upper byte
    /// @param lcod the lower byte
    virtual s_cnfo getcnfo (const t_byte ucod, const t_byte lcod) const;

    /// get the cipher info as a print table
    virtual PrintTable* getctbl (void) const;

    /// create a hasher by code
    /// @param code the cipher code
    virtual Hasher* gethasher (const t_word code) const;
    
    /// create a cipher by code, key buffer and mode
    /// @param code the cipher code
    /// @param rflg the cipher reverse flag
    /// @param kbuf the cipher key buffer
    /// @param kiv  the cipher initial vector
    virtual Cipher* getcipher (const t_word  code, const bool rflg,
			       const Buffer& kbuf, const Buffer& kiv) const;

    /// create a hmac by code and key buffer
    /// @param code the cipher code
    /// @param kbuf the hmac key buffer
    virtual Hmac* gethmac (const t_word code, const Buffer& kbuf) const;
    
  private:
    // make the copy constructor private
    TlsSuite (const TlsSuite&) =delete;
    // make the assignment operator private
    TlsSuite& operator = (const TlsSuite&) =delete;

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
