#pragma once
#include "Node.h"
#include "Connection.h"
#include <vector>
#include <memory>

class Genome {
public:
	Genome() {
	}

	void Init() {
		// Init nodes and connections to empty vectors
		//m_nodes = std::vector<std::shared_ptr<Node>>(5);
		m_connections = std::vector<std::shared_ptr<Connection>>(6);
	}
	
	void AddNode(std::shared_ptr<Node> node) { m_nodes.push_back(node); }
	void AddConnection(std::shared_ptr<Connection> connection) { m_connections.push_back(connection); }

	float GetConnectionWeight(int connection_num) { 
		if (m_connections[connection_num]->GetEnabled()) 
		{ 
			return m_connections[connection_num]->GetWeight();
		}
		
		return 0;
	}

	std::vector<int> GetInnovationNumbers(){std::vector<int> inv_nums;
		for (int i = 0; i < m_connections.size(); i++)
		{
			inv_nums.push_back(m_connections[i]->GetInnovation());
		}
		return inv_nums;
	}

	std::vector<std::shared_ptr<Connection>> GetConnections() { return m_connections; }

	bool HasConnectionWithInvNumber(int invNum) {
		return std::find_if(m_connections.begin(), m_connections.end(), [invNum](std::shared_ptr<Connection> connection) {
			return connection->GetInnovation() == invNum;
		}) != m_connections.end();
	}

	int MaxInnovation() {
		int max = 0;
		for (int i = 0; i < m_connections.size(); i++)
		{
			if (m_connections[i]->GetInnovation() > max)
			{
				max = m_connections[i]->GetInnovation();
			}
		}
		return max;
	}
	
private:
	std::vector<std::shared_ptr<Node>> m_nodes;
	std::vector<std::shared_ptr<Connection>> m_connections;
};