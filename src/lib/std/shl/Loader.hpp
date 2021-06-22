// ---------------------------------------------------------------------------
// - Loader.hpp                                                              -
// - standard object library - library loader class definition               -
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

#ifndef  AFNIX_LOADER_HPP
#define  AFNIX_LOADER_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_LIBRARY_HPP
#include "Library.hpp"
#endif

namespace afnix {

  /// The Loader class is a library loader. The loader keep a list of 
  /// loaded libraries and manage them by name. When requested, the library
  /// object is returned. when a library is created, the library is opened
  /// and the initial entry point is called.
  /// @author amaury darsch

  class Loader : public Object {
  protected:
    /// the library list
    Vector d_llib;

  public:
    /// create a default loader
    Loader (void);

    /// @return the class name
    String repr (void) const override;

    /// add a library by name
    /// @param name the library name to add
    virtual void add (const String& name);

    /// add a registered library by name with a handle
    /// @param name the library name to add
    /// @param hand the library handle
    virtual void add (const String& name, void* hand);

    /// add a library in the list
    /// @param lib the library to add
    virtual void add (Library* lib);

    /// @return the number of libraries
    virtual long length (void) const;

    /// @return true if a library exists by name
    virtual bool exists (const String& name) const;

    /// @return a library by index
    virtual Library* get (const long index) const;

    /// @return a library by name
    Library* lookup (const String& name) const;

  private:
    // make the copy constructor private
    Loader (const Loader&) =delete;
    // make the assignment operator private
    Loader& operator = (const Loader&) =delete;

  public:
    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
