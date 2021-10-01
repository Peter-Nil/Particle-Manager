#include "Benchmarker.h"
#include <numeric>

#include <sstream>
#include <iomanip>
#include <OpenTissue\GPU\Image\Image.h>
#include <OpenTissue\GPU\Image\ImageScreenCapture.h>
#include <OpenTissue\GPU\Image\IO\ImageWrite.h>
#include <filesystem>

using namespace pc;
namespace fs = std::experimental::filesystem;

Benchmarker::Benchmarker(){}

Benchmarker::~Benchmarker()
{
	for (unsigned i = 0; i < mStabilityErrors.size(); i++)
		delete mStabilityErrors[i];
}

void Benchmarker::reset()
{
	mDurations.clear();
	for (unsigned i = 0; i < mStabilityErrors.size(); i++)
		delete mStabilityErrors[i];
	mStabilityErrors.clear();
}

void Benchmarker::createGraphs(std::string const & matlab_filename)
{
	createTimeGraph(matlab_filename + "_Time");
	createStabilityGraph(matlab_filename + "_Stability_Errors");
}

void pc::Benchmarker::saveScreenshotOfWindow()
{
	std::stringstream filename;
	if (mWindowScreenshotDirectoryName != "")
	{
		

		filename << mWindowScreenshotDirectoryName << "/";
	}
	filename << "img" << std::setw(3) << std::setfill('0') << ++mSavedImageCount << ".png";

	OpenTissue::image::write(filename.str(), *OpenTissue::image::screen_capture());
}

void pc::Benchmarker::setWindowScreenshotDirectoryName(std::string const& directoryName)
{
	if (directoryName != "")
	{
		mWindowScreenshotDirectoryName = directoryName;

		// Create directory and remove old if it already exists
		if (fs::exists(mWindowScreenshotDirectoryName))
			fs::remove_all(mWindowScreenshotDirectoryName);
		fs::create_directory(mWindowScreenshotDirectoryName);

		mSavedImageCount = 0;
	}
}

void Benchmarker::setStartTimeToNow(){ mStartTime = Clock::now(); }

void Benchmarker::storeDuration()
{
	TimePoint timeNow = Clock::now();
	mDurations.push_back(timeNow - mStartTime);
}

void Benchmarker::createTimeGraph(std::string const & matlab_filename)
{
	std::ofstream matlab_graph(matlab_filename + ".m", std::ofstream::out);
	const char* prefix(matlab_filename.c_str());

	matlab_graph << "close all;" << std::endl;
	matlab_graph << "clear all;" << std::endl;
	matlab_graph << "filename1 = '" << prefix << "';" << std::endl;
	matlab_graph << "figure(1);" << std::endl;
	matlab_graph << "clf" << std::endl;
	matlab_graph << "set(gca, 'fontsize', 18);" << std::endl;
	matlab_graph << "hold on;" << std::endl;

	matlab_graph << "P0" << " = [";
	unsigned int end = mDurations.size() - 1;
	for (unsigned int i = 0; i < end; i++)
		matlab_graph << std::chrono::duration_cast<std::chrono::microseconds>(mDurations[i]).count() << ", ";
	matlab_graph << std::chrono::duration_cast<std::chrono::microseconds>(mDurations[end]).count() << "];" << std::endl;
	matlab_graph << "plot(P0, 'b');" << std::endl;

	matlab_graph << "axis tight;" << std::endl;
	matlab_graph << "avg = ['Mean: ' num2str(mean(P0)) ' " << (char)181 << "s'];" << std::endl;
	matlab_graph << "bsd = ['SD: ' num2str(std(P0)) ' " << (char)181 << "s'];" << std::endl;
	matlab_graph << "xlabel(['frame' newline avg newline bsd], 'fontsize', 18);" << std::endl;
	matlab_graph << "ylabel('microseconds', 'fontsize', 18);" << std::endl;
	matlab_graph << "hold off;" << std::endl;
	matlab_graph << "print('-f1', '-dpng', filename1);" << std::endl;
	matlab_graph.flush();
	matlab_graph.close();
}


void Benchmarker::storeStabilityError(std::vector<float>* errors){ mStabilityErrors.push_back(errors); }

void Benchmarker::createStabilityGraph(std::string const & matlab_filename)
{
	std::ofstream matlab_graph(matlab_filename + ".m", std::ofstream::out);
	const char* prefix(matlab_filename.c_str());

	matlab_graph << "close all;" << std::endl;
	matlab_graph << "clear all;" << std::endl;
	matlab_graph << "filename1 = '" << prefix << "';" << std::endl;
	matlab_graph << "figure(1);" << std::endl;
	matlab_graph << "clf" << std::endl;
	matlab_graph << "set(gca, 'fontsize', 18);" << std::endl;
	matlab_graph << "hold on;" << std::endl;

	matlab_graph << "P0" << " = [";
	unsigned int end = mStabilityErrors.size() - 1;
	for (unsigned int i = 0; i < end; i++)
		matlab_graph << std::accumulate(mStabilityErrors[i]->begin(), mStabilityErrors[i]->end(), 0.F) << ", ";
	matlab_graph << std::accumulate(mStabilityErrors[end]->begin(), mStabilityErrors[end]->end(), 0.F) << "];" << std::endl;
	matlab_graph << "plot(P0, 'b');" << std::endl;

	matlab_graph << "axis tight;" << std::endl;
	matlab_graph << "avg = ['Average: ' num2str(mean(P0)) ' " << (char)181 << "s'];" << std::endl;
	matlab_graph << "xlabel(['frame' newline avg], 'fontsize', 18);" << std::endl;
	matlab_graph << "ylabel('errors', 'fontsize', 18);" << std::endl;
	matlab_graph << "hold off;" << std::endl;
	matlab_graph << "print('-f1', '-dpng', filename1);" << std::endl;
	matlab_graph.flush();
	matlab_graph.close();
}

// Written by Peter Nilsson and Christian Larsson, licensed under CC BY-SA 4.0