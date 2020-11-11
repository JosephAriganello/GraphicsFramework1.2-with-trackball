#include "Skull.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "Camera.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include <SDL_image.h>

namespace GAME {

	Skull::Skull() {}
	Skull::Skull(const Vec3 pos_, const Vec3 orientation_) {
		pos = pos_;
		orientation = orientation_;
		shader = nullptr;
	}

	Skull::~Skull() {
		OnDestroy();
	}

	void Skull::setPos(const Vec3& pos_) {
		Model::setPos(pos_);
		updateModelMatrix();

	}

	void Skull::setOrientation(const Vec3& orientation_) {
		Model::setOrientation(orientation_);
		updateModelMatrix();
	}

	void Skull::updateModelMatrix() {
		modelMatrix = MMath::translate(pos);

		/// This transform is based on Euler angles - let's do it later
		/// modelMatrix = MMath::translate(pos) * MMath::rotate(orientation.z, Vec3(0.0f, 0.0f, 1.0f)) * MMath::rotate(orientation.x, Vec3(1.0f, 0.0f, 0.0f)) * MMath::rotate(orientation.y, Vec3(0.0f, 1.0f, 0.0f));
	}

	bool Skull::OnCreate() {
		 shader = new Shader("textureVert.glsl", "textureFrag.glsl", 2, 0, "vVertex", 1, "vNormal");

		 IMG_Init(IMG_INIT_JPG);
		 SDL_Surface* image = IMG_Load("skull_texture.jpg");

		 glGenTextures(1, &textureID);

		 glBindTexture(GL_TEXTURE_2D, textureID);

		 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		return true;
	}

	bool Skull::LoadMesh(const char* filename) {
		if (ObjLoader::loadOBJ(filename) == false) {
			return false;
		}
		/// Get the data out of the ObjLoader and into our own mesh
		meshes.push_back(new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords));
		return true;
	}

	void Skull::Update(const float deltaTime) {
		/// See Model.h
		/// Rotate(Vec3(0.0f, 50.0f * deltaTime, 0.0f));

		if (abs(elapsedTime) > 1.0f) {
			elapsedTime *= -1;
		}
		else if (abs(elapsedTime) < 0.001f) {
			elapsedTime *= -1;
		}

		elapsedTime += deltaTime;
	}

	void Skull::Render() const {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		GLint projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
		GLint viewMatrixID = glGetUniformLocation(shader->getProgram(), "viewMatrix");
		GLint modelMatrixID = glGetUniformLocation(shader->getProgram(), "modelMatrix");
		/// GLint modelViewMatrixID = glGetUniformLocation(shader->getProgram(), "modelViewMatrix");
		GLint normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
		GLint lightPosID = glGetUniformLocation(shader->getProgram(), "lightPos");
		GLint elapsedTimeID = glGetUniformLocation(shader->getProgram(), "elapsedTime");

		glUseProgram(shader->getProgram());
		/// glBindTexture(GL_TEXTURE_2D, textureID);

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, Camera::currentCamera->getProjectionMatrix());
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, Camera::currentCamera->getViewMatrix());
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, modelMatrix * Trackball::getInstance()->getMatrix4());
		glUniform1f(elapsedTimeID, abs(elapsedTime));
		/// glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, Camera::currentCamera->getViewMatrix() *
		///	(modelMatrix * Trackball::getInstance()->getMatrix4()));

		/// Assigning the 4x4 modelMatrix to the 3x3 normalMatrix 
		/// copies just the upper 3x3 of the modelMatrix
		Matrix3 normalMatrix = modelMatrix * Trackball::getInstance()->getMatrix4();
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);

		glUniform3fv(lightPosID, 1, SceneEnvironment::getInstance()->getLight());

		for (Mesh* mesh : meshes) {
			mesh->Render();
		}
	}

	void Skull::OnDestroy() {
		if (shader) delete shader;
	}
}