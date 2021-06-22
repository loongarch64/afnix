// ---------------------------------------------------------------------------
// - Session.hpp                                                             -
// - afnix:csm service - session class definition                            -
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

#ifndef  AFNIX_SESSION_HPP
#define  AFNIX_SESSION_HPP

#ifndef  AFNIX_VISA_HPP
#include "Visa.hpp"
#endif

#ifndef  AFNIX_COOKIE_HPP
#include "Cookie.hpp"
#endif

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Session class is a class that defines a session to be 
  /// associated with a transaction. The session object is designed 
  /// to be persistent so that its data information can be retreived at
  /// any time. A session object has also the particularity to have a 
  /// limited lifetime. A session object is created by name with an
  /// identifier. The session object is designed to hold a variety of
  /// parameters that are suitable for both the authentication and the
  /// session lifetime. A session is primarily defined by name with an
  /// optional information string. The session is generally associated
  /// an authentication visa which contains the session identity. The visa
  /// provides a secure mechanism compatible with a single sign on session.
  /// A session key is automatically generated when the session is created.
  /// Such key is used to generate a session hash id which can be used as a
  /// cookie value. The cookie name is also stored in the session object.
  /// When a cookie is generated, the session hash name is combined with the
  /// session hash id for the cookie production.
  /// @author amaury darsch

  class Session : public Taggable {
  private:
    /// the session key
    String d_skey;
    /// the session hash name
    String d_shnm;
    /// the session hash id
    String d_shid;
    /// the session path
    String d_path;
    /// the session creation time
    t_long d_ctim;
    /// the session modification time
    t_long d_mtim;
    /// the session maximum age
    t_long d_mage;
    /// the session visa
    Visa* p_visa;

  public:
    /// create an empty session
    Session (void);

    /// create a session by name
    /// @param name the session name
    Session (const String& name);

    /// create a session by name and info
    /// @param name the session name
    /// @param info the session info
    Session (const String& name, const String& info);

    /// create a session by name, info and maximum age
    /// @param name the session name
    /// @param info the session info
    /// @param mage the maximum age
    Session (const String& name, const String& info, const t_long mage);

    /// copy construct this object
    /// @param that the object to copy
    Session (const Session& that);

    /// assign an object to this one
    /// @param that the object to assign
    Session& operator = (const Session& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this session
    /// @param os the output stream
    void wrstream (OutputStream& os) const;

    /// deserialize this session
    /// @param is the input stream
    void rdstream (InputStream& os);

    /// @return the session name
    String getname (void) const;

    /// set the session hash id
    /// @param shid the session hash id
    virtual void setshid (const String& shid);

    /// @return the session hash id
    virtual String getshid (void) const;

    /// set the session path
    /// @param path the session path
    virtual void setpath (const String& path);

    /// @return the session path
    virtual String getpath (void) const;

    /// @return the session creation time
    virtual t_long getctim (void) const;

    /// @return the session modification time
    virtual t_long getmtim (void) const;

    /// @return true if a session has expired
    virtual bool isetim (void) const;

    /// set the session expiration time
    /// @param time the session allowed time
    virtual void setetim (const t_long time);

    /// @return the session expiration time
    virtual t_long getetim (void) const;

    /// @return the session remaining time
    virtual t_long getrtim (void) const;

    /// set the session maximum age
    /// @param mage the session age
    virtual void setmage (const t_long mage);

    /// @return the session maximum age
    virtual t_long getmage (void) const;

    /// set the session visa
    /// @param visa the session visa
    virtual void setvisa (Visa* visa);

    /// @return the session visa
    virtual Visa* getvisa (void) const;

    /// bake a cookie by name
    /// @param name the cookie name
    virtual Cookie* getcookie (const String& name);

    /// close a session by cookie name
    /// @param the cookie name
    virtual Cookie* close (void);

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
