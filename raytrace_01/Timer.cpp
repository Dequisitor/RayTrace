#include "Timer.h"

Timer::Timer()
{
	this->tickPerSec = this->getTickPerSec();
	this->pStart = 0;
	this->pStop = 0;
}

Timer::~Timer()
{
}

double Timer::getTickPerSec()
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	return double(li.QuadPart)/1000.0;
}

void Timer::Start()
{
	this->pStart = this->getTicks();
}

void Timer::Stop()
{
	if (this->pStart != 0) 
	{
		this->pStop = this->getTicks();
	}
}

long long Timer::getTicks()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}

double Timer::getElapsedTime()
{
	if (this->pStart <= this->pStop)
	{
		double diff = (double)this->pStop - (double)this->pStart;
		return diff / this->tickPerSec;
	}
	else
	{
		return -1;
	}
}