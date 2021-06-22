// ---------------------------------------------------------------------------
// - Resolver.hpp                                                            -
// - afnix engine - file path resolver class definition                      -
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

#ifndef  AFNIX_RESOLVER_HPP
#define  AFNIX_RESOLVER_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The Resolver class is a special class that is used to resolve a 
  /// a particular file by name. The resolver maintains a list of path
  /// or librarian object. When a file is requested, the list is search
  /// and an input stream is returned by the resolver.
  /// @author amaury darsch

  class Resolver : public Object {
  public:
    /// get an input stream locally or by the resolver
    static InputStream* tois (const String& name, Resolver* rslv);

    /// get a buffer locally or by the resolver
    static Buffer tobuffer (const String& name, Resolver* rslv);
    
  private:
    /// the list of path
    struct s_rpath* p_rpath;

  public:
    /// create an empty resolver
    Resolver (void);

    /// create a resolver with an initial path
    /// @param path the initial path to add
    Resolver (const String& path);

    /// create a resolver with a set of path
    /// @param paths the resolver paths
    Resolver (const Strvec& paths);

    /// destroy this resolver
    ~Resolver (void);

    /// @return the class name
    String repr (void) const;

    /// add a path to this resolver
    /// @param path the path to add
    void add (const String& path);

    /// add a librarian by resolving
    /// @param name the path to add
    void addlib (const String& name);

    /// @return true if the name is valid
    bool valid (const String& name) const;

    /// @return the start module name
    String getstm (void) const;

    /// @return the resolved full path
    String getpath (const String& name) const;

    /// @return an input stream by name
    InputStream* get (const String& name) const;

    /// @return an input stream by name or throw an exception
    InputStream* lookup (const String& name) const;

    /// @return true if the name is a valid afnix name
    bool isvnam (const String& name) const;

    /// @return the afnix file resolved name
    String tornam (const String& name) const;

    /// @return an input stream by an afnix name
    InputStream* tostream (const String& name) const;

    /// @return an input stream by an afnix name or throw an exception
    InputStream* tolookup (const String& name) const;

    /// @return a generic file as a string
    String tostring (const String& name) const;

  private:
    // make the copy constructor private
    Resolver (const Resolver&) =delete;
    // make the assignment operator private
    Resolver& operator = (const Resolver&) =delete;
    
  public:
    /// create  a new object in a generic way
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
