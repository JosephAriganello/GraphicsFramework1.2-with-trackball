#ifndef SKYBOX_H
#define SKYBOX_H

#include "Model.h"

namespace GAME {
	
	class SkyBox : public Model {

	private:
		const char* positive_x_image;
		const char* negative_x_image;
		const char* positive_y_image;
		const char* negative_y_image;
		const char* positive_z_image;
		const char* negative_z_image;
		class Shader* skyBoxShader;
		GLuint textureID;
		GLuint texture;
		GLuint projectionMatrixID;
		GLuint modelViewMatrixID;
		GLuint modelMatrixID;
		GLuint viewMatrixID;

	public:
		SkyBox();
		SkyBox(const char* positive_x_image, const char* negative_x_image, const char* positive_y_image,
			const char* negative_y_image, const char* positive_z_image, const char* negative_z_image);
		virtual ~SkyBox();
		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float time);
		virtual void Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix) const;
	};
}
#endif //SKYBOX_H