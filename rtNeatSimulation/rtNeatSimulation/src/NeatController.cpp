#include "NeatController.h"

void NeatController::Init() {
	for (int member_count = 0; member_count < MAX_POP; ++member_count)
	{
		m_population.push_back(std::make_shared<SpeciesMember>());
		// Set the X and Y location ro a random number between -50 and 600
		m_population[member_count]->SetPosition(glm::vec3(rand() % 600 - 50, rand() % 600 - 50, -90));
	}

	Crossover(m_population[0], m_population[1]);
}

void NeatController::Crossover(std::shared_ptr<SpeciesMember> parent1, std::shared_ptr<SpeciesMember> parent2){
	// Always have the first parent be the fittest
	if (parent1->CalculateFitness() < parent2->CalculateFitness())
	{
		Crossover(parent2, parent1);
	}

	Genome child; 
	
	// Itterate over parent1 and parent2 connections and add matching to the child

	for(auto connection : parent1->GetGenome()->GetConnections())
	{
		// Matching
		if (parent2->GetGenome()->HasConnectionWithInvNumber(connection->GetInnovation()))
		{
			child.AddConnection(connection);
		}
		else if (connection->GetInnovation() < parent2->GetGenome()->MaxInnovation()) { // Disjoint
			child.AddConnection(connection);
		}
		else { // Excess
			if (parent1->CalculateFitness() == parent2->CalculateFitness()) {
				// Add or don't add randomly
				std::uniform_int_distribution<int> coinFlip(1, 2);

				if (coinFlip(generator) == 1) {
					child.AddConnection(connection);
				}
			}
			else {
				child.AddConnection(connection);
			}
		}
	}

	for (auto connection : parent2->GetGenome()->GetConnections()) {
		if (parent2->GetGenome()->HasConnectionWithInvNumber(connection->GetInnovation()))
		{
			// Do nothing if they match as it is already added
		}
		else if (connection->GetInnovation() < parent1->GetGenome()->MaxInnovation()) { // Disjoint
			child.AddConnection(connection);
		}
		else { // Excess
			if (parent1->CalculateFitness() == parent2->CalculateFitness()) {
				// Add or don't add randomly
				std::uniform_int_distribution<int> coinFlip(1, 2);

				if (coinFlip(generator) == 1) {
					child.AddConnection(connection);
				}
			}
		}
	}
}

void NeatController::Mutate() {	
	// Select two offspring with highest fitness from each species
	// Sort m_population by fitness
	std::sort(m_population.begin(), m_population.end(), [](const std::shared_ptr<SpeciesMember> a, const std::shared_ptr<SpeciesMember> b) {
		return a->CalculateFitness() > b->CalculateFitness();
	});

	auto parent1 = m_population[0];
	auto parent2 = m_population[1];
	
	// Calculate how many offspring to create given keeping the maximum number. floor(MAX_POP/SPECIES_COUNT)
	
	auto populationToCreate = MAX_POP; // TODO: Ignoring different species for now. 
	
	// Line up their genes (connections) by the innovation number 
	
	// Four loops over the sets
	auto parentOneMax = std::max(parent1->GetInnovationNumbers().begin(), parent1->GetInnovationNumbers().end());
	auto parentTwoMax = std::max(parent2->GetInnovationNumbers().begin(), parent2->GetInnovationNumbers().end());
	
	auto parentOneMin = std::min(parent1->GetInnovationNumbers().begin(), parent1->GetInnovationNumbers().end());
	auto parentTwoMin = std::min(parent2->GetInnovationNumbers().begin(), parent2->GetInnovationNumbers().end());

	auto trueMax = std::max(parentOneMax, parentTwoMax);
	auto trueMin = std::min(parentOneMin, parentTwoMin);
	
	// Get Matching Inv Numbers
	// TODO: This is incorrect, but for now I am going to continue so I can flesh out the rest of the code. It is incorrect because I need to know which parent has the higher fitness or if they match.

	std::vector<int> matchingInnovations;
	std::vector<int> disjointInnovations;
	std::vector<int> excessInnovations;

	for (auto& inv_number : parent1->GetInnovationNumbers()) {
		if (std::find(parent2->GetInnovationNumbers().begin(), parent2->GetInnovationNumbers().end(), inv_number) != parent2->GetInnovationNumbers().end()) {
			matchingInnovations.push_back(inv_number);
		}
		else if (inv_number < *std::max_element(parent2->GetInnovationNumbers().begin(), parent2->GetInnovationNumbers().end())) {
			disjointInnovations.push_back(inv_number);
		}
		else {
			excessInnovations.push_back(inv_number);
		}
	}

	for (auto& inv_number : parent2->GetInnovationNumbers()) {
		if (inv_number < *std::max_element(parent1->GetInnovationNumbers().begin(), parent1->GetInnovationNumbers().end())) {
			disjointInnovations.push_back(inv_number);
		}
		else {
			excessInnovations.push_back(inv_number);
		}
	}
	

	// Matching genes are passed to the offspring(when the innovation numbesr are the same)
	// If the innovation numbers are different, the parent with the highest fitness passes their genes
	// If the innovation numbers are the same, the gene is passed with a random chance
	// Decide if the network will mutate by a given chance
	// If it will mutate, decicde if it adds a node, adds a connection, or modifys a weight\
	// Repeat steps for each offspring in each species.
}

void SelectParents() {
	// Calcualte fitness of each member by species
	// Pick the two highest fitness members of each species and return
}

void NeatController::AddConnection(){
	// Create a new connection between two exisiting nodes
}

void NeatController::AddNode() {
	// Add a new node between an exisiting connection
	// Create two new connections between the new node and the two exisiting nodes
	// Disable the origial connection
}

void NeatController::UpdateWeight() {
	// Select connection
	// Change the weight by a random amount
}