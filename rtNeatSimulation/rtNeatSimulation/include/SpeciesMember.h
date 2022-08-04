#pragma once
#include <memory>
#include <glm.hpp>
#include <math.h>
#include "GameObject.h"
#include "Genome.h"
#include "Collidable.h"

class SpeciesMember : public GameObject, public Collidable
{
public:
	SpeciesMember();

	void Update(float deltaTime);

	void SetSpeed(float speed) { m_speed = speed; }

	void GenerateDefaultNetwork();

	void RunNetwork();

	float CalculateFitness();
	
	const std::vector<int> GetInnovationNumbers() const { return m_genome->GetInnovationNumbers(); }

	std::shared_ptr<Genome> GetGenome() { return m_genome; }

	void OnCollision(std::shared_ptr<Collidable> other) override {}
	bool CheckCollision(std::shared_ptr<Collidable> other) override { return false; }

private:
	float m_speed = 0.f;
	// TODO: Come back here if this causes errors.
	std::shared_ptr<Genome> m_genome;
	glm::mat4x3 m_weights;
	glm::vec4 m_inputs;
};