#pragma once
#include "TreeNode.h"
#include "DL_Debug.h"
#include "Macros.h"

#define BINARY_TREE_TEMPLATE template <typename Type>
#define BINARY_TREE_OBJECT BinaryTree<Type>

namespace CommonUtilities
{
	BINARY_TREE_TEMPLATE
	class BinaryTree
	{
	public:

		BinaryTree();
		~BinaryTree();

		TreeNode<Type>* GetRootNode();

		void InsertNode(TreeNode<Type>* aNodeToInsert);
		void InsertNode(const Type& aDataToInsert);

		void DeleteNode(TreeNode<Type>* aNode);
		void DeleteNode(const Type& aData);

		TreeNode<Type>* Find(const Type& aData);

		void PostOrderTraverse();
		void PreOrderTraverse();
		void InOrderTraverse();

		void DSW_Balance();

	private:

		TreeNode<Type>* myRoot;

		void InsertNode(TreeNode<Type>* aNodeToInsert, TreeNode<Type>* aCurrentNode);
		TreeNode<Type>* Find(const Type& aData, TreeNode<Type>* aCurrentNode);

		TreeNode<Type>* FindHighestValueNode(TreeNode<Type>* aCurrentNode);
		TreeNode<Type>* FindNodeBeforeValue(const Type& aData, TreeNode<Type>* aPreviousNode, TreeNode<Type>* aCurrentNode);

		void Print(const Type& aData);

		void PreOrderTraverse(TreeNode<Type>* aCurrentNode);
		void PostOrderTraverse(TreeNode<Type>* aCurrentNode);
		void InOrderTraverse(TreeNode<Type>* aCurrentNode);

		void DeleteAll(TreeNode<Type>* aCurrentNode);

		void DeleteNodeWith2Children(TreeNode<Type>* aNodeToDelete);
		void DeleteNodeWithLeftChild(TreeNode<Type>* aNodeToDelete);
		void DeleteNodeWithRightChild(TreeNode<Type>* aNodeToDelete);
		void DeleteNodeWithNoChildren(TreeNode<Type>* aNodeToDelete);

		int ConvertToLinkedListTree();
		void ConvertLinkedListTreeToPerfectTree(int aComplexity);
		void RotationRight(TreeNode<Type>* aCurrent, TreeNode<Type>* aParent);
		void RotationLeft(TreeNode<Type>* aCurrent, TreeNode<Type>* aParent);

		void SwapChild(TreeNode<Type>* aCurrentNode, TreeNode<Type>* aNewChildNode);

	};

	BINARY_TREE_TEMPLATE
		BINARY_TREE_OBJECT::BinaryTree()
	{
		myRoot = nullptr;
	}

