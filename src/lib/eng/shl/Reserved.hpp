// ---------------------------------------------------------------------------
// - Reserved.hpp                                                            -
// - afnix engine - reserved name class definition                           -
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

#ifndef  AFNIX_RESERVED_HPP
#define  AFNIX_RESERVED_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Reserved class is similar to the Lexical class, except that it is
  /// used for reserved keywords. This means that the object can cache the 
  /// associated symbol once it has been evaluated.
  /// @author amaury darsch

  class Reserved : public Literal {
  private:
    /// the reserved quark
    long   d_quark;
    /// the reserved name
    String d_name;
    /// the cached object
    Object* p_object;
    /// the line number
    long d_lnum;

  public:
    /// create an empty reserved name
    Reserved (void);

    /// create a new reserved keyword with a name and a line number
    /// @param name the reserved name
    /// @param line the line number
    Reserved (const String& name, const long lnum);

    /// copy construct this reserved keyword
    /// @param that the reserved keyword to copy
    Reserved (const Reserved& that);

    /// destroy this reserved keyword
    ~Reserved (void);

    /// @return the class name
    String repr (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this reserved to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a reserved from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this reserved name
    void clear (void);

    /// @return a literal representation
    String toliteral (void) const;

    /// @return a string representation
    String tostring (void) const;

    /// @return the reserved name quark
    long toquark (void) const;

    /// @return the reserved cached object
    Object* getobj (void) const;

    /// @return the reserved name line number
    long getlnum (void) const;
    
  private:
    // make the assignment operator private
    Reserved& operator = (const Reserved&);

  public:
    /// evaluate this object
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    Object* eval (Evaluable* zobj, Nameset* nset);
  };
}

#endif
