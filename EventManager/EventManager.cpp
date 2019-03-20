#include "stdafx.h"
#include "Engine.h"
#include "EventManager.h"


EventManager* EventManager::Instance = 0;
int Event::count = 0;

EventManager* EventManager::GetInstance()
{
	if (Instance == 0)
	{
		Instance = new EventManager();
	}
	return Instance;
}

void EventManager::Update(float deltaTime)
{
	for (TimerType* CurrTimer : Timers)
	{
		if (CurrTimer->GetTime() > 0.f)
		{
			CurrTimer->SetCurrentTime(CurrTimer->GetCurrentAlfa() - deltaTime);
			if (Engine::IsNear(CurrTimer->GetCurrentAlfa(), 0.f, 0.01f))
			{
				CurrTimer->Event->Execute();
			}

			if (CurrTimer->GetCurrentAlfa() <= 0.f)
			{
				if (CurrTimer->IsLooped())
				{
					CurrTimer->SetLuncheTime(CurrTimer->GetTime(), CurrTimer->IsLooped());
				}
			} 
		}
	}

	Timers.erase(std::remove_if(Timers.begin(),
								Timers.end(),
								&EventManager::CheckExpierdTimers
								  ),
								Timers.end()
								);
}

void EventManager::SetTimer(char* TimerName, float newTime)
{
	std::vector<TimerType*>::iterator it;
	char* Name = TimerName;

	for (TimerType* Timer: Timers)
	{
		if (strcmp(Timer->name, TimerName) == 0)
		{
			Timer->SetLuncheTime(newTime, Timer->IsLooped());
			break;
		}
	}
}

bool EventManager::CheckTimer(char* TimerName)
{ 
	std::vector<TimerType*>::iterator it;
	char* Name = TimerName;

	for (TimerType* Timer : Timers)
	{
		if (strcmp(Timer->name, TimerName) == 0)
		{
			return true;
		}
	} 

	return false;
}

bool EventManager::CheckExpierdTimers(TimerType* Timer)
{
	float time = Timer->GetCurrentAlfa();
	if (time < 0.f)
		return true;
	else
		return false;
}

void EventManager::FireEvent(char* Name)
{
	for (EventType* Event : Events)
	{
		if (Event->name != nullptr)
		{
			if (strcmp(Event->name, Name) == 0)
			{
				Event->Event->Execute();
			}
		}
	}
}

EventManager::~EventManager()
{
}
