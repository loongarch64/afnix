// ---------------------------------------------------------------------------
// - Csv.hpp                                                                 -
// - afnix:sps module - csv importer class definition                        -
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

#ifndef  AFNIX_CSV_HPP
#define  AFNIX_CSV_HPP

#ifndef  AFNIX_VECTOR_HPP
#include "Vector.hpp"
#endif

#ifndef  AFNIX_FORMER_HPP
#include "Former.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {
  
  /// The Csv class is the comma separated value form reader, which is
  /// primarily used for sheet importation. By default, the csv importer
  /// defines a set of separators which are the comma ',', the semicolon ';'
  /// and tab character. When importing data, a form is returned for each
  /// row in the file. The reading process can start at a certain row and
  /// at another one.
  /// @author amaury darsch
  
  class Csv : public Former, public Nameable {
  private:
    /// the line number
    long d_lnum;
    /// the break sequence
    String d_sbrk;
    /// the locale vector
    Vector d_locv;
    /// the input stream name
    String d_name;
    /// the input stream
    InputStream* p_is;

    
  public:
    /// create a default reader
    Csv (void);

    /// create a new reader with an input stream
    /// @param is the input stream
    Csv (InputStream* is);

    /// create a new reader by stream name
    /// @param name the input stream name
    Csv (const String& name);

    /// create a new reader by stream and break sequence
    /// @param is   the input stream
    /// @param sbrk the break sequence
    Csv (InputStream* is, const String& sbrk);

    /// create a new reader by stream and break sequence
    /// @param is   the input stream
    /// @param sbrk the break sequence
    /// @param locv the locale vector
    Csv (InputStream* is, const String& sbrk, const Vector& locv);

    /// create a new reader by name and break sequence
    /// @param name the input stream name
    /// @param sbrk the break sequence
    Csv (const String& name, const String& sbrk);

    /// destroy this reader
    ~Csv (void);

    /// @return the class name
    String repr (void) const;

    /// @return true if the csv is valid
    bool valid (void) const;
		 
    /// @return a vector from this reader
    Vector* split (void);

    /// @return a form from this reader
    Form* parse (void);

    /// @return an approximate line number
    long getlnum (void) const;

    /// @return the stream file name
    String getname (void) const;
    
    /// set the break sequence
    /// @param sbrk the break sequence to set
    void setsbrk (const String& sbrk);

    /// @return the break sequence
    String getsbrk (void) const;

    /// set the locale vector
    /// @param locv the locale vector
    void setlocv (const Vector& locv);
    
  private:
    // make the copy constructor private
    Csv (const Csv&) =delete;
    // make the assignement operator private
    Csv& operator = (const Csv&) =delete;

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
