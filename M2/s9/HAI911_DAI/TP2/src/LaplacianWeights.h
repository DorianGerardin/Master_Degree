#ifndef LAPLACIANWEIGHTS_H
#define LAPLACIANWEIGHTS_H

#include <vector>
#include <map>
#include "Vec3.h"



//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//
// This class describes the implementation of the well known cotangent weights
// Here, weight( v1 , v2 ) is half the sum of the cotangent angles of the opposite corners in the triangles (v1,v2,other)
//   Careful ! these weights can be negative !
//
// Additionally, we provide a scheme for non-symmetric (!!!) POSITIVE edge weights
//   Careful ! these weights are non-symetric ! e_ij != e_ji
//
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//





// access to an edge is of complexity O( log(val) ), with val the average valence of the vertices
class LaplacianWeights{
private:
    unsigned int n_vertices;
    std::vector< std::map< unsigned int , double > > edge_weights;
    std::vector< double > vertex_weights;

public:
    LaplacianWeights() : n_vertices(0) {}
    void clear()
    {
        n_vertices = 0;
        edge_weights.clear();
        vertex_weights.clear();
    }
    ~LaplacianWeights() { clear(); }

    double sumVertexWeights() const {
        double s = 0.0;
        for(unsigned int v = 0 ; v < n_vertices ; ++v) {
            s += vertex_weights[v];
        }
        return s;
    }

    void resize( unsigned int nVertices )
    {
        clear();
        if( nVertices > 0 )
        {
            n_vertices = nVertices;
            edge_weights.resize(nVertices);
            vertex_weights.resize(nVertices , 0.0);

            for( unsigned int v = 0 ; v < nVertices ; ++v )
            {
                edge_weights[v].clear();
                vertex_weights[v] = 0.0;
            }
        }
    }
    unsigned int get_n_adjacent_edges( unsigned int vertex_index ) const
    {
        return edge_weights[vertex_index].size();
    }
    double get_edge_weight( unsigned int v1 , unsigned int v2 ) const
    {
        std::map< unsigned int , double >::const_iterator it = edge_weights[v1].find(v2);
        if( it == edge_weights[v1].end() ) return 0.0;
        return it->second;
    }
    unsigned int get_n_vertices() const
    {
        return n_vertices;
    }


    std::map< unsigned int , double >::iterator get_weight_of_adjacent_edges_it_begin( unsigned int v1 )
    {
        return edge_weights[v1].begin();
    }
    std::map< unsigned int , double >::iterator get_weight_of_adjacent_edges_it_end( unsigned int v1 )
    {
        return edge_weights[v1].end();
    }

    std::map< unsigned int , double >::const_iterator get_weight_of_adjacent_edges_it_begin( unsigned int v1 ) const
    {
        return edge_weights[v1].begin();
    }
    std::map< unsigned int , double >::const_iterator get_weight_of_adjacent_edges_it_end( unsigned int v1 ) const
    {
        return edge_weights[v1].end();
    }

    double get_vertex_weight( unsigned int v ) const
    {
        return vertex_weights[v];
    }










    //--------------------------   Cotangent weights:   ------------------------//
    // Weight of vertex i is its barycentric area
    // Weight of edge i<->j is the sum of cotangent of angles of the opposite corners divided by 2
    // Note that:
    //   noting c the circumcenter of a triangle, and m(ij) the center of edge ij, theta(ij) being the angle opposite to ij in the triangle,
    //   cot(theta(ij))/2 = |c-m(ij)| / |edge(ij)|

