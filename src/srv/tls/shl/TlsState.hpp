// ---------------------------------------------------------------------------
// - TlsState.hpp                                                            -
// - afnix:tls service - tls state class definition                          -
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

#ifndef  AFNIX_TLSSTATE_HPP
#define  AFNIX_TLSSTATE_HPP

#ifndef  AFNIX_TLSSUITE_HPP
#include "TlsSuite.hpp"
#endif

#ifndef  AFNIX_TLSCERTS_HPP
#include "TlsCerts.hpp"
#endif

#ifndef  AFNIX_TLSPARAMS_HPP
#include "TlsParams.hpp"
#endif

#ifndef  AFNIX_CIPHER_HPP
#include "Cipher.hpp"
#endif

namespace afnix {

  /// The TlsState class is the global tls connnection state object. The class
  /// is designed to carry all the information about the tls session, including
  /// the most wanted keys, cipher version, certificate list and so on and so
  /// forth. Since the class also holds the sequence number, it is here that
  /// the record hmac computation is performed although this does not seem
  /// natural.
  /// @author amaury darsch

  class TlsState : public TlsInfos {
  protected:
    /// the server flag
    bool   d_sflg;
    /// the requested major
    t_byte d_rmaj;
    /// the requested minor
    t_byte d_rmin;
    /// the major version
    t_byte d_vmaj;
    /// the minor version
    t_byte d_vmin;
    /// the selected cipher
    t_word d_cifr;
    /// the cipher key size
    long   d_ksiz;
    /// the cipher block size
    long   d_bsiz;
    /// the cipher iv size
    long   d_vsiz;
    /// the hash size
    long   d_hsiz;
    /// the client sequence number
    t_octa d_cnum;
    /// the server sequence number
    t_octa d_snum;    
    /// the client random
    Buffer d_crnd;
    /// the server random
    Buffer d_srnd;
    /// the premaster key
    Buffer d_mkey;
    /// the master secret
    Buffer d_mbuf;
    /// the key expansion
    Buffer d_ebuf;
    // the client mac buffer
    Buffer d_cmac;
    // the server mac buffer
    Buffer d_smac;
    // the client key buffer
    Buffer d_ckey;
    // the server key buffer
    Buffer d_skey;
    // the client iv buffer
    Buffer d_civb;
    // the server iv buffer
    Buffer d_sivb;
    /// the info plist
    Plist  d_info;
    /// the verify message buffer
    Buffer d_hvmb;
    /// the client block cipher
    Cipher* p_cbcf;
    /// the server block cipher
    Cipher* p_sbcf;
    /// the client hmac
    Hmac* p_chmc;
    /// the server hmac
    Hmac* p_shmc;
    /// the tls parameters
    TlsParams d_prms;
    /// the cipher suite
    TlsSuite* p_suit;
    /// the certificate list
    TlsCerts* p_cert;
    /// the extensions vector
    Vector* p_exts;

  public:
    /// create a  default tls state
    TlsState (void);

    /// create a tls state by server flag
    /// @param sflg the server flag
    TlsState (const bool sflg);

    /// create a tls state by server flag and parameters
    /// @param sflg the server flag
    /// @param prms the server flag
    TlsState (const bool sflg, const TlsParams& prms);

    /// copy construct this tls state
    /// @param that the object to copy
    TlsState (const TlsState& that);

    /// destroy this state
    ~TlsState (void);
    
    /// assign a tls state to this one
    /// @param that the state to assign
    TlsState& operator = (const TlsState& that);

    /// @return the class name
    String repr (void) const;

    /// reset this state object
    void reset (void);

    /// @return the info plist
    Plist getinfo (void) const;

    /// add some information to the state info
    /// @param name the info name to add
    /// @param info the info description
    /// @param bval the info value to add
    virtual void addinfo (const Plist& plst);

    /// add some information by name, info and boolean value
    /// @param name the info name to add
    /// @param info the info description
    /// @param bval the info value to add
    virtual void addinfo (const String& name, const String& info, 
			  const bool    bval);

    /// add some information by name, info and boolean value
    /// @param name the info name to add
    /// @param info the info description
    /// @param ival the info value to add
    virtual void addinfo (const String& name, const String& info, 
			  const t_long  ival);

    /// add some information by name, info and boolean value
    /// @param name the info name to add
    /// @param info the info description
    /// @param rval the info value to add
    virtual void addinfo (const String& name, const String& info, 
			  const t_real  rval);

