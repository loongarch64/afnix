// ---------------------------------------------------------------------------
// - Module.hpp                                                              -
// - afnix engine - module class definitions                                 -
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

#ifndef  AFNIX_MODULE_HPP
#define  AFNIX_MODULE_HPP

#ifndef  AFNIX_FORMER_HPP
#include "Former.hpp"
#endif

#ifndef  AFNIX_INPUTFILE_HPP
#include "InputFile.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The Module class is an in-core memory representation of an afnix file.
  /// The representation is either a text form or a serialized form. By 
  /// default the module constructor takes a name which is the input file
  /// name. The "parse" method returns the next form available within this 
  /// module. 
  /// @author amaury darsch

  class Module : public Nameable {
  public:
    /// supported module format
    enum t_mtype {REGULAR, COMPILED};

  private:
    /// the module type
    t_mtype d_type;
    /// the module name
    String  d_name;
    /// the input stream
    InputStream* p_is;
    /// the form reader
    Former* p_former;

  public:
    /// create an empty module
    Module (void);

    /// create a module by name
    /// @param name the module name
    Module (const String& name);

    /// create a module with a stream
    /// @param is   the input stream to use
    /// @param name the optional stream name
    Module (InputStream* is, const String& name);

    /// destroy this module
    ~Module (void);

    /// @return the class name
    String repr (void) const;

    /// @return the next form
    Form* parse (void);

    /// @return true is the module is regular
    bool isregular (void) const;

    /// @return the module type
    t_mtype gettype (void) const;

    /// @return the module name
    String getname (void) const;

    /// @return the reader line number
    long getlnum (void) const;

    /// write the module content
    void write (OutputStream& os);

  private:
    // make the copy constructor private
    Module (const Module&);
    // make the assignment operator private
    Module& operator = (const Module&);
  };
}

#endif
