#pragma once
#include <vector>
#include <SpeciesMember.h>
#include <Genome.h>
#include <memory>

class NeatController {
public:
	NeatController() {
		generator.seed(rd());
		Init();
	}
	
	// TODO: this can be prob be a const
	std::vector<std::shared_ptr<SpeciesMember>> GetPopulation() { return m_population; }
	
	void Init();
	
	void Crossover(std::shared_ptr<SpeciesMember> parent1, std::shared_ptr<SpeciesMember> parent2);
	void Mutate();
	void Reset(); // Generaet a fresh population with default networks
	
private:
	void AddConnection();
	void AddNode();
	void UpdateWeight();
	void SelectParents();

	std::vector<std::shared_ptr<SpeciesMember>> m_population;

	const int MAX_POP = 100;

	std::random_device rd;
	std::mt19937 generator;
};