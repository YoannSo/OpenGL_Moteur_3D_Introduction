#ifndef __WRAPPER__
#define __WRAPPER__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
// Wrapper qui va nous permettre pour le TP1 d'amiliore la lisibilté du code
namespace M3D_ISICG
{
	class ShaderWrapper
	{
	  public:
		 ShaderWrapper();
		~ShaderWrapper();
		 
		void static initVertexShader( GLuint vShader, const GLchar * vSrc ); // fonction pour init vertexShader
		void static initFragmentShader( GLuint fShader, const GLchar * fSrc );// fonction pour init Fragment shader
		void static setupShaders( GLuint vShader, const GLchar * vSrc, GLuint fShader, const GLchar * fSrc ); // fonction pour setup shader
		void static attachProgram( GLuint program, GLuint* vShader, GLuint* fShader ); // pour attacher le programme
		void static setupVbo( GLuint * VBO, int fullSize, GLvoid * data); // pour mettre en place le vbo
		void static setupVao( GLuint * VAO, int indexVAO, int nbValue, GLenum type, GLboolean normalized, int offset); // mettre en place le VAO
		void static linkVboVao( GLuint * VBO, GLuint * VAO, int indexVAO, int offset, int stride ); // associer le VBo au VAO
		
	};
} // namespace M3D_ISICG
#endif