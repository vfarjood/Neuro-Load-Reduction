#include <chrono>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}


	~Timer()
	{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;

	float second = duration.count();
	std::cout << "Timer took: " << second << "s" << std::endl;
	}
};