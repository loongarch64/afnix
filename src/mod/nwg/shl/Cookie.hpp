// ---------------------------------------------------------------------------
// - Cookie.hpp                                                              -
// - afnix:nwg module - http cookie class definition                         -
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

#ifndef  AFNIX_COOKIE_HPP
#define  AFNIX_COOKIE_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Cookie class is a simple class designed to handle cookie setting
  /// within a http transaction. This implenetation is conformant to RFC 2965.
  /// A cookie is a key/name pair associated with a domain and a maximum age.
  /// A cookie is set by the server and stored by the http client. Further 
  /// connection with the client will result with the cookie value transmitted
  /// by the client to the server. A cookie has various parameters that 
  /// controls its existence and behavior. The most important one is the 
  /// "cookie maximum age" that is defined in delta-seconds. A null value 
  /// tells the client to discard the cookie. A cookie without maximum age is
  /// valid only during the http client session. The default cookie version is
  /// 1 With a version 1, the maximum age is interpreted as the number of 
  /// seconds before the cookie expires. With version 0, the maximum age 
  /// is the absolute time.
  /// @author amaury darsch

  class Cookie : public Object {
  private:
    /// the cookie version
    long   d_vers;
    /// the cookie name
    String d_name;
    /// the value
    String d_cval;
    /// the cookie path
    String d_path;
    /// the cookie domain
    String d_hdom;
    /// the port value
    long   d_port;
    /// the cookie comment
    String d_comt;
    /// the comment url
    String d_curl;    
    /// the maximum age
    t_long d_mage;
    /// the expiration time
    t_long d_etim;
    /// the discard flag
    bool   d_disf;
    /// the secure flag
    bool   d_secf;
    /// the http only flag
    bool   d_hnly;

  public:
    /// create a cookie by header value
    /// @param hval the string header value
    Cookie (const String& hval);

    /// create a default cookie
    /// @param name the cookie name
    /// @param value the cookie value
    Cookie (const String& name, const String& value);

    /// create a default cookie with a maximum age
    /// @param name the cookie name
    /// @param value the cookie value
    /// @param mage the maximum age
    Cookie (const String& name, const String& value, const t_long mage);

    /// copy construct this cookie
    /// @param that the object to copy
    Cookie (const Cookie& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a cookie to this one
    /// @param that the object to assign
    Cookie& operator = (const Cookie& that);

    /// reset this cookie to factory value
    void reset (void);

    /// parse a cookie string header value
    /// @param hval the header value to parse
    void parse (const String& hval);

    /// @return a hhtp cookie string representation
    String tostring (void) const;

    /// set the cookie version
    /// @param vers the cookie version to set
    void setvers (const long vers);

    /// @return the cookie version
    long getvers (void) const;

    /// set the cookie name
    /// @param name the cookie name
    void setname (const String& name);

    /// @return the cookie name
    String getname (void) const;

    /// set the cookie value
    /// @param value the cookie value
    void setvalue (const String& value);

    /// @return the cookie value
    String getvalue (void) const;

    /// set the cookie maximum age
    /// @param mage the cookie maximum age
    void setmage (const t_long mage);

    /// @return the cookie maximum age
    t_long getmage (void) const;

    /// set the cookie expiration time
    /// @param etim the cookie expiration time
    void setetim (const t_long etim);

    /// @return the cookie expiration time
    t_long getetim (void) const;

    /// set the cookie path
    /// @param path the cookie path
    void setpath (const String& path);

    /// @return the cookie path
    String getpath (void) const;

    /// set the cookie domain
    /// @param hdom the cookie http domain
    void setdomain (const String& hdom);

    /// @return the cookie domain
    String getdomain (void) const;

    /// set the cookie port number
    /// @param port the cookie port
    void setport (const long port);

    /// @return the cookie port
    long getport (void) const;

    /// set the cookie comment
    /// @param comt the cookie comment
    void setcomt (const String& comt);

    /// @return the cookie comment
    String getcomt (void) const;

    /// set the cookie comment url
    /// @param curl the cookie comment url
    void setcurl (const String& curl);

    /// @return the cookie comment url
    String getcurl (void) const;

    /// set the cookie discard flag
    /// @param disf the discard flag
    void setdisf (const bool flag);

    /// @return the cookie discard flag
    bool getdisf (void) const;

    /// set the cookie secure flag
    /// @param secf the secure flag
    void setsecf (const bool secf);

    /// @return the cookie secure flag
    bool getsecf (void) const;

    /// set the cookie http only flag
    /// @param hnly the http only flag
    void sethnly (const bool hnly);

    /// @return the cookie http only flag
    bool gethnly (void) const;

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
