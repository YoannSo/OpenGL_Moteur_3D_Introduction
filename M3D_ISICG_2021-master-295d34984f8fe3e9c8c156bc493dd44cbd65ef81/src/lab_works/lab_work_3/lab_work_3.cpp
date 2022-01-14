#include "lab_work_3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include "lab_works/GL_Wrapper/ShaderWrapper.h"

#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";

	LabWork3::~LabWork3() { 
		glDeleteProgram( _program );

	}

	bool LabWork3::init()
	{
		std::cout << "Initializing lab work 3..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		if ( !_initProgram() )
			return false;

		_cube = _createCube();
		// on va creer les points de notre cube
		Vec3f pointA = Vec3f( -0.5, 0.5, -0.5 );
		Vec3f pointB = Vec3f( 0.5, 0.5, -0.5 );
		Vec3f pointC = Vec3f( 0.5, -0.5, -0.5 );
		Vec3f pointD = Vec3f( -0.5, -0.5, -0.5 );
		Vec3f pointE = Vec3f( -0.5, 0.5, 0.5f );
		Vec3f pointF = Vec3f( 0.5, 0.5, 0.5f );
		Vec3f pointG = Vec3f( 0.5, -0.5, 0.5f );
		Vec3f pointH = Vec3f( -0.5, -0.5, 0.5f );

		// on range les points de notre cube
		_cube._vertices = { pointA, pointB, pointC, pointD, pointE, pointF, pointG, pointH };

		// on va avoir des couleurs random pour notre cube
		for ( int i = 0; i < this->_cube._vertices.size(); i++ )
		{
			_cube._vertexColors.push_back( getRandomVec3f() );
		}
		
		// ici on va trier notre tableau en fonction de l'ebo, c'est a dire les triangles qu'on va devoir afficher 1 face = 2 triangle = 6 ponts
		_cube._indices = { 0, 1, 3, 1, 2, 3, //front
			1, 5, 6, 1, 2, 6,//coté
			4, 5, 6, 4, 7, 6,//coté
			 0, 4, 7, 0, 3, 7,
			4, 5, 1, 4, 0, 1,
			2, 6, 7, 7, 3, 2 };

		// on va initiliser la camera et les buffers
		_initCamera();
		_initBuffers();

		glUseProgram( _program );
		
		// on va update les matrix lié a la camera
		_updateViewMatrix();
		_updateProjectionMatrix();

		glEnable( GL_DEPTH_TEST );
		std::cout << "Done!" << std::endl;
			return true;
	}

	void LabWork3::animate( const float p_deltaTime )
	{

		// ici comme l'exercice precendent mais au lieu de faire subir une translation on fait une rotation, il faut donc envoyer une mat4 
		_cube._transformation
			= glm::rotate( _cube._transformation, glm::radians( p_deltaTime*5 ), glm::vec3( 0.0f, 0.5f, 1.0f ) );
		glProgramUniformMatrix4fv( _program, _uModelMatrixLoc, 1, GL_FALSE, &_cube._transformation[ 0 ][ 0 ] );
		
	}

	void LabWork3::render()
	{
		// Clear the color buffer.
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBindVertexArray( this->_cube._vao );

		// on va afficher tous les elements
		glDrawElements( GL_TRIANGLES, this->_cube._indices.size(), GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );

		
	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}

			// fonction qui va dire quand on se deplace on regarde avec look at
			_camera.setLookAt( Vec3f( 0.f, 0.f, 0.f ) );
			
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}
	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );

		// Background.
		if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		// Camera.
		if ( ImGui::SliderFloat( "fovy", &_fovy, 10.f, 160.f, "%01.f" ) )
		{
			_camera.setFovy( _fovy );
			_updateProjectionMatrix();
		}
		if ( ImGui::SliderFloat( "Speed", &_cameraSpeed, 0.1f, 10.f, "%01.1f" ) )
		{
			_camera.setFovy( _fovy );
			_updateProjectionMatrix();
		}

		ImGui::End();
	}

	void LabWork3::resize( const int p_width, const int p_height )
	{
		BaseLabWork::resize( p_width, p_height );
		_camera.setScreenSize( p_width, p_height );
	}

	bool LabWork3::_initProgram()
	{
		// creation des shader et du programme comme dh'abitude 
		const GLuint vertexShader	= glCreateShader( GL_VERTEX_SHADER );
		const GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		const std::string vertexShaderSrc	= readFile( _shaderFolder + "triangle.vert" );
		const std::string fragmentShaderSrc = readFile( _shaderFolder + "triangle.frag" );

		const GLchar * vSrc = vertexShaderSrc.c_str();
		const GLchar * fSrc = fragmentShaderSrc.c_str();

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		GLuint vShader = glCreateShader( GL_VERTEX_SHADER );
		GLuint fShader = glCreateShader( GL_FRAGMENT_SHADER );

		ShaderWrapper::setupShaders( vShader, vSrc, fShader, fSrc );

		this->_program = glCreateProgram();
		ShaderWrapper::attachProgram( this->_program, &vShader, &fShader );

		
		// on va recuperer la localisation de nos variable uniform qui sont des le vertex et fragment shader
		_uModelMatrixLoc	  = glGetUniformLocation( _program, "uModelMatrix" );
		_uViewMatrixLoc		  = glGetUniformLocation( _program, "uViewMatrix" );
		_uProjectionMatrixLoc = glGetUniformLocation( _program, "uProjectionMatrix" );

		return true;
	}

	void LabWork3::_initCamera()
	{
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( Vec3f( 0.f, 1.f, 3.f ) );
	}

	void LabWork3::_initBuffers()
	{
		
		
		/* INIT VBO POSITION AND VAO*/
		glCreateBuffers( 1,&this->_cube._vboPositions);
		glNamedBufferData( this->_cube._vboPositions,
						   this->_cube._vertices.size() * sizeof( Vec3f ),
						   this->_cube._vertices.data(),
						   GL_STATIC_DRAW );

		/* INIT VBO COLOR
		 */
		glCreateBuffers( 1, &this->_cube._vboColors );
		glNamedBufferData( this->_cube._vboColors,
						   this->_cube._vertexColors.size() * sizeof( Vec3f ),
						   this->_cube._vertexColors.data(),
						   GL_STATIC_DRAW );

		// comme d'habitude creation vao,vbo,ebo ........
		ShaderWrapper::setupVbo(
			&this->_cube._vboPositions, this->_cube._vertices.size() * sizeof( Vec3f ), this->_cube._vertices.data() );

		ShaderWrapper::setupVbo( 
			&this->_cube._vboColors,this->_cube._vertexColors.size() * sizeof( Vec2f ), this->_cube._vertexColors.data() );

		glCreateVertexArrays( 1, &this->_cube._vao);


		glEnableVertexArrayAttrib( this->_cube._vao, 0 );
		glEnableVertexArrayAttrib( this->_cube._vao, 1 );
		glVertexArrayAttribFormat( this->_cube._vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( this->_cube._vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( this->_cube._vao, 0, this->_cube._vboPositions, 0, sizeof( Vec3f ) );
		glVertexArrayVertexBuffer( this->_cube._vao, 1, this->_cube._vboColors, 0, sizeof( Vec3f ) );



		glCreateBuffers( 1, &this->_cube._ebo );
		glNamedBufferData( this->_cube._ebo,
						   this->_cube._indices.size() * sizeof( int ),
						   this->_cube._indices.data(),
						   GL_STATIC_DRAW );
		glVertexArrayElementBuffer( this->_cube._vao, this->_cube._ebo );
	}
	// update la ViewMatrix du shader a l'aide de l'uniform
	void LabWork3::_updateViewMatrix() {
		_uViewMatrixLoc = glGetUniformLocation( _program, "viewMatrix" );
		glProgramUniformMatrix4fv( _program, _uViewMatrixLoc, 1, GL_FALSE, &_camera.getViewMatrix()[0][0] );
		
	}
	// update la ProjectionMatrix du shader a l'aide de l'uniform

	void LabWork3::_updateProjectionMatrix()
	{
		_uProjectionMatrixLoc = glGetUniformLocation( _program, "uProjectionMatrix" );
		glProgramUniformMatrix4fv( _program, _uProjectionMatrixLoc, 1, GL_FALSE, &_camera.getProjectionMatrix()[ 0 ][ 0 ] );

	}
	//creation de notre cube, on va lui faire subir une transformation de sclaing, et envoyer la model matrix
	LabWork3::Mesh LabWork3::_createCube() {
		Mesh	  returned		 = Mesh();
		glm::mat4 transformation = glm::mat4( 1.f );
		transformation			 = glm::scale( transformation, glm::vec3( 0.8f ) );
		returned._transformation = transformation;
		_uModelMatrixLoc		 = glGetUniformLocation( _program, "modelMatrix" );
		glProgramUniformMatrix4fv( _program, _uModelMatrixLoc, 1, GL_FALSE, &transformation[ 0 ][ 0 ] );
		
		return returned;
	}
} // namespace M3D_ISICG
