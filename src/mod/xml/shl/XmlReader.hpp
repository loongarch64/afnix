// ---------------------------------------------------------------------------
// - XmlReader.hpp                                                           -
// - afnix:xml module - xml reader class definition                          -
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

#ifndef  AFNIX_XMLREADER_HPP
#define  AFNIX_XMLREADER_HPP

#ifndef  AFNIX_XMLROOT_HPP
#include "XmlRoot.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

namespace afnix {

  /// The XmlReader class is the xml parser that operates on an input
  /// stream. The reader creates a tree of nodes by reading the input
  /// stream and returns the root node when an end-of-stream is reached.
  /// Multiple read can be done sequentially. If the reset method is not 
  /// called between multiple read passes, the reader will accumulate the
  /// nodes in the current tree.
  /// @author amaury darsch

  class XmlReader : public Object {
  protected:
    /// the xml version
    String d_xvid;
    /// the root node
    XmlRoot* p_root;

  public:
    /// create an empty reader
    XmlReader (void);

    /// create a reader with a version
    /// @param xvid the version to use
    XmlReader (const String& xvid);

    /// destroy this reader
    ~XmlReader (void);

    /// @return the class name
    String repr (void) const;

    /// reset this reader
    void reset (void);

    /// @return the root node
    virtual XmlRoot* getroot (void) const;

    /// get the next available node
    /// @param is the input steam to read
    virtual XmlNode* getnode (InputStream* is);

    /// get the next available node
    /// @param value the string to parse
    virtual XmlNode* getnode (const String& value);

    /// parse an input stream
    /// @param is the input steam to read
    virtual void parse (InputStream* is);

    /// parse a string
    /// @param value the string to parse
    virtual void parse (const String& value);

  private:
    // make the copy constructor private
    XmlReader (const XmlReader&);
    // make the assignment operator private
    XmlReader& operator = (const XmlReader&);

  public:
    /// create an object in a generic way
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
