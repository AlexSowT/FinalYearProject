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
	
	this->m_genome = std::make_shared<Genome>();
	this->GenerateDefaultNetwork();
}

void SpeciesMember::Update(float deltaTime)
{
	this->SetVelocity(this->GetForward() * m_speed);
	this->SetPosition(this->GetPosition() + (this->GetVelocity() * deltaTime));
	
	if (this->GetPosition().x > 650) {
		this->SetPosition(glm::vec3(650.0f, this->GetPosition().y, this->GetPosition().z));
	}
	if (this->GetPosition().x < -50) {
		this->SetPosition(glm::vec3(-50, this->GetPosition().y, this->GetPosition().z));
	}
	if (this->GetPosition().y > 650) {
		this->SetPosition(glm::vec3(this->GetPosition().x, 650, this->GetPosition().z));
	}
	if (this->GetPosition().y < -40) {
		this->SetPosition(glm::vec3(this->GetPosition().x, -40, this->GetPosition().z));
	}

	this->RunNetwork();
}

void SpeciesMember::GenerateDefaultNetwork() {
	// 5 Nodes, 2 In 3 Out
	// 6 Connections, Fully connection, random weights, all enabled

	for (int node_id = 0; node_id < 8; ++node_id) {
		if(node_id <= 4) { // First two
			m_genome->AddNode(std::make_shared<Node>(node_id, NODE_TYPES::INPUT));
		}
		else { // Remaining
			m_genome->AddNode(std::make_shared<Node>(node_id, NODE_TYPES::OUTPUT));
		}
	}
	
	// Add the connections
	m_genome->AddConnection(std::make_shared<Connection>(0, 2, true, 0));
	m_genome->AddConnection(std::make_shared<Connection>(0, 3, true, 1));
	m_genome->AddConnection(std::make_shared<Connection>(0, 4, true, 2));
	m_genome->AddConnection(std::make_shared<Connection>(1, 2, true, 3));
	m_genome->AddConnection(std::make_shared<Connection>(1, 3, true, 4));
	m_genome->AddConnection(std::make_shared<Connection>(1, 4, true, 5));
	m_genome->AddConnection(std::make_shared<Connection>(2, 2, true, 6));
	m_genome->AddConnection(std::make_shared<Connection>(2, 3, true, 7));
	m_genome->AddConnection(std::make_shared<Connection>(2, 4, true, 8));
	m_genome->AddConnection(std::make_shared<Connection>(3, 2, true, 9));
	m_genome->AddConnection(std::make_shared<Connection>(3, 3, true, 10));
	m_genome->AddConnection(std::make_shared<Connection>(3, 4, true, 11));

	// Create the weight matrix
	m_weights = glm::mat2x3(0);
	m_weights[0][0] = m_genome->GetConnectionWeight(0);
	m_weights[0][1] = m_genome->GetConnectionWeight(1);
	m_weights[0][2] = m_genome->GetConnectionWeight(2);
	m_weights[1][0] = m_genome->GetConnectionWeight(3);
	m_weights[1][1] = m_genome->GetConnectionWeight(4);
	m_weights[1][2] = m_genome->GetConnectionWeight(5);
	m_weights[2][0] = m_genome->GetConnectionWeight(6);
	m_weights[2][1] = m_genome->GetConnectionWeight(7);
	m_weights[2][2] = m_genome->GetConnectionWeight(8);
	m_weights[3][0] = m_genome->GetConnectionWeight(9);
	m_weights[3][1] = m_genome->GetConnectionWeight(10);
	m_weights[3][2] = m_genome->GetConnectionWeight(11);

}

float SpeciesMember::CalculateFitness() {
	if (GetPosition().x > 600) {
		return 10;
	}
	
	return 0;
}

void SpeciesMember::RunNetwork() {
	/*Create a weight matrix from input layer to the output layer as described earlier; e.g.N - by - M matrix.
		Create an M - by - 1 matrix from the biases. - Ignored
		View your input layer as an N - by - 1 matrix(or vector of size N, just like the bias).
		Transpose the weight matrix, now we have an M - by - N matrix.
		Find the dot product of the transposed weights and the input.According to the dot - product rules, if you find the dot product of an M - by - N matrix and an N - by - 1 matrix, you get an M - by - 1 matrix.
		Add the output of step 5 to the bias matrix(they will definitely have the same size if you did everything right).
		Finally, you have the values of the neurons, it should be an M - by - 1 matrix(vector of size M)*/

	// Best way to do this?

	float min = -50.f;
	float max = 650.f;

	m_inputs[0] = (this->GetPosition().x - min) / (max - min);
	m_inputs[1] = (this->GetPosition().y - min) / (max - min);
	m_inputs[2] = this->GetForward().x;
	m_inputs[3] = this->GetForward().y;

	// Best way to mormalzie these values?
	auto m_outputs = m_weights * m_inputs;

	for(int i = 0; i < 3; ++i) {
		m_outputs[i] = tanh(m_outputs[i]);
	}
	
	// Get Argmax of m_outputs
	// TODO: See if there is a btter way to do this
	int max_id = 0;
	float max_val = m_outputs[0];
	for (int i = 1; i < 3; ++i) {
		if (m_outputs[i] > max_val) {
			max_val = m_outputs[i];
			max_id = i;
		}
	}

	switch (max_id) {
	case 0:
		this->SetSpeed(0.2f);
		break;
	case 1:
		this->SetRotation(this->GetRotation() + 0.1);
		break;
	case 2:
		this->SetRotation(this->GetRotation() - 0.1);
		break;
	default:
		break;
	}
		
}