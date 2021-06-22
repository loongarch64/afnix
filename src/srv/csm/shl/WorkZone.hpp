// ---------------------------------------------------------------------------
// - WorkZone.hpp                                                            -
// - afnix:csm module - abstract workzone class definition                   -
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

#ifndef  AFNIX_WORKZONE_HPP
#define  AFNIX_WORKZONE_HPP

#ifndef  AFNIX_URI_HPP
#include "Uri.hpp"
#endif

#ifndef  AFNIX_STRVEC_HPP
#include "Strvec.hpp"
#endif

#ifndef  AFNIX_PRINTTABLE_HPP
#include "PrintTable.hpp"
#endif

#ifndef  AFNIX_INPUTSTREAM_HPP
#include "InputStream.hpp"
#endif

#ifndef  AFNIX_OUTPUTSTREAM_HPP
#include "OutputStream.hpp"
#endif

namespace afnix {

  /// The WorkZone class is an abstract class designed to manipulate
  /// a set of entities in a confined zone. As such, a work zone can be seen
  /// as an abstract file system where the standard operations, like creating,
  /// reading and writing entities are available. Most of the time, an entity
  /// is a file, but this is not mandatory as long as such object can be
  /// created and/or mapped to a stream. Within a zone, an entity can be
  /// accessed by name or by uri.
  /// @author amaury darsch

  class WorkZone : public virtual Nameable {
  public:
    /// @return the zone uri scheme
    static String getschm (void);
    
    /// map a name to a zone uri
    /// @param name the name to map
    static String tozuri (const String& name);
    
  protected:
    /// the zone name
    String d_name;
    /// the zone info
    String d_info;

  public:
    /// create a default workzone
    WorkZone (void);

    /// create a workzone by name
    /// @param name the workzone name
    WorkZone (const String& name);

    /// create a workzone by name and info
    /// @param name the workzone name
    /// @param info the workzone info
    WorkZone (const String& name, const String& info);

    /// @return the workzone name
    String getname (void) const;

    /// set the workzone name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// @return the workzone info
    virtual String getinfo (void) const;

    /// set the workzone info
    /// @param info the info to set
    virtual void setinfo (const String& info);

    /// map an entity name to a normalized uri string
    /// @param name the entity name to map
    virtual String tonuri (const String& name) const =0;

    /// check if an entity exists by uri
    /// @param uri the uri to check
    virtual bool exists (const Uri& uri) const =0;

    /// check if an entity exists by string uri
    /// @param suri the string uri
    virtual bool exists (const String& suri) const;

    /// rename an entity by uri
    /// @param nuri the name uri
    /// @param turi the target uri
    virtual bool rename(const Uri& nuri, const Uri& turi) =0;

    /// rename an entity by string uri
    /// @param suri the string uri
    /// @param puri the string path
    virtual bool rename (const String& suri, const String& puri);

    /// remove an entity by uri
    /// @param uri the uri to remove
    virtual bool remove(const Uri& uri) =0;

    /// remove an entity by string uri
    /// @param suri the string uri
    virtual bool remove (const String& suri);

    /// clean the zone
    virtual bool clean (void) =0;
    
    /// get an input stream by and uri
    /// @param uri the uri to open
    virtual InputStream* getis (const Uri& uri) const =0;
    
    /// get an input stream by string uri
    /// @param suri the string uri
    virtual InputStream* getis (const String& suri) const;

    /// get an ouput stream by uri
    /// @param uri the uri to open
    virtual OutputStream* getos (const Uri& uri) const =0;
    
    /// get an output stream by string uri
    /// @param suri the normalized uri
    virtual OutputStream* getos (const String& suri) const;
    
    /// @return a workzone entity list
    virtual Strvec* getelst (void) const =0;

    /// @return a workzone entity list by idenity
    virtual Strvec* getelst (const String& idty) const =0;

    /// @return a workzone entity print table
    virtual PrintTable* toeptbl (void) const =0;

  private:
    // make the copy constructor private
    WorkZone (const WorkZone&) =delete;
    // make the assignment operator private
    WorkZone& operator = (const WorkZone&) =delete;

  public:
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
