// ---------------------------------------------------------------------------
// - Superset.hpp                                                            -
// - afnix engine - superset class definition                                -
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

#ifndef  AFNIX_SUPERSET_HPP
#define  AFNIX_SUPERSET_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_GLOBALSET_HPP
#include "Globalset.hpp"
#endif

namespace afnix {

  /// The Superset class is a global nameset that acts as the root global set.
  /// The super set also maintains a list of global nameset that are created
  /// at the root. This is important as it permits to clear everything 
  /// nicely when the interpreter exits
  /// @author amaury darsch

  class Superset : public Globalset {
  private:
    /// the globalset list
    Vector d_glst;

  public:
    /// create a default superset
    Superset (void);

    /// destroy this superset
    ~Superset (void);

    /// @return the class name
    String repr (void) const;

    /// reset this superset
    virtual void reset (void);

    /// @return the number of nameset
    virtual long length (void) const;

    /// @return a nameset by index
    virtual Nameset* getset (const long index) const;

    /// create a child nameset by quark
    /// @param quark the child nameset quark
    virtual Nameset* mknset (const long quark);

    /// create a child nameset by name
    /// @param name the child nameset name
    virtual Nameset* mknset (const String& name);

  private:
    // make the copy constructor private
    Superset (const Superset&);
    // make the assignment operator private
    Superset& operator = (const Superset&);
  };
}

#endif
