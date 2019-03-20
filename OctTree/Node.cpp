#include "Node.h"

Node::Node()
{
	mPtrParent = nullptr;
	mPtrChild = nullptr;
	mPtrNext = this;
	mPtrPrev = this;
}

Node::Node(Node* PtrParent)
{
	mPtrParent = nullptr;
	mPtrChild = nullptr;
	mPtrNext = this;
	mPtrPrev = this;
	AttachTo(PtrParent);
}

Node::~Node()
{

}

void Node::AttachTo(Node* PtrParent)
{
	if (PtrParent == nullptr)
		return;

	if (HasParent())
		Detache();

	mPtrParent = PtrParent;

	Node* PtrNode = PtrParent->mPtrChild;
	if (PtrNode != nullptr)
	{
		mPtrNext = PtrNode;
		mPtrPrev = PtrNode->mPtrPrev;
		PtrNode->mPtrPrev->mPtrNext = this;
		PtrNode->mPtrPrev = this;
	}
	else
	{
		mPtrParent->mPtrChild = this;
	}
}

void Node::Attach(Node* PtrChild)
{
	if (PtrChild == nullptr)
		return;

	if (PtrChild->HasParent())
		Detache();

	PtrChild->mPtrParent = this;

	if (HasChilde())
	{
		PtrChild->mPtrNext = mPtrChild;
		PtrChild->mPtrPrev = mPtrChild->mPtrPrev;

		mPtrChild->mPtrPrev->mPtrNext = PtrChild;
		mPtrChild->mPtrPrev = PtrChild;
	}
	else
	{
		mPtrChild = PtrChild;
	}
}

void Node::Detache()
{
	if (HasParent() && mPtrParent->mPtrChild == this)
	{
		if (IsLastChild())
			mPtrParent->mPtrChild = nullptr;
		else
			mPtrParent->mPtrChild = mPtrNext;
	}

	mPtrNext->mPtrPrev = mPtrPrev;
	mPtrPrev->mPtrNext = mPtrNext;
	mPtrNext = this;
	mPtrPrev = this;
}

bool Node::HasParent()
{
	return mPtrParent != nullptr;
}

bool Node::HasChilde()
{
	return mPtrChild != nullptr;
}

bool Node::IsLastChild()
{
	return HasParent() && mPtrParent->mPtrChild->mPtrPrev == this;
}

bool Node::IsFirstChild()
{
	return HasParent() && mPtrParent->mPtrChild == this;
}

Node* Node::GetRoot()
{
	Node* mPtrNode = this;
	while (mPtrNode->HasParent())
		mPtrNode = mPtrNode->mPtrParent;

	return mPtrNode;
}

int Node::GetChildCount()
{
	int i = 0;
	Node* mPtrNode = this;
	while ((mPtrNode = mPtrNode->mPtrChild) != nullptr)
		++i;

	return i;
}
