#pragma once

// Implement this class in this header file

#include <iostream>
#include <queue>
#include <stack>

using namespace std;

template <typename T>
class hw6_234tree
{
private:
	struct Node
	{
		// A, B and C are the actual data values in the node		
		T* A;
		T* B;
		T* C;

		// Child pointers
		Node *L, *M1, *M2, *R;

		// Constructor to initialize the node with one value. The other two value 
		// pointers will be set to null and all child pointers will be set to null. 
		// The pointer given as the parameter is directly copied to A.
		Node(T* valueForA)
		{
			A = valueForA;
			B = C = NULL;
			L = M1 = M2 = R = NULL;
		}

		// Constructor to initialize the node with one value and two child pointers.
		Node(T* valueForA, Node* leftChildOfA, Node* rightChildOfA)
		{
			A = valueForA;
			L = leftChildOfA;
			M1 = rightChildOfA;
			
			// Everything else is NULL
			B = C = NULL;
			M2 = R = NULL;
		}

		Node(T* aVal, T* bVal, T* cVal, Node* left, Node* m1, Node* m2, Node* right)
		{
			A = aVal;
			B = bVal;
			C = cVal;
			L = left;
			M1 = m1;
			M2 = m2;
			R = right;
		}

		bool ContainsKey(int key)
		{
			if (C && C->GetKey() == key) { return true; }
			if (B && B->GetKey() == key) { return true; }
			if (A->GetKey() == key) { return true; }
			return false;
		}
    
		T* FindsKey(int key)
		{
			if (C && C->GetKey() == key) { return C; }
			if (B && B->GetKey() == key) { return B; }
			if (A->GetKey() == key) { return A; }
			return NULL;
		}

		void DeleteData()
		{
			if (A) { delete A; A = NULL; }
			if (B) { delete B; B = NULL; }
			if (C) { delete C; C = NULL; }
		}

		// Returns true if all three data-value pointers, A, B, and C, are non-null, 
		// false otherwise.
		bool IsFull()
		{
			return (A != NULL && B != NULL && C != NULL);
		}

		// Returns true if all 4 child pointers are null,
    // implying that the node is a leaf
		bool IsLeaf()
		{
			return (NULL == L && NULL == M1 && NULL == M2 && NULL == R);
		}

		int ValueCount()
		{
			if (C) { return 3; }
			else if (B) { return 2; }
			else if (A) { return 1; }

			// Should never come here
			return 0;
		}
	};

	Node* m_root;

	void DeleteTree(Node* root)
	{
		if (root)
		{
			root->DeleteData();
			DeleteTree(root->L);
			DeleteTree(root->M1);
			DeleteTree(root->M2);
			DeleteTree(root->R);
			delete root;
		}
	}

	void Display(Node* n)
	{
		if (!n) { return; }
		// There will always be an A value with a subtree to left and right of it
		Display(n->L);
		string nodeAsString = n->A->ToString();
		cout << nodeAsString << endl;
		Display(n->M1);

		// There may or may not be a B and C value
		if (n->B)
		{
			nodeAsString = n->B->ToString();
			cout << nodeAsString << endl;
			Display(n->M2);

			if (n->C)
			{
				nodeAsString = n->C->ToString();
				cout << nodeAsString << endl;
				Display(n->R);
			}
		}
	}

	// Finds and returns the node containing the minimum value in the subtree.
	Node* FindMin(Node* root)
	{
		if (root->IsLeaf())
		{
			return root;
		}

		Node* n = root->L;
		while (n->L) { n = n->L; }
		return n;
	}

