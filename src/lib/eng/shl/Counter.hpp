// ---------------------------------------------------------------------------
// - Counter.hpp                                                             -
// - afnix engine - counter class definition                                 -
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

#ifndef  AFNIX_COUNTER_HPP
#define  AFNIX_COUNTER_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Counter class is a mechanism designed to count integer both upward
  /// or downward. The counter is initialized with a start and end value.
  /// With a single value, the start value is set to 0 and the counter 
  /// direction determined by the end value. The counter object is also
  /// a literal object, meaning that it can be directly printed. The
  /// object is also designed to be used directly in a loop.  
  /// @author amaury darsch

  class Counter : public Literal {
  private:
    /// the start value
    t_long d_sval;
    /// the end value
    t_long d_eval;
    /// the counter direction
    bool   d_cdir;
    /// the counter value
    t_long d_cval;

  public:
    /// create a default counter
    Counter (void);

    /// create a counter by value
    /// @param cval the start value
    Counter (const t_long cval);

    /// create a counter by start and end values
    /// @param sval the start value
    /// @param eval the end value
    Counter (const t_long sval, const t_long eval);

    /// copy construct this counter
    /// @param that the counter to copy
    Counter (const Counter& that);

    /// assign a counter to this one
    /// @param that the counter to assign
    Counter& operator = (const Counter& that);

    /// preincrement the counter
    t_long operator ++ (void);

    /// preincrement the counter
    t_long operator ++ (int);
    
    /// @return the class name
    String repr (void) const;

    /// @return a clone of this object
    Object* clone (void) const;

    /// clear this counter
    void clear (void);

    /// @return a literal representation of this counter
    String toliteral (void) const;

    /// @return a string representation of this counter
    String tostring (void) const;

    /// @return the serial did
    t_word getdid (void) const;

    /// @return the serial sid
    t_word getsid (void) const;

    /// serialize this counter to an output stream
    /// @param os the output stream to write
    void wrstream (class OutputStream& os) const;

    /// deserialize a counter from an input stream
    /// @param is the input steam to read in
    void rdstream (class InputStream& is);

    /// reset this counter
    void reset (void);

    /// @return true if the counter is valid
    bool valid (void) const;

    /// step the counter by one position
    /// @return true if the counter was running
    bool step (void);

    /// @return the counter value
    t_long get (void) const;

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