    template< class vertex_t , class triangle_t >
    void buildCotangentWeightsOfTriangleMesh( const std::vector< vertex_t > & vertices , const std::vector< triangle_t > & triangles )
    {
        resize(vertices.size());

        for( unsigned int t = 0 ; t < triangles.size() ; ++t )
        {
            unsigned int v0 = triangles[t][0];
            unsigned int v1 = triangles[t][1];
            unsigned int v2 = triangles[t][2];

            Vec3 p0( vertices[v0][0] , vertices[v0][1] , vertices[v0][2] );
            Vec3 p1( vertices[v1][0] , vertices[v1][1] , vertices[v1][2] );
            Vec3 p2( vertices[v2][0] , vertices[v2][1] , vertices[v2][2] );

            double p0p1_slength = (p1-p0).sqrnorm();
            double p1p2_slength = (p2-p1).sqrnorm();
            double p2p0_slength = (p0-p2).sqrnorm();

            double dot0 = Vec3::dot(p1-p0,p2-p0);
            double dot1 = Vec3::dot(p0-p1,p2-p1);
            double dot2 = Vec3::dot(p0-p2,p1-p2);

            //cotanWeight(p1pOp2) += 1/2 * cotan(p1pOp2)
            double cotW0_by_2 = dot0  / ( 2.0 * sqrt(p0p1_slength*p2p0_slength - dot0*dot0) );
            double cotW1_by_2 = dot1  / ( 2.0 * sqrt(p1p2_slength*p0p1_slength - dot1*dot1) );
            double cotW2_by_2 = dot2  / ( 2.0 * sqrt(p1p2_slength*p2p0_slength - dot2*dot2) );

            edge_weights[v1][v2] += cotW0_by_2;
            edge_weights[v2][v1] += cotW0_by_2;

            edge_weights[v0][v2] += cotW1_by_2;
            edge_weights[v2][v0] += cotW1_by_2;

            edge_weights[v1][v0] += cotW2_by_2;
            edge_weights[v0][v1] += cotW2_by_2;

            double t_area_by_3 = Vec3::cross( p1 - p0 , p2 - p0 ).norm() / 6.0;

            vertex_weights[v0] += t_area_by_3;
            vertex_weights[v1] += t_area_by_3;
            vertex_weights[v2] += t_area_by_3;
        }
    }




    //--------------------------   Clipped Cotangent weights:   ------------------------//
    // Weight of vertex i is its Voronoi area
    // Weight of edge i<->j is the sum of cotangent of angles of the opposite corners divided by 2
    // Note that:
    //   noting c the circumcenter of a triangle, and m(ij) the center of edge ij, theta(ij) being the angle opposite to ij in the triangle,
    //   cot(theta(ij))/2 = |c-m(ij)| / |edge(ij)|
    // Note that:
    //   if a triangle T is obtuse (one angle is greater than pi/2), then the circumcenter is outside the triangle
    //   in this case, we snap the circumcenter to the closest edge and compute the edge weights as |c-m(ij)| / |edge(ij)|
    //   the Voronoi area of the vertices in this case correspond to area(T)/2 for the obtuse vertex and area(T)/4 for the two others.

