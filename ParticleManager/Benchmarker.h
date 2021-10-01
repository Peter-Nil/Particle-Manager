#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>

using namespace std;

namespace pc
{
	class Benchmarker
	{
	private: // General stuff.
		typedef std::chrono::high_resolution_clock	Clock;
		typedef Clock::time_point					TimePoint;
		typedef Clock::duration						Duration;

		TimePoint mStartTime;
		std::vector<Duration> mDurations;
		std::vector<std::vector<float>*> mStabilityErrors;

		int mSavedImageCount = 0;
		std::string mWindowScreenshotDirectoryName = "";
	public:
		Benchmarker();
		~Benchmarker();

		void reset();
		void createGraphs(std::string const & matlab_filename);
		// Captures screenshot of window and saves it to a pre-defined directory
		void saveScreenshotOfWindow();
		void setWindowScreenshotDirectoryName(std::string const& directoryName);
	public: // Time Stuff
		void setStartTimeToNow();
		void storeDuration();
		void createTimeGraph(std::string const & matlab_filename);

		// Prints the delta time.
		template <class T, std::enable_if_t<std::chrono::_Is_duration_v<T>, int> = 0>
		void printDelaTime()
		{
			// Get time unit name.
			std::string timeUnit = getUnitName<T>();

			std::cout << "Delta time: "
				<< std::chrono::duration_cast<T>(Clock::now() - mStartTime).count()
				<< " " << timeUnit << std::endl;
		}

		template <class T, std::enable_if_t<std::chrono::_Is_duration_v<T>, int> = 0>
		void printResult()
		{
			// Calculate result (min, max, average, median).
			Duration min = Duration::max(),
				max = Duration::min(),
				sum = Duration::zero(),
				median = Duration::zero();
			int durationCount = mDurations.size();
			for (auto duration : mDurations)
			{
				if (min > duration)
					min = duration;
				else if (max < duration)
					max = duration;
				sum += duration;
			}
			if (durationCount % 2 == 0)
			{
				Duration number1 = mDurations[durationCount / 2.0f - 1];
				Duration number2 = mDurations[durationCount / 2.0f    ];
				median = (number1 + number2) / 2;
			}
			else
			{
				median = mDurations[durationCount / 2.0f];
			}

			// Get time unit name.
			std::string timeUnit = getUnitName<T>();

			// Print out result.
			std::cout << "Delta time (" << mDurations.size() << " runs) min: "
				<< std::chrono::duration_cast<T>(min).count()
				<< timeUnit << ", ";
			std::cout << "max: "
				<< std::chrono::duration_cast<T>(max).count()
				<< timeUnit << ", ";
			std::cout << "average: "
				<< std::chrono::duration_cast<T>(sum / mDurations.size()).count()
				<< timeUnit << ", ";
			std::cout << "median: "
				<< std::chrono::duration_cast<T>(median).count()
				<< timeUnit << std::endl;

			mDurations.clear();
		}

		template <class T, std::enable_if_t<std::chrono::_Is_duration_v<T>, int> = 0>
		std::string getUnitName();

		template <> std::string getUnitName<std::chrono::nanoseconds>()		{ return "ns"; }
		template <> std::string getUnitName<std::chrono::microseconds>()	{ return { (char)230, 's' }; }
		template <> std::string getUnitName<std::chrono::milliseconds>()	{ return "ms"; }
		template <> std::string getUnitName<std::chrono::seconds>()			{ return "s"; }
		template <> std::string getUnitName<std::chrono::minutes>()			{ return "min"; }
		template <> std::string getUnitName<std::chrono::hours>()			{ return "hour"; }
	public: // Stability stuff.
		void storeStabilityError(std::vector<float>* errors);
		void createStabilityGraph(std::string const & matlab_filename);
	};
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0