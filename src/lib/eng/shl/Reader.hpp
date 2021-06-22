// ---------------------------------------------------------------------------
// - Reader.hpp                                                              -
// - afnix engine - reader class definition                                  -
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

#ifndef  AFNIX_READER_HPP
#define  AFNIX_READER_HPP

#ifndef  AFNIX_LEXER_HPP
#include "Lexer.hpp"
#endif

#ifndef  AFNIX_FORMER_HPP
#include "Former.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The Reader class is the general purpose form reader which supports the
  /// unified afnix syntax. The reader is primarily used to parse file or
  /// be run interactively. The reader consumes tokens until a complete form
  /// can be built. The form does not have any particular meaning and must be
  /// post processed by the application.
  /// @author amaury darsch
  
  class Reader : public Former, public Nameable {
  private:
    /// the file name we are reading
    String d_name;
    /// the lexer class
    Lexer* p_lex;
    /// the input stream used by this reader
    InputStream* p_is;
    
  public:
    /// create a default reader
    Reader (void);

    /// create a new reader with a string
    /// @param sval the string to parse
    Reader (const String& sval);

    /// create a new reader with an input stream
    /// @param is the input s
    Reader (InputStream* is);

    /// destroy this reader
    ~Reader (void);

    /// @return the class name
    String repr (void) const;

    /// @return a form from this reader or nil in case of eos
    Form* parse (void);

    /// set the reader file name
    /// @param name the reader file name
    void setname (const String& name);

    /// @return the reader file name
    String getname (void) const;

    /// @return an approximate line number for this reader
    long getlnum (void) const;

  private:
    // make the copy constructor private
    Reader (const Reader&);
    // make the assignement operator private
    Reader& operator = (const Reader&);
    // private reader procedures
    Form* rform (bool);
    Form* bform (bool);

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
