// ---------------------------------------------------------------------------
// - Lexicon.hpp                                                             -
// - afnix:txt module - lexicon object class definition                      -
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

#ifndef  AFNIX_LEXICON_HPP
#define  AFNIX_LEXICON_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Lexicon class is special container designed to operate like a
  /// string set. The lexicon is organized like a huge trie that operate
  /// in reverse string order. Unlike a classical container, there is no
  /// binding between the name and an object. As a consequence, there is
  /// only search operation that can be done. On the other hand, the class
  /// is designed to operate like an iterable object, thus making the
  /// traversing easy when numerous objects are present.
  /// @author amaury darsch

  class Lexicon : public virtual Object {
  private:
    /// the lexicon length
    long d_llen;
    /// the reverse trie
    struct s_eirt* p_eirt;

  public:
    /// create a default lexicon
    Lexicon (void);
  
    /// destroy the lexicon
    ~Lexicon (void);

    /// return the class name
    String repr (void) const;

    /// reset the lexicon
    void reset (void);

    /// @return true if the lexicon is empty
    bool empty (void) const;

    /// @return the number of entries
    long length (void) const;

    /// @return true if a name exists in the lexicon
    bool exists (const String& name) const;

    /// add a new name in the lexicon
    /// @param name the name to add
    void add (const String& name);

  private:
    // make the copy constructor private
    Lexicon (const Lexicon&);
    // make the assignment operator private
    Lexicon& operator = (const Lexicon&);    

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
