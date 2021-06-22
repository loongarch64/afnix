// ---------------------------------------------------------------------------
// - Logtee.hpp                                                              -
// - afnix:sio module - message logging tee class definition                 -
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

#ifndef  AFNIX_LOGTEE_HPP
#define  AFNIX_LOGTEE_HPP

#ifndef  AFNIX_LOGGER_HPP
#include "Logger.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The Logtee class  is a message logger facility associted with an
  /// output stream. When a message is added to the logger object, the
  /// message is also sent to the output stream, depending on the controling
  /// flags. By default, the tee mode is false and should be turned on with
  /// the setteef method. The class output stream shoudl not be confused with
  /// the logger output stream.
  /// @author amaury darsch

  class Logtee : public Logger {
  private:
    /// the tee flag
    bool d_teef;
    /// the output stream
    OutputStream* p_tos;

  public:
    /// create a default logger
    Logtee (void);

    /// create a logger by size
    /// @param size the logger size
    Logtee (const long size);

    /// create a logger with an output stream
    /// @param os the output stream
    Logtee (OutputStream* os);

    /// create a logger by size with an output stream
    /// @param size the logger size
    /// @param os   the output stream
    Logtee (const long size, OutputStream* os);

    /// destroy this logtee accessor
    ~Logtee (void);

    /// @return the class name
    String repr (void) const;
    
    /// set the tee output stream
    /// @param os the output stream to set
    void settos (OutputStream* os);

    /// @return the tee output steram
    OutputStream* gettos (void) const;

    /// set the tee flag
    /// @param teef the tee flag to set
    void setteef (const bool teef);

    /// @return the logtee flag
    bool getteef (void) const;

    /// add a message by log level
    /// @param mlvl the message level
    /// @param mesg the message to add
    long add (const t_mlvl, const String& mesg);

  private:
    // make the copy constructor private
    Logtee (const Logtee&);
    // make the assignment operator private
    Logtee& operator = (const Logtee&);

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
    /// @param argv  the arguments  to apply
    Object* apply (Evaluable* zobj, Nameset* nset, const long quark,
                   Vector* argv);
  };
}

#endif
