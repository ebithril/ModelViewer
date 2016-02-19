#pragma once

#define TREE_NODE_TEMPLATE template <typename Type>
#define TREE_NODE_OBJECT TreeNode<Type>

namespace CommonUtilities
{
	template <typename Type>
	class BinaryTree;

	TREE_NODE_TEMPLATE
	class TreeNode
	{
		friend class BinaryTree < Type > ;
	public:
		TreeNode();
		TreeNode(const Type& aData, TreeNode* aLeftChild = nullptr, TreeNode* aRightChild = nullptr);
		~TreeNode();
		const Type& GetData()const;

	private:
		Type myData;
		TreeNode* myLeftChild;
		TreeNode* myRightChild;
	}; 

	TREE_NODE_TEMPLATE
		TREE_NODE_OBJECT::TreeNode()
	{
		myLeftChild = nullptr;
		myRightChild = nullptr;
	}

	TREE_NODE_TEMPLATE
		TREE_NODE_OBJECT::TreeNode(const Type& aData, TreeNode* aLeftChild, TreeNode* aRightChild)
	{
		myData = aData;
		myLeftChild = aLeftChild;
		myRightChild = aRightChild;
	}

	TREE_NODE_TEMPLATE
		TREE_NODE_OBJECT::~TreeNode()
	{

	}


	TREE_NODE_TEMPLATE
	const Type& TREE_NODE_OBJECT::GetData()const
	{
		return myData;
	}
}

namespace CU = CommonUtilities;

#undef TREE_NODE_TEMPLATE
#undef TREE_NODE_OBJECT