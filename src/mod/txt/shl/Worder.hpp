// ---------------------------------------------------------------------------
// - Worder.hpp                                                              -
// - afnix:txt module - word analyzer object class definition                -
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

#ifndef  AFNIX_WORDER_HPP
#define  AFNIX_WORDER_HPP

#ifndef  AFNIX_REGEX_HPP
#include "Regex.hpp"
#endif

#ifndef  AFNIX_PLIST_HPP
#include "Plist.hpp"
#endif

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

namespace afnix {

  /// The Worder class is a word analyzer framework designed to accumulate
  /// strings in the forms of words and to provide the methods that analyze
  /// the word constituent. Complex search operations based on regex are also
  /// part of this framework.
  /// @author amaury darsch

  class Worder : public Object {
  private:
    /// the words vector
    Strvec  d_words;

  public:
    /// create an empty worder
    Worder (void);
  
    /// copy constructs this worder
    /// @param that the worder to copy
    Worder (const Worder& that);

    /// return the class name
    String repr (void) const;
    
    /// @return a clone of this object
    Object* clone (void) const;

    /// reset the worder
    virtual void reset (void);

    /// @return true if the worder is empty
    virtual bool empty (void) const;

    /// @return the number of words
    virtual long length (void) const;

    /// @return the number of different words
    virtual long uwlen (void) const;

    /// @return true if a word exists in the worder
    virtual bool exists (const String& word) const;

    /// add a new word in the worder
    /// @param word the word to add
    virtual void add (const String& word);

    /// @return a word by index
    virtual String get (const long index) const;

    /// @return true if a word matches the regex
    virtual bool match (const Regex& re) const;

    /// @return true if the words matches the string regex
    virtual bool match (const String& re) const;

    /// @return the number of words that match the regex
    virtual long count (const Regex& re) const;

    /// @return the number of words that match the string regex
    virtual Plist count (const String& re) const;

  private:
    // make the assignment operator private
    Worder& operator = (const Worder&);    

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