	#if defined(DEBUG) || defined(_DEBUG)
	bool IsTreeValid(Node* n)
	{
		if (!n) { return true; }

		// First make sure that A is non-null. This should always be true for any 
		// node in the tree.
		if (!n->A)
		{
			cout << "DEBUG: Tree error: Value A in node is null!" << endl;
			return false;
		}

		// Now make sure A < B < C within the node. If C is null then we just 
		// need to verify that A < B and if B and C are NULL then it's ok.
		if (n->C)
		{
			// B must not be NULL in this case
			if (NULL == n->B)
			{
				cout << "DEBUG: Tree error: Value C in node is non-null but B is null!"
             << endl;
				return false;
			}

			if (n->C->GetKey() <= n->B->GetKey())
			{
				cout << "DEBUG: Tree error: Value C in node is <= B!" << endl;
				return false;
			}
		}
		else if (n->B)
		{
			if (n->B->GetKey() <= n->A->GetKey())
			{
				cout << "DEBUG: Tree error: Value B in node is <= A!" << endl;
				return false;
			}
		}

		// Next we make sure we have non-null child pointers where appropriate.
		if (!n->IsLeaf())
		{
			if (n->C)
			{
				if (NULL == n->R)
				{
					cout << "DEBUG: Tree error: R pointer in node is "
               << "NULL but value C is not!"
               << endl;
					return false;
				}
			}
			if (n->B)
			{
				if (NULL == n->M2)
				{
					cout << "DEBUG: Tree error: M2 pointer in node is "
               << "NULL but value B is not!"
               << endl;
					return false;
				}
			}
			if (NULL == n->M1)
			{
				cout << "DEBUG: Tree error: M1 pointer in node (key=";
				cout << n->A->GetKey() << ") is NULL!" << endl;
				return false;
			}
			if (NULL == n->L)
			{
				cout << "DEBUG: Tree error: L pointer in node is NULL!" << endl;
				return false;
			}
		}

		// For each value in the node, the left subtree should be all less and
		// the right subtree all greater.
		T* vals[] = {n->A, n->B, n->C};
		Node* children[] = {n->L, n->M1, n->M1, n->M2, n->M2, n->R};
		for (int i = 0; i < 3; i++)
		{
			if (NULL == vals[i]) { break; }
			int key = vals[i]->GetKey();

			// Make sure all keys in left subtree are less
			stack<Node*> nodes;
			nodes.push(children[i/2]);
			while (!nodes.empty())
			{
				Node* temp = nodes.top();
				nodes.pop();
				if (!temp) { continue; }

				if (temp->A->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}
				if (temp->B && temp->B->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}
				if (temp->C && temp->C->GetKey() >= key)
				{
					cout << "DEBUG ERROR: Node in left subtree of key=" << key;
					cout << " has a key of " << temp->A->GetKey() << ", which is";
					cout << "greater and thus improperly structured." << endl;
					return false;
				}

				// Push children
				nodes.push(temp->L);
				nodes.push(temp->M1);
				nodes.push(temp->M2);
				nodes.push(temp->R);
			}

			// TODO: Right subtree - implement it on your own if you need the 
			// extra error checking
		}

		// Recurse for all children
		return (IsTreeValid(n->L) && IsTreeValid(n->M1) && 
			IsTreeValid(n->M2) && IsTreeValid(n->R));
	}
#endif

public:
	hw6_234tree(void)
	{
		m_root = NULL;
	}

	virtual ~hw6_234tree(void)
	{
		DeleteTree(m_root);
		m_root = NULL;
	}

