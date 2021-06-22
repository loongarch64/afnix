// ---------------------------------------------------------------------------
// - Constant.hpp                                                            -
// - afnix engine - constant object class definition                         -
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

#ifndef  AFNIX_CONSTANT_HPP
#define  AFNIX_CONSTANT_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Constant class is a special object which holds a literal object
  /// and that evaluates to a brand new object. The evaluation is done by
  /// cloning the literal object. A constant object is built by the lexical
  /// analyzer with a literal value.
  /// @author amaury darsch

  class Constant : public Literal {
  private:
    /// the literal object
    Literal* p_lobj;

  public:
    /// create an empty constant
    Constant (void);

    /// create a new constant
    /// @param lobj the literal object
    Constant (Literal* lobj);

    /// copy construct this constant
    /// @param that the constant to copy
    Constant (const Constant& that);

    /// destroy this constant
    ~Constant (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this constant
    void clear (void);

    /// @return a literal representation of this constant
    String toliteral (void) const;

    /// @return a string representation of this constant
    String tostring (void) const;

    /// @return the serial did
    t_word getdid (void) const;
    
    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this lexical to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a lexical from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// reduce this object
    Object* reduce (void) const;
    
  private:
    // make the assignment operator private
    Constant& operator = (const Constant&) =delete;

  public:
    /// evaluate this object in the current nameset
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    Object* eval (Evaluable* zobj, Nameset* nset);
  };
}

#endif
