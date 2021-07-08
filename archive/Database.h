#pragma once

#include <cstddef>
#include <utility>

template<typename T>
class Database
{
private:
	//--------------------------------------------------
	// stores the entered data
	//--------------------------------------------------

	struct Node
	{
		//--------------------------------------------------
		// properties
		//--------------------------------------------------
		T data;
	
		Node *greaterNode = nullptr;
		Node *lesserNode = nullptr;
		
		long descendants = 0;

		//--------------------------------------------------
		// constructors
		//  - prevent the creation of empty nodes or by moving/copying
		//  - use the type's constructors to initialize the data
		//--------------------------------------------------

		Node() = delete;
		Node(const Node& node) = delete;
		Node(Node&& node) = delete;

		template<typename...Args>
		Node(Args...args)
		: data(std::forward<Args>(args)...)
		{}

		Node(const T& data)
		: data(data)
		{}

		Node(T&& data)
		: data(std::move(data))
		{}

		//--------------------------------------------------
		// balance functions
		//--------------------------------------------------
		
		/*
		 * recalculate the descendent values of this node and its children, using recursion
		 * use only when the tree has been modified
		 */
		void recalculateDescendants()
		{
			// calculate the number of descendants each branch has, including the child itself
			long lesserDescendants = 0, greaterDescendants = 0;

			if(greaterNode != nullptr)
			{
				greaterNode->recalculateDescendants();
				greaterDescendants = greaterNode->descendants + 1;
			}
			if(lesserNode != nullptr)
			{
				lesserNode->recalculateDescendants();
				lesserDescendants = lesserNode->descendants + 1;
			}

			// recognize only the branch with the greater number of descendants, setting descendants to that value
			descendants = (greaterDescendants > lesserDescendants) ? greaterDescendants : lesserDescendants;
		}

		bool balanced()
		{
			// ensure the descendants information is up-to-date by recalculating it
			// calculate the number of descendants each branch has, including the child itself
			long lesserDescendants = 0, greaterDescendants = 0;
			
			if(greaterNode != nullptr)
				greaterDescendants = greaterNode->descendants + 1;
			if(lesserNode != nullptr)
				lesserDescendants = lesserNode->descendants + 1;
			
			// recognize only the branch with the greater number of descendants, setting descendants to that value
			descendants = (greaterDescendants > lesserDescendants) ? greaterDescendants : lesserDescendants;

			// find the difference between the two branches
			// check if the node is balanced by seeing if -2 < difference < 2
			char difference = greaterDescendants - lesserDescendants;
			return (-2 < difference) && (difference < 2);
		}

		//--------------------------------------------------
		// comparison operators
		//--------------------------------------------------

		// comparing against a comparable datatype, such as T

		template<typename Comparable>
		bool operator<(const Comparable& comparable) const
		{
			return data < comparable;
		}

		template<typename Comparable>
		bool operator==(const Comparable& comparable) const
		{
			return data == comparable;
		}

		template<typename Comparable>
		bool operator>(const Comparable& comparable) const
		{
			return data > comparable;
		}

		// comparing against another node

		bool operator<(const Node& node) const
		{
			return data < node.data;
		}

		bool operator==(const Node& node) const
		{
			return data == node.data;
		}

		bool operator>(const Node& node) const
		{
			return data > node.data;
		}

		//--------------------------------------------------
		// destructors
		//  - delete the lesser and greater node, which in turn delete their children
		//--------------------------------------------------
		~Node()
		{
			delete greaterNode;
			delete lesserNode;
		}
	};

	//--------------------------------------------------
	// tree properties
	//--------------------------------------------------

	Node* root = nullptr;
	size_t count = 0;
	
	//--------------------------------------------------
	// insertion and retrieval functions
	//--------------------------------------------------

	/*
	 *  C          P	C = CurrentNode
	 *   \        / \
	 *    P   -> C   I	P = Pivot
	 *     \
	 *      I		I = InsertedNode
	 */
	void shiftLeft(Node *&currentNode)
	{
		Node *pivot = currentNode->greaterNode;
		currentNode->greaterNode = pivot->lesserNode;
		pivot->lesserNode = currentNode;
		currentNode = pivot;
	}

	/*
	 *      C      P	C = CurrentNode
	 *     /      / \
	 *    P   -> I   C	P = Pivot
	 *   /
	 *  I			I = InsertedNode
	 */

