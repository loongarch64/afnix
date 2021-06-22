// ---------------------------------------------------------------------------
// - Vertex.hpp                                                              -
// - afnix:gfx module - graph vertex class definition                        -
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

#ifndef  AFNIX_VERTEX_HPP
#define  AFNIX_VERTEX_HPP

#ifndef  AFNIX_SET_HPP
#include "Set.hpp"
#endif

#ifndef  AFNIX_COLLECTABLE_HPP
#include "Collectable.hpp"
#endif

namespace afnix {

  /// The Vertex class is the class used to represent an vertex in the standard
  /// graph. The vertex holds two arrays for the incoming and outgoing edges.
  /// @author amaury darsch

  class Vertex : public Collectable {
  protected:
    /// the vertex marker
    bool    d_mrk;
    /// the edge object set
    Set     d_eos;
    /// the client object
    Object* p_clo;

  public:
    /// create an empty vertex
    Vertex (void);

    /// create a vertex with a client object
    /// @param clo the client object
    Vertex (Object* clo);

    /// destroy this vertex
    ~Vertex (void);

    /// @return the class name
    String repr (void) const;

    /// release this object
    void release (void);

    /// reset this vertex
    virtual void reset (void);

    /// @return the degree of this vertex
    virtual long degree (void) const;

    /// add an edge to this vertex
    /// @param edge the edge to add
    virtual void add (class Edge* edge);

    /// @return an edge by index
    virtual class Edge* get (const long index) const;

    /// remove an edge by object
    /// @param edge the edge to remove
    virtual bool remove (class Edge* edge);

    /// set the vertex client object
    /// @param clo the client object
    virtual void setclo (Object* clo);

    /// @return the vertex client object
    virtual Object* getclo (void) const;

  private:
    // make the copy constructor private
    Vertex (const Vertex&);
    // make the assignment operator private
    Vertex& operator = (const Vertex&);

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
