#ifndef MESH_H
#define MESH_H


#include <vector>
#include <string>
#include "Vec3.h"
#include "Skeleton.h"

#include <cmath>

#include <GL/glut.h>


// -------------------------------------------
// Basic Mesh class
// -------------------------------------------

struct MeshVertex {
    inline MeshVertex () {
        w.clear();
    }
    inline MeshVertex (const Vec3 & _p, const Vec3 & _n) : p (_p), n (_n) {
        w.clear();
    }
    inline MeshVertex (const MeshVertex & vertex) : p (vertex.p), n (vertex.n) , w(vertex.w) {
    }
    inline virtual ~MeshVertex () {}
    inline MeshVertex & operator = (const MeshVertex & vertex) {
        p = vertex.p;
        n = vertex.n;
        w = vertex.w;
        return (*this);
    }
    // membres :
    Vec3 p; // une position
    Vec3 n; // une normale
    std::vector< double > w; // skinning weights
    std::vector< Vec3 > wColor; // weights Color
};

struct MeshTriangle {
    inline MeshTriangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline MeshTriangle (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline MeshTriangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    inline virtual ~MeshTriangle () {}
    inline MeshTriangle & operator = (const MeshTriangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};




class Mesh {
public:
    std::vector<MeshVertex> V;
    std::vector<MeshTriangle> T;

    Vec3 scalarToRGB( float scalar_value ) //Scalar_value ∈ [0, 1]
    {
        Vec3 rgb;
        float H = scalar_value*360., S = 1., V = 0.85,
                P, Q, T,
                fract;

        (H == 360.)?(H = 0.):(H /= 60.);
        fract = H - floor(H);

        P = V*(1. - S);
        Q = V*(1. - S*fract);
        T = V*(1. - S*(1. - fract));

        if      (0. <= H && H < 1.)
            {rgb[0] = V; rgb[1] = T; rgb[2] = P;}
        else if (1. <= H && H < 2.)
            {rgb[0] = Q; rgb[1] = V; rgb[2] = P;}
        else if (2. <= H && H < 3.)
            {rgb[0] = P; rgb[1] = V; rgb[2] = T;}
        else if (3. <= H && H < 4.)
            {rgb[0] = P; rgb[1] = Q; rgb[2] = V;}
        else if (4. <= H && H < 5.)
            {rgb[0] = T; rgb[1] = P; rgb[2] = V;}
        else if (5. <= H && H < 6.)
            {rgb[0] = V; rgb[1] = P; rgb[2] = Q;}
        else
            {rgb[0] = 0.; rgb[1] = 0.; rgb[2] = 0.;}

        return rgb;
    }

    void loadOFF (const std::string & filename);
    void recomputeNormals ();


    void computeSkinningWeights( Skeleton const & skeleton ) {
        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :

        // Indications:
        // you should compute weights for each vertex w.r.t. the skeleton bones
        // so each vertex will have B weights (B = number of bones)
        // these weights shoud be stored in vertex.w:

        std::vector< Articulation > articulations = skeleton.articulations;
        std::vector< Bone > bones = skeleton.bones;

        unsigned int bonesSize = bones.size();

        for( unsigned int i = 0 ; i < V.size() ; ++i ) {
            MeshVertex & vertex = V[ i ];
            double weightSum = 0.;
            for (unsigned int j = 0; j < bonesSize; ++j)
            {
                unsigned int n = 1;
                double weight;
                Articulation a0 = articulations[ bones[j].joints[0] ];
                Articulation a1 = articulations[ bones[j].joints[1] ];
                Vec3 p = vertex.p;

                Vec3 v1 = (a1.p - a0.p);
                v1.normalize();
                Vec3 v2 = p - a0.p;

                Vec3 v1Prime = a0.p - a1.p;
                Vec3 v2Prime = p - a1.p;

                float dot1 = Vec3::dot(v1, v2);
                float dot2 = Vec3::dot(v1Prime, v2Prime);

                Vec3 pPrime1 = a0.p + dot1 * v1;

                if(dot1 <= 0) weight = pow(1./(p - a0.p).length(), n);
                else if(dot2 <= 0) weight = pow(1./(p - a1.p).length(), n);
                else weight = pow(1./(pPrime1 - p).length(), n);
                vertex.w.push_back(weight);
                weightSum += weight;
            }
            for (int i = 0; i < vertex.w.size(); ++i)
            {
                vertex.w[i] /= weightSum;
                vertex.wColor.push_back(scalarToRGB(vertex.w[i]));
            }
        }
    }

    void draw(int displayedBone) const {
        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (v.p[0], v.p[1], v.p[2]);
                glColor3f(v.wColor[displayedBone][0], v.wColor[displayedBone][1], v.wColor[displayedBone][2]);
            }
        glEnd ();
    }

    void drawTransformedMesh( SkeletonTransformation const & transfo) const {
        std::vector< Vec3 > newPositions( V.size() );

        //---------------------------------------------------//
        //---------------------------------------------------//
        // code to change :
        std::vector< BoneTransformation > bonesTransfo = transfo.boneTransformations;
        unsigned int bonesSize = bonesTransfo.size();

        for( unsigned int i = 0 ; i < V.size() ; ++i ) {
            Vec3 p = V[i].p;
            MeshVertex vertex = V[ i ];
            Vec3 newP = {0,0,0};

            // Indications:
            // you should use the skinning weights to blend the transformations of the vertex position by the bones.

            for (int j = 0; j < bonesSize; ++j)
            {
                Mat3 boneRotation = transfo.boneTransformations[j].worldSpaceRotation;
                Vec3 boneTranslation = transfo.boneTransformations[j].worldSpaceTranslation;
                newP += vertex.w[j] * (boneRotation * p + boneTranslation);
            }


            newPositions[ i ] = newP;

        }
        //---------------------------------------------------//
        //---------------------------------------------------//
        //---------------------------------------------------//

        glEnable(GL_LIGHTING);
        glBegin (GL_TRIANGLES);
        for (unsigned int i = 0; i < T.size (); i++)
            for (unsigned int j = 0; j < 3; j++) {
                const MeshVertex & v = V[T[i].v[j]];
                Vec3 p = newPositions[ T[i].v[j] ];
                glNormal3f (v.n[0], v.n[1], v.n[2]);
                glVertex3f (p[0], p[1], p[2]);
            }
        glEnd ();
    }
};



#endif