  /*****************************
   * IMPLEMENTED THIS FUNCTION *
   *****************************/
	bool Add(T& toCopyAndAdd)
	{
    // Create root node if 2-3-4 tree is empty
    if (!m_root)
    {
      m_root = new Node(new T(toCopyAndAdd));
      return true;
    }
    
    int key = toCopyAndAdd.GetKey();
    
    Node* n_ptr = m_root;  // traversing pointer
    Node* parent = n_ptr;  // previous pointer
    
    while (n_ptr)
    {
      // Check for duplicates
      if (n_ptr->ContainsKey(key)) { return false; }
    
    
      /*** CASE 1: If it's 3 nodes split it ***/
      if (n_ptr->IsFull())
      {
        // Case 1a: If parent of node has 1 key
        if (1 == parent->ValueCount())
        {
          if (parent->A->GetKey() < n_ptr->B->GetKey())
          {
            parent->B = n_ptr->B;
            parent->M1 = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
            parent->M2 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
          }
          else
          {
            parent->B = parent->A;
            parent->M2 = parent->M1;
            parent->A = n_ptr->B;
            parent->L = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
            parent->M1 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
          }
          
          delete n_ptr;
          
          if (key < parent->A->GetKey())
            n_ptr = parent->L;
          else if (key < parent->B->GetKey())
            n_ptr = parent->M1;
          else if (key > parent->B->GetKey())
            n_ptr = parent->M2;
          else
            return false; // duplicate

        } // end Case 1a
        
        // Case 1b: If parent of node has 2 keys
        else if (2 == parent->ValueCount())
        {
          if (parent->B->GetKey() < n_ptr->B->GetKey())
          {
            parent->C = n_ptr->B;
            parent->M2 = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
            parent->R = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
          }
          else if (parent->A->GetKey() < n_ptr->B->GetKey())
          {
            parent->C = parent->B;
            parent->R = parent->M2;
            parent->B = n_ptr->B;
            parent->M1 = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
            parent->M2 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
          }
          else
          {
            parent->C = parent->B;
            parent->R = parent->M2;
            parent->B = parent->A;
            parent->M2 = parent->M1;
            parent->A = n_ptr->B;
            parent->L = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
            parent->M1 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
          }
          
          delete n_ptr;
          
          if (key < parent->A->GetKey())
            n_ptr = parent->L;
          else if (key < parent->B->GetKey())
            n_ptr = parent->M1;
          else if (key < parent->C->GetKey())
            n_ptr = parent->M2;
          else if (key > parent->C->GetKey())
            n_ptr = parent->R;
          else
            return false; // duplicate
        } // end of Case 1b
        
        // Case 1c: If parent of node has 3 keys
        else
        {
          m_root = new Node(n_ptr->B);
          m_root->L = new Node(n_ptr->A, n_ptr->L, n_ptr->M1);
          m_root->M1 = new Node(n_ptr->C, n_ptr->M2, n_ptr->R);
          delete n_ptr;
          parent = m_root;
          if (key < m_root->A->GetKey())
            n_ptr = m_root->L;
          else if (key > m_root->A->GetKey())
            n_ptr = m_root->M1;
          else
            return false; // duplicate
        } // end Case 1c

      } // end Case 1
      
      
      /*** CASE 2: Is it a leaf node ***/
      if (n_ptr->IsLeaf())
      {
        // Case 2a: Has 1 key
        if (1 == n_ptr->ValueCount())
        {
          if (key > n_ptr->A->GetKey())
          {
            n_ptr->B = new T(toCopyAndAdd);
            return true;
          }
          else if (key < n_ptr->A->GetKey())
          {
            T* temp = new T(toCopyAndAdd);
            n_ptr->B = n_ptr->A;
            n_ptr->A = temp;
            return true;
          }
        } // end Case 2a
        
        // Case 2b: Has 2 keys
        else if (2 == n_ptr->ValueCount())
        {
          if (key > n_ptr->B->GetKey())
          {
            n_ptr->C = new T(toCopyAndAdd);
            return true;
          }
          else if (key > n_ptr->A->GetKey())
          {
            T* temp = new T(toCopyAndAdd);
            n_ptr->C = n_ptr->B;
            n_ptr->B = temp;
            return true;
          }
          else if (key < n_ptr->A->GetKey())
          {
            T* temp = new T(toCopyAndAdd);
            n_ptr->C = n_ptr->B;
            n_ptr->B = n_ptr->A;
            n_ptr->A = temp;
            return true;
          }
        } // end Case 2b
      } // end Case 2
  
      
      /*** CASE 3: Not a leaf node ***/
      else
      {
        // Assigns previous pointer
        parent = n_ptr;
        
        // Case 3a: Has 1 key
        if (1 == n_ptr->ValueCount())
        {
          if (key < n_ptr->A->GetKey())
            n_ptr = n_ptr->L;
          else
            n_ptr = n_ptr->M1;
        }
        
        // Case 3b: Has 2 keys
        else if (2 == n_ptr->ValueCount())
        {
          if (key < n_ptr->A->GetKey())
            n_ptr = n_ptr->L;
          else if (key < n_ptr->B->GetKey())
            n_ptr = n_ptr->M1;
          else
            n_ptr = n_ptr->M2;
        }
      } // end Case 3
      
    } // end while
    
    
    // Should never get here!
    return false;
  }

