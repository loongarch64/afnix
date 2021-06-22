// ---------------------------------------------------------------------------
// - TlsExtension.hpp                                                        -
// - afnix:tls service - tls extension class definition                      -
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

#ifndef  AFNIX_TLSEXTENSION_HPP
#define  AFNIX_TLSEXTENSION_HPP

#ifndef  AFNIX_CRYPTO_HPP
#include "Crypto.hpp"
#endif

#ifndef  AFNIX_TLSINFOS_HPP
#include "TlsInfos.hpp"
#endif

namespace afnix {

  /// The TlsExtension class is the tls supported extension class definition.
  /// An extension is an opaque structure defined by a type.
  /// @author amaury darsch

  class TlsExtension : public TlsInfos {
  public:
    /// tls extension code 13
    static const t_word TLS_EXTN_0013 = 0x0013U;
    
    /// create a vector of extension by date
    /// @param size the extension array size
    /// @param data the extension array data
    static Vector* toexts (const long size, const t_byte* data);

    /// decode an extension in a vector by code
    /// @param exts the extension vector
    /// @param code the extension code
    static struct s_extn* decode (Vector* exts, const t_word code);
    
  protected:
    /// the extension code
    t_word d_code;
    /// the extension size
    long d_size;
    /// the extension data
    t_byte* p_data;

  public:
    /// create a null extension
    TlsExtension (void);

    /// create a extension by code and data
    /// @param code the extension code
    /// @param size the extension size
    /// @param data the etxension data
    TlsExtension (const t_word code, const long size, const t_byte* data);

    /// destroy this extension
    ~TlsExtension (void);

    /// @return the class name
    String repr (void) const;

    /// reset this extension
    void reset (void);

    /// @return the extension info as a plist
    Plist getinfo (void) const;

    /// @return the extension code
    virtual t_word getcode (void) const;
    
    /// @return the extension buffer
    virtual Buffer* getebuf (void) const;

    /// decode an extension
    virtual struct s_extn* decode (void) const;
    
  private:
    // make the copy constructor private
    TlsExtension (const TlsExtension&) =delete;
    // make the assignment operator private
    TlsExtension& operator = (const TlsExtension&) =delete;

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
