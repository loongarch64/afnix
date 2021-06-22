// ---------------------------------------------------------------------------
// - Librarian.hpp                                                           -
// - afnix engine - librarian class definition                               -
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

#ifndef  AFNIX_LIBRARIAN_HPP
#define  AFNIX_LIBRARIAN_HPP

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif


namespace afnix {

  /// The Librarian class is a management class that allows to combine several
  /// files into a single one. Without argument, an empty librarian is created.
  /// With a string argument a librarian is opened for input. Writing a 
  /// librarian is done with the write method using a string as the file name.
  /// The class can be used inside the Afnix interpreter as well.
  /// @author amaury darsch

  class Librarian : public Nameable {
  public:
    /// @return true if the path is a valid librarian file
    static bool valid (const String& path);

  protected:
    /// the librarian mode
    bool   d_mode;
    /// the librarian name
    String d_name;
    /// the librarian structure
    struct s_desc* p_desc;

  public:
    /// create an empty librarian
    Librarian (void);

    /// create a library by name
    /// @param name the librarian name
    Librarian (const String& name);

    /// destroy this librarian
    ~Librarian (void);

    /// @return the class name
    String repr (void) const;

    /// @return the librarian name
    virtual String getname (void) const;
    
    /// add a new file to this librarian
    /// @param path the file path to add
    virtual void add (const String& path);

    /// @return the number of files in this librarian
    virtual long length (void) const;

    /// @return true if a name exists in the librarian
    virtual bool exists (const String& name) const;

    /// @return a vector of file name in this librarian
    virtual Strvec getlist (void) const;

    /// @return a vector of string object in this librarian
    virtual Vector* getstr (void) const;

    /// @return an input stream by name
    virtual InputStream* extract (const String& name) const;

    /// write this librarian to an output file
    /// @param lname the librarian name
    virtual void write (const String& lname) const;

    /// format to an output stream the librarian structure
    /// @param os the output stream to write
    virtual void format (OutputStream& os) const;

    /// @return true if the start module is set
    virtual bool isstm (void) const;

    /// set the start module bit by name
    /// @param name the module name to set
    virtual void setstm (const String& name);

    /// @return the start module name
    virtual String getstm (void) const;

  protected:
    /// @return the expected file flag in the librarian 
    virtual t_byte mapflag (const t_byte flags) const;

    /// @return the expected file size in the librarian
    virtual t_long mapsize (const t_long size) const;

    /// @return an input stream by name in the librarian
    virtual InputStream* mapfile (const String& name) const;
    
  private:
    // make the copy constructor private
    Librarian (const Librarian&);
    // make the assignment operator private
    Librarian& operator = (const Librarian&);

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
