#ifndef COLLIDER_H
#define COLLIDER_H

#define CGAL_CFG_NO_CPP0X_VARIADIC_TEMPLATES 1
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/centroid.h>
#include <CGAL/Simple_cartesian.h>

#include <CGAL/intersections.h>
#include <CGAL/Bbox_3.h>

typedef CGAL::Simple_cartesian<float> K;
typedef K::Point_3 Point;
typedef K::Segment_3 Segment;
typedef K::Vector_3 Vector;
typedef K::Triangle_3 Triangle;
typedef K::Ray_3 Ray;

typedef std::vector<Triangle>::iterator Iterator;
typedef CGAL::AABB_triangle_primitive<K, Iterator> Primitive;

typedef CGAL::AABB_traits<K, Primitive> AABB_triangle_traits;
typedef AABB_triangle_traits::Point_and_primitive_id Point_and_Primitive_id;
typedef CGAL::AABB_tree<AABB_triangle_traits> Tree;
typedef Tree::Object_and_primitive_id Object_and_Primitive_id;

#include <glm/glm.hpp>
#include <iostream>

#include "Mesh.h"

class Collider
{
public:
	Collider(glm::vec3 min_ = glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3 max_ = glm::vec3(0.1f, 0.1f, 0.1f));
	~Collider();
	void updateMinMax(const glm::vec3 &point);
	void updateOnScalePos(glm::vec3 scale_, glm::vec3 position_);
	bool containPoint(const glm::vec3 point) const;
public:
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 box_min;//in world
	glm::vec3 box_max;//in world
};

class AABBTree
{
public:
	AABBTree()
	{
		
	}

	~AABBTree()
	{
	}


	void Build(std::vector<Mesh>& mesh, bool is_ground)
	{
		std::vector<int> m_index;
		if(is_ground){
			for (int i = 0; i < 16; i++)
				m_index.push_back(i);
			for (int i = 18; i < mesh.size(); i++)
				m_index.push_back(i);
		}
		else {
			m_index.push_back(16);
			m_index.push_back(17);
		}
		for (int mm = 0; mm < m_index.size(); mm++)
		{
			int m = m_index[mm];
			std::vector<Triangle> tmp_triangles;
			std::vector<Point> tmp_points;

			tmp_points.resize(mesh[m].vertices.size());
			for (int i = 0; i < mesh[m].vertices.size(); i++)
			{
				Vertex tmp = mesh[m].vertices[i];
				tmp_points[i] = Point(tmp.Position[0], tmp.Position[1], tmp.Position[2]);
			}
			tmp_triangles.resize(mesh[m].indices.size() / 3);
			for (int i = 0; i < mesh[m].indices.size() / 3; i++)
			{
				tmp_triangles[i] = Triangle(tmp_points[mesh[m].indices[i * 3 + 0]],
					tmp_points[mesh[m].indices[i * 3 + 1]],
					tmp_points[mesh[m].indices[i * 3 + 2]]);
			}

			points.insert(points.end(), tmp_points.begin(), tmp_points.end());
			triangles.insert(triangles.end(), tmp_triangles.begin(), tmp_triangles.end());
		}

		tree.rebuild(triangles.begin(), triangles.end());
		tree.accelerate_distance_queries();
	}

	float Query(float x, float y, float z)
	{
		Point point_query(x * 100, y * 100, z * 100);
		float sqd = tree.squared_distance(point_query);
		return sqd;
	}

public:
	Tree tree;
	std::vector<Triangle> triangles;
	std::vector<Point> points;
};

#endif
