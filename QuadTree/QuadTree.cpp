
QuadTree* QuadTree::GetIndex(const CShape& Box)
{
	float Vertical = (Bounds.Pos.m_X + Bounds.Size.m_X) / 2;
	float Horizontal = (Bounds.Pos.m_Y + Bounds.Size.m_Y) / 2;


	if (Box.GetPos().m_X < Vertical)
	{
		if ((Box.GetPos().m_Y < Horizontal))
			return  Nodes[1];
		else if ((Box.GetPos().m_Y > Horizontal))
			return Nodes[2];
	}
	if (Box.GetPos().m_X > Vertical)
	{
		if (Box.GetPos().m_Y < Horizontal)
			return  Nodes[0];
		if (Box.GetPos().m_Y > Horizontal)
			return Nodes[3];
	}

	return nullptr;
}

QuadTree::~QuadTree()
{
	Clear();
	for (QuadTree* Leaf : Nodes)
	{
		if (Leaf) delete Leaf;
	}
}


void QuadTree::Retrive(std::vector<CShape*>& Obj, CShape* Shape)
{
	Obj.clear();
	if (!isLeaf)
	{
		if (QuadTree* Child = GetIndex(*Shape))
		{
			Child->Retrive(Obj, Shape);
		}
		else
		{
			for (QuadTree* Node : Nodes)
			{
				Node->Retrive(Obj, Shape);
			}
		}
	}
	Obj.reserve(NodeShapes.size());
	std::copy(NodeShapes.begin(), NodeShapes.end(), std::back_inserter(Obj));
}

bool QuadTree::Insert(CShape* Shape)
{
	if (Shape->qt != nullptr) return false;

	if (!isLeaf)
	{
		if (QuadTree* Child = GetIndex(*Shape))
		{
			return Child->Insert(Shape);
		}
	}

	NodeShapes.push_back(Shape);
	Shape->qt = this;

	if (isLeaf && (NodeShapes.size() >= MAX_CAPACITY) && (Level < MAX_LEVEL))
	{
		Split();
		Update(Shape);
	}
	return true;
}

void QuadTree::Split()
{
	float Width = Bounds.Size.m_X / 2;
	float Height = Bounds.Size.m_Y / 2;

	float X = 0.f, Y = 0.f;

	for (short i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0: X = Bounds.Pos.m_X + Width;		Y = Bounds.Pos.m_Y; break;
		case 1:	X = Bounds.Pos.m_X;				Y = Bounds.Pos.m_Y; break;
		case 2: X = Bounds.Pos.m_X, Y = Bounds.Pos.m_Y + Height; break;
		case 3: X = Bounds.Pos.m_X + Width;		Y = Bounds.Pos.m_X + Height; break;
		default:
			break;
		}
		Nodes[i] = new QuadTree(Level + 1, BBox(CPoint2d(X, Y), CPoint2d(Width, Height)));
		Nodes[i]->Parent = this;
	}

	isLeaf = false;
}

void QuadTree::discardEmptyBuckets()
{
	if (!NodeShapes.empty()) return;
	if (!isLeaf) {
		for (QuadTree *Child : Nodes)
			if (!Child->isLeaf || !Child->NodeShapes.empty())
				return;
	}
	if (Clear(), Parent != nullptr)
		Parent->discardEmptyBuckets();
}

bool QuadTree::Remove(CShape* Box)
{
	if (Box->qt == nullptr) return true;

	if (Box->qt != this)
	{
		return	Box->qt->Remove(Box);
	}


	std::swap(Box, NodeShapes.back());
	NodeShapes.pop_back();
	Box->qt = nullptr;
	discardEmptyBuckets();

	return true;
}

bool QuadTree::Update(CShape* Box)
{
	if (!Remove(Box)) return false;

	if (Parent != nullptr && !Bounds.Contains(*Box))
		return Parent->Insert(Box);

	if (!isLeaf)
	{
		if (QuadTree* Child = GetIndex(*Box))
		{
			return Child->Insert(Box);
		}
	}

	return Insert(Box);
}

void QuadTree::Clear()
{
	NodeShapes.clear();

	for (QuadTree *Leaf : Nodes)
	{
		if (Leaf) Leaf->Clear();
	}
}
