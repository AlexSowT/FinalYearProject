#include "NEATLayer.h"

void NEATLayer::Init()
{
	std::vector<glm::vec3> planeVertices =
	{
		// Positions // Colors // Texture Coords
		glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.f), // Top Right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.f), // Bottom Right
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.f), // Bottom Left
		glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.f) // Top Left
	};

	std::vector<int> planeIndices =
	{
		0, 1, 3, // First Triangle
		1, 2, 3 // Second Triangle
	};

	std::vector<glm::vec3> memberVertices =
	{
		// Positions // Colors // Texture Coords
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.f), // Top Right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.f), // Bottom Right
		glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.f), // Bottom Left
	};

	std::vector<int> memberIndices =
	{
		0, 1, 2
	};

	m_grassPlane = std::make_shared<GameObject>(planeVertices, planeIndices, "./data/textures/grass.png");
	m_SpeciesMember = std::make_shared<SpeciesMember>();
	m_SpeciesMember->SetPosition(glm::vec3(140, 140, -90));
}

void NEATLayer::Update(float deltaTime, bool& running){
	if (SDL_PollEvent(&m_windowEvent))
	{
		if (m_windowEvent.type == SDL_QUIT)
		{
			running = false;
		}
		else if (m_windowEvent.type == SDL_KEYDOWN)
		{
			switch (m_windowEvent.key.keysym.sym)
			{
			case SDLK_LEFT:
				m_camera->ProcessKeyboard(LEFT, deltaTime);
				break;
			case SDLK_RIGHT:
				m_camera->ProcessKeyboard(RIGHT, deltaTime);
				break;

			case SDLK_UP:
				m_camera->ProcessKeyboard(FORWARD, deltaTime);
				break;

			case SDLK_DOWN:
				m_camera->ProcessKeyboard(BACKWARD, deltaTime);
				break;
			case SDLK_w:
				m_SpeciesMember->SetSpeed(0.2f);
				break;
			case SDLK_a:
				m_SpeciesMember->SetRotation(m_SpeciesMember->GetRotation() + 0.1);
				break;
			case SDLK_d:
				m_SpeciesMember->SetRotation(m_SpeciesMember->GetRotation() - 0.1);
				break;

			default:
				break;
			}
		} else if (m_windowEvent.type == SDL_KEYUP)
		{
			switch (m_windowEvent.key.keysym.sym)
			{
			case SDLK_w:
				m_SpeciesMember->SetSpeed(0.f);
				break;

			default:
				break;
			}
		} else if (m_windowEvent.type == SDL_MOUSEWHEEL)
		{
			m_camera->ProcessMouseScroll(m_windowEvent.wheel.y);
		}
	}

	m_SpeciesMember->Update(deltaTime);
}

void NEATLayer::Render(ShaderLibrary& shaderLibrary, Renderer& renderer, SDL_Window* window){
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// -- Textuered Rendered -- 
	shaderLibrary.Use(ShaderLibrary::SHADER_TYPE::TEXTURE);
	GLint viewLoc = glGetUniformLocation(shaderLibrary.GetProgramID(ShaderLibrary::SHADER_TYPE::TEXTURE), "view");
	GLint projectionLoc = glGetUniformLocation(shaderLibrary.GetProgramID(ShaderLibrary::SHADER_TYPE::TEXTURE), "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMat()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMat()));

	int grid_width = 10;
	int grid_height = 10;
	for (int i = 0; i < grid_height; ++i)
	{
		for (int j = 0; j < grid_width; ++j)
		{
			glm::mat4 model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(70 * j, 70 * i, -100));
			model = glm::scale(model, glm::vec3(70));
			renderer.render(shaderLibrary, ShaderLibrary::SHADER_TYPE::TEXTURE, m_grassPlane, model);
		}
	}

	////-- Text Renderer
	shaderLibrary.Use(ShaderLibrary::SHADER_TYPE::TEXT);
	auto text_ortho = glm::ortho(0.0f, (GLfloat)WIDTH, 0.0f, (GLfloat)HEIGHT, -1000.f, 1000.0f);
	projectionLoc = glGetUniformLocation(shaderLibrary.GetProgramID(ShaderLibrary::SHADER_TYPE::TEXT), "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(text_ortho));

	std::string xPos = std::to_string(m_SpeciesMember->GetForward().x);
	std::string yPos = std::to_string(m_SpeciesMember->GetForward().y);

	renderer.render_text(shaderLibrary, ShaderLibrary::SHADER_TYPE::TEXT, xPos, 0.f, HEIGHT - 50, 1.f, glm::vec4(255, 0, 0, 1));
	renderer.render_text(shaderLibrary, ShaderLibrary::SHADER_TYPE::TEXT, yPos, 0.f, HEIGHT - 100, 1.f, glm::vec4(255, 0, 0, 1));

	////-- Color Rendender
	shaderLibrary.Use(ShaderLibrary::SHADER_TYPE::COLOR);
	viewLoc = glGetUniformLocation(shaderLibrary.GetProgramID(ShaderLibrary::SHADER_TYPE::COLOR), "view");
	projectionLoc = glGetUniformLocation(shaderLibrary.GetProgramID(ShaderLibrary::SHADER_TYPE::COLOR), "projection");

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMat()));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMat()));

	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, m_SpeciesMember->GetPosition());
	model = glm::rotate(model, m_SpeciesMember->GetRotation() - glm::pi<float>()/2, glm::vec3(0, 0, 1)); // Take away pi/2 from intial value to correct for rotation 0 pointing in the positive Y
	model = glm::scale(model, glm::vec3(20));
	renderer.render(shaderLibrary, ShaderLibrary::SHADER_TYPE::COLOR, m_SpeciesMember, model);

	SDL_GL_SwapWindow(window);
}
 