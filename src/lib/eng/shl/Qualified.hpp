// ---------------------------------------------------------------------------
// - Qualified.hpp                                                           -
// - afnix engine - qualified name class definition                          -
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

#ifndef  AFNIX_QUALIFIED_HPP
#define  AFNIX_QUALIFIED_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The qualified class is similiar to the lexical one, except that it 
  /// defines an object with a path. Each component of the qualified name is
  /// a lexical name. During construction, the qualified name is checked for
  /// adherence to this rule.
  /// @author amaury darsch

  class Qualified : public Literal {
  private:
    /// the qualified name
    String d_name;
    /// the qualified path
    long*  p_quarks;
    /// the qualified length
    long   d_length;
    /// the line number
    long   d_lnum;

  public:
    /// create an empty qualified
    Qualified (void);

    /// create a new qualified with a name
    /// @param name the qualified name
    Qualified (const String& name);

    /// create a new qualified with a name and a line number
    /// @param name the qualified name
    /// @param line the line number
    Qualified (const String& name, const long lnum);

    /// copy constructor for this qualified
    /// @param that the qualified to copy
    Qualified (const Qualified& that);

    /// destroy this qualified name
    ~Qualified (void);

    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this qualified name
    void clear (void);

    /// @return a literal representation of this qualified
    String toliteral (void) const;

    /// @return a string representation of this qualified
    String tostring (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;
    
    /// serialize this qualified to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a qualified from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// @return the length of the qualified name path
    long length (void) const;

    /// @return a partial lexical name by index
    String getname (const long index) const;

    /// @return the qualified line number
    long getlnum (void) const;
  
  private:
    // make the assignment operator private
    Qualified& operator = (const Qualified&);
    
  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const;

    /// set a constant object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* cdef (Evaluable* zobj, Nameset* nset, Object* object);

    /// set an object to this object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    /// @param object the object to set
    Object* vdef (Evaluable* zobj, Nameset* nset, Object* object);

    /// unreference an object
    /// @param zobj   the current evaluable
    /// @param nset   the current nameset
    Object* udef (Evaluable* zobj, Nameset* nset);

    /// evaluate this object
    /// @param zobj the current evaluable
    /// @param nset the current nameset
    Object* eval (Evaluable* zobj, Nameset* nset);

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