    template< class vertex_t , class triangle_t >
    void buildClippedCotangentWeightsOfTriangleMesh( const std::vector< vertex_t > & vertices , const std::vector< triangle_t > & triangles )
    {
        resize(vertices.size());

        for( unsigned int t = 0 ; t < triangles.size() ; ++t )
        {
            unsigned int v0 = triangles[t][0];
            unsigned int v1 = triangles[t][1];
            unsigned int v2 = triangles[t][2];

            Vec3 p0( vertices[v0] );
            Vec3 p1( vertices[v1] );
            Vec3 p2( vertices[v2] );

            double p0p1_slength = (p1-p0).sqrnorm();
            double p1p2_slength = (p2-p1).sqrnorm();
            double p2p0_slength = (p0-p2).sqrnorm();

            double dot0 = Vec3::dot(p1-p0,p2-p0);
            double dot1 = Vec3::dot(p0-p1,p2-p1);
            double dot2 = Vec3::dot(p0-p2,p1-p2);

            if( dot0 < 0.0 )
            {
                // Then the triangle is obtuse, and p1p0p2 is greater than pi/2
                // In this case, we snap the circumcenter to the center of the edge p1p2
                Vec3 const & fakeCircumcenter = (p1+p2)/2.0;

                // double edge12Weight = sqrt( ( (p2+p1)/2.0 - fakeCircumcenter ).sqrnorm()  /  p1p2_slength ); // ( == 0.0 )
                // edge_weights[v1][v2] += edge12Weight;
                // edge_weights[v2][v1] += edge12Weight;

                double edge02Weight = sqrt( ( (p0+p2)/2.0 - fakeCircumcenter ).sqrnorm()  /  p2p0_slength );
                edge_weights[v0][v2] += edge02Weight;
                edge_weights[v2][v0] += edge02Weight;

                double edge01Weight = sqrt( ( (p0+p1)/2.0 - fakeCircumcenter ).sqrnorm()  /  p0p1_slength );
                edge_weights[v0][v1] += edge01Weight;
                edge_weights[v1][v0] += edge01Weight;

                double t_area = Vec3::cross( p1 - p0 , p2 - p0 ).norm() / 2.0;

                vertex_weights[v0] += t_area/2.0;
                vertex_weights[v1] += t_area/4.0;
                vertex_weights[v2] += t_area/4.0;
            }
            else if(dot1 < 0.0)
            {
                // Then the triangle is obtuse, and p0p1p2 is greater than pi/2
                // In this case, we snap the circumcenter to the center of the edge p0p2
                Vec3 const & fakeCircumcenter = (p0+p2)/2.0;

                double edge12Weight = sqrt( ( (p2+p1)/2.0 - fakeCircumcenter ).sqrnorm()  /  p1p2_slength );
                edge_weights[v1][v2] += edge12Weight;
                edge_weights[v2][v1] += edge12Weight;

                // double edge02Weight = sqrt( ( (p0+p2)/2.0 - fakeCircumcenter ).sqrnorm()  /  p2p0_slength ); // ( == 0.0 )
                // edge_weights[v0][v2] += edge02Weight;
                // edge_weights[v2][v0] += edge02Weight;

                double edge01Weight = sqrt( ( (p0+p1)/2.0 - fakeCircumcenter ).sqrnorm()  /  p0p1_slength );
                edge_weights[v0][v1] += edge01Weight;
                edge_weights[v1][v0] += edge01Weight;

                double t_area = Vec3::cross( p1 - p0 , p2 - p0 ).norm() / 2.0;

                vertex_weights[v0] += t_area/4.0;
                vertex_weights[v1] += t_area/2.0;
                vertex_weights[v2] += t_area/4.0;
            }
            else if(dot2 < 0.0)
            {
                // Then the triangle is obtuse, and p0p2p1 is greater than pi/2
                // In this case, we snap the circumcenter to the center of the edge p0p1
                Vec3 const & fakeCircumcenter = (p0+p1)/2.0;

                double edge12Weight = sqrt( ( (p2+p1)/2.0 - fakeCircumcenter ).sqrnorm()  /  p1p2_slength );
                edge_weights[v1][v2] += edge12Weight;
                edge_weights[v2][v1] += edge12Weight;

                double edge02Weight = sqrt( ( (p0+p2)/2.0 - fakeCircumcenter ).sqrnorm()  /  p2p0_slength );
                edge_weights[v0][v2] += edge02Weight;
                edge_weights[v2][v0] += edge02Weight;

                // double edge01Weight = sqrt( ( (p0+p1)/2.0 - fakeCircumcenter ).sqrnorm()  /  p0p1_slength ); // ( == 0.0 )
                // edge_weights[v0][v1] += edge01Weight;
                // edge_weights[v1][v0] += edge01Weight;

                double t_area = Vec3::cross( p1 - p0 , p2 - p0 ).norm() / 2.0;

                vertex_weights[v0] += t_area/4.0;
                vertex_weights[v1] += t_area/4.0;
                vertex_weights[v2] += t_area/2.0;
            }
            else
            {
                // Then the triangle is not obtuse, every angle is smaller than pi/2, and we compute the traditionnal cotangent and Voronoi weights:

                //cotanWeight(p1pOp2) += 1/2 * cotan(p1pOp2)
                double cotW0_by_2 = dot0  / ( 2.0 * sqrt(p0p1_slength*p2p0_slength-dot0*dot0) );
                double cotW1_by_2 = dot1  / ( 2.0 * sqrt(p1p2_slength*p0p1_slength-dot1*dot1) );
                double cotW2_by_2 = dot2  / ( 2.0 * sqrt(p1p2_slength*p2p0_slength-dot2*dot2) );

                // Cotangent weights:
                edge_weights[v1][v2] += cotW0_by_2;
                edge_weights[v2][v1] += cotW0_by_2;

                edge_weights[v0][v2] += cotW1_by_2;
                edge_weights[v2][v0] += cotW1_by_2;

                edge_weights[v1][v0] += cotW2_by_2;
                edge_weights[v0][v1] += cotW2_by_2;

                // Voronoi areas:
                vertex_weights[v1] += cotW0_by_2 * p1p2_slength/2.0;
                vertex_weights[v2] += cotW0_by_2 * p1p2_slength/2.0;

                vertex_weights[v0] += cotW1_by_2 * p2p0_slength/2.0;
                vertex_weights[v2] += cotW1_by_2 * p2p0_slength/2.0;

                vertex_weights[v0] += cotW2_by_2 * p0p1_slength/2.0;
                vertex_weights[v1] += cotW2_by_2 * p0p1_slength/2.0;
            }
        }
    }







