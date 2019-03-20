
#pragma once
#include "Node.h"
#include "Engine.h"
#include <stdio.h>
#include <string.h>
#include <cstdlib>

struct Vector
{
	int X;
	int Y;

	Vector() :X(0), Y(0) {};
	Vector(int x , int y ) : X(x), Y(y) {};

	Vector operator * (const double& s)
	{
		Vector ret;
		ret.X = (int)((float)this->X * (float)s);
		ret.Y = (int)((float)this->Y * (float)s);
		return ret;
	}

	Vector operator + (const Vector& S)
	{
		Vector ret;
		ret.X = this->X + S.X;
		ret.Y = this->Y + S.Y;
		return ret;
	}

	Vector operator + (const int& s)
	{
		Vector ret;
		ret.X = this->X + s;
		ret.Y = this->Y + s;
		return ret;
	}
	Vector operator += (const Vector& S)
	{
		this->X += S.X;
		this->Y += S.Y;
		return * this;
	}
};


class GameObject : public Node
{
public:

	GameObject();
	GameObject(char* Name);
	GameObject(const GameObject& Source);
	virtual ~GameObject();

	void Init(Engine* mEngine);

	void Update(const double& DeltaTime);

	void Draw();

	const bool IsDestroyed() const { return bIsDestroyed; };

	virtual GameObject* Clone() const;

	static void Destroy(GameObject* gameObject);
	static void HandleDestroyed();

	static GameObject* Instantiate(GameObject* gameObject);

	char* GetRawName() const { return mName; };

	void SetPosition(Vector NewPos);
	Vector GetPosition() { return Position; };

protected:


	bool bIsVisible;
	bool bIsDestroyed = false;

	int ID;
	static unsigned int NextID;
	char* mName;

	virtual void OnInit() {};

	virtual void OnUpdate() {};

	virtual void OnDraw() {};

	virtual void OnDestroy() {};

	Engine* PtrEngine;

	double GetDeltaTime(){ return dDeltaTime; };

private:

	static GameObject** mToDestroy;

	Vector Position;

	double dDeltaTime = 0;

	void CloneBranch(GameObject* PtrGameObject);
};
