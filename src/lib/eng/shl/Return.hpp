// ---------------------------------------------------------------------------
// - Return.hpp                                                              -
// - afnix engine - return exception class definition                        -
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

#ifndef  AFNIX_RETURN_HPP
#define  AFNIX_RETURN_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Return class is the object used when returning an object during 
  /// execution. The Return class holds an object and is later thrown as an
  /// exception. The afnix engine catch the returned instance.
  /// @author amaury darsch

  class Return : public Object {
  private:
    /// the object to return
    Object* p_object;

  public:
    /// create an empty return object
    Return (void);

    /// create a new return object with an object
    /// @param object the object to return
    Return (Object* object);

    /// copy constructor for this return object
    /// @param that the return to copy
    Return (const Return& that);

    /// destroy this return object
    ~Return (void);

    /// @return the class name
    String repr (void) const;

    /// @return the returned object
    Object* getobj (void) const;

  private:
    // make the assignment operator private
    Return& operator = (const Return&);
  };
}

#endif
