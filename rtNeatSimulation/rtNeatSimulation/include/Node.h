#pragma once

enum NODE_TYPES {
	INPUT,
	HIDDEN,
	OUTPUT
};

class Node {
public:
	Node(int id, NODE_TYPES type) {
		m_id = id;
		m_type = type;
		m_value = 0;
	}
private:
	int m_id;
	float m_value;
	NODE_TYPES m_type;
};