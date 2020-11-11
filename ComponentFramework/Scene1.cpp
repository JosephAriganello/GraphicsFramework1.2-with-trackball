#include <GL/glew.h>
#include <SDL.h>
#include <iostream>
#include "MMath.h"
#include "Window.h"
#include "Shader.h"
#include "Scene1.h"
#include "SceneEnvironment.h"
#include "Trackball.h"
#include "SkyBox.h"
#include "ObjLoader.h"
#include "Skull.h"

using namespace GAME;
using namespace MATH;


Scene1::Scene1(Window& windowRef):Scene(windowRef) {

}

bool Scene1::OnCreate() {
	
	camera = nullptr;

	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();

	float aspect = float(windowPtr->getWidth() / float(windowPtr->getHeight()));
	
	projectionMatrix = MMath::perspective(45.0f, aspect, 0.75f, 100.0f);

	skyboxes.push_back(new SkyBox("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg"));
	skyboxes[0]->OnCreate();

	/// Load Assets: as needed 
	if (addModel("Skull.obj") == false) {
		return false;
	}

	/// Create a shader with attributes
	SceneEnvironment::getInstance()->setLight(Vec3(0.0f, 3.0f, 7.0f));

	OnResize(windowPtr->getWidth(), windowPtr->getHeight());
	return true;
}


bool GAME::Scene1::addModel(const char* filename)
{
	models.push_back(new Skull(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f)));

	models[models.size() - 1]->OnCreate();

	if (models[models.size() - 1]->LoadMesh(filename) == false) {
		return false;
	}

	return true;
}

void Scene1::OnResize(int w_, int h_){
	windowPtr->setWindowSize(w_,h_);
	glViewport(0,0,windowPtr->getWidth(),windowPtr->getHeight());
	if (camera) delete camera;
	camera = new Camera(w_, h_, Vec3(0.0f, 0.0f, 10.0f));
	Camera::currentCamera = camera;
	Trackball::getInstance()->setWindowDimensions(windowPtr->getWidth(), windowPtr->getHeight());
}

void Scene1::Update(const float deltaTime) {
	for (Model* model : models) {
		model->Update(deltaTime);
	}
}

void Scene1::Render() const{
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	skyboxes[0]->Render(projectionMatrix, viewMatrix);

	/// Draw your scene here
	for (Model* model : models) {
		model->Render();
	}

	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
}

void Scene1::HandleEvents(const SDL_Event& SDLEvent){
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN) {
		Trackball::getInstance()->onLeftMouseDown(SDLEvent.button.x, SDLEvent.button.y);
	}else if(SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONUP){
		Trackball::getInstance()->onLeftMouseUp(SDLEvent.button.x, SDLEvent.button.y);
	}else if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION && 
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		Trackball::getInstance()->onMouseMove(SDLEvent.button.x, SDLEvent.button.y);
	}
}

Scene1::~Scene1() {
	OnDestroy();
}

void Scene1::OnDestroy() {
	/// Cleanup Assets
	if (camera) delete camera;
	for (Model* model : models) {
		if (model) delete model;
	}

	delete skybox;
	skybox = nullptr;

	for (SkyBox* sb : skyboxes) {
		if (sb) delete sb;
	}

	delete skull;
	skull = nullptr;
}
