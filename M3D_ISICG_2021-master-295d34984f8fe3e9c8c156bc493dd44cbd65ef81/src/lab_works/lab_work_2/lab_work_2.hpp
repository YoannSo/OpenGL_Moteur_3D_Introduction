#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "define.hpp"
#include <vector>
namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2() ;

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		GLuint program;

		// ================ Scene data.
		// ================

		// ================ GL data.
		// ================
		std::vector<Vec2f> pSommets;
		std::vector<int>   idSommets;
		std::vector<Vec3f>  colors;
		GLuint			   colorVBO;
		GLuint			   EBO;
		GLuint idVBO;
		GLuint idVAO;
		GLuint				uTranslationXUniform;
		float					_time = 0;
		GLfloat					uLuminosity = 1;
		float					luminosity=1;
		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color

		// ================
		
		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_2_HPP__
