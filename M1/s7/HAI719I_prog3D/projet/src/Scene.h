#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Square.h"
#include <math.h>  

#include <GL/glut.h>

#define MESH_INTERSECTION 0
#define SPHERE_INTERSECTION 1
#define SQUARE_INTERSECTION 2

enum LightType {
    LightType_Spherical,
    LightType_Quad
};


struct Light {
    Vec3 material;
    bool isInCamSpace;
    LightType type;

    Vec3 pos;
    float radius;

    Mesh quad;

    float powerCorrection;

    Light() : powerCorrection(1.0) {}

    float randomFloat(float min, float max)    
    {    
        return (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1));    
    }

    std::vector<Vec3> randomPointsForSquare(size_t n) {

        //int randNum = rand()%(max-min + 1) + min;
        std::vector<Vec3> result;
        Vec3 bottom_left = quad.vertices[0].position;
        Vec3 top_right = quad.vertices[2].position;

        float minX = min(top_right[0], bottom_left[0]);
        float minY = min(top_right[1], bottom_left[1]);
        float minZ = min(top_right[2], bottom_left[2]);

        float maxX = max(top_right[0], bottom_left[0]);
        float maxY = max(top_right[1], bottom_left[1]);
        float maxZ = max(top_right[2], bottom_left[2]);
        
        for (unsigned int i = 0; i < n; ++i)
        {
            float x = randomFloat(minX, maxX);
            float y = randomFloat(minY, maxY);
            float z = randomFloat(minZ, maxZ);
            result.push_back(Vec3(x, y, z));
        }
        return result;
    }

};

struct RaySceneIntersection{
    bool intersectionExists;
    unsigned int typeOfIntersectedObject;
    unsigned int objectIndex;
    float t;
    RayTriangleIntersection rayMeshIntersection;
    RaySphereIntersection raySphereIntersection;
    RaySquareIntersection raySquareIntersection;
    RaySceneIntersection() : intersectionExists(false) , t(FLT_MAX) {}
};



class Scene {
    std::vector< Mesh > meshes;
    std::vector< Sphere > spheres;
    std::vector< Square > squares;
    std::vector< Light > lights;

public:


    Scene() {
    }

    void draw() {
        // iterer sur l'ensemble des objets, et faire leur rendu :
        for( unsigned int It = 0 ; It < meshes.size() ; ++It ) {
            Mesh const & mesh = meshes[It];
            mesh.draw();
        }
        for( unsigned int It = 0 ; It < spheres.size() ; ++It ) {
            Sphere const & sphere = spheres[It];
            sphere.draw();
        }
        for( unsigned int It = 0 ; It < squares.size() ; ++It ) {
            Square const & square = squares[It];
            square.draw();
        }
    }


    RaySceneIntersection computeShadows(Ray const & ray) {
        RaySceneIntersection result;

        //Spheres
        RaySphereIntersection rSpherei;
        int nbSpheres = this->spheres.size();
        for (int i = 0; i < nbSpheres; ++i)
        {
            rSpherei = this->spheres[i].intersect(ray);
            if (rSpherei.intersectionExists && rSpherei.t < result.t) {
                result.raySphereIntersection = rSpherei;
                result.t = rSpherei.t;
                result.objectIndex = i;
                result.intersectionExists = true;
                result.typeOfIntersectedObject = SPHERE_INTERSECTION;
                return result;
            }
        }

        //square
        RaySquareIntersection rSquarei;
        int nbSquares = this->squares.size();
        for (int i = 0; i < nbSquares; ++i)
        {
            rSquarei = this->squares[i].intersect(ray);
            if (rSquarei.intersectionExists && rSquarei.t < result.t) {
                result.raySquareIntersection = rSquarei;
                result.t = rSquarei.t;
                result.objectIndex = i;
                result.intersectionExists = true;
                result.typeOfIntersectedObject = SQUARE_INTERSECTION;
                return result;
            }
        }

        return result;

    }


    RaySceneIntersection computeIntersection(Ray const & ray) {
        RaySceneIntersection result;
        //TODO calculer les intersections avec les objets de la scene et garder la plus proche

        //meshes
        /*RayTriangleIntersection rMeshi;
        int nbMeshes = this->meshes.size();
        for (int i = 0; i < nbMeshes; ++i)
        {
            rMeshi = this->meshes[i].intersect(ray);
            if (rMeshi.intersectionExists && rMeshi.t < result.t) {
                result.rayMeshIntersection = rMeshi;
                result.t = rMeshi.t;
                result.objectIndex = i;
                result.intersectionExists = true;
                result.typeOfIntersectedObject = MESH_INTERSECTION;
            }
        }*/

        //Spheres
        RaySphereIntersection rSpherei;
        int nbSpheres = this->spheres.size();
        for (int i = 0; i < nbSpheres; ++i)
        {
            rSpherei = this->spheres[i].intersect(ray);
            if (rSpherei.intersectionExists && rSpherei.t < result.t) {
                result.raySphereIntersection = rSpherei;
                result.t = rSpherei.t;
                result.objectIndex = i;
                result.intersectionExists = true;
                result.typeOfIntersectedObject = SPHERE_INTERSECTION;
            }
        }

        //square
        RaySquareIntersection rSquarei;
        int nbSquares = this->squares.size();
        for (int i = 0; i < nbSquares; ++i)
        {
            rSquarei = this->squares[i].intersect(ray);
            if (rSquarei.intersectionExists && rSquarei.t < result.t) {
                result.raySquareIntersection = rSquarei;
                result.t = rSquarei.t;
                result.objectIndex = i;
                result.intersectionExists = true;
                result.typeOfIntersectedObject = SQUARE_INTERSECTION;
            }
        }

        return result;
    }




