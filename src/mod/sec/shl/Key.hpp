// ---------------------------------------------------------------------------
// - Key.hpp                                                                 -
// - afnix:sec module - key class definition                                 -
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

#ifndef  AFNIX_KEY_HPP
#define  AFNIX_KEY_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

#ifndef  AFNIX_RELATIF_HPP
#include "Relatif.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Key class is an original class used to store a particular key
  /// or to generate one. A key is designed to operate with a variery of
  /// cipher that can be either symmetric or asymmetric. In the symmetric
  /// case, the key is generally an array of bytes. Asymmetric key are 
  /// generally stored in the form of number list that can be computed or
  /// loaded by value. By default, a random 128 bit symmetric key is created.
  /// @author amaury darsch

  class Key : public Taggable {
  public:
    /// the type of keys
    enum t_ckey : t_byte {
      CKEY_KNIL = 0x00U, // nil key
      CKEY_KSYM = 0x01U, // symmetric key
      CKEY_KRSA = 0x02U, // rsa key
      CKEY_KMAC = 0x03U, // mac key
      CKEY_KDSA = 0x04U  // dsa key
    };

    /// the key index accessor
    enum t_ikey {
      KRSA_PMOD, // rsa modulus
      KRSA_PEXP, // rsa public exponent
      KRSA_SEXP, // rsa secret exponent
      KDSA_PPRM, // dsa prime p
      KDSA_QPRM, // dsa prime q
      KDSA_SKEY, // dsa secret key
      KDSA_PKEY, // dsa public key
      KDSA_PGEN  // dsa public generator
    };

  private:
    /// the cipher key type
    t_ckey  d_type;
    /// the key structure
    union {
      struct s_knil* p_knil;
      struct s_ksym* p_ksym;
      struct s_krsa* p_krsa;
      struct s_kmac* p_kmac;
      struct s_kdsa* p_kdsa;
    };

  public:
    /// create a nil key
    Key (void);

    /// create a key by type
    /// @param type the key type
    Key (const t_ckey type);

    /// create a symetric key by octet string
    /// @param ostr the octet string key
    Key (const String& ostr);

    /// create a symetric key by buffer
    /// @param kbuf the buffer key
    Key (const Buffer& kbuf);

    /// create a key by type and bit size 
    /// @param type the key type
    /// @param bits the key size
    Key (const t_ckey type, const long bits);

    /// create a key by type and buffer
    /// @param type the key type
    /// @param kbuf the key buffer
    Key (const t_ckey type, const Buffer& kbuf);

    /// create a key by type and octet string
    /// @param type the key type
    /// @param ostr the octet string key
    Key (const t_ckey type, const String& ostr);

    /// create a key by type and byte buffer
    /// @param type the key type
    /// @param size the buffer size
    /// @param kval the key value buffer
    Key (const t_ckey type, const long size, const t_byte* kval);

    /// create a key by type and object vector
    /// @param type the key type
    /// @param ovec the object vector
    Key (const t_ckey type, const Vector& ovec);

    /// copy construct this key
    /// @param that the key key to copy
    Key (const Key& that);

    /// destroy this key
    ~Key (void);

    /// assign a key to this one
    /// @param that the key to assign
    Key& operator = (const Key& that);

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

    /// reset this key
    void reset (void);
    
    /// @return true if the key is a block cipher key
    bool isbk (void) const;

    /// @return true if the key is a public cipher key
    bool ispk (void) const;

    /// @return true if the key is a mac key
    bool ismk (void) const;

    /// @return true if the key is a serial cipher key
    bool isck (void) const;

    /// @return true if the key is a signature key
    bool issk (void) const;

    /// @return the key type
    t_ckey gettype (void) const;

    /// @return  the key size in bits
    long getbits (void) const;

    /// @return  the key size in bytes
    long getsize (void) const;

    /// @return a formatted key representation
    String format (void) const;

    /// @return a formatted key representation by type
    String format (const t_ikey type) const;

    /// @return a relatif key by type
    Relatif getrkey (const t_ikey type) const;

    /// @return a key byte by index
    t_byte getbyte (const long index) const;

    /// @return a public key by removing the secret part
    Key topublic (void) const;
    
  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
