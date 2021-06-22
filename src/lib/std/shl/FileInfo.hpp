// ---------------------------------------------------------------------------
// - FileInfo.hpp                                                            -
// - standard object library - file info class definition                    -
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

#ifndef  AFNIX_FILEINFO_HPP
#define  AFNIX_FILEINFO_HPP

#ifndef  AFNIX_NAMEABLE_HPP
#include "Nameable.hpp"
#endif

namespace afnix {

  /// The FileInfo class is a file information class that holds the primary
  /// information related to a file, such like its size or its modification
  /// time. The file information is set at construction but can be updated
  /// with the help of the "update" method.
  /// @author amaury darsch

  class FileInfo : public Nameable {
  private:
    /// the file name
    String d_name;
    /// the file size
    t_long d_size;
    /// the modification time
    t_long d_mtim;
    /// the serialize identity
    String d_idty;
    
  public:
    /// create a new information object by name
    /// @param name the file name
    FileInfo (const String& name);

    /// create a new information object by name and identity
    /// @param name the file name
    /// @param idty the identty flag
    FileInfo (const String& name, const bool idty);

    /// @return the class name
    String repr (void) const override;

    /// @return the file name
    String getname (void) const override;

    /// update the file information
    /// @param the identity flag
    virtual void update (const bool idty);

    /// @return the file size
    virtual t_long length (void) const;

    /// @return the file modification time
    virtual t_long mtime (void) const;

    /// @return the file identity
    virtual String identity (void) const;

  private:
    // make the copy constructor private
    FileInfo (const FileInfo&);
    // make the assignment operator private
    FileInfo& operator = (const FileInfo&);

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
