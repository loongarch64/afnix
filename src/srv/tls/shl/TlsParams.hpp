// ---------------------------------------------------------------------------
// - TlsParams.hpp                                                           -
// - afnix:tls service - tls parameters class definition                     -
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

#ifndef  AFNIX_TLSPARAMS_HPP
#define  AFNIX_TLSPARAMS_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_TLSINFOS_HPP
#include "TlsInfos.hpp"
#endif

#ifndef  AFNIX_SOCKPARAMS_HPP
#include "SockParams.hpp"
#endif

namespace afnix {

  /// The TlsParams class is a container class designed for the storage
  /// and management of the tls parameters. The class is design to operate
  /// with a plist to ease the storage and display and these parameters.
  /// @author amaury darsch

  class TlsParams : public TlsInfos {
  private:
    /// the major version
    t_byte d_vmaj;
    /// the minor version
    t_byte d_vmin;
    /// the certificate list
    String d_cert;
    /// the certificate key
    String d_ckey;
    /// the host name
    String d_host;
    /// the host port
    long d_port;
    /// the debug extension flag
    bool d_dext;
    
  public:
    /// create a default tls parameters
    TlsParams (void);

    /// create a tls parameters by host and port
    /// @param host the tls host
    /// @param port the tls port
    TlsParams (const String& host,const t_word port);

    /// copy coonstruct a tls parameters
    /// @param that the object to copy
    TlsParams (const TlsParams& that);

    /// assign a tls parameters to this one
    /// @param that the object to copy
    TlsParams& operator = (const TlsParams& that);

    /// @return the class nameq
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// reset this parameter object
    void reset (void);

    /// @return the parameters as a plist
    Plist getinfo (void) const;

    /// @return the tls major version
    virtual t_byte getvmaj (void) const;

    /// @return the tls minor version
    virtual t_byte getvmin (void) const;
    
    /// set the tls version at once
    /// @param vmaj the major version
    /// @param vmin the minor version
    virtual void setvers (const t_byte vmaj, const t_byte vmin);

    /// set the tls certificate list
    /// @param cert the certificate list
    virtual void setcert (const String& cert);

    /// @return the tls certificate list
    virtual String getcert (void) const;
    
    /// set the tls certificate key
    /// @param ckey the certificate key
    virtual void setckey (const String& ckey);

    /// @return the tls certificate key
    virtual String getckey (void) const;
    
    /// set the tls certificate list and key
    /// @param cert the certificate list
    /// @param ckey the certificate key
    virtual void setcert (const String& cert, const String& ckey);

    /// set the tls host name
    /// @param host the host name
    virtual void sethost (const String& host);
    
    /// @return the tls host name
    virtual String gethost (void) const;

    /// set the tls host port
    /// @param host the host port
    virtual void setport (const long port);
    
    /// @return the tls host port
    virtual long getport (void) const;

    /// set the debug extension flag
    /// @param dext the debg flag
    virtual void setdext (const bool dext);
    
    /// @return the debug extension flag
    virtual bool getdext (void) const;
    
    /// @return the server socket parameters
    virtual SockParams tosprms (void) const;
    
    /// @return the client socket parameters
    virtual SockParams tocprms (void) const;
    
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