    /// add some infomration by name, info and boolean value
    /// @param name the info name to add
    /// @param info the info description
    /// @param lval the info value to add
    virtual void addinfo (const String&  name, const String& info, 
			  const Literal& lval);

    /// @return the server flag
    virtual bool getsflg (void) const;

    /// @return the tls major requested
    virtual t_byte getrmaj (void) const;

    /// @return the tls minor requested
    virtual t_byte getrmin (void) const;
    
    /// @return the tls major version
    virtual t_byte getvmaj (void) const;

    /// @return the tls minor version
    virtual t_byte getvmin (void) const;

    /// update the tls version at once
    /// @param vmaj the major version
    /// @param vmin the minor version
    virtual void setvers (const t_byte vmaj, const t_byte vmin);

    /// set the selected cipher
    /// @param cifr the cifr code to set
    virtual void setcifr (const t_word cifr);
    
    /// @return the selected cipher
    virtual t_word getcifr (void) const;

    /// @return the cipher key size
    virtual long getksiz (void) const;

    /// @return the cipher block size
    virtual long getbsiz (void) const;
    
    /// @return the cipher iv size
    virtual long getvsiz (void) const;

    /// @return the cipher hash size
    virtual long gethsiz (void) const;

    /// set the cipher suite by cipher code
    /// @param the cifr code to bind
    virtual void setsuit (const t_word cifr);
    
    /// set the cipher suite
    /// @param suit the cipher suite
    virtual void setsuit (TlsSuite* suit);

    /// @return the tls suite
    virtual TlsSuite* getsuit (void) const;

    /// set the extensions vector
    virtual void setexts (Vector* exts);
    
    /// set the selected certificate
    /// @param cert the certificate list
    /// @param ckey the certificate key
    virtual void setcert (const String& cert, const String& ckey);

    /// set tls certificate list
    virtual void setcert (TlsCerts* cert);
    
    /// @return the tls certificate list
    virtual TlsCerts* getcert (void) const;

    /// set the client random
    /// @param crnd the client random buffer
    virtual void setcrnd (const Buffer& crnd);

    /// @return the client random buffer
    virtual Buffer getcrnd (void) const;
    
    /// set the server random
    /// @param srnd the server random buffer
    virtual void setsrnd (const Buffer& srnd);
    
    /// @return the server random buffer
    virtual Buffer getsrnd (void) const;

    /// set the premaster key
    /// @param mkey the premaster key
    virtual void setmkey (const Buffer& srnd);
    
    /// @return the premaster key
    virtual Buffer getmkey (void) const;
    
    /// @return the private exchange key
    virtual Key* getprvk (void) const;

    /// @return the public exchange key
    virtual Key* getpubk (void) const;

    /// set the state master secret
    /// @param mbuf the master secret buffer
    virtual void setmbuf (const Buffer& mbuf);

    /// get the state master secret
    /// @param mbuf the master secret buffer
    virtual Buffer getmbuf (void) const;

    /// set the state expansion block
    /// @param ebuf the expansion block buffer
    virtual void setebuf (const Buffer& ebuf);

    /// get the expansion block buffer
    /// @param ebuf the master secret buffer
    virtual Buffer getebuf (void) const;

    /// @return the client block cipher
    virtual Cipher* getcbcf (void) const;

    /// @return the server block cipher
    virtual Cipher* getsbcf (void) const;
    
    /// @return the client hmac
    virtual Hmac* getchmc (void) const;

    /// @return the server hmac
    virtual Hmac* getshmc (void) const;
    
    /// @return the handshake verify message buffer
    virtual Buffer& gethvmb (void);

    /// @return the handshake verify message buffer
    virtual const Buffer& gethvmb (void) const;

    /// @return a new state hasher
    virtual Hasher* newhash (void) const;
    
    /// @return the client sequence number
    virtual t_octa getcnum (void) const;

    /// @return the server sequence number
    virtual t_octa getsnum (void) const;

    /// get a new client sequence number
    /// @param fflg the force flag
    virtual t_octa newcnum (const bool fflg);
    
    /// get a new server sequence number
    /// @param fflg the force flag
    virtual t_octa newsnum (const bool fflg);
    
    /// change the client cipher spec
    virtual bool chgccs (void);

    /// change the server cipher spec
    virtual bool chgscs (void);

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
