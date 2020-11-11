#include "SkyBox.h"
#include "ObjLoader.h"
#include "Shader.h"
#include "SDL_image.h"
#include <iostream>
#include "Trackball.h"

namespace GAME {

	SkyBox::SkyBox() {}

	SkyBox::SkyBox(const char* positive_x_image, const char* negative_x_image, const char* positive_y_image,
		const char* negative_y_image, const char* positive_z_image, const char* negative_z_image) {

		this->positive_x_image = positive_x_image;
		this->negative_x_image = negative_x_image;
		this->positive_y_image = positive_y_image;
		this->negative_y_image = negative_y_image;
		this->positive_z_image = positive_z_image;
		this->negative_z_image = negative_z_image;
	}

	SkyBox::~SkyBox() {
		OnDestroy();
	}

	bool SkyBox::OnCreate() {

		ObjLoader obj;
		if (obj.loadOBJ("cube.obj") == false) {
			return false;
		}

		meshes.push_back(new Mesh(GL_TRIANGLES, obj.vertices, obj.normals, obj.uvCoords));

		SDL_Surface* textureSurface;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		textureSurface = IMG_Load(positive_x_image);
		if (textureSurface == nullptr) return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		textureSurface = IMG_Load(negative_x_image);
		if (textureSurface == nullptr) return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		textureSurface = IMG_Load(positive_y_image);
		if (textureSurface == nullptr) return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		textureSurface = IMG_Load(negative_y_image);
		if (textureSurface == nullptr) return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		textureSurface = IMG_Load(positive_z_image);
		if (textureSurface == nullptr) return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		textureSurface = IMG_Load(negative_z_image);
		if (textureSurface == nullptr) return false;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, textureSurface->w, textureSurface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
		textureSurface = nullptr;

		skyBoxShader = new Shader("skyboxVert.glsl", "skyboxFrag.glsl", 1, 0, "vVertex");
		projectionMatrixID = glGetUniformLocation(skyBoxShader->getProgram(), "projectionMatrix");
		/// modelViewMatrixID = glGetUniformLocation(skyBoxShader->getProgram(), "modelViewMatrix");
		modelMatrixID = glGetUniformLocation(skyBoxShader->getProgram(), "modelMatrix");
		viewMatrixID = glGetUniformLocation(skyBoxShader->getProgram(), "viewMatrix");
		textureID = glGetUniformLocation(skyBoxShader->getProgram(), "cubeTexture");

		return true;
	}

	void SkyBox::Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix) const{

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		glUseProgram(skyBoxShader->getProgram());
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, projectionMatrix);
		/// glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, modelViewMatrix);
		/// glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4());
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, viewMatrix);

		for (Mesh* mesh : meshes) {
			mesh->Render();
		}

		glEnableVertexAttribArray(0); /// Disable the VAO
	}

	void SkyBox::OnDestroy() {
		if (shader) delete shader;
	}
	void SkyBox::Update(const float time) {}
}