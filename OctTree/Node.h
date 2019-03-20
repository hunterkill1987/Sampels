#pragma once

class Node
{
public:
	Node();

	Node(Node* PtrNode);

	virtual ~Node();

	bool HasParent();

	bool HasChilde();

	bool IsLastChild();

	bool IsFirstChild();

	int GetChildCount();

	void AttachTo(Node* PtrNode);
	void Attach(Node* PtrNode);

	void Detache();

	Node* GetParent() const { return mPtrParent; };

	Node* GetChild() const { return mPtrChild; };

	Node* GetNext() const { return mPtrNext; };

	Node* GetPrevies()const { return mPtrPrev; };

	Node* GetRoot();

protected:

	Node* mPtrParent;
	Node* mPtrChild;
	Node* mPtrNext;
	Node* mPtrPrev;
};