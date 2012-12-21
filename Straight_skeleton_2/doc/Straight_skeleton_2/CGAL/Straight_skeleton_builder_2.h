namespace CGAL {

/*!
\ingroup PkgStraightSkeleton2Classes

The class `Dummy_straight_skeleton_builder_2_visitor` provides a model for the 
`StraightSkeletonBuilder_2_Visitor` concept which is the visitor 
type required by the `StraightSkeletonBuilder_2` 
concept. The class `Dummy_straight_skeleton_builder_2_visitor` has only one template argument: a model of the `StraightSkeleton_2` concept. 

This class is the default visitor parameter of the straight skeleton builder. 
All its methods are empty. 

\cgalModels `StraightSkeletonBuilder_2_Visitor`
\cgalModels `DefaultConstructible`

\sa `CGAL::Straight_skeleton_builder_2<Gt,Ss,Visitor>` 

*/
template< typename Ss >
class Dummy_straight_skeleton_builder_2_visitor {
public:

/// @}

}; /* end Dummy_straight_skeleton_builder_2_visitor */
} /* end namespace CGAL */

namespace CGAL {

/*!
\ingroup PkgStraightSkeleton2Classes

The class `Straight_skeleton_builder_2` encapsulates the construction of the 2D straight skeleton in the interior of a polygon with holes. Its first template parameter must be a model of the `StraightSkeletonBuilderTraits_2` concept, its second template parameter, must be a model of the `StraightSkeleton_2` concept, and its third template paramter must be a model of the `StraightSkeletonBuilder_2_Visitor` concept. If this last parameter is ommitted, the class `CGAL::Dummy_straight_skeleton_builder_2_visitor<Skel>` is used. 

### Algorithm ###

The implemented algorithm is closely based on \cite cgal:fo-ss-98 with the addition of <I>vertex events</I> as described in \cite cgal:ee-rrccpp-98. 

It simulates a grassfire propagation of moving polygon edges as they move inward at constant 
and equal speed. That is, the continuous inward offsetting of the polygon. 

Since edges move at equal speed their movement can be characterized in a simpler setup as the movement of vertices. Vertices move along the angular bisector of adjacent edges. 

The trace of a moving vertex is described by the algorithm as a <I>bisector</I>. 
Every position along a bisector corresponds to the vertex between two offset (moved) edges. Since edges move at constant speed, every position along a bisector also corresponds to the distance those two edges moved so far. 

From the perspective of a dynamic system of moving edges, such a distance can be regarded as an 
<I>instant</I> (in time). Therefore, every distinct position along a bisector corresponds to a distinct instant in the offsetting process. 


As they move inward, edges can expand or contract w.r.t to the endpoints sharing a vertex. If a vertex has an internal angle \f$ <\pi\f$, its incident edges will contract but if its internal angle \f$ >\pi\f$, they will expand. The movement of the edges, along with their extent change, result in collisions between non-adjacent edges. These collisions are called <I>events</I>, and they occur when the colliding edges have moved a certain distance, that is, at certain <I>instants</I>. 

If non-consecutive edges `E(j),E(k)` move while edge `E(i)` contracts, they can collide at the point when `E(i)` shrinks to nothing (that is, the three edges might meet at a certain offset). This introduces a <I>topological change</I> in the polygon: Edges `E(j),E(k)` are now adjacent, edge `E(i)` disappears, and a new vertex appears. This topological change is called an <I>edge event</I>. 

Similarly, consecutive expanding edges `E(i),E(i+1)` sharing a reflex vertex (internal angle \f$ >=\pi\f$) might collide with any edge `E(j)` on the rest of the same connected component of the polygon boundary (even far away from the initial edge's position). This also introduces a topological change: `E(j)` gets split in two edges and the connected component having `E(i),E(i+1) and E(j)` is split in two unconnected parts: one having `E(i)` and the corresponding subsegment of `E(j)` and the other with `E(i+1)` and the rest of `E(j)`. This is called a <I>split event</I>. 
If a reflex vertex hits not an edge `E(j)` but another reflex vertex `E(j),E(j+1)`, and vice-versa (the reflex vertex V(j) hits V(i)), there is no actual split and the two unconnected parts have `E(i),E(j)` and `E(i+1),E(j+1)` (or `E(i),E(j+1)` and `E(i+1),E(j)`). This topological change is called a <I>vertex event</I>. Although similar to a split event in the sense that two new unconnected contours emerge introducing two new contour vertices, in the case of a vertex event one of the new contour vertices might be reflex; that is, a vertex event <I>may</I> result in one of the offset polygons having a <I>reflex</I> contour vertex which was not in the original polygon. 

Edges movement is described by vertices movement, and these by bisectors. Therefore, the collision between edges `E(j),E(i),E(k)` (all in the same connected component) occurs when the moving vertices `E(j)->E(i)` and `E(i)->E(k)` meet ; that is, when the two bisectors describing the moving vertices 
<I>intersect</I> (Note: as the edges move inward and events occur, a vertex between edges A and B might exist even if A and B are not consecutive; that is, j and k are not necessarily i-1 and i+1 respectively, although initially they are). 

Similarly, the collision between `E(i),E(i+1)` with `E(j)` (all in the same connected component) occurs when the bisector corresponding to the moving vertex `E(i)->E(i+1)` hits the moving edge `E(j)`. 

Since each event changes the topology of the moving polygon, it is not possible or practical to foresee all events at once. Rather, the algorithm starts by estimating an initial set of potential events and from there it computes one next event at a time based on the previous one. The chaining of events is governed by their relative instants: events that occur first are processed first. 

An initial set of <I>potential</I> split events is first computed independently (the computation of a potential split event is based solely on a reflex vertex and all other edges in the same connected component); and an initial set of <I>potential</I> edge events between initially consecutive bisectors is first computed independently (based solely on each bisector pair under consideration). 

Events occur at certain instants and the algorithm must be able to order them 
accordingly. The correctness of the algorithm is uniquely and directly governed by the correct computation and ordering of the events. Any potential event might no longer be applicable after the topological change introduced by a prior event. 

A grassfire propagation picks the next unprocessed event (starting from the first) and if it is still applicable processes it. Processing an event involves connecting edges, adding a new skeleton vertex (which corresponds the a contour vertex of the offset polygon) and calculating one new potential future event (which can be either an edge event or a split event -because of a prior vertex event-), based on the topological change just introduced. The propagation finishes when there are no new future events. 

\sa `StraightSkeletonBuilderTraits_2` 
\sa `StraightSkeletonVertex_2` 
\sa `StraightSkeletonHalfedge_2` 
\sa `StraightSkeleton_2` 
\sa `CGAL::Straight_skeleton_builder_traits_2<K>` 
\sa `CGAL::Straight_skeleton_vertex_base_2<Refs,P,FT>` 
\sa `CGAL::Straight_skeleton_halfedge_base_2<Refs>` 
\sa `CGAL::Straight_skeleton_2<Traits,Items,Alloc>` 

*/
template< typename StraightSkeletonBuilderTraits_2 , typename StraightSkeleton_2, typename StraightSkeletonBuilder_2_Visitor >
class Straight_skeleton_builder_2 {
public:

/// \name Types 
/// @{

/*! 
The geometric traits (first template parameter) 
*/ 
typedef StraightSkeletonBuilderTraits_2 Gt; 

/*! 
The straight skeleton (second template parameter) 
*/ 
typedef StraightSkeleton_2 Ss; 

/*! 
The 2D point type as defined by the geometric traits 
*/ 
typedef Hidden_type Point_2; 

/// @} 

/// \name Creation 
/// @{

/*! 
%Default constructs the builder class. 
*/ 
StraightSkeletonBuilder_2(); 

/// @} 

/// \name Methods 
/// @{

/*! 
Defines the <I>contours</I> that form the <I>non-degenerate strictly-simple polygon with holes</I> whose <I>straight skeleton</I> is to be built. 

Each contour must be input in turn starting with the <I>outer contour</I> and following with the holes (if any). The order of the holes is unimportant but the outer contour must be entered first. The outer contour must be oriented counter-clockwise while holes must be oriented clockwise. 

It is an error to enter more than one outer contour or to enter a hole which is not inside the outer contour or inside another hole. It is also an error to enter a contour which crosses or touches any one another. It is possible however to enter a contour that touches itself in such a way that its interior region is still well defined and singly-connected (see the User Manual for examples). 

The sequence [aBegin,aEnd) must iterate over each 2D point that corresponds to a vertex of the contour being entered. Vertices cannot be coincident (except consecutively since the method simply skip consecutive coincident vertices). Consecutive collinear edges are allowed. 

InputPointIterator must be an <I>InputIterator</I> whose `value_type` is `Point_2`. 
*/ 
template<class InputPointIterator> Straight_skeleton_builder_2& enter_contour( InputPointIterator aBegin, InputPointIterator aEnd ); 

/*! 
Constructs and returns the 2D straight skeleton in the interior of the polygon with holes as defined by the contours entered first by calling `enter_contour`. All the contours of the polygon with holes must be entered before calling `construct_skeleton`. 

After `construct_skeleton` completes, you cannot enter more contours and/or call `construct_skeleton()` again. If you need another straight skeleton for another polygon you must instantiate and use another builder. 

The result is a dynamically allocated instance of the `Ss` class, wrapped in a `boost::shared_ptr`. 

If the construction process fails for whatever reason (such as a nearly-degenerate vertex whose internal or external angle is almost zero), the return value will be <I>null</I>, represented by a default constructed `shared_ptr`. 

The algorithm automatically checks the consistency of the result, thus, if it is not <I>null</I>, it is guaranteed to be valid. 

*/ 
boost::shared_ptr<Ss> construct_skeleton(); 

/// @}

}; /* end Straight_skeleton_builder_2 */
} /* end namespace CGAL */