// ---------------------------------------------------------------------------
// - CookieJar.hpp                                                           -
// - afnix:nwg module - cookie jar class definition                          -
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

#ifndef  AFNIX_COOKIEJAR_HPP
#define  AFNIX_COOKIEJAR_HPP

#ifndef  AFNIX_SET_HPP
#include "Set.hpp"
#endif

#ifndef  AFNIX_COOKIE_HPP
#include "Cookie.hpp"
#endif

namespace afnix {

  /// The CookieJar class is a class designed for the collection of cookie
  /// objects. The jar can be filled with cookies or with an original string
  /// which is parsed to form a list of name/value pairs.
  /// @author amaury darsch

  class CookieJar : public Object {
  private:
    // the cookie jar
    Set d_cjar;

  public:
    /// create an empty jar
    /// @param hval the string header value
    CookieJar (void);

    /// create a jar by string
    /// @param sval the string to parse 
    CookieJar (const String& sval);

    /// @return the class name
    String repr (void) const;

    /// reset this cookie jar
    void reset (void);

    /// parse a cookie string value
    /// @param sval the string value to parse
    void parse (const String& sval);

    /// check if a cookie exists by name
    /// @param name the name to check
    bool exists (const String& name) const;

    /// @return the number of cookie in the jar
    long length (void) const;

    /// add a new cookie to this jar
    /// @param cook the cookie to add
    void add (const Cookie& cook);

    /// get a cookie by index
    /// @param cidx the cookie index
    Cookie get (const long cidx) const;

    /// get a cookie by name
    /// @param name the cookie name
    Cookie getcook (const String& name) const;

    /// get a cookie value by name
    /// @param name the cookie name
    String getcval (const String& name) const;

  private:
    // make the copy constructor private
    CookieJar (const CookieJar&);
    // make the assignment operator private
    CookieJar& operator = (const CookieJar&);

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
