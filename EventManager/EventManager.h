#pragma once
#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <utility>
#include <vector>
#include "Math.h"
#include <map>

class EventHandlerBase
{
public:
	virtual void Execute() = 0;
};

template<typename TargetT>
class EventHandler : public EventHandlerBase
{
private:
	typedef void (TargetT::*method_t)();
	TargetT* Object;
	method_t Method;
public:
	EventHandler(TargetT* ObjectT, method_t EventMethod)
		:Object(ObjectT), Method(EventMethod)
	{
	}

	void Execute() override
	{
		(Object->*Method)();
	}
};


class Event
{
private:

	std::map<int, EventHandlerBase*> EventHandlerMap;
	int id;
	static int count;

public:

	Event() :id(count++){ } ;
	template<typename TargetT>
	void AddListener(TargetT* Object, void (TargetT::*Method)())
	{
		EventHandlerMap[id] = new EventHandler<TargetT>(Object, Method);
	}
	
	void Execute()
	{
		for (auto& Event : EventHandlerMap)
		{
			Event.second->Execute();
		}
	}

};

class EventManager
{
private:

	struct EventType
	{
		Event* Event;
		char* name;

	};

	struct TimerType: public EventType
	{
		public:
			void SetLuncheTime(float Alfa, bool bIsLooped)
			{
				AlfaTime = Alfa;
				CurrnetAlfa = Alfa;
				bLooped = bIsLooped;
			}

			void SetCurrentTime(float Time)
			{
				CurrnetAlfa = Time;
			}

			float GetTime()
			{
				return AlfaTime;
			}

			float GetCurrentAlfa()
			{
				return CurrnetAlfa;
			}

			bool IsLooped()
			{
				return bLooped;
			}
		private: 
			  
			float AlfaTime = 0.f;
			float CurrnetAlfa = 0.f;
			bool bLooped = false;
	};

	std::vector<EventType*> Events;

	std::vector<TimerType*> Timers;

	static EventManager* Instance;
	EventManager() {}
public:
	
	static EventManager* GetInstance();
	static bool CheckExpierdTimers(TimerType* Timer);
	void Init();

	void SetTimer(char* TimerName, float newTime);
	bool CheckTimer(char* TimerName);
	 
	template<typename TargetT>
	bool SetTimer(TargetT* Object, void(TargetT::*method_t)(), float Alfa, bool bIsLooped,char* Name)
	{
		if (Alfa != 0.f)
		{
			TimerType* e = new TimerType();
			e->Event = new Event();
			e->Event->AddListener(Object, method_t);
			e->name = Name;
			e->SetLuncheTime(Alfa, bIsLooped);
			this->Timers.push_back(e);
			return true;
		}
		return false;
	}

	template<typename TargetT>
	bool Bind(TargetT* Object, void(TargetT::*method_t)(), char* Name)
	{
		for (EventType* CurrEvent : Events)
		{
			if (strcmp(CurrEvent->name, Name) == 0)
			{
				return false;
			}
		}
		 
		EventType* e = new EventType();
		e->Event = new Event();
		e->name = Name;
		e->Event->AddListener(Object, method_t);
		Events.push_back(e);

		return true;
	}

	void FireEvent(char* Name);

	void Update(float DeltaTime);

	~EventManager();
};

#endif