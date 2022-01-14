#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "lab_work_5.hpp"
#include "utils/random.hpp"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/shaders/";

	LabWork5::~LabWork5()
	{
		_bunny.cleanGL();
		glDeleteProgram( _program );
	}

	bool LabWork5::init()
	{
		std::cout << "Initializing lab work 5..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		glEnable( GL_DEPTH_TEST );

		if ( !_initProgram() )
			return false;

		this->_bunny.load( "Sponza", "data/sponza/sponza.obj" );

		_initCamera();

		glUseProgram( _program );
		this->_uMVPMatrixLoc	  = glGetUniformLocation( this->_program, "uMVPMatrix" );
		this->_uNormalMatrixLoc	  = glGetUniformLocation( this->_program, "uNormalMatrix" );
		this->_uMVMatrixLoc		  = glGetUniformLocation( this->_program, "uMVMatrix" );
		this->_uCameraPositionLoc = glGetUniformLocation( this->_program, "cameraPosition" );
		this->_uViewMatrixLoc	  = glGetUniformLocation( this->_program, "uViewMatrix" );

		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork5::animate( const float p_deltaTime ) {}

	void LabWork5::render()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		Mat4f mvp = this->_camera.getProjectionMatrix() * this->_camera.getViewMatrix() * this->_bunny._transformation;
		Mat4f MVMatrix = this->_camera.getViewMatrix() * this->_bunny._transformation;

		Mat3f normalMatrix = Mat3f( glm::transpose( glm::inverse( MVMatrix ) ) );

		glProgramUniformMatrix4fv( _program, this->_uMVPMatrixLoc, 1, GL_FALSE, glm::value_ptr( mvp ) );
		glProgramUniformMatrix3fv( _program, this->_uNormalMatrixLoc, 1, GL_FALSE, glm::value_ptr( normalMatrix ) );
		glProgramUniformMatrix4fv(
			_program, this->_uViewMatrixLoc, 1, GL_FALSE, glm::value_ptr( this->_camera.getViewMatrix() ) );

		glProgramUniformMatrix4fv( _program, this->_uMVMatrixLoc, 1, GL_FALSE, glm::value_ptr( MVMatrix ) );
		glProgramUniformMatrix3fv(
			_program, this->_uCameraPositionLoc, 1, GL_FALSE, glm::value_ptr( this->_camera.getPos() ) );

		// Clear the color buffer.
		_bunny.render( _program );
	}

	void LabWork5::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				break;
			case SDL_SCANCODE_SPACE: // Print camera info
				_camera.print();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
		}
	}

	void LabWork5::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );

		// Background.
		if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
		{
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		}

		// Camera.
		if ( ImGui::SliderFloat( "fovy", &_fovy, 10.f, 160.f, "%01.f" ) )
		{
			_camera.setFovy( _fovy );
		}
		if ( ImGui::SliderFloat( "Speed", &_cameraSpeed, 0.1f, 10.f, "%01.1f" ) ) {}

		ImGui::End();
	}

	void LabWork5::resize( const int p_width, const int p_height )
	{
		BaseLabWork::resize( p_width, p_height );
		_camera.setScreenSize( p_width, p_height );
	}

	bool LabWork5::_initProgram()
	{
		// ====================================================================
		// Shaders.
		// ====================================================================
		// Create shaders.
		const GLuint vertexShader	= glCreateShader( GL_VERTEX_SHADER );
		const GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		// Get sources from files.
		const std::string vertexShaderSrc	= readFile( _shaderFolder + "mesh_texture.vert" );
		const std::string fragmentShaderSrc = readFile( _shaderFolder + "mesh_texture.frag" );

		// Convert to GLchar *
		const GLchar * vSrc = vertexShaderSrc.c_str();
		const GLchar * fSrc = fragmentShaderSrc.c_str();

		// Compile vertex shader.
		glShaderSource( vertexShader, 1, &vSrc, NULL );
		glCompileShader( vertexShader );
		// Check if compilation is ok.
		GLint compiled;
		glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vertexShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling vertex shader: " << log << std::endl;
			return false;
		}

		// Compile vertex shader.
		glShaderSource( fragmentShader, 1, &fSrc, NULL );
		glCompileShader( fragmentShader );
		// Check if compilation is ok.
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compiled );
		if ( compiled != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fragmentShader, sizeof( log ), NULL, log );
			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			std::cerr << "Error compiling fragment shader: " << log << std::endl;
			return false;
		}
		// ====================================================================

		// ====================================================================
		// Program.
		// ====================================================================
		// Create program.
		_program = glCreateProgram();

		// Attach shaders.
		glAttachShader( _program, vertexShader );
		glAttachShader( _program, fragmentShader );

		// Link program.
		glLinkProgram( _program );
		// Check if link is ok.
		GLint linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &linked );
		if ( !linked )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std::cerr << "Error linking program: " << log << std::endl;
			return false;
		}

		// Shaders are now useless.
		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		// ====================================================================

		// ====================================================================
		// Get uniform locations.
		// ====================================================================

		// ====================================================================

		return true;
	}

	void LabWork5::_initCamera()
	{
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( Vec3f( 0.f, 0.f, 0.2 ) );
	}
} // namespace M3D_ISICG
