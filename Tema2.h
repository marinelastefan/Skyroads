#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#include"Platform.h"
#include"LabCameraTema.h"

class Tema2 : public SimpleScene
{
    public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		GLboolean CheckCollision(float xPlayer, float yPlayer, float zPlayer, float xPlatform, float yPlatform, float zPlatform, float zScale);
		Mesh * CreateMesh(const char * name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);
		
		glm::vec3 lightPosition;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;

		float playerX, playerY, playerZ;
		float speedPlayer;
		bool jumpUpPlayerLeft;
		bool jumpDownPlayerLeft;
		bool jumpUpPlayerRight;
		bool jumpDownPlayerRight;
		bool jumpUp, jumpDown;
		int onePlatformPassed;
		bool firstPerson;
		float scaleX, scaleY, scaleZ;
		glm::mat4 projectionMatrix;
		Laborator::Camera *camera;

		std::vector<Platform> platformCol1;
		std::vector<Platform> platformCol2;
		std::vector<Platform> platformCol3;
};