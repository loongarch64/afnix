// ---------------------------------------------------------------------------
// - Edge.hpp                                                                -
// - afnix:gfx module - graph edge class definition                          -
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

#ifndef  AFNIX_EDGE_HPP
#define  AFNIX_EDGE_HPP

#ifndef  AFNIX_SET_HPP
#include "Set.hpp"
#endif

#ifndef  AFNIX_COLLECTABLE_HPP
#include "Collectable.hpp"
#endif

namespace afnix {

  /// The Edge class is the class used to represent the connectivity between 
  /// two vertices or more in a graph. If the edge is associated with more
  /// than two vertices, it is called an hyperedge and the resulting graph is
  /// called an hypergraph. If the edge contains only one vertex, the edge 
  /// is assumed to be a self-loop. The total number of vertices attached to
  /// the edge is called the edge cardinality.
  /// @author amaury darsch

  class Edge : public Collectable {
  protected:
    /// the edge marker
    bool    d_mrk;
    /// the vertices object set
    Set     d_vos;
    /// the edge client object
    Object* p_clo;

  public:
    /// create an empty edge
    Edge (void);

    /// create an edge with a client object
    /// @param clo the client object
    Edge (Object* clo);

    /// destroy this edge
    ~Edge (void);

    /// @return the class name
    String repr (void) const;

    /// release this object
    void release (void);

    /// reset this edge
    virtual void reset (void);

    /// @return the edge cardinality
    virtual long cardinality (void) const;

    /// attach a vertex to this edge
    /// @param vrtx the vertex to attach
    virtual void add (class Vertex* vrtx);

    /// @return a vertex by index
    virtual class Vertex* get (const long index) const;

    /// remove a vertex by object
    /// @param vrtx the vertex to remove
    virtual bool remove (class Vertex* vrtx);

    /// set the edge client object
    /// @param clo the client object
    virtual void setclo (Object* clo);

    /// @return the edge client object
    virtual Object* getclo (void) const;

  private:
    // make the copy constructor private
    Edge (const Edge&);
    // make the assignment operator private
    Edge& operator = (const Edge&);

  public:
    /// create a object in a generic way
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