	void Display(ostream& outstream)
	{
		if (!m_root)
		{
			outstream << "(tree is empty)";
		}
		Display(m_root);
	}

	void DisplayLevelKeys(ostream& outstream)
	{
		if (!m_root)
		{
			outstream << "(tree is empty, no levels to display)";
			return;
		}

    struct NodeLevel
		{
			Node* node;
			int level;
			NodeLevel(Node* n, int l) { node = n; level = l; }
		};
		
		queue<NodeLevel> nodes;
		nodes.push(NodeLevel(m_root, 0));
		int maxLevel = 0;
		while (!nodes.empty())
		{
			NodeLevel nl = nodes.front();
			nodes.pop();
			Node* n = nl.node;
			if (nl.level > maxLevel)
			{
				outstream << endl;
				maxLevel = nl.level;
			}

			outstream << "< " << n->A->GetKey() << " ";
			if (n->B) { outstream << n->B->GetKey() << " "; }
			if (n->C) { outstream << n->C->GetKey() << " "; }
			outstream << ">";

			// Enqueue children
			if (n->L) { nodes.push(NodeLevel(n->L, nl.level + 1)); }
			if (n->M1) { nodes.push(NodeLevel(n->M1, nl.level + 1)); }
			if (n->M2) { nodes.push(NodeLevel(n->M2, nl.level + 1)); }
			if (n->R) { nodes.push(NodeLevel(n->R, nl.level + 1)); }
		}
		outstream << endl;
	}

  /*****************************
   * IMPLEMENTED THIS FUNCTION *
   *****************************/
	T* Find(int key)
	{
    Node* n_ptr = m_root;
    
    T* result = n_ptr->FindsKey(key);
    
    if (result) { return result; }
    else
    {
      if (n_ptr->C && n_ptr->C->GetKey() < key)
      {
        return Find (key, n_ptr->R);
      }
      else if (n_ptr->B && n_ptr->B->GetKey() < key)
      {
        return Find (key, n_ptr->M2);
      }
      else if (n_ptr->A->GetKey() < key)
      {
        return Find (key, n_ptr->M1);
      }
      else
      {
        return Find (key, n_ptr->L);
      }
    }
    
		return NULL;
	}
  
  T* Find(int const& key, Node* &n_ptr)
  {
    if (!n_ptr)
      return NULL;
    
    T* result = n_ptr->FindsKey(key);
    if (result) { return result; }
      else
    {
      if (n_ptr->C && n_ptr->C->GetKey() < key)
      {
        return Find (key, n_ptr->R);
      }
      else if (n_ptr->B && n_ptr->B->GetKey() < key)
      {
        return Find (key, n_ptr->M2);
      }
      else if (n_ptr->A->GetKey() < key)
      {
        return Find (key, n_ptr->M1);
      }
      else
      {
        return Find (key, n_ptr->L);
      }
    }
  }

	T* GetMin()
	{
		if (!m_root) { return NULL; }
		Node* n = FindMin(m_root);
		return n->A;
	}

#if defined(DEBUG) || defined(_DEBUG)
	bool IsTreeValid()
	{
		return IsTreeValid(m_root);
	}
#endif

