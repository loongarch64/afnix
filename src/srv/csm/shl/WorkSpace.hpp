// ---------------------------------------------------------------------------
// - WorkSpace.hpp                                                           -
// - afnix:csm module - abstract workspace class definition                  -
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

#ifndef  AFNIX_WORKSPACE_HPP
#define  AFNIX_WORKSPACE_HPP

#ifndef  AFNIX_WORKZONE_HPP
#include "WorkZone.hpp"
#endif

namespace afnix {

  /// The WorkSpace class is an abstract class designed to manipulate
  /// entities structured by zone and as such, a workspace is defined as a set
  /// of workzones. Entities can be accessed directly uri or name, provided
  /// that the zone is known.
  /// @author amaury darsch

  class WorkSpace : public virtual Nameable {
  protected:
    /// the workspace name
    String d_name;
    /// the workspace info
    String d_info;

  public:
    /// create a default workspace
    WorkSpace (void);

    /// create a workspace by name
    /// @param name the workspace name
    WorkSpace (const String& name);

    /// create a workspace by name and info
    /// @param name the workspace name
    /// @param info the workspace info
    WorkSpace (const String& name, const String& info);

    /// @return the workspace name
    String getname (void) const;

    /// set the workspace name
    /// @param name the name to set
    virtual void setname (const String& name);

    /// @return the workspace info
    virtual String getinfo (void) const;

    /// set the workspace info
    /// @param info the info to set
    virtual void setinfo (const String& info);

    /// check if a zone exists by name
    /// @param zone the zone to check
    virtual bool iszone (const String& zone) const =0;

    /// remove a zone by name
    /// @param zone the zone to remove
    virtual bool rmzone (const String& zone) = 0;
    
    /// add a new working zone by name
    /// @param zone the zone name
    virtual WorkZone* addzone (const String& zone) =0;

    /// get a zone by name
    /// @param name the zone name
    virtual WorkZone* getzone (const String& name) const =0;
    
    /// map an entity name to a normalized uri if possible
    /// @param zone the working zone
    /// @param name the entity name to map
    virtual String tonuri (const String& zone, const String& name) const =0;

    /// check if an entity exists by zone and uri
    /// @param zone the working zone
    /// @param uri  the uri to check
    virtual bool exists (const String& zone, const Uri& uri) const =0;

    /// check if an entity exists by zone and string uri
    /// @param zone the working zone
    /// @param suri the string uri
    virtual bool exists (const String& zone, const String& suri) const;

    /// get an input stream by zone and uri
    /// @param zone the working zone
    /// @param uri  the uri to open
    virtual InputStream* getis (const String& zone, const Uri& uri) const =0;
    
    /// get an input stream by zone and normalized uri
    /// @param zone the working zone
    /// @param suri the string uri
    virtual InputStream* getis (const String& zone, const String& suri) const;

    /// get an ouput stream by zone and uri
    /// @param zone the working zone
    /// @param uri  the uri to open
    virtual OutputStream* getos (const String& zone, const Uri& uri) const =0;
    
    /// get an output stream by zone and normalized uri
    /// @param zone the working zone
    /// @param suri the string uri
    virtual OutputStream* getos (const String& zone, const String& suri) const;

    /// get a workspace zone entity list
    /// @param zone the working zone
    virtual Strvec* getelst (const String& zone) const =0;

    /// get a workspace zone entity print table
    /// @param zone the working zone
    virtual PrintTable* toeptbl (const String& zone) const =0;

  private:
    // make the copy constructor private
    WorkSpace (const WorkSpace&) =delete;
    // make the assignment operator private
    WorkSpace& operator = (const WorkSpace&) =delete;

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
