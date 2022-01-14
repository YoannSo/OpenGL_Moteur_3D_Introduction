#include "triangle_mesh.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace M3D_ISICG
{
	TriangleMesh::TriangleMesh( const std::string &				  p_name,
								const std::vector<Vertex> &		  p_vertices,
								const std::vector<unsigned int> & p_indices,
								const Material &				  p_material ) :
		_name( p_name ),
		_vertices( p_vertices ), _indices( p_indices ), _material( p_material )
	{
		_vertices.shrink_to_fit();
		_indices.shrink_to_fit();
		_setupGL();
	}

	void TriangleMesh::render( const GLuint p_glProgram ) const
	{
		glBindVertexArray( this->_vao );
		// nous allons envoyez toutes nos valeurs pour calculer l'eclairage
		glProgramUniform3fv(p_glProgram, glGetUniformLocation( p_glProgram, "ambientColor" ), 1, glm::value_ptr( _material._ambient ) );
		glProgramUniform3fv(p_glProgram, glGetUniformLocation( p_glProgram, "difusColor" ), 1, glm::value_ptr( _material._diffuse ) );
		glProgramUniform3fv( p_glProgram,glGetUniformLocation( p_glProgram, "speculaireColor" ),1,glm::value_ptr( _material._specular ) );
		glProgramUniform1f( p_glProgram, glGetUniformLocation( p_glProgram, "shininess" ), _material._shininess );

		// nous allons envoyer toutes nos valeurs pour calculer la texture
		glProgramUniform1i(p_glProgram, glGetUniformLocation( p_glProgram, "_hasDiffuseMap" ), _material._hasDiffuseMap );
		glProgramUniform1i(p_glProgram, glGetUniformLocation( p_glProgram, "_hasSpecularMap" ), _material._hasDiffuseMap );

		// si on a une map, alors il faut bind la texture 
		if ( this->_material._hasDiffuseMap )
			glBindTextureUnit( 1, this->_material._diffuseMap._id );
		if ( this->_material._hasSpecularMap )
			glBindTextureUnit( 2, this->_material._specularMap._id );

		//Debind 
		glBindTextureUnit( 0, this->_material._diffuseMap._id );
		glBindTextureUnit( 0, this->_material._specularMap._id );

		// on draw les elements
		glDrawElements( GL_TRIANGLES, this->_indices.size(), GL_UNSIGNED_INT, 0 );

		glBindVertexArray( 0 );
	}

	void TriangleMesh::cleanGL()
	{	// on clean les vao,vbo etc
		glDisableVertexArrayAttrib( _vao, 0 );
		glDisableVertexArrayAttrib( _vao, 1 );
		glDisableVertexArrayAttrib( _vao, 2 );
		glDisableVertexArrayAttrib( _vao, 3 );
		glDisableVertexArrayAttrib( _vao, 4 );
		glDeleteVertexArrays( 1, &_vao );
		glDeleteBuffers( 1, &_vbo );
		glDeleteBuffers( 1, &_ebo );
	}

	// ici cela va etre differents car il faut calculer un offset
	void TriangleMesh::_setupGL()
	{
		// creation vbo
		glCreateBuffers( 1, &_vbo );
		glNamedBufferData( _vbo, _vertices.size() * sizeof( Vertex ), _vertices.data(), GL_STATIC_DRAW );
		// creation ebo
		glCreateBuffers( 1, &_ebo );
		glNamedBufferData( _ebo, _indices.size() * sizeof( unsigned int ), _indices.data(), GL_STATIC_DRAW );

		// creation vao
		glCreateVertexArrays( 1, &_vao );
		// lie vao et vbo
		glVertexArrayVertexBuffer( _vao, 0, _vbo, 0, sizeof( Vertex ) );

		// chaque id pour un atribut diffenrents
		// 0: Cela va etre pour la position
		glEnableVertexArrayAttrib( _vao, 0 );
		glVertexArrayAttribFormat( _vao,
								   0,
								   3 ,// 3 pour le nombre de valeurs vec3
									
								   GL_FLOAT, // GL_Float car on traite des flottant vec3F
								   GL_FALSE,
								   offsetof( Vertex, _position ) ); // offset a utilisé
		glVertexArrayAttribBinding( _vao, 0, 0 );

		// 1: Cela va etre pour la position
		glEnableVertexArrayAttrib( _vao, 1 );
		glVertexArrayAttribFormat( _vao,1,3,GL_FLOAT ,GL_FALSE,offsetof( Vertex, _normal ) );
		glVertexArrayAttribBinding( _vao, 1, 0 );

		// 2: Cela va etre pour la texCoor
		glEnableVertexArrayAttrib( _vao, 2 );
		glVertexArrayAttribFormat( _vao,2,2,GL_FLOAT,GL_FALSE,offsetof( Vertex, _texCoords ) );
		glVertexArrayAttribBinding( _vao, 2, 0 );

		// 3: Cela va etre pour la tangent
		glEnableVertexArrayAttrib( _vao, 3 );
		glVertexArrayAttribFormat( _vao,3,3,GL_FLOAT,GL_FALSE, offsetof( Vertex, _tangent ) );
		glVertexArrayAttribBinding( _vao, 3, 0 );

		// 4: Cela va etre pour la bitangent
		glEnableVertexArrayAttrib( _vao, 4 );
		glVertexArrayAttribFormat( _vao, 4,3,GL_FLOAT,GL_FALSE, offsetof( Vertex, _bitangent ) );
		glVertexArrayAttribBinding( _vao, 4, 0 );


		// on lie avec le vao
		glVertexArrayElementBuffer( _vao, _ebo );
	}
} // namespace M3D_ISICG
