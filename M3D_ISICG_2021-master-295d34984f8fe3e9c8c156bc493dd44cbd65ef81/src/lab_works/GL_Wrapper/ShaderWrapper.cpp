#include "ShaderWrapper.h"
#include <iostream>
namespace M3D_ISICG
{
	 void ShaderWrapper::initVertexShader( GLuint vShader, const GLchar * vSrc )
	{ 
				glShaderSource( vShader, 1, &vSrc, NULL ); // on va initialiser notre code source
				glCompileShader( vShader ); // on va le compilier 

	 }
	void ShaderWrapper::initFragmentShader( GLuint fShader, const GLchar * fSrc )
	{ 
			glShaderSource( fShader, 1, &fSrc, NULL );
			glCompileShader( fShader ); }
	void ShaderWrapper::setupShaders( GLuint vShader, const GLchar * vSrc, GLuint fShader, const GLchar * fSrc )
	{
		initVertexShader( vShader, vSrc );
		initFragmentShader( fShader, fSrc );

		GLint compiled;
		glGetShaderiv( vShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vShader, sizeof( log ), NULL, log );
			glDeleteShader( vShader );
			glDeleteShader( fShader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return;
		}
	}
	void ShaderWrapper::attachProgram( GLuint program, GLuint* vShader, GLuint* fShader) 
	{ 
		glAttachShader(program, *vShader ); // on va attcher nos shader a notre programme
		glAttachShader( program, *fShader );
		glLinkProgram(program ); // on va faire une liaison du programme
		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked );
		if ( !linked ) // si ca se passe pas bien
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog(program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return;
		}
		glDeleteShader( *vShader );
		glDeleteShader( *fShader );
	}

		
	void ShaderWrapper::setupVbo(GLuint* VBO, int fullSize, GLvoid* data) {
		glCreateBuffers( 1, VBO ); // on va creer le buffer avec id VBO
		glNamedBufferData( *VBO, fullSize, data, GL_STATIC_DRAW ); // ici on  va remplir notre buffer
	}
	void ShaderWrapper::setupVao( GLuint *	VAO,int	indexVAO,int nbValue,GLenum	type,GLboolean normalized,int offset )
	{
		// CREATION DU VAO
		glCreateVertexArrays( 1, VAO );

		//activer les attributs
		glEnableVertexArrayAttrib( *VAO, indexVAO );

		// On active le format de l'attribut
		glVertexArrayAttribFormat( *VAO, indexVAO, nbValue, type, normalized, offset );
	}
	void ShaderWrapper::linkVboVao(GLuint* VBO, GLuint* VAO, int indexVAO, int offset, int stride) {
		// on va specidier le vbo a lire
		glVertexArrayVertexBuffer( *VAO, indexVAO, *VBO, offset, stride );
	}
}
 // namespace M3D_ISICG