#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	void	Start();
	void	Stop();
	double	getElapsedTime();

private:
	long long	pStart, pStop;
	long long	pElapsedTime;
	double		tickPerSec;

	double		getTickPerSec();
	long long	getTicks();
};

#endif