#pragma once
#include <random>

class Connection {
public:
	Connection(int in, int out, bool enabled, int innovation) {
		this->m_in = in;
		this->m_out = out;
		this->m_enabled = enabled;
		this->m_innovation = innovation;
		
		// Assign a random float between 0 and 1 to the weight
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0, 1);
		this->m_weight = dis(gen);
	}

	float GetWeight() { return m_weight; }
	bool GetEnabled() { return m_enabled; }
	int GetInnovation(){return m_innovation;}
	
private:
	int m_in;
	int m_out;
	float m_weight;
	bool m_enabled;
	int m_innovation;
};