    Vec3 rayTraceRecursive( Ray ray , int NRemainingBounces ) {

        //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        Vec3 color = Vec3(0, 0, 0);

        RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        if(raySceneIntersection.intersectionExists) {

            Vec3 N, L, R, V, intersection, Ka, Kd, Ks;
            float LN, RV;
            float Ia, Isa;
            float Id, Isd;
            float Is, Iss;
            double shininess;

            switch(raySceneIntersection.typeOfIntersectedObject) {

                case(SPHERE_INTERSECTION):

                intersection = raySceneIntersection.raySphereIntersection.intersection;
                N = raySceneIntersection.raySphereIntersection.normal;
                Ka = this->spheres[raySceneIntersection.objectIndex].material.ambient_material;
                Kd = this->spheres[raySceneIntersection.objectIndex].material.diffuse_material;
                Ks = this->spheres[raySceneIntersection.objectIndex].material.specular_material;
                shininess = this->spheres[raySceneIntersection.objectIndex].material.shininess;

                break;

                case(SQUARE_INTERSECTION):

                intersection = raySceneIntersection.raySquareIntersection.intersection;
                N = raySceneIntersection.raySquareIntersection.normal;
                Ka = this->squares[raySceneIntersection.objectIndex].material.ambient_material;
                Kd = this->squares[raySceneIntersection.objectIndex].material.diffuse_material;
                Ks = this->squares[raySceneIntersection.objectIndex].material.specular_material;
                shininess = this->squares[raySceneIntersection.objectIndex].material.shininess;

                break;

                case(MESH_INTERSECTION):
                    break;
            } 

            int nbShadowRays = 20;
            float percentShadow = 0;
            int nbShadowIntersect = 0;
        
            for (unsigned int j = 0; j < lights.size(); ++j)
            {

                L = lights[j].pos - intersection;
                L.normalize();
                N.normalize();
                LN = (Vec3::dot(L, N));
                if(LN < 0){
                    LN = 0.;
                }

                R = 2 * LN * N - L;
                R.normalize();
                V = ray.origin() - intersection; 
                V.normalize();
                RV = (Vec3::dot(R, V));

                std::vector<Vec3> randomPointsSquare = lights[j].randomPointsForSquare(nbShadowRays);
                for (int i = 0; i < nbShadowRays; ++i)
                {
                    Ray rayTestShadow = Ray(intersection, randomPointsSquare[i] - intersection);
                    RaySceneIntersection ombre = computeShadows(rayTestShadow);
                    if(ombre.intersectionExists && ombre.t < 1 && ombre.t > 0.01) {
                        nbShadowIntersect++;
                    }
                }
                if (nbShadowRays != 0) {
                    percentShadow = 1 - ((float)nbShadowIntersect/(float)nbShadowRays);
                }
                nbShadowIntersect = 0;

                for (unsigned int rgb = 0; rgb < 3; ++rgb){
                    Isa = lights[j].material[rgb];
                    Isd = lights[j].material[rgb];
                    Iss = lights[j].material[rgb];

                    Ia = Isa * Ka[rgb];
                    Id = Isd * Kd[rgb] * LN * percentShadow;
                    Is = Iss * Ks[rgb] * pow(RV, shininess) * percentShadow;

                    color[rgb] += (Ia + Id + Is);
                }
            
            }
        }

        return color;
    }


    Vec3 rayTrace( Ray const & rayStart ) {
        //TODO appeler la fonction recursive
        Vec3 color = rayTraceRecursive(rayStart, 1); 

        return color;
    }

    void setup_single_sphere() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }
        {
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(0. , 0. , 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,1.,1 );
            s.material.specular_material = Vec3( 0.2,0.2,0.2 );
            s.material.shininess = 20;
        }
    }

    void setup_single_square() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        {
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.8,0.8,0.8 );
            s.material.specular_material = Vec3( 0.8,0.8,0.8 );
            s.material.shininess = 20;
        }
    }

    void setup_cornell_box(){
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3( 0.0, 1.5, 0.0 );
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            Square s = Square(Vec3(-0.5, 1.5, 0.5), Vec3(1, 0, 0.), Vec3(0., 0., 1.), 1., 1.);
            light.quad = s;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        { //Back Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.5,0.5,1.);
            s.material.specular_material = Vec3( 0.5,0.5,1. );
            s.material.shininess = 16;
        }

        { //Left Wall

            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.rotate_y(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.,0.,0. );
            s.material.specular_material = Vec3( 1.,0.,0. );
            s.material.shininess = 16;
        }

        { //Right Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.0,1.0,0.0 );
            s.material.specular_material = Vec3( 0.0,1.0,0.0 );
            s.material.shininess = 16;
        }

        { //Floor
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,0.5,0.5 );
            s.material.specular_material = Vec3( 1.0,0.5,0.5 );
            s.material.shininess = 16;
        }

        { //Ceiling
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1., 1., 1. );
            s.material.specular_material = Vec3( 1., 1., 1. );
            s.material.shininess = 16;
        }

/*        { //Front Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(180);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,1.0,1.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }*/


        { //GLASS Sphere

            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(1.0, -1.25, 0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,0.,0. );
            s.material.specular_material = Vec3( 1.,0.,0. );
            s.material.shininess = 16;
            s.material.transparency = 1.0;
            s.material.index_medium = 1.4;
        }


        { //MIRRORED Sphere
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(-1.0, -1.25, -0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Glass;
            s.material.diffuse_material = Vec3( 1.,1.,1. );
            s.material.specular_material = Vec3(  1.,1.,1. );
            s.material.shininess = 16;
            s.material.transparency = 0.;
            s.material.index_medium = 0.;
        }

    }

};



#endif
