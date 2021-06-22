// ---------------------------------------------------------------------------
// - Pki.hpp                                                                 -
// - afnix:tls service - public key infrastructure class definition          -
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

#ifndef  AFNIX_PKI_HPP
#define  AFNIX_PKI_HPP

#ifndef  AFNIX_PEM_HPP
#include "Pem.hpp"
#endif

namespace afnix {

  /// The Pki class is a base class for the public key cryptographic
  /// standard. The class defines the basic encoding format (DER/PEM)
  /// which is supported for reading or writing streams.
  /// @author amaury darsch
  
  class Pki : public virtual Object {
  public:
    /// the encoding format
    enum t_efmt {
      EFMT_XPEM, // pem encoding / base64
      EFMT_XDER  // asn distinguished
    };

    /// map a string to an encoding type
    /// @param sfmt the string format
    static t_efmt toefmt (const String& sfmt);

    /// map an encoding type to a string
    /// @param efmt the encoding to map
    static String tostring (const t_efmt efmt);

    /// map a file to a buffer
    /// @param path the path to map
    /// @param pemc the pem encoding
    static Buffer* tobuffer (const String& path, const Pem::t_pemc pemc);
    
  protected:
    // the pki type
    Pem::t_pemc d_pemc;
    /// the pki buffer
    Buffer d_xbuf;
    
  public:
    /// create a default pki
    Pki (void);

    /// create a pki by type
    /// @param pemc the pki pem type
    Pki (const Pem::t_pemc pemc);

    /// copy construct this pki
    /// @param the pki to copy
    Pki (const Pki& that);

    /// assign a pki to this one
    /// @param that the pki to assign
    Pki& operator = (const Pki& that);
    
    /// reset this pki object
    virtual void reset (void);

    /// read an object by path
    /// @param path the pki path
    virtual void read (const String& path);

    /// read an object by stream and format
    /// @param is the input stream
    /// @param efmt the encoding format
    virtual void read (InputStream* is, const t_efmt efmt);

    /// write an object by path
    /// @param os the output stream
    virtual void write (const String& path);

    /// write an object by stream and format
    /// @param os the output stream
    /// @param efmt the encoding format
    virtual void write (OutputStream* os, const t_efmt efmt);

  protected:
    /// encode a pki buffer
    virtual bool encode (void) =0;

    /// decode a pki buffer
    virtual bool decode (void) =0;
    
  public:
    /// evaluate an object data member
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);
    
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
