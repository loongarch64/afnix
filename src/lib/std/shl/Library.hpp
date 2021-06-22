// ---------------------------------------------------------------------------
// - Library.hpp                                                             -
// - standard object library - dynamic library class definition              -
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

#ifndef  AFNIX_LIBRARY_HPP
#define  AFNIX_LIBRARY_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The Library class is used to manipulate system shared libraries. A 
  /// library instance is created by name, and an attempt to open the shared
  /// library is done. Once the library is open, it is possible to initialize
  /// the library with a evaluable object and an argument vector. When ran
  /// the initialize procedure cannot be run again. The library can also be
  /// registered statically. It it is also possible to mark the library as a
  /// legal alien. In this case, it is assumed that it is a resident library
  /// which cannot be closed. This situation occurs most likely when the 
  /// library has low level global exit handlers. Note that the registered
  /// mutator is not exposed at the interpreter level.
  /// @author amaury darsch

  class Library : public Nameable {
  protected:
    /// the library name
    String d_name;
    /// the legal alien flag
    bool d_aflg;
    /// the registered flag
    bool d_rflg;
    /// the initialized flag
    mutable bool d_iflg;
    /// the library handle
    void* p_hand;

  public:
    /// create a default main library
    Library (void);

    /// create a shared library by name
    /// @param name the library name
    Library (const String& name);

    /// create a registered library by name and handle
    /// @param name the library name
    /// @param hand the library handle
    Library (const String& name, void* hand);

    /// destroy and close this shared library
    ~Library (void);

    /// @return the class name
    String repr (void) const override;

    /// @return the library name
    String getname (void) const override;

    /// set the alien flag - priviledge operation
    /// @param aflg the alien flag
    virtual void setaflg (const bool aflg);

    /// @return the alien flag
    virtual bool getaflg (void) const;

    /// find a symbol by name
    /// @param name the symbol name
    virtual void* find (const String& name) const;

    /// look for a symbol or throw an exception
    /// @param name the symbol name
    virtual void* lookup (const String& name) const;

    /// initialize a dynamic library
    /// @param zobj zobj the current evaluable
    /// @param argv the arguments for initialization
    virtual Object* dlinit (Evaluable* zobj, Vector* argv) const;

  private:
    // make the copy constructor private
    Library (const Library&) =delete;
    // make the assignment operator private
    Library& operator = (const Library&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);
    
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
