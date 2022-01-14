#include "lab_work_2.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>
#include "lab_works/GL_Wrapper/ShaderWrapper.h"


namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2() { 
		// il faut penser a clear nos programmes, vbo et vao
		glDeleteProgram( this->program );
		glDeleteBuffers( 1, &this->idVBO );
		glDeleteBuffers( 1, &this->colorVBO );
		glDisableVertexArrayAttrib(this->idVAO,0);
		glDisableVertexArrayAttrib( this->idVAO, 1 );
		glDeleteVertexArrays( 1, &this->idVAO );
	}

	bool LabWork2::init()
	{
		 std::cout << "Initializing lab work 2..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		const std::string vertexShaderStr = readFile( _shaderFolder + "lw2.vert" );
		const std::string fragmentShaderStr = readFile( _shaderFolder + "lw2.frag" );
		const GLchar * vSrc	= vertexShaderStr.c_str();
		const GLchar * fSrc	= fragmentShaderStr.c_str();

		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		GLuint	vShader = glCreateShader( GL_VERTEX_SHADER );
		GLuint	fShader	 = glCreateShader( GL_FRAGMENT_SHADER );
	
		ShaderWrapper::setupShaders( vShader, vSrc, fShader, fSrc );

		
		this->program = glCreateProgram();
		ShaderWrapper::attachProgram( this->program, &vShader, &fShader );

		// Creation de notre rectangle, rectangle => 2 triangles
		this->pSommets =   std::vector<Vec2f>();
		this->idSommets = std::vector<int>();
		this->colors	= std::vector<Vec3f>();
		Vec2f pointA   =  Vec2f( -0.5, 0.5 );
		Vec2f pointB   = Vec2f( 0.5, 0.5 );
		Vec2f pointC   = Vec2f( 0.5, -0.5 );
		Vec2f pointD = Vec2f( -0.5, -0.5 );
		


		this->pSommets.push_back( pointA );
		this->pSommets.push_back( pointB );
		this->pSommets.push_back( pointC );
		this->pSommets.push_back( pointD );
		
		
		// ici c'est pour savoirt comment afficher nos triangles, triangle(0,1,2) et (3,0,2)
		this->idSommets = { 0, 1, 2, 3, 0, 2 };
		
		
		Vec3f red	  = Vec3f( 1.f, 0.f, 0.f );
		Vec3f green	  = Vec3f( 0.f, 1.f, 0.f );
		Vec3f blue	  = Vec3f( 0.f, 0.f, 1.f );
		Vec3f magenta = Vec3f( 1.f, 0.f, 1.f );
		// on fait 4 couleurs pour 4 coins

		colors		  = { red, green, blue, magenta };

		// nous allons avoir deux uniform afin de faire notre translation du rectangle de gauche a droite, et une pour la luminosité des couleurs
		this->uTranslationXUniform=glGetUniformLocation( this->program, "uTranslationX" );
		this->uLuminosity = glGetUniformLocation( this->program, "uLuminosity" );
		
		//on envoie a la valeur uLuminosity dans le shader le chiffre 1
		glProgramUniform1f( program, this->uLuminosity, 1 );

		 
		// creation de nos deux vbo
		
		ShaderWrapper::setupVbo( &this->colorVBO, this->colors.size() * sizeof( Vec3f ), this->colors.data() );
		ShaderWrapper::setupVbo( &this->idVBO, this->pSommets.size() * sizeof( Vec2f ), this->pSommets.data() );


		// creatio n du vao
		glCreateVertexArrays( 1, &this->idVAO );


		// un active deux attributs pour notre couleurs, et nos indices
		glEnableVertexArrayAttrib( this->idVAO, 0 );
		glEnableVertexArrayAttrib( this->idVAO, 1 );

		// on attribut le format
		glVertexArrayAttribFormat( this->idVAO, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayAttribFormat( this->idVAO, 1, 3, GL_FLOAT, GL_FALSE, 0 );


		//on attribut notre vbo a une certainne partie du vao
		glVertexArrayVertexBuffer( this->idVAO, 0, this->idVBO, 0, sizeof( Vec2f ) );
		glVertexArrayVertexBuffer( this->idVAO, 1, this->colorVBO, 0, sizeof( Vec3f ) );


		// creation du ebo qui va nous permettre d'afficher le rectangle correctement ( ordre des triangles)
		glCreateBuffers( 1, &this->EBO );
		glNamedBufferData( this->EBO, this->idSommets.size() * sizeof( int ), this->idSommets.data(), GL_STATIC_DRAW );
		//lié ebo a vao
		glVertexArrayElementBuffer( this->idVAO, this->EBO );

		glUseProgram( this->program );
		std::cout << "Done!" << std::endl;
		return true;
	}

	void LabWork2::animate( const float p_deltaTime ) { 
		// on va effectuer un translation en fonction du temps et l'envoyer a notre shader
		float translation=glm::sin(this->_time )/2;
		glProgramUniform1f( program, this->uTranslationXUniform, translation );
		this->_time += p_deltaTime;
	}

	void LabWork2::render()
	{
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( this->idVAO );
		// on a 6 points cette fois
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
		
	}

	void LabWork2::handleEvents( const SDL_Event & p_event ) {}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 2" );
		ImGui::Text( "No setting available!" );
		
		
		if ( ImGui::SliderFloat( "Luminosity", &this->luminosity, 0, 1 ))
		{
			glProgramUniform1f( program, this->uLuminosity, this->luminosity );
		}
	
		
		if ( ImGui::ColorEdit3( "color", glm::value_ptr( this->_bgColor ) ) )
			glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );

		ImGui::End();
	}

} // namespace M3D_ISICG
