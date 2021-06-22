// ---------------------------------------------------------------------------
// - String.hpp                                                              -
// - standard object library - string class definition                       -
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

#ifndef  AFNIX_STRING_HPP
#define  AFNIX_STRING_HPP

#ifndef  AFNIX_LITERAL_HPP
#include "Literal.hpp"
#endif

namespace afnix {

  /// The String class is a literal class used to manipulate c-strings.
  /// The basic constructors are provided to operate with a c-string or 
  /// a regular string. Most of the standard operators are also implemented
  /// but care should be taken when using them with literals.
  /// @author amaury darsch

  class String : public Literal {
  public:
    /// intern a string and return a quark
    /// @param name the name to intern
    static long intern (const String& name);

    /// map a string with a quark
    /// @param quark the quark to map
    static String qmap (const long quark);

    /// compute the hashid of a quad array
    /// @param s the quad array to hash
    static long hashid (const t_quad* s);

    /// case insensitive string comparision
    /// @param s1 the string argument
    /// @param s2 the string argument
    static bool strcic (const String& s1, const String& s2);

  private:
    /// the string buffer
    t_quad* p_sval;
    /// the normal flag
    bool d_nrmf;

  public:
    /// create an empty string
    String (void);

    /// create a string from a single character
    /// @param c the character to map to a string
    String (const char c);

    /// create a string from a single unicode character
    /// @param c the character to map to a string
    String (const t_quad c);

    /// create a string from a c-string
    /// @param s the c-string to copy
    String (const char* s);

    /// create a string from a unicode string
    /// @param s the unicode string to copy
    String (const t_quad* s);

    /// copy construct this string
    /// @param that the string to copy
    String (const String& that);

    /// copy move this string
    /// @param that the string to copy
    String (String&& that) noexcept;

    /// destroy this string
    ~String (void);

    /// assign a string to this one
    /// @param that the string to assign
    /// @return the current string
    String& operator = (const String& that);

    /// move a string to this one
    /// @param that the string to assign
    /// @return the current string
    String& operator = (String&& that) noexcept;

    /// assign a c-string to this string
    /// @param s the string to assign
    /// @return the current string
    String& operator = (const char* s);

    /// assign a character to this string
    /// @param  c the character to assign
    /// @return the current string
    String& operator = (const char c);

    /// assign an unicode string to this string
    /// @param s the string to assign
    /// @return the current string
    String& operator = (const t_quad* s);

    /// assign a unicode character to this string
    /// @param  c the character to assign
    /// @return the current string
    String& operator = (const t_quad c);

    /// get a unicode character at a certain position
    /// @param  pos the character position
    /// @return the character at this position
    t_quad operator [] (const long index) const;

    /// concatenate the string with another one
    /// @param  s the string to add to the current one
    /// @return a newly created string
    String operator + (const String& s) const;

    /// concatenate the string with a character
    /// @param  c the character to add to this string
    /// @return a newly created string
    String operator + (const char c) const;

    /// concatenate the string with a unicode character
    /// @param  c the character to add to this string
    /// @return a newly created string
    String operator + (const t_quad c) const;

    /// concatenate the string with an integer
    /// @param value the integer to add to the string
    /// @return a newly created string
    String operator + (const long value) const;

    /// concatenate the string with a long integer
    /// @param value the integer to add to the string
    /// @return a newly created string
    String operator + (const t_long value) const;

    /// add a string to this one
    /// @param s the string to add
    String& operator += (const String& s);

    /// add a character to this one
    /// @param c the character to add
    String& operator += (const char c);

    /// add a unicode character to this one
    /// @param c the character to add
    String& operator += (const t_quad c);

    /// add an integer to this one
    /// @param value the integer to add
    String& operator += (const long value);

    /// add a long integer to this one
    /// @param value the integer to add
    String& operator += (const t_long value);

    /// compare this string with another one
    /// @param s the string to compare with
    /// @return true if the string are equal
    bool operator == (const String& s) const;

    /// compare this string with a c-string
    /// @param s the string to compare with
    /// @return true if the string are equal
    bool operator == (const char* s) const;

    /// compare this string with a unicode string
    /// @param s the string to compare with
    /// @return true if the string are equal
    bool operator == (const t_quad* s) const;

    /// compare this string with another one
    /// @param s the string to compare with
    /// @return true if the string are equal
    bool operator != (const String& s) const;

    /// compare this string with a c-string
    /// @param s the string to compare with
    /// @return true if the string are equal
    bool operator != (const char* s) const;

    /// compare this string with a unicode string
    /// @param s the string to compare with
    /// @return true if the string are equal
    bool operator != (const t_quad* s) const;

    /// compare this string with another one
    /// @param s the string to compare with
    /// @return true if the string is less than the other
    bool operator < (const String& s) const;

    /// compare this string with a c-string
    /// @param s the string to compare with
    /// @return true if the string is less than the other
    bool operator < (const char* s) const;

    /// compare this string with a unicode string
    /// @param s the string to compare with
    /// @return true if the string is less than the other
    bool operator < (const t_quad* s) const;

