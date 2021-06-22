// ---------------------------------------------------------------------------
// - NameFifo.hpp                                                          -
// - afnix:sio module - fifo stream class definition                         -
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

#ifndef  AFNIX_NAMEDFIFO_HPP
#define  AFNIX_NAMEDFIFO_HPP

#ifndef  AFNIX_STRFIFO_HPP
#include "Strfifo.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The NamedFifo class is a string vector designed to operate as a stream
  /// fifo object. The class provides the facility to read or write the fifo
  /// content from a stream. The stream can be created by name for writing,
  /// in which case the named fifo operates as a backup object.
  /// @author amaury darsch

  class NamedFifo : public Strfifo, public Nameable {
  private:
    /// the fifo name
    String d_name;

  public:
    /// create a default named fifo
    NamedFifo (void);

    /// create a named fifo by name
    /// @param name the fifo name
    NamedFifo (const String& name);

    /// create a named fifo by name
    /// @param name the fifo name
    /// @param sdir the stream direction
    NamedFifo (const String& name, const bool sdir);

    /// @return the class name
    String repr (void) const;

    /// @return the fifo name
    String getname (void) const;

    /// set the fifo name
    /// @param name the fifo name to set
    void setname (const String& name);

    /// read the named fifo
    void read (void);

    /// read the named fifo by name
    /// @param name fifo path name
    void read (const String& name);

    /// write the named fifo
    void write (void) const;

    /// write the named fifo by name
    /// @param name fifo path name
    void write (const String& name) const;

  private:
    // make the copy constructor private
    NamedFifo (const NamedFifo&);
    // make the assignment operator private
    NamedFifo& operator = (const NamedFifo&);

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
