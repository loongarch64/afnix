// ---------------------------------------------------------------------------
// - UriQuery.hpp                                                            -
// - afnix:nwg module - uri query class definition                           -
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

#ifndef  AFNIX_URIQUERY_HPP
#define  AFNIX_URIQUERY_HPP

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

namespace afnix {

  /// The UriQuery class is a simple class that parses a uri query string
  /// and build property list. during the parsing process, a special 
  /// transliteration process is done as specified by RFC 3986. This class
  /// is primarily used with CGI scripts. Note that the string to parse
  /// is exactly the one produced by the 'getquery' method of the Uri class.
  /// @author amaury darsch

  class UriQuery : public Plist {
  public:
    /// create an empty uri query
    UriQuery (void);

    /// create a uri query by string
    /// @param query the string to parse
    UriQuery (const String& query);

    /// create a uri query by initial value
    /// @param name the initial parameter name
    /// @param pval the initial parameter value
    UriQuery (const String& name, const String& pval);

    /// @return the class name
    String repr (void) const;

    /// reset the query
    void reset (void);

    /// parse a uri query by name
    /// @param query the query to parse
    void parse (const String& query);

    /// @return the query string
    String getquery (void) const;

    /// add a path as a prefix
    /// @param path the prefix path to add
    String prefix (const String& path) const;

  private:
    // make the copy constructor private
    UriQuery (const UriQuery&);
    // make the assignment operator private
    UriQuery& operator = (const UriQuery&);

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