    /// compare this string with another one
    /// @param s the string to compare with
    /// @return true if the string is less equal than the other
    bool operator <= (const String& s) const;

    /// compare this string with a c-string
    /// @param s the string to compare with
    /// @return true if the string is less equal than the other
    bool operator <= (const char* s) const;

    /// compare this string with a unicode string
    /// @param s the string to compare with
    /// @return true if the string is less equal than the other
    bool operator <= (const t_quad* s) const;

    /// compare this string with another one
    /// @param s the string to compare with
    /// @return true if the string is greater than the other
    bool operator > (const String& s) const;

    /// compare this string with a c-string
    /// @param s the string to compare with
    /// @return true if the string is greater than the other
    bool operator > (const char* s) const;

    /// compare this string with a unicode string
    /// @param s the string to compare with
    /// @return true if the string is greater than the other
    bool operator > (const t_quad* s) const;

    /// compare this string with another one
    /// @param s the string to compare with
    /// @return true if the string is greater equal than the other
    bool operator >= (const String& s) const;

    /// compare this string with a c-string
    /// @param s the string to compare with
    /// @return true if the string is greater equal than the other
    bool operator >= (const char* s) const;

    /// compare this string with a unicode string
    /// @param s the string to compare with
    /// @return true if the string is greater equal than the other
    bool operator >= (const t_quad* s) const;

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    // clear this string
    void clear (void);

    /// @return a literal representation of this string
    String toliteral (void) const;

    /// @return a printable representation of this string
    String tostring (void) const;

    /// @return a styled string of this string
    String format (const Style& lstl) const;
    
    /// @return the string did
    t_word getdid (void) const;

    /// @return the string sid
    t_word getsid (void) const;

    /// serialize this string to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a string from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// @return a quark by interning the string
    long toquark (void) const;

    /// @return true if the string is nil
    bool isnil (void) const;

    /// @return true if the string is eos
    bool iseos (void) const;

    /// @return the length of this string
    long length (void) const;

    /// @return the non combining length of this string
    long ncclen (void) const;

    /// compare a string with a unicode array in normal form
    /// @param s the unicode array in normal form
    bool equals (const t_quad* s) const;

    /// @return an ascii character representation of this string or nil.
    char* tochar (void) const;

    /// @return an unicode character representation of this string or nil.
    t_quad* toquad (void) const;

    /// @return the first character
    t_quad first (void) const;

    /// @return the last character
    t_quad last (void) const;

    /// remove leading blanks and tabs from this string
    /// @return a new clean string
    String stripl (void) const;

    /// strip left a string with a separator specification
    /// @param sep the separator specification
    String stripl (const String& sep) const;

    /// remove trailing blanks and tabs from this string
    /// @return a new clean string
    String stripr (void) const;

    /// strip right a string with a separator specification
    /// @param sep the separator specification
    String stripr (const String& sep) const;

    /// remove leading and trailing blanks and tabs from this string
    /// @return a new clean string
    String strip (void) const;

    /// strip a string with a separator specification
    /// @param sep the separator specification
    String strip (const String& sep) const;

    /// reduce the string into a small form
    String redex (void) const;

    /// remove the quote from the string
    String rmquote (void) const;

    /// @return a string in normal decomposition form
    String tonfd (void) const;

    /// @return a string converter to upper case
    String toupper (void) const;

    /// @return a string converter to lower case
    String tolower (void) const;

    /// return the right substring starting at index
    /// @param index the start index
    /// @return the extracted substring
    String rsubstr (const long index) const;

    /// return the left substring upto (but not including) the index
    /// @param index the end index
    /// @return the extracted substring
    String lsubstr (const long index) const;

    /// extract a sub string by index
    /// @param lidx the left index
    /// @param ridx the right index
    String substr (const long lidx, const long ridx) const;

    /// @return the hashid for this string
    long hashid (void) const;

    /// case insensitive string comparision
    /// @param s the string to compare
    bool strcic (const String& s) const;

    /// fill with string on the left with a character for a given size
    /// @param c    the fill character
    /// @param size the target size
    String lfill (const char c, const long size) const;

    /// fill with string on the left with a character for a given size
    /// @param c    the fill character
    /// @param size the target size
    String lfill (const t_quad c, const long size) const;

    /// fill with string on the rigth with a character for a given size
    /// @param c    the fill character
    /// @param size the target size
    String rfill (const char c, const long size) const;

    /// fill with string on the rigth with a character for a given size
    /// @param c    the fill character
    /// @param size the target size
    String rfill (const t_quad c, const long size) const;

    /// split a string with blank and tab
    Vector* split (void) const;

    /// split a string based on a split argument
    /// @param sbrk the split string
    Vector* split (const String& sbrk) const;

    /// extract a string between a control character
    /// @param cbrk the break character
    Vector* extract (const t_quad cbrk) const;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;
    
    /// operate this object with another object
    /// @param type   the operator type
    /// @param object the operand object
    Object* oper (t_oper type, Object* object);

    /// set an object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object);

    /// apply this object with a set of arguments by quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
		   Vector* argv);
  };
}

#endif
