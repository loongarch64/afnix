// ---------------------------------------------------------------------------
// - UriPath.hpp                                                             -
// - afnix:nwg module - uri path mapping class definition                    -
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

#ifndef  AFNIX_URIPATH_HPP
#define  AFNIX_URIPATH_HPP

#ifndef  AFNIX_URI_HPP
#include "Uri.hpp"
#endif

namespace afnix {

  /// The UriPath is a class designed for the management of file system path
  /// associated with a uri. Typically, this class will be used with a http
  /// server or client when an association between a uri and a file name
  /// needs to be made. The general operation principle is to associate a 
  /// path with a uri authority. The uri path is then concatanated to produce
  /// a new path. If the uri path is empty, it can be eventually replaced by
  /// a file name, known as the diretory index in the http terminology.
  /// @author amaury darsch

  class UriPath : public Object {
  private:
    /// the root path
    String d_root;
    /// the directory index
    String d_didx;
    /// the uri authority
    String d_auth;
    
  public:
    /// create an empty uri path
    UriPath (void);

    /// create a uri path with a root path
    /// @param root the root path
    UriPath (const String& root);

    /// create a uri path by root path and index
    /// @param root the root path
    /// @param didx the directory index
    UriPath (const String& root, const String& didx);

    /// create a uri path by root path, index and authority
    /// @param root the root path
    /// @param didx the directory index
    /// @param auth the uri authority
    UriPath (const String& root, const String& didx, const String& auth);

    /// copy construct this uri path
    /// @param that the object to copy
    UriPath (const UriPath& that);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a uri path to this one
    /// @param that the object to assign
    UriPath& operator = (const UriPath& that);

    /// @return the uri path root
    String getroot (void) const;

    /// @return the uri path index
    String getdidx (void) const;

    /// @return the uri path authority
    String getauth (void) const;

    /// map a request uri to a path
    /// @param ruri the request uri to map
    String mapruri (const String& ruri) const;

    /// normalize a path to an absolute path
    /// @param path the path to normalize
    String normalize (const String& path) const;

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
