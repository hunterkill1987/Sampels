#include "GameObject.h"

unsigned int GameObject::NextID ;
GameObject** GameObject::mToDestroy;

GameObject::GameObject()
{
}

GameObject::GameObject(char* Name)
{
	ID = NextID++;
	mName = Name;
	bIsDestroyed = false;
}


GameObject::GameObject(const GameObject& Source)
{
	ID = NextID++;
	mName = Source.mName;
	bIsDestroyed = false;
}

GameObject::~GameObject()
{
}
void GameObject::Init(Engine* mEngine)
{
	OnInit();

	PtrEngine = mEngine;

	if (HasChilde())
		((GameObject*)mPtrChild)->Init(PtrEngine);

	if (HasParent() && !IsLastChild())
		((GameObject*)mPtrNext)->Init(PtrEngine);
}

void GameObject::Update(const double& DeltaTime)
{
	if (!bIsDestroyed)
	{
		OnUpdate();

		dDeltaTime = DeltaTime;

		if (HasChilde())
			((GameObject*)mPtrChild)->Update(DeltaTime);

		if (HasParent() && !IsLastChild())
			((GameObject*)mPtrNext)->Update(DeltaTime);


		Collision();
	}
}

void GameObject::Draw()
{
	if (!bIsDestroyed)
	{
		OnDraw();

		if (HasChilde())
			((GameObject*)mPtrChild)->Draw();

		if (HasParent() && !IsLastChild())
			((GameObject*)mPtrNext)->Draw();
	}
}


void GameObject::SetPosition(Vector NewPosition)
{
	Position = NewPosition;
}

void GameObject::Destroy(GameObject* PtrGameObject)
{
	if (PtrGameObject == nullptr)
		return;
	
	if (PtrGameObject->bIsDestroyed)
		return;

	PtrGameObject->OnDestroy();
	PtrGameObject->bIsDestroyed = true;

	PtrGameObject->Detache();
	int NewSize = ((sizeof(mToDestroy) / sizeof(*mToDestroy)) + 1);
	GameObject** tooDestroy = new GameObject*[NewSize];

	int i = 0;
	int OldSize = (sizeof(mToDestroy) / sizeof(*mToDestroy)) - 1;
	if (OldSize > 0)
	{
		int size = (sizeof(mToDestroy) / sizeof(*mToDestroy));
		for (i; i < size; i++)
		{
			tooDestroy[i] = mToDestroy[i];
		}
	}
	mToDestroy = tooDestroy;
	mToDestroy[(sizeof(mToDestroy) / sizeof(*mToDestroy)) - 1] = PtrGameObject;
}

void GameObject::HandleDestroyed()
{
	if (mToDestroy == nullptr)
		return;
	for (int i = 0; i < (sizeof(mToDestroy) / sizeof(*mToDestroy)); i++)
	{
		delete mToDestroy[i];
	}
	mToDestroy = nullptr;
}


GameObject* GameObject::Instantiate(GameObject* PtrGameObject)
{
	if (PtrGameObject == nullptr)
		return nullptr;

	GameObject* PtrClone = PtrGameObject->Clone();

	PtrGameObject->CloneBranch(PtrClone);

	return PtrClone;
}

void GameObject::CloneBranch(GameObject* PtrGameObject)
{
	if (PtrGameObject == nullptr)
		return;
		
	if (!HasChilde())
		return;

	GameObject* PtrChild = (GameObject*)mPtrChild;
	do
	{
		GameObject* PtrClone = Instantiate(PtrChild);
		PtrClone->AttachTo(PtrGameObject);
		PtrChild = (GameObject*)PtrChild->GetChild();
	} while (!PtrChild->IsLastChild());

}

GameObject* GameObject::Clone() const
{
	return new GameObject(*this);
}