	void shiftRight(Node *&currentNode)
	{
		Node *pivot = currentNode->lesserNode;
		currentNode->lesserNode = pivot->greaterNode;
		pivot->greaterNode = currentNode;
		currentNode = pivot;
	}

	void insert(Node *&currentNode, Node* insertNode)
	{
		// check if the current node is available, insert the new node there if so
		if(currentNode == nullptr)
		{
			currentNode = insertNode;
			count++;
		}

		// direct the new node to the proper node, and attempt to insert it there
		else if(*insertNode > *currentNode)
		{
			insert(currentNode->greaterNode, insertNode);

			// check if the balance of the tree is preseved
			// rebalance the tree if not
			if(!currentNode->balanced())
			{
				if(*insertNode < *(currentNode->greaterNode))
					shiftRight(currentNode->greaterNode);

				shiftLeft(currentNode);
				currentNode->recalculateDescendants();
			}
		}
		else if(*insertNode < *currentNode)
		{
			insert(currentNode->lesserNode, insertNode);

			// check if the balance of the tree is preserved
			// rebalance the tree if not
			if(!currentNode->balanced())
			{
				if(*insertNode > *(currentNode->lesserNode))
					shiftLeft(currentNode->lesserNode);

				shiftRight(currentNode);
				currentNode->recalculateDescendants();
			}
		}

		// the new node is equal to the current node, discard the insert node
		else delete insertNode;
	}

	T* find(Node* currentNode, const T& data)
	{
		// check if the end of tree has been reached, return nullptr if so
		if(currentNode == nullptr)
			return nullptr;

		// direct the data to the proper node, to continue searching
		if(*currentNode > data)
			return find(currentNode->lesserNode, data);
		if(*currentNode < data)
			return find(currentNode->greaterNode, data);

		// the data is equal to the current node, return a pointer to that node
		return &currentNode->data;
	}

public:
	//--------------------------------------------------
	// constructors 
	//  - allow instantiation/moving only, no copying 
	//--------------------------------------------------

	Database()
	{}

	Database(const Database& database) = delete;

	Database(Database&& database)
	: root(database.root), count(database.count)
	{
		// leave the entered database in a hollow, but valid state
		database.root = nullptr;
		database.count = 0;
	}

	//--------------------------------------------------
	// insertion functions
	//--------------------------------------------------

	template<typename...Args>
	void emplace(Args...args)
	{
		Node *insertNode = new Node(std::forward<Args>(args)...);
		insert(root, insertNode);
	}
	
	void insert(const T& data)
	{
		Node *insertNode = new Node(data);
		insert(root, insertNode);
	}

	void insert(T&& data)
	{
		Node *insertNode = new Node(std::move(data));
		insert(root, insertNode);
	}

	//--------------------------------------------------
	// retrieval functions
	//--------------------------------------------------

	// return the largest piece of data in the tree
	T* max()
	{
		Node *currentNode = root;
		while(currentNode->greaterNode != nullptr)
			currentNode = currentNode->greaterNode;

		return (currentNode == nullptr) ? nullptr : &(currentNode->data);
	}
	
	// find and return a particular piece of data equal to the entered data
	T* find(const T& data)
	{
		return find(root, data);
	}

	// return the smallest piece of data stored in the tree
	T* min()
	{
		Node *currentNode = root;
		while(currentNode->lesserNode != nullptr)
			currentNode = currentNode->lesserNode;

		return (currentNode == nullptr) ? nullptr : &(currentNode->data);
	}

	//--------------------------------------------------
	// removal functions
	//--------------------------------------------------

	void clear()
	{
		delete root;
		count = 0;
	}

	//--------------------------------------------------
	// tree properties functions
	//--------------------------------------------------

	bool contains(const T& data) const
	{
		return find(root, data) != nullptr;
	}

	size_t size() const
	{
		return count;
	}

	//--------------------------------------------------
	// assignment operators
	//  - allow moving, but not copying
	//--------------------------------------------------

	Database& operator=(const Database& database) = delete;

	Database& operator=(Database&& database)
	{
		// ensure the tree is empty
		delete root;

		// commandeer database's variables
		root = database.root;
		count = database.count;

		// leave the entered database in a hollow, but valid state
		database.root = nullptr;
		database.count = 0;

		return *this;
	}

	//--------------------------------------------------
	// Destructor 
	//  - delete the root node, causing a chain reaction
	//  - this chain reaction deletes the entire tree
	//--------------------------------------------------
	~Database()
	{
		delete root;
	}
};
