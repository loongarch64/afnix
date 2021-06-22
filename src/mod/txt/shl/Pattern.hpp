// ---------------------------------------------------------------------------
// - Pattern.hpp                                                             -
// - afnix:txt module - pattern description class definition                 -
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

#ifndef  AFNIX_PATTERN_HPP
#define  AFNIX_PATTERN_HPP

#ifndef  AFNIX_REGEX_HPP
#include "Regex.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif
 
namespace afnix {

  /// The Pattern class is a pattern matching class based either on regular
  /// expression or balanced string. In the regex mode, the pattern is
  /// defined with a regex and a matching is said to occur when a regex
  /// match is achieved. In the balanced string mode, the pattern is defined
  /// with a start pattern and end pattern strings. A name and a tag is also
  /// bound to the pattern object as a mean to ease the integration within
  /// a scanner.

  class Pattern : public virtual Object {
  public:
    /// the pattern mode
    enum t_pmode {
      REGEX = 0, // regex mode
      BMODE = 1, // balanced mode
      RMODE = 2  // recursive balanced mode
    };
      
  private:
    /// the pattern mode
    t_pmode d_mode;
    /// the pattern regex
    Regex   d_regex;
    /// the start balanced string
    String  d_sbs;
    /// the end balanced string
    String  d_ebs;
    /// the element name
    String  d_name;
    /// the element tag
    long    d_rtag;
    /// the escape character
    t_quad  d_escc;

  public:
    /// create an empty  pattern
    Pattern (void);

    /// create a regex pattern by string
    /// @param re the regex description
    Pattern (const String& re);

    /// create a regex pattern by name and string
    /// @param name the pattern name
    /// @param re   the regex description
    Pattern (const String& name, const String& re);

    /// create a regex pattern by name and regex
    /// @param name the pattern name
    /// @param re   the regex element
    Pattern (const String& name, const Regex& re);

    /// create a balanced pattern by name, control string and escape character
    /// @param name the pattern name
    /// @param sbs the starting control string
    /// @param ebs the ending control string
    /// @param escc the escape control character
    Pattern (const String& name, const String& sbs, const String& ebs,
	     const t_quad escc);

    /// create a balanced pattern by name, control strings and escape character
    /// @param name the pattern name
    /// @param cs   the control string
    /// @param escc the escape control character
    Pattern (const String& name, const String& cs, const t_quad escc);

    /// create a balanced pattern by name and control strings
    /// @param name the pattern name
    /// @param sbs the starting control string
    /// @param ebs the ending control string
    Pattern (const String& name, const String& sbs, const String& ebs);

    /// create a balanced pattern by name, control strings and flags
    /// @param name the pattern name
    /// @param sbs the starting control string
    /// @param ebs the ending control string
    /// @param rfl the recursive control flag
    Pattern (const String& name, const String& sbs, const String& ebs,
	     const bool rfl);

    /// copy construct this pattern
    /// @param that the pattern to copy
    Pattern (const Pattern& that);
 
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// assign a pattern to this one
    /// @param that the pattern to assign
    Pattern& operator = (const Pattern& that);

    /// set the pattern name
    /// @param name the name to set
    void setname (const String& name);

    /// @return the pattern name
    String getname (void) const;

    /// set the pattern with a regex string
    /// @param re the regex string
    void setregex (const String& re);

    /// set the pattern with a regex
    /// @param re the regex object
    void setregex (const Regex& re);

    /// set the pattern with the balanced control strings
    /// @param sbs the starting control string
    /// @param ebs the ending control string
    void setbcs (const String& sbs, const String& ebs);

    /// set the escape character
    /// @param escc the escape character
    void setesc (const t_quad escc);

    /// @return the escape character
    t_quad getesc (void) const;

    /// set the element tag
    /// @param rtag the tag to set
    void settag (const long rtag);

    /// @return the element tag
    long gettag (void) const;

    /// check this pattern against a string
    /// @param s the string to check
    bool check (const String& s) const;

    /// match an input stream with this pattern
    /// @param is the input stream used for matching
    String match (InputStream* is) const;

    /// match an input stream with this pattern and a prefix
    /// @param is the input stream used for matching
    /// @param ps the prefix string
    String match (InputStream* is, const String& ps) const;

  public:
    /// evaluate an object  data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
