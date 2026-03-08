/////////////////////////////////////////////////////////////////////////////////

// chapter : Number Processing

/////////////////////////////////////////////////////////////////////////////////

// section : Chrono Management

/////////////////////////////////////////////////////////////////////////////////

// content : Timing

/////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <print>
#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////////////////

template < typename D = std::chrono::duration < double > > class Timer
{
public :

	Timer(std::string const & scope)
		: m_scope(scope)
		, m_running(true)
		, m_start(clock_t::now())
	{}

//  -----------------------------------------------------------------------------

   ~Timer()
	{
		if (m_running)
		{
			stop();
		}
		if (!m_durations.empty())
		{
			double sum = 0.0;
			for (auto const & d : m_durations)
				sum += d.count();
			std::print("{} : average = {:.6f} s\n", m_scope, sum / m_durations.size());
		}
	}

//  -----------------------------------------------------------------------------

	void start()
	{
		if (m_running)
		{
			stop();
		}
		m_running = true;
		m_start = clock_t::now();
	}

	void stop()
	{
		if (m_running)
		{
			auto end = clock_t::now();
			m_durations.push_back(std::chrono::duration_cast<D>(end - m_start));
			m_running = false;
		}
	}

//  -----------------------------------------------------------------------------

	double average() const
	{
		if (m_durations.empty())
			return 0.0;
		double sum = 0.0;
		for (auto const & d : m_durations)
			sum += d.count();
		return sum / m_durations.size();
	}

//  -----------------------------------------------------------------------------

	auto elapsed() const
	{
		return std::chrono::duration_cast < D > (clock_t::now() - m_start);
	}

private :

	using clock_t = std::chrono::steady_clock;

//  -----------------------------------------------------------------------------

	std::string m_scope;

	bool m_running;

	clock_t::time_point m_start;

	std::vector < D > m_durations;
};

/////////////////////////////////////////////////////////////////////////////////

auto calculate(std::size_t size)
{
	auto x = 0.0;

	for (auto i = 0uz; i < size; ++i)
	{
		x += std::pow(std::sin(i), 2) + std::pow(std::cos(i), 2);
	}

	return x;
}

/////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
	return std::abs(x - y) < epsilon;
}

/////////////////////////////////////////////////////////////////////////////////

int main()
{
	{
		Timer timer("single measurement");
		assert(equal(calculate(1'000'000), 1'000'000));
	}

	{
		Timer timer("series of 5 measurements");
		for (int i = 0; i < 5; ++i)
		{
			timer.start();
			volatile double dummy = calculate(100'000);
			timer.stop();
		}
		assert(timer.average() > 0.0);
	}

	{
		Timer timer("manual average check");
		timer.start();
		calculate(500'000);
		timer.stop();
		timer.start();
		calculate(500'000);
		timer.stop();
		double avg = timer.average();
		assert(avg > 0.0);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////