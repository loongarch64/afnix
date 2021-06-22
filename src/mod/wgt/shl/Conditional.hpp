// ---------------------------------------------------------------------------
// - Conditional.hpp                                                         -
// - afnix:wgt module - conditional widget class definition                  -
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

#ifndef  AFNIX_CONDITIONAL_HPP
#define  AFNIX_CONDITIONAL_HPP

#ifndef  AFNIX_TAGGABLE_HPP
#include "Taggable.hpp"
#endif

namespace afnix {

  /// The Conditional class is a widget class which represents a selectable
  /// condition. A condition is defined by a string operator and one or two
  /// literal arguments. If the condition is active, it can be valuated by
  /// an expressable interface.
  /// @author amaury darsch

  class Conditional : public Taggable {
  protected:
    /// the operator name
    String d_oper;
    /// the literal lhs
    Literal* p_llhs;
    /// the literal rhs
    Literal* p_lrhs;
    /// the active flag
    bool d_actv;

  public:
    /// create a default conditional
    Conditional (void);

    /// create a conditional by name
    /// @param name the conditional name
    Conditional (const String& name);
    
    /// create a conditional by name and info
    /// @param name the conditional name
    /// @param info the conditional info
    Conditional (const String& name, const String& info);

    /// copy construct this conditional
    /// @param that the conditonal to copy
    Conditional (const Conditional& that);

    /// copy move this conditional
    /// @param that the conditional class to move
    Conditional (Conditional&& that) noexcept;
    
    /// destroy this conditional
    ~Conditional (void);

    /// assign a conditional to this one
    /// @param that the conditional to assign
    Conditional& operator = (const Conditional& that);

    /// move a conditional to this one
    /// @param that the conditional to move
    Conditional& operator = (Conditional&& that) noexcept;
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// @return the conditional did
    t_word getdid (void) const;

    /// @return the conditional sid
    t_word getsid (void) const;

    /// serialize this conditional to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a conditional from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// set the conditional activate flag
    /// @param actv the activate flag to set
    virtual void setactv (const bool actv);

    /// @return the activate flag
    virtual bool getactv (void) const;

    /// set the conditional expression
    /// @param oper the operator to set
    /// @param llhs the literal lhs
    virtual void set (const String& oper, Literal* llhs);
    
    /// set the conditional expression
    /// @param oper the operator to set
    /// @param llhs the literal lhs
    /// @param lrhs the literal rhs
    virtual void set (const String& oper, Literal* llhs, Literal* lrhs);

    /// @return the conditional operator
    virtual String getoper (void) const;

    /// set the conditional lhs
    /// @param llhs the literal lhs
    virtual void setllhs (Literal* llhs);
    
    /// @return the conditional lhs
    virtual Literal* getllhs (void) const;

    /// set the conditional rhs
    /// @param lrhs the literal rhs
    virtual void setlrhs (Literal* lrhs);
    
    /// @return the conditional rhs
    virtual Literal* getlrhs (void) const;
    
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
