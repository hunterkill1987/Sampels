#include "Point2d.h"
#include <vector>

#define MAX_LEVEL 10
#define MAX_CAPACITY 50

///////////////////////////////
//
// Part of a 2d Collision Checking system
//
///////////////////////////////


// Bounding Box
class BBox
{
public:

	BBox() :Pos(Point2d()), Size(Point2d()) {};

	BBox(Point2d pos, Point2d size) : Pos(pos), Size(size)
	{};
	
	// Check if Bounding Box Containt one other
	bool Contains(const BBox& Other)
	{
		if (Pos.m_X > Other.Pos.m_X) return false;
		if (Pos.m_Y > Other.Pos.m_Y) return false;
		if (Pos.m_X + Size.m_X > Other.Pos.m_X + Other.Size.m_X) return false;
		if (Pos.m_Y + Size.m_Y > Other.Pos.m_Y + Other.Size.m_Y) return false;
		return true;
	}

	// Check if Bounding Box intersect with each other
	bool Intersect(const BBox& Other)
	{
		if (Pos.m_X > Other.Pos.m_X + Other.Size.m_X) return false;
		if (Pos.m_X + Size.m_X < Other.Pos.m_X) return false;
		if (Pos.m_Y > Other.Pos.m_Y + Other.Size.m_Y) return false;
		if (Pos.m_Y + Size.m_Y < Other.Pos.m_Y) return false;
		return true;
	}

	QuadTree* qt = nullptr;

	Point2d Pos;
	Point2d Size;
};

class QuadTree
{
public:

	QuadTree() : Level(0), Bounds(BBox()) {};
	QuadTree(int level, BBox bounds) : Level(level), Bounds(bounds) { };
	~QuadTree();

	void Clear();

	// Insert new BouindingBox
	bool Insert(BBox* Box);

	// Retrevie Objects that are for Quadtree leaf, that given object is related too
	void Retrive(std::vector<BBox*>& Obj, BBox* Shape);

	// Update BoudingBox Positions inside a tree, Changeing leaf for a BBox in tree
	bool Update(BBox* Box);

	// Remove BBox form leaf to reasinge to a proper leaf
	bool Remove(BBox* Box);

private:

	bool isLeaf = true;

	QuadTree* Parent = nullptr;

	QuadTree* Nodes[4] = { nullptr, nullptr , nullptr, nullptr };
	std::vector<BBox*> NodeShapes;

	// Level of a current Node
	short Level = 0;

	// BBox area for node, and objects assinged to a node
	BBox Bounds;

	// Get Leaf of a node
	QuadTree* GetIndex(const BBox& Box);

	//Create new leaf for a node
	void Split();
};