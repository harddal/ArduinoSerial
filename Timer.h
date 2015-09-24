#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <ostream>

class Timer 
{
	typedef std::chrono::high_resolution_clock high_resolution_clock;
	typedef std::chrono::seconds seconds;

public:
	explicit Timer(bool run = false)
	{
		if (run)
			Reset();
	}

	void Reset()
	{
		_start = high_resolution_clock::now();
	}

	seconds Elapsed() const
	{
		return std::chrono::duration_cast<seconds>(high_resolution_clock::now() - _start);
	}

	template <typename T, typename Traits>
	friend std::basic_ostream<T, Traits>& operator<<(std::basic_ostream<T, Traits>& out, const Timer& timer)
	{
		return out << timer.Elapsed().count();
	}

private:
	high_resolution_clock::time_point _start;

};

#endif