  /*****************************
   * IMPLEMENTED THIS FUNCTION *
   *****************************/
	bool Remove(int key)
	{
		if (!m_root) { return false; }

		// Implement the rest of this function for extra credit
		// Return false if the value is not present in the tree
		// Don't forget the rules for fusing nodes
    
    Node* n_ptr = m_root;
    Node* parent = m_root;
    
    T* the_item = NULL;
    Node* n_item = NULL;
    
    while (n_ptr)
    {
      // Case 1: If root and root has single key
      // and both descendents has single keys
      if (n_ptr == m_root && n_ptr->ValueCount() == 1 &&
          n_ptr->L->ValueCount() == 1 &&
          n_ptr->M1->ValueCount() == 1)
      {
        // Set value for C and R & M2 pointers
        Node* right = n_ptr->M1;
        n_ptr->C = right->A;
        n_ptr->M2 = right->L;
        n_ptr->R = right->M1;
        delete right;
        
        // Set value for B
        n_ptr->B = n_ptr->A;
        
        // Set value for A and L & M1 pointers
        Node* left = n_ptr->L;
        n_ptr->A = left->A;
        n_ptr->L = left->L;
        n_ptr->M1 = left->M1;
        delete left;
        
        parent = m_root;
        n_ptr = m_root;
      } // end Case 1
    
      // Cases 2: Transformations (Ensure the_item does not occur in a 2-node)
      // a) Transform each node encountered into a 3 node or a 4 node
      if (n_ptr->ValueCount() == 1)
      {
        // single key on L pointer
        if (parent->L == n_ptr)
        {
          // Parent has more than 1 key and M1 has 1 key, FUSE IT
          if (parent->M1->ValueCount() == 1)
          {
            // set C and pointers R & M2
            parent->M1->C = parent->M1->A;
            parent->M1->R = parent->M1->M1;
            parent->M1->M2 = parent->M1->L;
            // set B
            parent->M1->B = parent->A;
            // set A and pointer L & M1
            parent->M1->A = n_ptr->A;
            parent->M1->L = n_ptr->L;
            parent->M1->M1 = n_ptr->M1;
            
            delete parent->L;
            parent->A = parent->B;
            parent->B = parent->C;
            parent->C = NULL;
            parent->L = parent->M1;
            parent->M1 = parent->M2;
            parent->M2 = parent->R;
            parent->R = NULL;
            
            n_ptr = parent->L;
          }
          
          // Sibling has more than 1 key. Borrow
          else if (parent->M1->ValueCount() >= 2)
          {
            // set B and pointer M2
            n_ptr->B = parent->A;
            n_ptr->M2 = parent->M1->L;
            
            // set parent A
            parent->A = parent->M1->A;
            
            // set A, B, C of pointer M1 and its pointers
            parent->M1->A = parent->M1->B;
            parent->M1->B = parent->M1->C;
            parent->M1->C = NULL;
            parent->M1->L = parent->M1->M1;
            parent->M1->M1 = parent->M1->M2;
            parent->M1->M2 = parent->M1->R;
            parent->M1->R = NULL;
          }
        } // end single key on L pointer
        
        // single key on M1 pointer
        else if (parent->M1 == n_ptr)
        {
          // L & M2 have single key, FUSE L & M1
          if (parent->L->ValueCount() == 1 && parent->M2->ValueCount() == 1)
          {
            // set C and pointers R & M2
            n_ptr->C = n_ptr->A;
            n_ptr->R = n_ptr->M1;
            n_ptr->M2 = n_ptr->L;
            // set B
            n_ptr->B = parent->A;
            // set A and pointers L & M1
            n_ptr->A = parent->L->A;
            n_ptr->M1 = parent->L->M1;
            n_ptr->L = parent->L->L;
            
            delete parent->L;
            parent->A = parent->B;
            parent->B = parent->C;
            parent->C = NULL;
            parent->L = parent->M1;
            parent->M1 = parent->M2;
            parent->M2 = parent->R;
            parent->R = NULL;
            
            // n_ptr already good
          }
          
          // borrow from M2
          else if (parent->L->ValueCount() < parent->M2->ValueCount())
          {
            // set B and pointer M2
            n_ptr->B = parent->B;
            n_ptr->M2 = parent->M2->L;
            
            // set parent B
            parent->B = parent->M2->A;
            
            // set M2 values and pointers
            parent->M2->A = parent->M2->B;
            parent->M2->B = parent->M2->C;
            parent->M2->C = NULL;
            parent->M2->L = parent->M2->M1;
            parent->M2->M1 = parent->M2->M2;
            parent->M2->M2 = parent->M2->R;
            parent->M2->R = NULL;
          }
          
          // borrow from L
          else
          {
            // set B and pointer M1 & M2
            n_ptr->B = n_ptr->A;
            n_ptr->M2 = n_ptr->M1;
            n_ptr->M1 = n_ptr->L;
            
            // set A, parent A and pointer L
            n_ptr->A = parent->A;
            if (parent->L->C)
            {
              n_ptr->L = parent->L->R;
              parent->A = parent->L->C;
              parent->L->C = NULL;
              parent->L->R = NULL;
            }
            else
            {
              n_ptr->L = parent->L->M2;
              parent->A = parent->L->B;
              parent->L->B = NULL;
              parent->L->M2 = NULL;
            }
          }
          
        } // end single key on M1 pointer
        
        // single key on M2 pointer
        else if (parent->M2 == n_ptr)
        {
          // M1 & R have single key, FUSE M2 & R
          if (parent->R && parent->M1->ValueCount() == 1 &&
              parent->R->ValueCount() == 1)
          {
            // set B
            n_ptr->B = parent->C;
            parent->C = NULL;
            
            // set C and pointer M2 & R
            n_ptr->C = parent->R->A;
            n_ptr->M2 = parent->R->L;
            n_ptr->R = parent->R->M1;
            
            delete parent->R;
            parent->R = NULL;
            
            // n_ptr is good
          }
          
          // borrow from R
          else if (parent->R &&
                   parent->R->ValueCount() >=  parent->M1->ValueCount())
          {
            // set B and pointer M2
            n_ptr->B = parent->C;
            n_ptr->M2 = parent->R->L;
            
            // set parent C
            parent->C = parent->R->A;
            
            // set R values and pointers
            parent->R->A = parent->R->B;
            parent->R->B = parent->R->C;
            parent->R->C = NULL;
            parent->R->L = parent->R->M1;
            parent->R->M1 = parent->R->M2;
            parent->R->M2 = parent->R->R;
            parent->R->R = NULL;
            
            // n_ptr is good
          }
          
          // borrow from M1
          else
          {
            // set B and pointer M1 & M2
            n_ptr->B = n_ptr->A;
            n_ptr->M2 = n_ptr->M1;
            n_ptr->M1 = n_ptr->L;
            
            // set A, parent B and pointer L
            n_ptr->A = parent->B;
            if (parent->M1->C)
            {
              n_ptr->L = parent->M1->R;
              parent->B = parent->M1->C;
              parent->M1->R = NULL;
              parent->M1->C = NULL;
            }
            else
            {
              n_ptr->L = parent->M1->M2;
              parent->B = parent->M1->B;
              parent->M1->M2 = NULL;
              parent->M1->B = NULL;
            }
          }
          
        } // end single key on M2 pointer
        
        // single key on R pointer
        else if (parent->R == n_ptr)
        {
          // M2 has a single key, FUSE M2 & R
          if (parent->M2->ValueCount() == 1)
          {
            // set B
            parent->M2->B = parent->C;
            parent->C = NULL;
            
            // set C and pointer M2 & R
            parent->M2->C = n_ptr->A;
            parent->M2->M2 = n_ptr->L;
            parent->M2->R = n_ptr->M1;
            
            n_ptr->A = NULL;
            n_ptr->L = NULL;
            n_ptr->M1 = NULL;
            delete parent->R;
            
            n_ptr = parent->M2;
          }
          
          // borrow from M2
          else if (parent->M2->ValueCount() >= 2)
          {
            // set B and pointer M1 & M2
            n_ptr->B = n_ptr->A;
            n_ptr->M2 = n_ptr->M1;
            n_ptr->M1 = n_ptr->L;
            
            // set A, parent C and pointer L
            n_ptr->A = parent->C;
            if (parent->M2->C)
            {
              n_ptr->L = parent->M2->R;
              parent->C = parent->M2->C;
              parent->M2->R = NULL;
              parent->M2->C = NULL;
            }
            else
            {
              n_ptr->L = parent->M2->M2;
              parent->C = parent->M2->B;
              parent->M2->M2 = NULL;
              parent->M2->C = NULL;
            }
          }
        
        } // end single key on R pointer
      }
    
      // Case 3: Item hasn't been found
      // but this node contains key
      if (!the_item && n_ptr->ContainsKey(key) && !n_ptr->IsLeaf())
      {
        parent = n_ptr;
        n_item = n_ptr;      // points to the node where the key is found
        if (n_ptr->C && key == n_ptr->C->GetKey())
        {
          the_item = n_ptr->C;
          n_ptr = n_ptr->R;  // go to right of key
        }
        else if (n_ptr->B && key == n_ptr->B->GetKey())
        {
          the_item = n_ptr->B;
          n_ptr = n_ptr->M2; // go to right of key
        }
        else
        {
          the_item = n_ptr->A;
          n_ptr = n_ptr->M1; // go to right of key
        }
      } // Case 3
    
      // Case 4: Item already found
      // and node contains key (assume its a leaf)
      // The key here is the inorder successor key
      else if ((the_item && n_ptr->ContainsKey(key)) ||
               (n_ptr->ContainsKey(key) && n_ptr->IsLeaf()))
      {
        while (1)
        {
          // # of keys greater than or equal to 2 keys
          // delete the key
          if (n_ptr->ValueCount() >= 2)
          {
            // key is C
            if (n_ptr->C && key == n_ptr->C->GetKey())
            {
              delete n_ptr->C;
              n_ptr->C = NULL;
            }
            // key is B
            else if (n_ptr->B && key == n_ptr->B->GetKey())
            {
              delete n_ptr->B;
              n_ptr->B = n_ptr->C;
              n_ptr->C = NULL;
            }
            // key is A
            else // <----- PROBLEM MIGHT BE HERE
            {
              delete n_ptr->A;
              n_ptr->A = n_ptr->B;
              n_ptr->B = n_ptr->C;
              n_ptr->C = NULL;
            }
            return true;
          }
          
          // key on R pointer
          else if (parent->R && parent->R == n_ptr)
          {
            // M2 & R has single keys. FUSE IT
            if (parent->M2->ValueCount() == 1 && parent->R->ValueCount() == 1)
            {
              parent->M2->B = parent->C;
              parent->M2->C = n_ptr->A;
              n_ptr->A = NULL;
              delete parent->R;
              parent->R = NULL;
              parent->C = NULL;
              n_ptr = parent->M2;
            }
            // R has one key and M2 has more than one key
            else
            {
              // borrow from parent C
              n_ptr->B = n_ptr->A;
              n_ptr->A = parent->C;
              // parent C borrow C from M2
              if (parent->M2->C)
              {
                parent->C = parent->M2->C;
                parent->M2->C = NULL;
              }
              // parent C borrow B from M2
              else
              {
                parent->C = parent->M2->B;
                parent->M2->B = NULL;
              }
            }
          }
        
          // key on M2 pointer
          else if (parent->M2 && parent->M2 == n_ptr)
          {
            // M1, M2 & R has single keys. FUSE M2 & R
            if ((parent->M1->ValueCount() == 1 &&
                 parent->M2->ValueCount() == 1 && !parent->C) ||
                (parent->R && parent->M1->ValueCount() == 1 &&
                 parent->M2->ValueCount() == 1 && parent->R->ValueCount() == 1))
            {
              n_ptr->B = parent->C;
              n_ptr->C = parent->R->A;
              parent->R->A = NULL;
              delete parent->R;
              parent->R = NULL;
              parent->C = NULL;
            }
            // M2 has one key and R has more keys than M1
            else if (parent->R &&
                     parent->R->ValueCount() >= parent->M1->ValueCount())
            {
              // borrow from parent C
              n_ptr->B = parent->C;
              // parent C borrow A from R
              parent->C = parent->R->A;
              parent->R->A = parent->R->B;
              parent->R->B = parent->R->C;
              parent->R->C = NULL;
            }
            // M2 has one key and M1 has more keys than R
            else
            {
              // borrow from parent B
              n_ptr->B = n_ptr->A;
              n_ptr->A = parent->B;
              // parent B borrow C from M1
              if (parent->M1->C)
              {
                parent->B = parent->M1->C;
                parent->M1->C = NULL;
              }
              // parent B borrow B from M1
              else
              {
                parent->B = parent->M1->B;
                parent->M1->B = NULL;
              }
            }
          }
          
          // key on M1 pointer
          else if (parent->M1 && parent->M1 == n_ptr)
          {
            // L, M1, & M2 has single keys. FUSE L & M1
            if (parent->L->ValueCount() == 1 &&
                parent->M1->ValueCount() == 1 &&
                parent->M2->ValueCount() == 1)
            {
              n_ptr->C = n_ptr->A;
              n_ptr->B = parent->A;
              n_ptr->A = parent->L->A;
              parent->L->A = NULL;
              delete parent->L;
              parent->A = parent->B;
              parent->B = parent->C;
              parent->L = parent->M1;
              parent->M1 = parent->M2;
              parent->M2 = parent->R;
              parent->R = NULL;
            }
            // M1 has one key and M2 has more keys than L
            else if (parent->L->ValueCount() < parent->M2->ValueCount())
            {
              // borrow from parent B
              n_ptr->B = parent->B;
              // parent B borrow A from M2
              parent->B = parent->M2->A;
              parent->M2->A = parent->M2->B;
              parent->M2->B = parent->M2->C;
              parent->M2->C = NULL;
            }
            // M1 has one key and L has more keys than M2
            else
            {
              // borrow from parent A
              n_ptr->B = n_ptr->A;
              n_ptr->A = parent->A;
              // parent A borrow C from L
              if (parent->L->C)
              {
                parent->A = parent->L->C;
                parent->L->C = NULL;
              }
              // parent A borrow B from L
              else
              {
                parent->A = parent->L->B;
                parent->L->B = NULL;
              }
            }
          }
          
          // key on L pointer
          else
          {
            // L, M1 has single keys. FUSE IT!
            if (parent->L->ValueCount() == 1 && parent->M1->ValueCount() == 1)
            {
              parent->M1->C = parent->M1->A;
              parent->M1->B = parent->A;
              parent->M1->A = parent->L->A;
              parent->L->A = NULL;
              delete parent->L;
              parent->A = parent->B;
              parent->B = parent->C;
              parent->L = parent->M1;
              parent->M1 = parent->M2;
              parent->M2 = parent->R;
              parent->R = NULL;
              
              n_ptr = parent->M1;
            }
            // L has one key, borrow from M1
            else
            {
              // borrow from parent A
              n_ptr->B = parent->A;
              // parent A borrow A from M1
              parent->A = parent->M1->A;
              parent->M1->A = parent->M1->B;
              parent->M1->B = parent->M1->C;
              parent->M1->C = NULL;
            }
          }
        } // end while (1)
      } // end Case 4
      
      // Case: Item not found yet so continue looking
      if (!the_item)
      {
        parent = n_ptr;
        if (n_ptr->C && key > n_ptr->C->GetKey())      { n_ptr = n_ptr->R;  }
        else if (n_ptr->B && key > n_ptr->B->GetKey()) { n_ptr = n_ptr->M2; }
        else if (key > n_ptr->A->GetKey())             { n_ptr = n_ptr->M1; }
        else                                           { n_ptr = n_ptr->L;  }
      }
      // Case: Item found. Look for inorder successor
      else
      {
        if (n_ptr->L == NULL) // n_ptr key A is the inorder successor
        {
          if (n_item->A == the_item)
          {
            delete n_item->A;
            n_item->A = new T(*(n_ptr->A));
          }
          else if (n_item->B == the_item)
          {
            delete n_item->B;
            n_item->B = new T(*(n_ptr->A));
          }
          else
          {
            delete n_item->C;
            n_item->C = new T(*(n_ptr->A));
          }

          key = n_ptr->A->GetKey();
        }
        else
        {
          parent = n_ptr;
          n_ptr = n_ptr->L;   // go to left of key
        }
      }
    }
    
		return false;
	}
  
};
