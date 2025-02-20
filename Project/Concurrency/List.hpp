#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <atomic>

template<class T>
class SList {
	struct Node {
		T m_data;
		Node* m_pNext;
	};

	std::atomic<Node*> m_head{ nullptr };

public:
	void push_front(T data) {
		//prep locally
		auto pNode = new Node();
		pNode->m_data = data;
		pNode->m_pNext = m_head;
		//commit, retry until succeed
		while (!m_head.compare_exchange_weak(pNode->m_pNext, pNode))
		{ }
	}
};

#endif
