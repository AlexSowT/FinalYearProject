#include <SpeciesMember.h>

SpeciesMember::SpeciesMember() : GameObject()
{
	const std::vector<glm::vec3> vertices =
	{
		// Positions // Colors // Texture Coords
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.f), // Top Right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.f), // Bottom Right
		glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.f), // Bottom Left
	};

	const std::vector<int> indices =
	{
		0, 1, 2
	};

	this->Init(vertices, indices);
}

void SpeciesMember::Update(float deltaTime)
{
	this->SetVelocity(this->GetForward() * m_speed);
	this->SetPosition(this->GetPosition() + (this->GetVelocity() * deltaTime));
}