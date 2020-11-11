#ifndef SKULL_H
#define SKULL_H

#include <vector>
#include "MMath.h"
#include "Mesh.h"
#include "Entity.h"
#include "Model.h"

namespace GAME {

	class Skull : public Model {

	protected:

		class Shader *shader;

		Matrix4 modelMatrix;
		std::vector<Mesh*> meshes;

	public:

		GLuint textureID;
		GLuint texture;
		float elapsedTime;
		GLuint noiseTexture;
		double noiseValue;
		double vec[3];

		Skull();
		Skull(const Vec3 _pos, const Vec3 _orientation);
		Skull(const Skull&) = delete;
		Skull(Skull&&) = delete;
		Skull& operator = (const Skull&) = delete;
		Skull& operator = (Skull&&) = delete;

		virtual ~Skull();

		void setPos(const Vec3& pos_) override;
		void setOrientation(const Vec3& orienration_) override;
		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Render() const;
		virtual void Update(const float deltaTime);

		virtual bool LoadMesh(const char* filename);

	protected:

		void updateModelMatrix();
	};
}


#endif // SKULL_H