    //------------------------------------   Barycentric weights:  --------------------------------//
    // Weight of vertex i is its barycentric area (sum of areas of connected triangles divided by 3)
    // Weight of directed edge i->j is its barycentric area divided by the weight of vertex i
    // CAREFUL: weight(i->j) is different from weight(j->i) !
    //          EDGE WEIGHTS ARE NOT SYMMETRIC !
    // These weights are all positive

    template< class vertex_t , class triangle_t >
    void buildBarycentricWeightsOfTriangleMesh( const std::vector< vertex_t > & vertices , const std::vector< triangle_t > & triangles )
    {
        resize(vertices.size());

        for( unsigned int t = 0 ; t < triangles.size() ; ++t )
        {
            unsigned int v0 = triangles[t][0];
            unsigned int v1 = triangles[t][1];
            unsigned int v2 = triangles[t][2];

            Vec3 p0( vertices[v0][0] , vertices[v0][1] , vertices[v0][2] );
            Vec3 p1( vertices[v1][0] , vertices[v1][1] , vertices[v1][2] );
            Vec3 p2( vertices[v2][0] , vertices[v2][1] , vertices[v2][2] );

            double t_area_by_3 = Vec3::cross( p1 - p0 , p2 - p0 ).norm() / 6.0;

            // Barycentric weights:
            edge_weights[v1][v2] += t_area_by_3;
            edge_weights[v2][v1] += t_area_by_3;

            edge_weights[v0][v2] += t_area_by_3;
            edge_weights[v2][v0] += t_area_by_3;

            edge_weights[v1][v0] += t_area_by_3;
            edge_weights[v0][v1] += t_area_by_3;

            vertex_weights[v0] += t_area_by_3;
            vertex_weights[v1] += t_area_by_3;
            vertex_weights[v2] += t_area_by_3;
        }
        for( unsigned int v = 0 ; v < n_vertices ; ++v )
        {
            double v_area = vertex_weights[v];
            for( std::map< unsigned int , double >::iterator it = edge_weights[v].begin() ; it != edge_weights[v].end() ; ++it )
            {
                // todo:: make sure it does not mess up the iterator (it should not, they are supposed to be ordered solely based on it->first, but, not sure)
                edge_weights[v][it->first] = it->second / v_area;
            }
        }
    }

};





#endif // LAPLACIANWEIGHTS_H
