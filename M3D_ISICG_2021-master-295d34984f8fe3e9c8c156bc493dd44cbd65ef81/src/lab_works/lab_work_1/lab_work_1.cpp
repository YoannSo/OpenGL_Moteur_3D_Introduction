#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>
#include "lab_works/GL_Wrapper/ShaderWrapper.h"


namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1() { 
		// il faut penser a detruire nos programmes/vbo/vao
		glDeleteProgram( this->program );
		glDeleteBuffers( 1, &this->idVBO );
		glDisableVertexArrayAttrib(this->idVAO,0);
		glDeleteVertexArrays( 1, &this->idVAO );
	}

	bool LabWork1::init()
	{
		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );
		const GLchar * vSrc	= vertexShaderStr.c_str();
		const GLchar * fSrc	= fragmentShaderStr.c_str();

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		// setup de nos shader a l'aide de notre wrapper
		GLuint	vShader = glCreateShader( GL_VERTEX_SHADER );
		GLuint	fShader	 = glCreateShader( GL_FRAGMENT_SHADER );
		ShaderWrapper::setupShaders( vShader, vSrc, fShader, fSrc );

		// setup de notre programme a l'aide de notre wrapper
		this->program = glCreateProgram();
		ShaderWrapper::attachProgram( this->program, &vShader, &fShader );

		// on va construire un tableau de sommet (3 sommets pour 1 triangle)
		this->pSommets =   std::vector<Vec2f>();
		Vec2f pointA   =  Vec2f( -0.5, 0.5 );
		Vec2f pointB   = Vec2f( 0.5, 0.5 );
		Vec2f pointC   = Vec2f( 0.5, -0.5 );

		this->pSommets.push_back( pointA );
		this->pSommets.push_back( pointB );
		this->pSommets.push_back( pointC );
		/* this->nbPoints				   = 12;
		std::vector<Vec2f> allCirclePoints = std::vector<Vec2f>();
		this->pSommets					   = std::vector<Vec2f>();

		float alpha;
		for ( int i = 0; i < nbPoints; i++ )
		{
			alpha = ( 2 * i * PI ) / nbPoints;

			allCirclePoints.push_back( Vec2f( glm::cos( alpha ), glm::sin( alpha ) ) );
		}
		for ( int i = 0; i < nbPoints; i++ )
		{
			if ( i == nbPoints - 1 )
			{
				this->pSommets.push_back( Vec2f( 0.0, 0.0 ) );
				this->pSommets.push_back( allCirclePoints.data()[ i ] );
				this->pSommets.push_back( allCirclePoints.data()[ 0 ] );
			}
			else
			{
				this->pSommets.push_back( Vec2f( 0.0, 0.0 ) );
				this->pSommets.push_back( allCirclePoints.data()[ i ] );
				this->pSommets.push_back( allCirclePoints.data()[ i + 1 ] );
			}
			i++;
		}*/

		// a l'aide du wrapper on va seup le vbo et le vao en utilisant les bons parametre(la taille des données, les données, les id vao,vbo), ainsi que les links
		ShaderWrapper::setupVbo( &this->idVBO, this->pSommets.size() * sizeof( Vec2f ), this->pSommets.data() );
		ShaderWrapper::setupVao( &this->idVAO, 0, 2, GL_FLOAT, GL_FALSE,0);
		ShaderWrapper::linkVboVao( &this->idVBO, &this->idVAO, 0, 0, sizeof( Vec2f ) );

		//utilisation du programme
		glUseProgram( this->program );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render()
	{	// netoyage du framebuffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		//Lier le Vao au programme
		glBindVertexArray( this->idVAO );
		// on va la geometrier au pipeline, ici on a specifier que l'on voulait des triangles et qu'on avait 3 sommets
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindVertexArray( 0 );
	}

	void LabWork1::handleEvents( const SDL_Event & p_event ) {}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
