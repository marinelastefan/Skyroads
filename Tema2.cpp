#include "Tema2.h"
#include "Platform.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

int auxTime = 0;
bool fall = false;
bool buttonPressed = false;
float fuel = 10;
float scale = 1;
float speedZ = 0.1f;
float maxLength;
float minLength;
float timeToWait = 50;


Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}
void Tema2::Init()
{
	//
	playerX = 0;
	playerY = 0.2f;
	playerZ = 0;
	speedPlayer = 2;
	jumpUpPlayerLeft = false;
	jumpDownPlayerLeft = false;
	jumpUpPlayerRight = false;
	jumpDownPlayerRight = false;
	jumpUp = false;
	jumpDown = false;
	onePlatformPassed = 0;
	scaleX = 0.25f;
	scaleY = 0.25f;
	scaleZ = 0.25f;

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	firstPerson = false;

	// Create a simple cube
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
			VertexFormat(glm::vec3( 1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
			VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
			VertexFormat(glm::vec3( 1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
			VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
			VertexFormat(glm::vec3( 1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
			VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
			VertexFormat(glm::vec3( 1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cubeSpeed", vertices, indices);
	}

    //pentru platforme
    {
		Mesh* mesh = new Mesh("cube");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
    // pentru jucator

	{
		Mesh* mesh = new Mesh("player");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	// creez un shader
	{
		Shader *shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Light & material properties
	{
		lightPosition = glm::vec3(0, 1, 1);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
	}
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}
void Tema2::Update(float deltaTimeSeconds)
{
	
	if (firstPerson == true) {
		camera->Set(glm::vec3(playerX, playerY, playerZ + 0.5), glm::vec3(playerX, playerY, playerZ), glm::vec3(0, 1, 0));
	} else {
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	}
	auxTime++;
	if ( auxTime > timeToWait ) {

		platformCol1.push_back(Platform());
		platformCol2.push_back(Platform());
		platformCol3.push_back(Platform());

		//setez coordonatele pentru fiecare platforma
		//aleg o culoarea random pentru fiecare platforma
		platformCol1[platformCol1.size() - 1].setX(1.5f);
		platformCol2[platformCol2.size() - 1].setX(0);
		platformCol3[platformCol3.size() - 1].setX(-1.5f);

		platformCol1[platformCol1.size() - 1].setY(0);
		platformCol2[platformCol2.size() - 1].setY(0);
		platformCol3[platformCol3.size() - 1].setY(0);

		platformCol1[platformCol1.size() - 1].setZ(-10);
		platformCol2[platformCol2.size() - 1].setZ(-10);
		platformCol3[platformCol3.size() - 1].setZ(-10);

		maxLength = 5.0;
		float minLength = 1.5f;
		float random = minLength+ ((float)rand()/(float)(RAND_MAX)) * (maxLength - minLength);
		platformCol1[platformCol1.size() - 1].setInitialZ(random);
		random = minLength + ((float)rand()/(float)(RAND_MAX)) * (maxLength - minLength);
		platformCol2[platformCol2.size() - 1].setInitialZ(random);
		random = minLength + ((float)rand()/(float)(RAND_MAX)) * (maxLength - minLength);
		platformCol3[platformCol3.size() - 1].setInitialZ(random);

		platformCol1[platformCol1.size() - 1].setColor(rand() % 5);
		platformCol2[platformCol2.size() - 1].setColor(rand() % 5);
		platformCol3[platformCol3.size() - 1].setColor(rand() % 5);

		platformCol1[platformCol1.size() - 1].setCollision(false);
		platformCol3[platformCol2.size() - 1].setCollision(false);
		platformCol3[platformCol3.size() - 1].setCollision(false);

		auxTime = 0;
	}
	fall = true;
	for (int i = 0; i < platformCol1.size(); i++) {
		glm::mat4 modelMatrix1 = glm::mat4(1);
		glm::mat4 modelMatrix2 = glm::mat4(1);
		glm::mat4 modelMatrix3 = glm::mat4(1);

		//setez culoarea pentru platforme
		glm::vec3 color1 = glm::vec3(1);
		switch(platformCol1[i].getColor()) {
			case 0:
				color1 = glm::vec3(1,0,0);
				break;
			case 1:
				color1 = glm::vec3(0.9,1,0);
				break;
			case 2:
				color1 = glm::vec3(1,0.5f,0.2f);
				break;
			case 3:
				color1 = glm::vec3(0,1,0);
				break;
			case 4:	
				color1 = glm::vec3(0.f, 0, 1.f);
				break;
			case 5:
				color1 = glm::vec3(0.3f, 0, 0.5f);
				break;
			default:
				break;
		}
		// verific coliziunea cu prima coloana
		if (CheckCollision(playerX, playerY, playerZ, platformCol1[i].getX(), platformCol1[i].getY(), platformCol1[i].getZ(), platformCol1[i].getInitialZ())) {
			// platforma este galbena, jucatorul pierde o parte din combustibil
			if (platformCol1[i].getColor() == 1) {
				fuel -= 1;
				if (fuel == 0) {
					std::exit(-1);
				}
				
			}
			// platforma este verde
			// jucatorul recupereaza o parte din combustibil
			if (platformCol1[i].getColor() == 3 && fuel < 10) {
				fuel += 1;
				
			}
			// platforma este rosie, jocul se termina
			if (platformCol1[i].getColor() == 0) {
				std::exit(-1);
			}
			// dupa aterizare, platforma se face violet
			color1 = glm::vec3(0.3f, 0, 0.5f);
			platformCol1[i].setColor(5);
			platformCol1[i].setCollision(true);
			onePlatformPassed++;
			fall = false;
		}		
		modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(platformCol1[i].getX(),platformCol1[i].getY() , platformCol1[i].getZ()));
		modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(1,0.1f,platformCol1[i].getInitialZ()));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix1, color1);

		glm::vec3 color2 = glm::vec3(1);
		switch(platformCol2[i].getColor()) {
			case 0:
				color2 = glm::vec3(1,0,0);
				break;
			case 1:
				color2 = glm::vec3(0.9,1,0);
				break;
			case 2:
				color2 = glm::vec3(1,0.5f,0.2f);
				break;
			case 3:
				color2 = glm::vec3(0,1,0);
				break;
			case 4:	
				color2 = glm::vec3(0, 0, 1);
				break;
			case 5:
				color2 = glm::vec3(0.3f, 0, 0.5f);
				break;

			default:
				break;
		}
		// verific coliziunea cu a doua coloana
		if (CheckCollision(playerX, playerY, playerZ, platformCol2[i].getX(), platformCol2[i].getY(), platformCol2[i].getZ(), platformCol2[i].getInitialZ())) {
			//culoarea este galbena, jucatorul pierde o parte din combustibil
			if (platformCol2[i].getColor() == 1) {
				fuel -= 1;
				if (fuel == 0) {
					std::exit(-1);
				}
				
			}
			//platforma e verde, jucatorul recupereaza o parte din combustibil
			if (platformCol2[i].getColor() == 3 && fuel < 10) {
				fuel += 1;
				
			}
			//platforma e rosie, jocul se termina
			if (platformCol2[i].getColor() == 0) {
				std::exit(-1);
			}
			color2 = glm::vec3(0.3f, 0, 0.5f);
			platformCol2[i].setColor(5);
			platformCol2[i].setCollision(true);
			onePlatformPassed++;
			fall = false;
		}
		modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(platformCol2[i].getX(),platformCol2[i].getY() , platformCol2[i].getZ()));
		modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(1,0.1f,platformCol2[i].getInitialZ()));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix2, color2);

		glm::vec3 color3 = glm::vec3(1);
		switch(platformCol3[i].getColor()) {
			case 0:
				color3 = glm::vec3(1,0,0);
				break;
			case 1:
				color3 = glm::vec3(0.9,1,0);
				break;
			case 2:
				color3 = glm::vec3(1,0.5f,0.2f);
				break;
			case 3:
				color3 = glm::vec3(0,1,0);
				break;
			case 4:	
				color3 = glm::vec3(0, 0, 1);
				break;
			case 5:
				color3 = glm::vec3(0.3f, 0, 0.5f);
			default:
				break;
		}
		// verific coliziunea jucatorului cu a treia coloana
		if (CheckCollision(playerX, playerY, playerZ, platformCol3[i].getX(), platformCol3[i].getY(), platformCol3[i].getZ(), platformCol3[i].getInitialZ())) {
			// platforma este galbena, pierde o parte din comubstibil
			if (platformCol3[i].getColor() == 1) {
				fuel -= 1;
				if (fuel == 0) {
					std::exit(-1);
				}
				
			}
			// platforma e verde, recupereaza o parte din combustibil
			if (platformCol3[i].getColor() == 3 && fuel < 10) {
				fuel += 1;
				
			}
			// platforma e rosie, jocul se opreste
			if (platformCol3[i].getColor() == 0) {
				std::exit(-1);
			}
			color3 = glm::vec3(0.3f, 0, 0.5f);
			platformCol3[i].setColor(5);
			platformCol3[i].setCollision(true);
			onePlatformPassed++;
			fall = false;

		}
		modelMatrix3 = glm::translate(modelMatrix3, glm::vec3(platformCol3[i].getX(),platformCol3[i].getY() , platformCol3[i].getZ()));
		modelMatrix3 = glm::scale(modelMatrix3, glm::vec3(1,0.1f,platformCol3[i].getInitialZ()));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTema2"], modelMatrix3, color3);

		platformCol1[i].setZ(platformCol1[i].getZ() + speedZ);
		platformCol2[i].setZ(platformCol2[i].getZ() + speedZ);
		platformCol3[i].setZ(platformCol3[i].getZ() + speedZ);


	}
	    // playeryl cade intre platforme
		if (fall == true && onePlatformPassed > 0 && playerY <= 0.2f) {
			playerY -=0.1f;
	}

	glm::mat4 modelMatrix = glm::mat4(1);
	if (onePlatformPassed > 0) {
		// playerul cade pe laterale
		if (playerX > 1.8f || playerX < -1.8f || ( playerX > -0.82f && playerX < -0.52f) || (playerX > 0.52f && playerX < 0.82f)) {
			playerY -= 0.15f;
			
		}
	}
	// sa se miste la stanga	
	if (jumpUpPlayerLeft == true) {
		playerY += 0.1f;
		if (playerY > 1.2) {
			jumpDownPlayerLeft = true;
			jumpUpPlayerLeft = false;
					
		}
	} else {
		if (jumpDownPlayerLeft == true) {
			playerY -= 0.1f;
			if (playerY < 0.2f) {
				playerY = 0.2f;
				jumpDownPlayerLeft = false;
				}
			}
	}
			
	// sa se miste la dreapta
	if (jumpUpPlayerRight == true) {
		playerY += 0.1f;
		if (playerY > 1.2f) {
			jumpUpPlayerRight= false;
			jumpDownPlayerRight = true;
		}
	} else {
		if (jumpDownPlayerRight == true) {
			playerY -= 0.1f;
			if (playerY < 0.2f) {
				playerY = 0.2f;
				jumpDownPlayerRight = false;
			}
		}
	}
	//sa sara
	if (jumpUp == true) {
		playerY += 3   * deltaTimeSeconds;
		if (playerY > 1.3f) {
			jumpDown = true;
			jumpUp = false;
			buttonPressed = false;
		}
	}
	if (jumpDown == true) {
		playerY -= 3 * deltaTimeSeconds;
		if (playerY < 0.2f) {
			playerY = 0.2f;
			jumpDown = false;
		}
	}
	
	//in fisrt person nu se afiseaza playerul
	if (firstPerson == false) {
		modelMatrix = glm::translate(modelMatrix, glm::vec3(playerX, playerY, playerZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleX, scaleY, scaleZ));
		RenderSimpleMesh(meshes["player"], shaders["ShaderTema2"], modelMatrix,glm::vec3(0, 191, 255));
	}
	// a cazut
	if (playerY < -2) {
		std::exit(-1);
	}

	//combustibilul este format din 2 cuburi suprapuse
	glm::mat4 modelMatrixCubeSpeed = glm::mat4(1);
	modelMatrixCubeSpeed = glm::translate(modelMatrixCubeSpeed, glm::vec3(1, 1, 0));
	modelMatrixCubeSpeed = glm::scale(modelMatrixCubeSpeed, glm::vec3(1, 0.1f, 0));
	modelMatrixCubeSpeed = glm::translate(modelMatrixCubeSpeed, glm::vec3(1,10,0));
	RenderSimpleMesh(meshes["cubeSpeed"], shaders["ShaderTema2"], modelMatrixCubeSpeed,glm::vec3(1, 1, 1));

	//cel din fata se scaleaza pe ox in functie de combustibil
	modelMatrixCubeSpeed = glm::mat4(1);
	modelMatrixCubeSpeed = glm::translate(modelMatrixCubeSpeed, glm::vec3(1, 1, 0.01f));
	modelMatrixCubeSpeed = glm::scale(modelMatrixCubeSpeed, glm::vec3((fuel / 10), 0.1f, 0));
	modelMatrixCubeSpeed = glm::translate(modelMatrixCubeSpeed, glm::vec3(1,10,0.0f));
	RenderSimpleMesh(meshes["cubeSpeed"], shaders["ShaderTema2"], modelMatrixCubeSpeed,glm::vec3(1, 0, 0));
	
	


	

}
Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	//la exercitiul 6 aici se va seta culoarea
	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
		if (!mesh || !shader || !shader->program)
        return;

 

    // render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

 

    // render an object using the specified shader and the specified position
    glUseProgram(shader->program);

 

    //material property uniforms for object color
    int colorPos = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(colorPos, 1, glm::value_ptr(color));

 

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	
	//schimba la coloana din stanga
	if (window->KeyHold(GLFW_KEY_A)) { 
		jumpUpPlayerLeft = true;
		jumpDownPlayerLeft = false;
		playerX -= 8.0f * deltaTime;

		
	}
	//schimba la coloana din dreapta
	if (window->KeyHold(GLFW_KEY_D)) {
		jumpUpPlayerRight = true;
		jumpDownPlayerRight = false;
		playerX += 8.0f* deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_SPACE)) {
		jumpUp = true;
		jumpDown = false;
		buttonPressed = true;
	}

    
}

void Tema2::OnKeyPress(int key, int mods)
{
	//switch intre first camera si third camera
	if (key == GLFW_KEY_C) {
		firstPerson = !firstPerson;
	}
	//mareste viteza de mers
	if (key == GLFW_KEY_W) {
		if (speedZ < 0.2f) {
			speedZ += 0.01f;
			maxLength+=10;
			minLength+=10;
			timeToWait -=2;
		} 
	}
	//micosreaza viteza de mers
	if (key == GLFW_KEY_S) {
		if (speedZ > 0.1f) {
			speedZ -= 0.01f;
			maxLength-=10;
			minLength-=10;
			timeToWait +=2;
		} 
	}
	

	
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
float max (float x, float y) {
	if (x > y) {
		return x;
	}
	return y;
}
float min(float x, float y) {
	if (x < y) {
		return x;
	}
	return y;
}
// verifica coliziunea intre platforma si jucator
GLboolean Tema2::CheckCollision(float xPlayer, float yPlayer, float zPlayer, float xPlatform, float yPlatform, float zPlatform, float zScale) {
	float boxXMin = xPlatform - (float) 1 / 2;
	float boxXMax = xPlatform + (float)1 /2;
	float boxYMin = yPlatform - (float)0.1 / 2;
	float boxYmax = yPlatform + (float)0.1 / 2;
	float boxZMin = zPlatform - (float) zScale  /2;
	float boxZMax =  zPlatform + (float) zScale  /2;
	float x = max(boxXMin, min(xPlayer, boxXMax));
	float y = max ( boxYMin, min(playerY, boxYmax));
	float z = max (boxZMin, min(zPlayer, boxZMax));
	

	float distance = sqrt((x - xPlayer) * (x - xPlayer) + (y - yPlayer) * (y - yPlayer) + (z - zPlayer) * (z - zPlayer));
	return distance < 0.2f;
}
