// ---------------------------------------------------------------------------
// - Json.hpp                                                                -
// - afnix:nwg module - json converter class definition                      -
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

#ifndef  AFNIX_JSON_HPP
#define  AFNIX_JSON_HPP

#ifndef  AFNIX_BUFFER_HPP
#include "Buffer.hpp"
#endif

namespace afnix {

  /// The Json class is an object convert that produces a json buffer from an
  /// object. The class supports various object which can be easily converted
  /// to a json representation.
  /// @author amaury darsch

  class Json : public virtual Object {
  protected:
    /// the json buffer
    Buffer d_jbuf;
    
  public:
    /// create a default json
    Json (void);

    /// @return the class name
    String repr (void) const override;

    /// reset this object
    virtual void reset (void);

    /// @return the buffer string value
    virtual String tostring (void) const;
      
    /// stringify an object
    /// @param obj the object to convert
    virtual bool stringify (Object* obj);
    
  private:
    // make the copy constructor private
    Json (const Json&) =delete;
    // make the assignement operator private
    Json& operator = (const Json&) =delete;

  public:
    /// create a new object in a generic way
    /// @param argv the argument vector
    static Object* mknew (Vector* argv);

    /// @return true if the given quark is defined
    bool isquark (const long quark, const bool hflg) const override;

    /// apply this object with a set of arguments and a quark
    /// @param zobj  the current evaluable
    /// @param nset  the current nameset    
    /// @param quark the quark to apply these arguments
    /// @param argv  the arguments to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv) override;
  };
}

#endif
