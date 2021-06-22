// ---------------------------------------------------------------------------
// - Logger.hpp                                                              -
// - standard object library - message logger class definition               -
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

#ifndef  AFNIX_LOGGER_HPP
#define  AFNIX_LOGGER_HPP

#ifndef  AFNIX_STRING_HPP
#include "String.hpp"
#endif

namespace afnix {

  /// The Logger class is a message logger that stores messages in a
  /// buffer with a level. The messages are stored in a circular buffer.
  /// When the logger is full, a new message replace the oldest one. 
  /// By default, the logger is initialized with a 256 messages capacity 
  /// that can be resized.
  /// @author amaury darsch

  class Logger : public virtual Object {
  public:
    /// the message log level
    enum t_mlvl {
      MLVL_FATL, // fatal error
      MLVL_XERR, // error
      MLVL_WRNG, // warning
      MLVL_INFO, // information
      MLVL_DBUG  // debug
    };
    
  private:
    /// the logger size
    long d_size;
    /// the logger length
    long d_mcnt;
    /// the logger position
    long d_mpos;
    /// the logger base
    long d_base;
    /// the log debug flag
    bool d_ldbg;
    /// the logger array
    struct s_mlog* p_mlog;
    /// the output stream
    OutputStream* p_os;

  public:
    /// create a default logger
    Logger (void);
  
    /// create a logger by size
    /// @param size the logger size
    Logger (const long size);

    /// destroy this logger
    ~Logger (void);

    /// return the class name
    String repr (void) const;

    /// reset this logger
    virtual void reset (void);

    /// @return the logger length
    virtual long length (void) const;

    /// add a message by log level
    /// @param mlvl the logging level
    /// @param mesg the message to add
    virtual long add (const t_mlvl mlvl, const String& mesg);

    /// @eturn a message level by index
    virtual t_mlvl getmlvl (const long index) const;

    /// @eturn a message time by index
    virtual t_long gettime (const long index) const;

    /// @return a message by index
    virtual String getmesg (const long index) const;

    /// @return a full message by index
    virtual String getfull (const long index) const;

    /// set the logger debug flag
    /// @param ldbg the debug flag to set
    virtual void setldbg (const bool ldbg);
    
    /// set the logger output stream
    /// @param os the output stream to bind
    virtual void setos (OutputStream* os);

    /// set the logger output file name
    /// @param name the output file name
    virtual void setos (const String& name);

    /// resize this message logger
    /// @param size the new logger size
    void resize (const long size);

  private:
    // make the copy constructor private
    Logger (const Logger&);
    // make the assignment operator private
    Logger& operator = (const Logger&);    

  public:
    /// evaluate an object data member
    /// @param zobj  zobj the current evaluable
    /// @param nset  the current nameset
    /// @param quark the quark to evaluate
    static Object* meval (Evaluable* zobj, Nameset* nset, const long quark);

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
