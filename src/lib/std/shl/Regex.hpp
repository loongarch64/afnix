// ---------------------------------------------------------------------------
// - Regex.hpp                                                               -
// - standard object library - regex class definition                        -
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

#ifndef  AFNIX_REGEX_HPP
#define  AFNIX_REGEX_HPP

#ifndef  AFNIX_THRMAP_HPP
#include "Thrmap.hpp"
#endif

namespace afnix {

  /// The Regex class is a builtin object used to represent and evaluate
  /// regular expression. A regular expression is constructed with the
  /// a string implicitely or explicitely with operators.
  /// @author amaury darsch

  class Regex : public Literal {
  private:
    /// the regex value
    String d_reval;
    /// the regex structure
    struct s_regex* p_recni;
    /// the group vector in map
    mutable Thrmap d_gmap;

  public:
    /// create a new regex
    Regex (void);

    /// create a new regex from a c-string
    /// @param re the regex description
    Regex (const char* re);

    /// create a new regex from a string
    /// @param re the regex description
    Regex (const String& re);

    /// copy construct a regex
    /// @param that the regex to copy
    Regex (const Regex& that);

    /// destroy this regex
    ~Regex (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this regex
    void clear (void);

    /// @return a literal representation of this regex
    String toliteral (void) const;

    /// @return a string representation of this regex
    String tostring (void) const;

    /// @return the regex did
    t_word getdid (void) const;

    /// @return the regex sid
    t_word getsid (void) const;

    /// serialize this regex to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a regex from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// compile a regex as a string
    /// @param re the regex string
    void compile (const String& re);

    /// assign a regex to this one
    /// @param that the regex to assign
    Regex& operator = (const Regex& that);

    /// assign a new description to this regex
    /// @param that the description to assign
    Regex& operator = (const char* re);

    /// assign a new description to this regex
    /// @param that the description to assign
    Regex& operator = (const String& re);

    /// match a regex against a string
    /// @param s the string to match
    bool operator == (const String& s) const;

    /// reverse match of regex against a string
    /// @param s the string to reverse match
    bool operator != (const String& s) const;

    /// match a regex partially against a string
    /// @param s the string to match
    bool operator < (const String& s) const;

    /// match a regex with an input stream
    /// @param is the input stream used for matching
    String match (InputStream* is) const;

    /// match a regex with an input stream and an initial string
    /// @param is the input stream used for matching
    /// @param ps the prefix string
    String match (InputStream* is, const String& ps) const;

    /// match a regex with a string and return the substring
    /// @param s the string to match
    String match (const String& s) const;

    /// replace a regex match with a string
    /// @param s the string to match
    /// @param val the value to replace
    String replace (const String& s, const String& val) const;

    /// @return the length of the group vector
    long length (void) const;

    /// get a group object at a certain index
    /// @param index the group index
    Object* getobj (const long index) const;

    /// get a group string at a certain index
    /// @param index the group index
    String getstring (const long index) const;

    /// get a group integer at a certain index
    /// @param index the group index
    t_long getlong (const long index) const;

    /// get a group real at a certain index
    /// @param index the group index
    t_real getreal (const long index) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// operate this object with a literal object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object);

    /// set an object to this regex
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object);

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
