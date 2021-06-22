// ---------------------------------------------------------------------------
// - Graph.hpp                                                               -
// - standard object module - graph base class definition                    -
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

#ifndef  AFNIX_GRAPH_HPP
#define  AFNIX_GRAPH_HPP

#ifndef  AFNIX_EDGE_HPP
#include "Edge.hpp"
#endif

#ifndef  AFNIX_VERTEX_HPP
#include "Vertex.hpp"
#endif

namespace afnix {

  /// The Graph class is the base class used to hold a graph. A graph is
  /// a set of edges and vertices. If the edges used to compose the graph are
  /// hyperedges, then the graph is called a hypergraph.
  /// @author amaury darsch

  class Graph : public Collectable {
  protected:
    /// the set of edges
    Set     d_edges;
    /// the set of vertices
    Set     d_vrtxs;
    /// the graph client object
    Object* p_clo;

  public:
    /// create an empty graph
    Graph (void);

    /// create a graph with a client object
    /// @param clo the client object
    Graph (Object* clo);

    /// destroy this graph
    ~Graph (void);

    /// @return the class name
    String repr (void) const;

    /// release this object
    void release (void);

    /// reset the graph
    virtual void reset (void);

    /// reset the graph edges
    virtual void ereset (void);

    /// reset the graph vertices
    virtual void vreset (void);

    /// @return true is an edge exists in this graph
    virtual bool exists (Edge* edge) const;

    /// @return true if a vertex exists in this graph
    virtual bool exists (Vertex* vertex) const;

    /// add a vertex to this graph.
    /// @param vertex the vertex to add
    virtual void add (Vertex* vertex);

    /// add an edge to this graph.
    /// @param edge the edge to add
    virtual void add (Edge* edge);

    /// @return the number of edges
    virtual long getne (void) const;

    /// @return the number of vertices
    virtual long getnv (void) const;

    /// @return an edge by index
    virtual Edge* getedge (const long index) const;

    /// @return a vertex by index
    virtual Vertex* getvrtx (const long index) const;

    /// set the edge client object
    /// @param clo the client object
    virtual void setclo (Object* clo);

    /// @return the edge client object
    virtual Object* getclo (void) const;

  private:
    // make the copy constructor private
    Graph (const Graph&);
    // make the assignment operator private
    Graph& operator = (const Graph&);

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