	BINARY_TREE_TEMPLATE
		BINARY_TREE_OBJECT::~BinaryTree()
	{
		if (myRoot != nullptr)
		{
			DeleteAll(myRoot);
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::Print(const Type& aData)
	{
		std::string printText;

		printText = std::to_string(aData) + " ";

		OutputDebugStringA(printText.c_str());
		std::cout << printText;
	}

	BINARY_TREE_TEMPLATE
		TreeNode<Type>* BINARY_TREE_OBJECT::GetRootNode()
	{
		return myRoot;
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::InsertNode(TreeNode<Type>* aNodeToInsert)
	{
		if (aNodeToInsert == nullptr)
		{
			DL_ASSERT("Can't insert a nullpointer node.");
		}

		if (myRoot != nullptr)
		{
			InsertNode(aNodeToInsert, myRoot);
		}
		else
		{
			myRoot = aNodeToInsert;
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::InsertNode(const Type& aDataToInsert)
	{
		TreeNode<Type>* newNode = new TreeNode<Type>(aDataToInsert);
		InsertNode(newNode);
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::InsertNode(TreeNode<Type>* aNodeToInsert, TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->GetData() > aNodeToInsert->GetData())
		{
			if (aCurrentNode->myLeftChild == nullptr)
			{
				aCurrentNode->myLeftChild = aNodeToInsert;
			}
			else
			{
				InsertNode(aNodeToInsert, aCurrentNode->myLeftChild);
			}
		}
		else if (aCurrentNode->GetData() < aNodeToInsert->GetData())
		{
			if (aCurrentNode->myRightChild == nullptr)
			{
				aCurrentNode->myRightChild = aNodeToInsert;
			}
			else
			{
				InsertNode(aNodeToInsert, aCurrentNode->myRightChild);
			}
		}
		else
		{
			return;
		}
	}
	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteNodeWith2Children(TreeNode<Type>* aNodeToDelete)
	{
		TreeNode<Type>* replacingNode = FindHighestValueNode(aNodeToDelete->myLeftChild);
		Type data = replacingNode->GetData();

		if (replacingNode->myLeftChild != nullptr)
		{
			TreeNode<Type>* nodeToDelete = replacingNode;
			TreeNode<Type>* previousNode = FindNodeBeforeValue(data, nullptr, myRoot);

			if (previousNode != aNodeToDelete)
			{
				previousNode->myRightChild = replacingNode->myLeftChild;
			}
			else
			{
				aNodeToDelete->myLeftChild = replacingNode->myLeftChild;
			}
			aNodeToDelete->myData = replacingNode->myData;
			SAFE_DELETE(nodeToDelete);
		}
		else
		{
			TreeNode<Type>* previousNode = FindNodeBeforeValue(data, nullptr, myRoot);
			if (previousNode != aNodeToDelete)
			{
				aNodeToDelete->myData = data;
				previousNode->myRightChild = nullptr;
			}
			else
			{
				aNodeToDelete->myData = data;
				aNodeToDelete->myLeftChild = nullptr;
			}
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteNodeWithLeftChild(TreeNode<Type>* aNodeToDelete)
	{
		if (aNodeToDelete != myRoot)
		{
			TreeNode<Type>* previousNode = FindNodeBeforeValue(aNodeToDelete->GetData(), nullptr, myRoot);
			if (aNodeToDelete->GetData() < previousNode->GetData())
			{
				previousNode->myLeftChild = aNodeToDelete->myLeftChild;
			}
			else
			{
				previousNode->myRightChild = aNodeToDelete->myLeftChild;
			}
		}
		else
		{
			myRoot = aNodeToDelete->myLeftChild;
		}

		SAFE_DELETE(aNodeToDelete);
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteNodeWithRightChild(TreeNode<Type>* aNodeToDelete)
	{
		if (aNodeToDelete != myRoot)
		{
			TreeNode<Type>* previousNode = FindNodeBeforeValue(aNodeToDelete->GetData(), nullptr, myRoot);
			if (aNodeToDelete->GetData() < previousNode->GetData())
			{
				previousNode->myLeftChild = aNodeToDelete->myRightChild;
			}
			else
			{
				previousNode->myRightChild = aNodeToDelete->myRightChild;
			}
		}
		else
		{
			myRoot = aNodeToDelete->myRightChild;
		}
		SAFE_DELETE(aNodeToDelete);
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteNodeWithNoChildren(TreeNode<Type>* aNodeToDelete)
	{
		TreeNode<Type>* previousNode = FindNodeBeforeValue(aNodeToDelete->GetData(), nullptr, myRoot);

		if (previousNode == nullptr)
		{
			SAFE_DELETE(myRoot);
		}
		else
		{
			if (previousNode->myLeftChild != nullptr &&
				previousNode->myLeftChild->GetData() == aNodeToDelete->GetData())
			{
				SAFE_DELETE(previousNode->myLeftChild);
			}
			else if (previousNode->myRightChild != nullptr&&
				previousNode->myRightChild->GetData() == aNodeToDelete->GetData())
			{
				SAFE_DELETE(previousNode->myRightChild);
			}
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteNode(TreeNode<Type>* aNode)
	{
		if (aNode == nullptr)
		{
			DL_ASSERT("Can't delete a node that is a nullpointer.")
		}

		TreeNode<Type>* foundNode = Find(aNode->GetData());

		if (foundNode == nullptr || foundNode != aNode)
		{
			DL_ASSERT("Node does not exist in tree");
		}

		if (aNode->myLeftChild != nullptr && aNode->myRightChild != nullptr)
		{
			DeleteNodeWith2Children(aNode);
		}
		else if (aNode->myRightChild != nullptr)
		{
			DeleteNodeWithRightChild(aNode);
		}
		else if (aNode->myLeftChild != nullptr)
		{
			DeleteNodeWithLeftChild(aNode);
		}
		else
		{
			DeleteNodeWithNoChildren(aNode);
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteNode(const Type& aData)
	{
		TreeNode<Type>* foundNode = Find(aData);

		if (foundNode == nullptr)
		{
			std::string errorText = "No node found with data ";

			errorText += std::to_string(aData);

			DL_ASSERT(errorText.c_str());
		}

		DeleteNode(foundNode);
	}

	BINARY_TREE_TEMPLATE
		TreeNode<Type>* BINARY_TREE_OBJECT::FindNodeBeforeValue(const Type& aData, TreeNode<Type>* aPreviousNode, TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->GetData() == aData)
		{
			return aPreviousNode;
		}
		else if (aCurrentNode->GetData() < aData)
		{
			if (aCurrentNode->myRightChild == nullptr)
			{
				return nullptr;
			}
			else
			{
				return FindNodeBeforeValue(aData, aCurrentNode, aCurrentNode->myRightChild);
			}
		}
		else if (aCurrentNode->GetData() > aData)
		{
			if (aCurrentNode->myLeftChild == nullptr)
			{
				return nullptr;
			}
			else
			{
				return FindNodeBeforeValue(aData, aCurrentNode, aCurrentNode->myLeftChild);
			}
		}
		else
		{
			return nullptr;
		}
	}

	BINARY_TREE_TEMPLATE
		TreeNode<Type>* BINARY_TREE_OBJECT::FindHighestValueNode(TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->myRightChild == nullptr)
		{
			return aCurrentNode;
		}
		else
		{
			return FindHighestValueNode(aCurrentNode->myRightChild);
		}
	}

	BINARY_TREE_TEMPLATE
		TreeNode<Type>* BINARY_TREE_OBJECT::Find(const Type& aData)
	{
		if (myRoot != nullptr)
		{
			return Find(aData, myRoot);
		}
		else
		{
			return nullptr;
		}
	}

	BINARY_TREE_TEMPLATE
		TreeNode<Type>* BINARY_TREE_OBJECT::Find(const Type& aData, TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->GetData() == aData)
		{
			return aCurrentNode;
		}
		else if (aCurrentNode->GetData() < aData)
		{
			if (aCurrentNode->myRightChild == nullptr)
			{
				return nullptr;
			}
			else
			{
				return Find(aData, aCurrentNode->myRightChild);
			}
		}
		else if (aCurrentNode->GetData() > aData)
		{
			if (aCurrentNode->myLeftChild == nullptr)
			{
				return nullptr;
			}
			else
			{
				return Find(aData, aCurrentNode->myLeftChild);
			}
		}
		else
		{
			return nullptr;
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::PostOrderTraverse()
	{
		if (myRoot != nullptr)
		{
			PostOrderTraverse(myRoot);
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::PreOrderTraverse()
	{
		if (myRoot != nullptr)
		{
			PreOrderTraverse(myRoot);
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::InOrderTraverse()
	{
		if (myRoot != nullptr)
		{
			InOrderTraverse(myRoot);
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::PreOrderTraverse(TreeNode<Type>* aCurrentNode)
	{
		Print(aCurrentNode->GetData());

		if (aCurrentNode->myLeftChild != nullptr)
		{
			PreOrderTraverse(aCurrentNode->myLeftChild);
		}
		if (aCurrentNode->myRightChild != nullptr)
		{
			PreOrderTraverse(aCurrentNode->myRightChild);
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::PostOrderTraverse(TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->myLeftChild != nullptr)
		{
			PostOrderTraverse(aCurrentNode->myLeftChild);
		}
		if (aCurrentNode->myRightChild != nullptr)
		{
			PostOrderTraverse(aCurrentNode->myRightChild);
		}

		Print(aCurrentNode->GetData());
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::InOrderTraverse(TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->myLeftChild != nullptr)
		{
			InOrderTraverse(aCurrentNode->myLeftChild);
		}

		Print(aCurrentNode->GetData());

		if (aCurrentNode->myRightChild != nullptr)
		{
			InOrderTraverse(aCurrentNode->myRightChild);
		}

	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DeleteAll(TreeNode<Type>* aCurrentNode)
	{
		if (aCurrentNode->myLeftChild != nullptr)
		{
			DeleteAll(aCurrentNode->myLeftChild);
		}
		if (aCurrentNode->myRightChild != nullptr)
		{
			DeleteAll(aCurrentNode->myRightChild);
		}

		SAFE_DELETE(aCurrentNode);
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::DSW_Balance()
	{
		int complexity = ConvertToLinkedListTree();
		ConvertLinkedListTreeToPerfectTree(complexity);
	}

	BINARY_TREE_TEMPLATE
		int BINARY_TREE_OBJECT::ConvertToLinkedListTree()
	{
		TreeNode<Type>* currentNode = myRoot;
		int complexity = 0;

		while (currentNode != nullptr)
		{
			if (currentNode->myLeftChild != nullptr)
			{
				TreeNode<Type>* nextNode = currentNode->myLeftChild;
				RotationRight(currentNode->myLeftChild, currentNode);
				currentNode = nextNode;
			}
			else
			{
				currentNode = currentNode->myRightChild;
				complexity++;
			}
			
		}
		return complexity;
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::ConvertLinkedListTreeToPerfectTree(int aComplexity)
	{
		int m = static_cast<int>(pow(2, floor(log2(aComplexity + 1))) - 1);

		TreeNode<Type>* parentNode = myRoot;

		for (int i = 0; i < (aComplexity - m); ++i)
		{
			TreeNode<Type>* nextNode = parentNode->myRightChild->myRightChild;
			RotationLeft(parentNode->myRightChild, parentNode);
			parentNode = nextNode;
		}

		while (m > 1)
		{		
			parentNode = myRoot;
			m = m / 2;
			for (int i = 0; i < m; ++i)
			{
				TreeNode<Type>* nextNode = parentNode->myRightChild->myRightChild;
				RotationLeft(parentNode->myRightChild, parentNode);
				parentNode = nextNode;
			}
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::RotationRight(TreeNode<Type>* aCurrent, TreeNode<Type>* aParent)
	{
		if (aParent != nullptr)
		{
			SwapChild(aParent, aCurrent);

			TreeNode<Type>* currentRightChild = aCurrent->myRightChild;
			aCurrent->myRightChild = aParent;
			aParent->myLeftChild = currentRightChild;

			if (aParent == myRoot)
			{
				myRoot = aCurrent;
			}
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::RotationLeft(TreeNode<Type>* aCurrent, TreeNode<Type>* aParent)
	{
		if (aParent != nullptr)
		{
			SwapChild(aParent, aCurrent);

			TreeNode<Type>* currentLeftChild = aCurrent->myLeftChild;
			aCurrent->myLeftChild = aParent;
			aParent->myRightChild = currentLeftChild;

			if (aParent == myRoot)
			{
				myRoot = aCurrent;
			}
		}
	}

	BINARY_TREE_TEMPLATE
		void BINARY_TREE_OBJECT::SwapChild(TreeNode<Type>* aCurrentNode, TreeNode<Type>* aNewChildNode)
	{
		if (aCurrentNode != myRoot)
		{
			TreeNode<Type>* parent = FindNodeBeforeValue(aCurrentNode->GetData(), nullptr, myRoot);

			if (parent->myLeftChild == aCurrentNode)
			{
				parent->myLeftChild = aNewChildNode;
			}
			else if (parent->myRightChild == aCurrentNode)
			{
				parent->myRightChild = aNewChildNode;
			}
		}
		else
		{
			myRoot = aNewChildNode;
		}
	}
}

template class CU::BinaryTree < int > ;

#undef BINARY_TREE_TEMPLATE
#undef BINARY_TREE_OBJECT