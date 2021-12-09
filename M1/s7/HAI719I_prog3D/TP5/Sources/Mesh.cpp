#define _USE_MATH_DEFINES

#include "Mesh.h"

#include <cmath>
#include <algorithm>

using namespace std;

Mesh::~Mesh () {
	clear ();
}

void Mesh::computeBoundingSphere (glm::vec3 & center, float & radius) const {
	center = glm::vec3 (0.0);
	radius = 0.f;
	for (const auto & p : m_vertexPositions)
		center += p;
	center /= m_vertexPositions.size ();
	for (const auto & p : m_vertexPositions)
		radius = std::max (radius, distance (center, p));
}

void Mesh::recomputePerVertexNormals (bool angleBased) {
	m_vertexNormals.clear ();
	// Change the following code to compute a proper per-vertex normal
        m_vertexNormals.resize (m_vertexPositions.size (), glm::vec3 (0.0, 0.0, 0.0));

        for( unsigned int tIt = 0 ; tIt < m_triangleIndices.size() ; ++tIt ) {
            glm::uvec3 t = m_triangleIndices[ tIt ];
            glm::vec3 n_t = glm::cross( m_vertexPositions[t[1]] - m_vertexPositions[t[0]] , m_vertexPositions[t[2]] - m_vertexPositions[t[0]] );
            m_vertexNormals[ t[0] ] += n_t;
            m_vertexNormals[ t[1] ] += n_t;
            m_vertexNormals[ t[2] ] += n_t;
        }
        for( unsigned int nIt = 0 ; nIt < m_vertexNormals.size() ; ++nIt ) {
            glm::normalize( m_vertexNormals[nIt] );
        }
}

void Mesh::recomputePerVertexTextureCoordinates() {
    m_vertexTexCoords.clear ();
    // Change the following code to compute a proper per-vertex texture coordinates
    m_vertexTexCoords.resize (m_vertexPositions.size (), glm::vec2 (0.0, 0.0));

    float xMin = FLT_MAX , xMax = FLT_MIN;
    float yMin = FLT_MAX , yMax = FLT_MIN;
    for ( glm::vec3 & p : m_vertexPositions) {
        xMin = std::min( xMin , p[0] );
        xMax = std::max( xMax , p[0] );
        yMin = std::min( yMin , p[1] );
        yMax = std::max( yMax , p[1] );
    }
    for( unsigned int pIt = 0 ; pIt < m_vertexTexCoords.size() ; ++pIt ) {
        m_vertexTexCoords[ pIt ] = glm::vec2( (m_vertexPositions[pIt][0] - xMin)/(xMax-xMin) , (m_vertexPositions[pIt][1] - yMin)/(yMax-yMin) );
    }
}

void Mesh::addPlan(float square_half_side) {
    m_vertexPositions.push_back(glm::vec3 (-square_half_side,-square_half_side , -1));
    m_vertexPositions.push_back(glm::vec3 (+square_half_side,-square_half_side , -1));
    m_vertexPositions.push_back(glm::vec3 (+square_half_side,+square_half_side , -1));
    m_vertexPositions.push_back(glm::vec3 (-square_half_side,+square_half_side , -1));

    m_vertexTexCoords.push_back(glm::vec2 (0.0, 0.0));
    m_vertexTexCoords.push_back(glm::vec2 (1.0, 0.0));
    m_vertexTexCoords.push_back(glm::vec2 (1.0, 1.0));
    m_vertexTexCoords.push_back(glm::vec2 (0.0, 1.0));

    m_vertexNormals.push_back(glm::vec3 (0,0, 1));
    m_vertexNormals.push_back(glm::vec3 (0,0, 1));
    m_vertexNormals.push_back(glm::vec3 (0,0, 1));
    m_vertexNormals.push_back(glm::vec3 (0,0, 1));

    m_triangleIndices.push_back( glm::uvec3( m_vertexPositions.size() - 4 , m_vertexPositions.size() - 3 , m_vertexPositions.size() - 2 ) );
    m_triangleIndices.push_back( glm::uvec3( m_vertexPositions.size() - 4 , m_vertexPositions.size() - 2 , m_vertexPositions.size() - 1 ) );
}

void Mesh::init () {
	glCreateBuffers (1, &m_posVbo); // Generate a GPU buffer to store the positions of the vertices
	size_t vertexBufferSize = sizeof (glm::vec3) * m_vertexPositions.size (); // Gather the size of the buffer from the CPU-side vector
	glNamedBufferStorage (m_posVbo, vertexBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT); // Create a data store on the GPU
	glNamedBufferSubData (m_posVbo, 0, vertexBufferSize, m_vertexPositions.data ()); // Fill the data store from a CPU array
	
	glCreateBuffers (1, &m_normalVbo); // Same for normal
	glNamedBufferStorage (m_normalVbo, vertexBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT); 
	glNamedBufferSubData (m_normalVbo, 0, vertexBufferSize, m_vertexNormals.data ());
	
	glCreateBuffers (1, &m_texCoordVbo); // Same for texture coordinates
	size_t texCoordBufferSize = sizeof (glm::vec2) * m_vertexTexCoords.size ();
	glNamedBufferStorage (m_texCoordVbo, texCoordBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData (m_texCoordVbo, 0, texCoordBufferSize, m_vertexTexCoords.data ());

	glCreateBuffers (1, &m_ibo); // Same for the index buffer, that stores the list of indices of the triangles forming the mesh
	size_t indexBufferSize = sizeof (glm::uvec3) * m_triangleIndices.size ();
	glNamedBufferStorage (m_ibo, indexBufferSize, NULL, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData (m_ibo, 0, indexBufferSize, m_triangleIndices.data ());
	
	glCreateVertexArrays (1, &m_vao); // Create a single handle that joins together attributes (vertex positions, normals) and connectivity (triangles indices)
	glBindVertexArray (m_vao);

	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, m_posVbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);

	glEnableVertexAttribArray (1);
	glBindBuffer (GL_ARRAY_BUFFER, m_normalVbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (GLfloat), 0);

	glEnableVertexAttribArray (2);
	glBindBuffer (GL_ARRAY_BUFFER, m_texCoordVbo);
	glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof (GLfloat), 0);

	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBindVertexArray (0); // Desactive the VAO just created. Will be activated at rendering time. 
}

void Mesh::render () {
	glBindVertexArray (m_vao); // Activate the VAO storing geometry data
        glDrawElements (GL_TRIANGLES, static_cast<GLsizei> (m_triangleIndices.size () * 3), GL_UNSIGNED_INT, 0);
        // Call for rendering: stream the current GPU geometry through the current GPU program
}

void Mesh::clear () {
	m_vertexPositions.clear ();
	m_vertexNormals.clear ();
	m_vertexTexCoords.clear ();
	m_triangleIndices.clear ();
	if (m_vao) {
		glDeleteVertexArrays (1, &m_vao);
		m_vao = 0;
	}
	if(m_posVbo) {
		glDeleteBuffers (1, &m_posVbo);
		m_posVbo = 0;
	}
	if (m_normalVbo) {
		glDeleteBuffers (1, &m_normalVbo);
		m_normalVbo = 0;
	}
	if (m_texCoordVbo) {
		glDeleteBuffers (1, &m_texCoordVbo);
		m_texCoordVbo = 0;
	}
	if (m_ibo) {
		glDeleteBuffers (1, &m_ibo);
		m_ibo = 0;
	}
}
