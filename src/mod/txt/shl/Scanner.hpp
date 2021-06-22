// ---------------------------------------------------------------------------
// - Scanner.hpp                                                             -
// - afnix:txt module - scanner class definition                             -
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

#ifndef  AFNIX_SCANNER_HPP
#define  AFNIX_SCANNER_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_LEXEME_HPP
#include "Lexeme.hpp"
#endif

#ifndef  AFNIX_PATTERN_HPP
#include "Pattern.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif
 
namespace afnix {
  
  /// The Scanner class is a complex class that operates on an input stream
  /// and permits to match one or several patterns. The scanner is built 
  /// by adding patterns to the scanner object. With an input stream, the 
  /// scanner object attempts to build a buffer that match at least one 
  /// pattern. When such matching occurs, a lexeme is built. When building 
  /// a lexeme, the pattern tag is used to mark the lexeme.
  /// @author amaury darsch

  class Scanner : public Object {
  private:
    /// the pattern vector
    Vector  d_vpat;
    /// the minimum mode flag
    bool    d_mmin;

  public:
    /// create a default scanner
    Scanner (void);

    /// @return the class name
    String repr (void) const;

    /// @return the scanner length
    long length (void) const;

    /// add a new pattern to the scanner 
    /// @param pat the pattern to add
    void add (Pattern* pat);

    /// @return a pattern by index
    Pattern* get (const long index) const;

    /// check a string and return a lexeme
    /// @param s the string to check
    Lexeme* check (const String& s) const;

    /// scan an input stream and return a lexeme
    /// @param is the input stream to scan
    Lexeme* scan (InputStream& is) const;

  private:
    // make the copy constructor private
    Scanner (const Scanner&);
    // make the assignement operator private
    Scanner& operator = (const Scanner&);   

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
