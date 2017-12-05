#ifndef H_EDGE
#define H_EDGE

#include "vector2.h"

template <class T>

class Edge {

public:
	using VertexType = Vector2<T>;

	Edge(const VertexType &p1, const VertexType &p2) : p1(p1), p2(p2) {};  //SFML custom vertex
	Edge(const Edge &e) : p1(e.p1), p2(e.p2) {};
	float Length() {
		return sqrtf((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
	}

	VertexType p1;
	VertexType p2;
};

	template <class T>
	inline std::ostream &operator << (std::ostream &str, Edge<T> const &e) {
		return str  << e.p1 
			<< e.p2;
	}

	template <class T>
	inline bool operator == (const Edge<T> & e1, const Edge<T> & e2) {
		return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
			(e1.p1 == e2.p2 && e1.p2 == e2.p1);
	}
	



#endif 

