#ifndef SQUARE_H
#define SQUARE_H
#include "Vec3.h"
#include <vector>
#include <stdio.h>
#include "Mesh.h"
#include <cmath>

using namespace std;

struct RaySquareIntersection{
    bool intersectionExists;
    float t;
    float u,v;
    Vec3 intersection;
    Vec3 normal;
};


class Square : public Mesh {
public:
    Vec3 m_normal;
    Vec3 m_bottom_left;
    Vec3 m_right_vector;
    Vec3 m_up_vector;

    Square() : Mesh() {}
    Square(Vec3 const & bottomLeft , Vec3 const & rightVector , Vec3 const & upVector , float width=2. , float height=1. ,
           float uMin = 0.f , float uMax = 1.f , float vMin = 0.f , float vMax = 1.f) : Mesh() {
        setQuad(bottomLeft, rightVector, upVector, width, height, uMin, uMax, vMin, vMax);
    }

    void setQuad( Vec3 const & bottomLeft , Vec3 const & rightVector , Vec3 const & upVector , float width=1. , float height=1. ,
                  float uMin = 0.f , float uMax = 1.f , float vMin = 0.f , float vMax = 1.f) {
        m_right_vector = rightVector;
        m_up_vector = upVector;
        m_normal = Vec3::cross(rightVector , upVector);
        m_bottom_left = bottomLeft;

        m_normal.normalize();
        m_right_vector.normalize();
        m_up_vector.normalize();

        m_right_vector = m_right_vector*width;
        m_up_vector = m_up_vector*height;

        vertices.clear();
        vertices.resize(4);
        vertices[0].position = bottomLeft;                                      vertices[0].u = uMin; vertices[0].v = vMin;
        vertices[1].position = bottomLeft + m_right_vector;                     vertices[1].u = uMax; vertices[1].v = vMin;
        vertices[2].position = bottomLeft + m_right_vector + m_up_vector;       vertices[2].u = uMax; vertices[2].v = vMax;
        vertices[3].position = bottomLeft + m_up_vector;                        vertices[3].u = uMin; vertices[3].v = vMax;
        vertices[0].normal = vertices[1].normal = vertices[2].normal = vertices[3].normal = m_normal;
        triangles.clear();
        triangles.resize(2);
        triangles[0][0] = 0;
        triangles[0][1] = 1;
        triangles[0][2] = 2;
        triangles[1][0] = 0;
        triangles[1][1] = 2;
        triangles[1][2] = 3;


    }

    RaySquareIntersection intersect(const Ray &ray) const {
        RaySquareIntersection intersection;

        Plane plane = Plane(m_bottom_left, m_normal);

        if (!plane.isParallelTo(ray)) {
            Vec3 notSureIntersection = plane.getIntersectionPoint(ray);

            Vec3 m_top_right = m_bottom_left + m_right_vector + m_up_vector;
            cout << "m_bottom_left : " << m_bottom_left << endl;
            cout << "m_right_vector : " << m_right_vector << endl;
            cout << "m_up_vector : " << m_up_vector << endl;
            cout << "m_top_right : " << m_top_right << endl;

            float minX = min(m_top_right[0], m_bottom_left[0]);
            float minY = min(m_top_right[1], m_bottom_left[1]);
            float minZ = min(m_top_right[2], m_bottom_left[2]);

            cout << "minx : " << minX << endl;
            cout << "miny : " << minY << endl;
            cout << "minz : " << minZ << endl;

            float maxX = max(m_top_right[0], m_bottom_left[0]);
            float maxY = max(m_top_right[1], m_bottom_left[1]);
            float maxZ = max(m_top_right[2], m_bottom_left[2]);

            cout << "maxx : " << maxX << endl;
            cout << "maxy : " << maxY << endl;
            cout << "maxz : " << maxZ << endl;

            cout << endl;

            if (notSureIntersection[0] >= minX && notSureIntersection[0] <= maxX && 
                notSureIntersection[1] >= minY && notSureIntersection[1] <= maxY &&
                notSureIntersection[2] >= minZ && notSureIntersection[2] <= maxZ) {

                    intersection.intersectionExists = true;
                    intersection.intersection = notSureIntersection;
                    intersection.normal = m_normal;

                    Vec3 a = m_bottom_left;
                    Vec3 n = m_normal;

                    float D = Vec3::dot(a, n);
                    Vec3 o = ray.origin();
                    Vec3 d = ray.direction();

                    float t = (D - Vec3::dot(o, n)) / Vec3::dot(d, n);
                    intersection.t = t;

            } 
            else {
                intersection.intersectionExists = false;
            }
        } else intersection.intersectionExists = false;

        /*if (!plane.isParallelTo(ray)) {
            Vec3 notSureIntersection = plane.getIntersectionPoint(ray);
            Vec3 vec = notSureIntersection - m_bottom_left;
            float f = Vec3::dot(vec, m_right_vector);
            float f2 = Vec3::dot(vec, m_up_vector);
            if (f < 0|| f2 < 0 || f > m_right_vector.length() || f2 > m_up_vector.length()) {
                intersection.intersectionExists = false;
            }
        }*/

        return intersection;
    }
};
#endif // SQUARE_H
