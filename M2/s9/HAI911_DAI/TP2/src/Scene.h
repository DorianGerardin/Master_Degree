#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"

#include <GL/glut.h>

class Scene {
    // put here everything that you want
    std::vector< Mesh > meshes;

public:
    Scene() {}

    void addMesh(std::string const & modelFilename) {
        meshes.resize( meshes.size() + 1 );
        meshes[ meshes.size() - 1 ].loadOFF (modelFilename);
    }

    void draw() const {
        // iterer sur l'ensemble des objets, et faire leur rendu.
        for( unsigned int mIt = 0 ; mIt < meshes.size() ; ++mIt ) {
            Mesh const & mesh = meshes[mIt];
            mesh.draw();
        }
    }
};



#endif
