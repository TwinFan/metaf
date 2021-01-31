/*
* Copyright (C) 2018-2020 Nick Naumenko (https://gitlab.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "metaf.hpp"
#include "testdata_real.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <utility>
#include <functional>
#include <regex>
#include <sstream>

#include <iostream>
#include <fstream>
#include <memory>
#include <map>
#include <chrono>

using namespace std;

///////////////////////////////////////////////////////////////////////////////

/// Get an index from type included in variant type at compile time
template<typename V, typename T, size_t I = 0>
constexpr size_t variant_index() {
    if constexpr (I >= variant_size_v<V>) {
    	return variant_size_v<V>;
    } else {
    	if constexpr (is_same_v<variant_alternative_t<I, V>, T>) {
    		return I;
    	} else {
    		return variant_index<V, T, I + 1>();
    	}
    }
}

///////////////////////////////////////////////////////////////////////////////

string_view groupName (size_t index) {
	switch (index) {
		case variant_index<metaf::Group, metaf::KeywordGroup>():
		return "KeywordGroup";

		case variant_index<metaf::Group, metaf::LocationGroup>():
		return "LocationGroup";

		case variant_index<metaf::Group, metaf::ReportTimeGroup>():
		return "ReportTimeGroup";

		case variant_index<metaf::Group, metaf::TrendGroup>():
		return "TrendGroup";

		case variant_index<metaf::Group, metaf::WindGroup>():
		return "WindGroup";

		case variant_index<metaf::Group, metaf::VisibilityGroup>():
		return "VisibilityGroup";

		case variant_index<metaf::Group, metaf::CloudGroup>():
		return "CloudGroup";

		case variant_index<metaf::Group, metaf::WeatherGroup>():
		return "WeatherGroup";

		case variant_index<metaf::Group, metaf::TemperatureGroup>():
		return "TemperatureGroup";

		case variant_index<metaf::Group, metaf::PressureGroup>():
		return "PressureGroup";

		case variant_index<metaf::Group, metaf::RunwayStateGroup>():
		return "RunwayStateGroup";

		case variant_index<metaf::Group, metaf::SeaSurfaceGroup>():
		return "SeaSurfaceGroup";

		case variant_index<metaf::Group, metaf::MinMaxTemperatureGroup>():
		return "MinMaxTemperatureGroup";

		case variant_index<metaf::Group, metaf::PrecipitationGroup>():
		return "PrecipitationGroup";

		case variant_index<metaf::Group, metaf::LayerForecastGroup>():
		return "LayerForecastGroup";

		case variant_index<metaf::Group, metaf::PressureTendencyGroup>():
		return "PressureTendencyGroup";

		case variant_index<metaf::Group, metaf::CloudTypesGroup>():
		return "CloudTypesGroup";

		case variant_index<metaf::Group, metaf::LowMidHighCloudGroup>():
		return "LowMidHighCloudGroup";

		case variant_index<metaf::Group, metaf::LightningGroup>():
		return "LightningGroup";

		case variant_index<metaf::Group, metaf::VicinityGroup>():
		return "VicinityGroup";

		case variant_index<metaf::Group, metaf::MiscGroup>():
		return "MiscGroup";

		case variant_index<metaf::Group, metaf::UnknownGroup>():
		return "UnknownGroup";

		default: return "UNDEFINED";
	}
}

string_view reportPartName (metaf::ReportPart rp) {
	switch (rp) {
		case metaf::ReportPart::UNKNOWN:	return "unknown";
		case metaf::ReportPart::HEADER:		return "header";
		case metaf::ReportPart::METAR:		return "metar";
		case metaf::ReportPart::TAF:		return "taf";
		case metaf::ReportPart::RMK:		return "remark";
	}
}

size_t reportPartIndex (metaf::ReportPart rp) {
	switch (rp) {
		case metaf::ReportPart::UNKNOWN: 	return 0;
		case metaf::ReportPart::HEADER:		return 1;
		case metaf::ReportPart::METAR:		return 2;
		case metaf::ReportPart::TAF:		return 3;
		case metaf::ReportPart::RMK:		return 4;
	}
}

optional<metaf::ReportPart> reportPartFromName (const string & name) {
	if (name == "unknown") return metaf::ReportPart::UNKNOWN;
	if (name == "header") return metaf::ReportPart::HEADER;
	if (name == "metar") return metaf::ReportPart::METAR;
	if (name == "taf") return metaf::ReportPart::TAF;
	if (name == "remark") return metaf::ReportPart::RMK;
	return optional<metaf::ReportPart>();
}

optional<metaf::ReportPart> reportPartFromIndex (size_t index) {
	switch (index) {
		case 0: 	return metaf::ReportPart::UNKNOWN;
		case 1:		return metaf::ReportPart::HEADER;
		case 2:		return metaf::ReportPart::METAR;
		case 3:		return metaf::ReportPart::TAF;
		case 4:		return metaf::ReportPart::RMK;
		default: 	return optional<metaf::ReportPart>();
	}
}

string reportErrorName(metaf::ReportError e) {
	switch (e) {
		case metaf::ReportError::NONE:
		return "no error";

		case metaf::ReportError::EMPTY_REPORT:
		return "empty report";

		case metaf::ReportError::EXPECTED_REPORT_TYPE_OR_LOCATION:
		return "expected report_type_or_location";

		case metaf::ReportError::EXPECTED_LOCATION:
		return "expected location";

		case metaf::ReportError::EXPECTED_REPORT_TIME:
		return "expected report time";

		case metaf::ReportError::EXPECTED_TIME_SPAN:
		return "expected time span";

		case metaf::ReportError::UNEXPECTED_REPORT_END:
		return "unexpected report end";

		case metaf::ReportError::UNEXPECTED_GROUP_AFTER_NIL:
		return "unexpected group after NIL";

		case metaf::ReportError::UNEXPECTED_GROUP_AFTER_CNL:
		return "unexpected group after CNL";

		case metaf::ReportError::UNEXPECTED_NIL_OR_CNL_IN_REPORT_BODY:
		return "unexpected NIL or CNL in report body";

		case metaf::ReportError::AMD_ALLOWED_IN_TAF_ONLY:
		return "unexpected AMD in non TAF report";

		case metaf::ReportError::CNL_ALLOWED_IN_TAF_ONLY:
		return "unexpected CNL in non TAF report";

		case metaf::ReportError::MAINTENANCE_INDICATOR_ALLOWED_IN_METAR_ONLY:
		return "unexpected maintenance indicator in TAF report";

		case metaf::ReportError::REPORT_TOO_LARGE:
		return "report too large";

		default:
		return "unknown error";
	}
}

static const size_t reportPartSize = 5;
static const size_t reportErrorSize = 14;


///////////////////////////////////////////////////////////////////////////////

class PerformanceStats {
public:
	PerformanceStats() = default;
	void add(const string & name, uint64_t microsec, uint64_t items) {
		stats[name].microsec += microsec;
		stats[name].items += items;
	}
	void print(const string & name, ostream & output = cout) const;
	void print(ostream & output = cout) const;
private:
	struct Data {
		Data() = default;
		uint64_t microsec = 0;
		uint64_t items = 0;
	};
	map<string, Data> stats;
};


void PerformanceStats::print(const string & name, ostream & output) const {
	auto quantity = [&] (const string & itemname, uint64_t quantity) {
		if (!quantity) return "<1 " + itemname;
		return to_string(quantity) + " " + itemname + ((quantity > 1) ? "s" : "");
	};
	if (!stats.count(name)) {
		output << "No performance stats exist for " << name << ".\n";
		return;
	}
	const auto items = stats.at(name).items;
	if (!items) {
		output << "No processed " << name << "s.\n";
		return;
	}
	const auto microsec = stats.at(name).microsec;
	output << quantity("microsecond", microsec) << ", ";
	output << quantity(name, items) << ", ";
	static const uint64_t microsecondsPerSecond = 1000000;
	const auto itemsPerSecond = microsecondsPerSecond * items / microsec;
	output << quantity(name, itemsPerSecond) << " per second.\n";
}

void PerformanceStats::print(ostream & output) const {
	for (const auto & stat : stats) {
		print(stat.first, output);
	}
}

///////////////////////////////////////////////////////////////////////////////

class BatchProcess {
public:
	BatchProcess() = default;
	virtual const vector<string> & getCurrentBatch() const = 0;
	virtual bool nextBatch() = 0;
	virtual size_t totalSize() const = 0;
	virtual ~BatchProcess() {}
};

class BatchFromMemory : public BatchProcess {
public:
	BatchFromMemory() = default;
	BatchFromMemory(const vector<string> * src) : shallowLoadedBatch(src) {}
	BatchFromMemory(vector<string> src) : loadedBatch(move(src)) {}
	virtual const vector<string> & getCurrentBatch() const {
		if (shallowLoadedBatch) return *shallowLoadedBatch;
		return loadedBatch;
	}
	virtual bool nextBatch() { return false; }
	virtual size_t totalSize() const { return getCurrentBatch().size(); }
	virtual ~BatchFromMemory() {}
private:
	vector<string> loadedBatch;
	const vector<string> * shallowLoadedBatch = nullptr;
};

class BatchFromTestdataReal : public BatchFromMemory {
public:
	BatchFromTestdataReal(size_t repetitions);
	virtual bool nextBatch() {
		if (repetitionsLeft) repetitionsLeft--;
		if (!repetitionsLeft) return false;
		return true;
	}
	virtual size_t totalSize() const { return getCurrentBatch().size() * repetitionsTotal; }
	virtual ~BatchFromTestdataReal() {}
private:
	size_t repetitionsLeft = 0;
	size_t repetitionsTotal = 0;
	static vector<string> getReports();
};

BatchFromTestdataReal::BatchFromTestdataReal(size_t repetitions) : 
	BatchFromMemory(getReports()), 
	repetitionsLeft(repetitions), 
	repetitionsTotal(repetitions)
{}

vector<string> BatchFromTestdataReal::getReports() {
	vector<string> result;
	for (const auto & data : testdata::realDataSet) {
 		if (!data.metar.empty()) result.push_back(data.metar);
		if (!data.taf.empty()) result.push_back(data.taf);
	}		
	return result;
}

class BatchFromFile : public BatchProcess{
public:
	BatchFromFile() = default;
	BatchFromFile(const string & path, size_t sz) : f(path), batchSize(sz), fpath(path) {
		loadedBatch.reserve(batchSize);
		nextBatch();
	}
	virtual const vector<string> & getCurrentBatch() const { return loadedBatch; }
	virtual bool nextBatch();
	virtual size_t totalSize() const;
	virtual ~BatchFromFile() {}
private:
	vector<string> loadedBatch;
	ifstream f;
	size_t batchSize;
	string fpath;
	mutable size_t totalFileSize = 0;
};

bool BatchFromFile::nextBatch() {
	loadedBatch.clear();
	if (!f.is_open()) { loadedBatch.shrink_to_fit(); return false; }
	for (auto i = 0u; i < batchSize; i++) {
		std::string s;
		if (!getline(f, s)) { loadedBatch.shrink_to_fit(); return false; }
		loadedBatch.push_back(move(s));
	}
	return true;
}

size_t BatchFromFile::totalSize() const {
	if (totalFileSize) return totalFileSize;
	ifstream f(fpath);
	const size_t sz = 
		count(istreambuf_iterator<char>(f), istreambuf_iterator<char>(), '\n');
	totalFileSize = sz;
	return totalFileSize;
}

///////////////////////////////////////////////////////////////////////////////

class FrequencyStats {
public:
	FrequencyStats() = default;
	void add(const string & str) {
		stats[str]++;
	}
	void add(unique_ptr<BatchProcess> src);
	void compensateRepetitions(uint64_t repetitions) {
		for (auto & e : stats) {
			e.second = e.second / repetitions;
		}
	}
	void printDescending(ostream & output) const;
private:
	map<string, uint64_t> stats;
};

void FrequencyStats::add(unique_ptr<BatchProcess> src) {
	do {
		for (const auto & s : src->getCurrentBatch()) {
			add(s);
		}
	} while (src->nextBatch());
}

void FrequencyStats::printDescending(ostream & out) const {
	multimap<uint64_t, string_view> statsByNumber;
	for (const auto & e : stats) {
		statsByNumber.insert({e.second, e.first});
	}
	for (auto i = statsByNumber.rbegin(); i != statsByNumber.rend() ; i++) {
		out << i->first << ": " << i->second << '\n';
	}
}

///////////////////////////////////////////////////////////////////////////////

class Archive {
public:
	Archive() = default;
	virtual bool add(string s) = 0;
	virtual unique_ptr<BatchProcess> getBatchProcess(size_t desiredSize) = 0;
	virtual size_t size() const = 0;
	void print(ostream & out);
	virtual ~Archive() {}
};

void Archive::print(ostream & out) {
	const auto bpSize = 1000; //arbitrary number
	auto bp = getBatchProcess(bpSize);
	do {
		for (const auto & record : bp->getCurrentBatch()) {
			out << record << 'n';
		}
	} while (bp->nextBatch());
}

class ArchiveToMemory : public Archive {
public:
	ArchiveToMemory() = default;
	virtual bool add(string s) { storage.push_back(move(s)); return true; }
	virtual unique_ptr<BatchProcess> getBatchProcess(size_t desiredSize) { 
		(void)desiredSize;
		return make_unique<BatchFromMemory>(&storage);
	}
	virtual size_t size() const { return storage.size(); }
	virtual ~ArchiveToMemory() {}
private:
	vector<string> storage;
};

class ArchiveToFile : public Archive {
public:
	ArchiveToFile() = delete;
	ArchiveToFile(const string & path) : 
		outFilePath(path), outfile(path, ios_base::out | ios_base::trunc) {}
	virtual bool add(string s) {
		if (!outfile.is_open()) outfile.open(outFilePath, ios_base::out | ios_base::app);
		outfile << s;
		if (s.back() != '\n') outfile << '\n';
		count++;
		return true;
	}
	virtual unique_ptr<BatchProcess> getBatchProcess(size_t desiredSize) {
		outfile.close();
		return make_unique<BatchFromFile>(outFilePath, desiredSize);
	}
	virtual size_t size() const { return count; }
	virtual ~ArchiveToFile() {}
private:
	string outFilePath;
	ofstream outfile;
	size_t count = 0;
};

///////////////////////////////////////////////////////////////////////////////

class Archiver {
public:
	Archiver(bool files = false);
	void addGroup(const metaf::GroupInfo & groupInfo);
	void addResult(const metaf::ParseResult & result);
	unique_ptr<BatchProcess>getBatchGroup(size_t index, metaf::ReportPart reportPart);
	void addErrorReport(metaf::ReportError e, const std::string & report);
	unique_ptr<BatchProcess>getBatchReportError(size_t index, metaf::ReportPart reportPart);
	bool printErrorReports(ostream & out) const; 
private:
	static const auto size = variant_size_v<metaf::Group>;
	array<unique_ptr<Archive>, size * reportPartSize> archives;
	unique_ptr<Archive> errorReports;
	size_t groupIndex(const metaf::Group & group, metaf::ReportPart reportPart) {
		return groupIndex(group.index(), reportPartIndex(reportPart));
	}
	size_t groupIndex(size_t groupIndex, size_t reportPartIndex) {
		return groupIndex * reportPartSize + reportPartIndex;
	}
	void makeGroupArchives(bool files);
	void makeErrorArchives(bool files);
};

Archiver::Archiver(bool files) {
	makeGroupArchives(files);
	makeErrorArchives(files);
}

void Archiver::makeGroupArchives(bool files) {
	for (auto i = 0u; i < variant_size_v<metaf::Group>; i++) {
		for (auto j = 0u; j < reportPartSize; j++) {
			auto idx = groupIndex(i, j);
			if (files) {
				const auto reportPart = reportPartFromIndex(j).value();
				const string filename = string(groupName(i)) + string("-") + 
					string(reportPartName(reportPart)) + string(".txt");
				archives.at(idx) = make_unique<ArchiveToFile>(filename);
			} else {
				archives.at(idx) = make_unique<ArchiveToMemory>();			
			}
		}
	}
}

void Archiver::makeErrorArchives(bool files) {
	if (!files) {
		errorReports = make_unique<ArchiveToMemory>();
		return;
	}
	errorReports = make_unique<ArchiveToFile>("reportError");
}

void Archiver::addGroup(const metaf::GroupInfo & groupInfo) {
	auto idx = groupIndex(groupInfo.group.index(), reportPartIndex(groupInfo.reportPart));
	archives.at(idx)->add(groupInfo.rawString);
}

void Archiver::addResult(const metaf::ParseResult & result) {
	for (const auto & gi : result.groups) {
		addGroup(gi);
	}
}

unique_ptr<BatchProcess> Archiver::getBatchGroup(size_t index, metaf::ReportPart reportPart) {
	const auto idx = groupIndex(index, reportPartIndex(reportPart));
	const auto batchSize = 1000; // arbitrary number
	return archives.at(idx)->getBatchProcess(batchSize);
}

void Archiver::addErrorReport(metaf::ReportError e, const std::string & report) {
	errorReports->add(reportErrorName(e));
	errorReports->add(report);
}

bool Archiver::printErrorReports(ostream & out) const {
	const auto s = errorReports->size();
	if (!s) return false;
	out << "Reports with errors: " << s / 2 << '\n';
	errorReports->print(out);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

class ParserPerformanceTester {
public:
	ParserPerformanceTester(size_t repetitions);
	ParserPerformanceTester(const string & filePath, size_t batchSize);
	size_t getTestSetSize() const { return src->totalSize(); }
	void test();
	void setStats(PerformanceStats & s) { stats = &s; }
	PerformanceStats * getStats() const { return stats; }
	void setArchiver(Archiver & a) { archiver = &a; }
	Archiver * getArchiver() const { return archiver; }
private:
	unique_ptr<BatchProcess> src;
	PerformanceStats * stats = nullptr;
	Archiver * archiver = nullptr;
	bool verbose = true;
	uint64_t reportCount = 0;
	uint64_t groupCount = 0;

	static uint64_t countGroups(const vector<metaf::ParseResult> & result);
	void toArchives(const vector<metaf::ParseResult> & result, const vector<string> & batch);
	void toStats(const vector<metaf::ParseResult> & result, uint64_t totalTimeMicrosec);
	void verbosePrintBatch();
};

ParserPerformanceTester::ParserPerformanceTester(size_t repetitions) :
	src(make_unique<BatchFromTestdataReal>(repetitions))
{}

ParserPerformanceTester::ParserPerformanceTester(const string & filePath, size_t batchSize) :
	src(make_unique<BatchFromFile>(filePath, batchSize))
{}

// TODO: archive reports with errors too
// TODO: calculate percentage of reports with errors
// TODO: calculate percentage of unrecognised groups
// TODO: collate unrecognised groups and display most frequent ones
void ParserPerformanceTester::test() {
	reportCount = 0;
	groupCount = 0;
	do {
		vector<metaf::ParseResult> result;
		const auto batchSize = src->getCurrentBatch().size();
		result.reserve(batchSize);
		const auto currentBatch = src->getCurrentBatch();
		const auto beginTime = chrono::system_clock::now();
		for (const auto & report : currentBatch) {
			result.push_back(metaf::Parser::parse(report));
		}
		const auto endTime = chrono::system_clock::now();
		auto totalTimeMicrosec = 
			chrono::duration_cast<chrono::microseconds>(endTime - beginTime).count();
		toStats(result, totalTimeMicrosec);
		toArchives(result, currentBatch);
		if (verbose) verbosePrintBatch();
	} while (src->nextBatch());
}

uint64_t ParserPerformanceTester::countGroups(const vector<metaf::ParseResult> & result) {
	uint64_t count = 0;
	for (const auto & r : result) {
		count += r.groups.size();
	} 
	return count;
}

void ParserPerformanceTester::toArchives(const vector<metaf::ParseResult> & result, 
	const vector<string> & batch)
{
	if (!archiver) return;
	for (auto i = 0u; i < result.size(); i++) { // assume result.size() == batch.size()
		if (auto e = result.at(i).reportMetadata.error; e != metaf::ReportError::NONE) {
			archiver->addErrorReport(e, batch.at(i));
		}
		archiver->addResult(result.at(i));
	} 
}

void ParserPerformanceTester::toStats(const vector<metaf::ParseResult> & result, 
	uint64_t totalTimeMicrosec)
{
	if (!stats) return;
	auto gc = countGroups(result);
	stats->add("report", totalTimeMicrosec, result.size());
	stats->add("group", totalTimeMicrosec, gc);
	reportCount += result.size();
	groupCount += gc;
}

void ParserPerformanceTester::verbosePrintBatch() {
	if (!verbose) return;
	cout << "Processed " << reportCount << " reports of " << getTestSetSize();
	cout << " (" << reportCount * 100 / getTestSetSize() << "%)";
	cout << ", " << groupCount << " groups parsed\n";
}

///////////////////////////////////////////////////////////////////////////////

class GroupPerformanceTester {
public:
	GroupPerformanceTester(Archiver & a) : archiver(&a) {}
private:
	Archiver * archiver = nullptr;
};

///////////////////////////////////////////////////////////////////////////////
/*
void printSize(string_view name, size_t size) {
	cout << "Size of " << name << ' ' << size << endl;
}
*/
void printDataSize(){
	auto printSize = [] (string_view name, size_t size){
		cout << "Size of " << name << ' ' << size << endl;
	};
	cout << "Data type sizes:" << endl; 	
	printSize("Runway", sizeof(metaf::Runway));
	printSize("MetafTime", sizeof(metaf::MetafTime));
	printSize("Temperature", sizeof(metaf::Temperature));
	printSize("Speed", sizeof(metaf::Speed));
	printSize("Distance", sizeof(metaf::Distance));
	printSize("Direction", sizeof(metaf::Direction));
	printSize("Pressure", sizeof(metaf::Pressure));
	printSize("Precipitation", sizeof(metaf::Precipitation));
	printSize("SurfaceFriction", sizeof(metaf::SurfaceFriction));
	printSize("WaveHeight", sizeof(metaf::WaveHeight));
	printSize("WeatherPhenomena", sizeof(metaf::WeatherPhenomena));
	printSize("CloudType", sizeof(metaf::CloudType));
	cout << endl;
	printSize("Group", sizeof(metaf::Group));
	cout << endl;
	cout << "Group sizes:" << endl; 	
	printSize(groupName(0), sizeof(variant_alternative_t<0, metaf::Group>));
	printSize(groupName(1), sizeof(variant_alternative_t<1, metaf::Group>));
	printSize(groupName(2), sizeof(variant_alternative_t<2, metaf::Group>));
	printSize(groupName(3), sizeof(variant_alternative_t<3, metaf::Group>));
	printSize(groupName(4), sizeof(variant_alternative_t<4, metaf::Group>));
	printSize(groupName(5), sizeof(variant_alternative_t<5, metaf::Group>));
	printSize(groupName(6), sizeof(variant_alternative_t<6, metaf::Group>));
	printSize(groupName(7), sizeof(variant_alternative_t<7, metaf::Group>));
	printSize(groupName(8), sizeof(variant_alternative_t<8, metaf::Group>));
	printSize(groupName(9), sizeof(variant_alternative_t<9, metaf::Group>));
	printSize(groupName(10), sizeof(variant_alternative_t<10, metaf::Group>));
	printSize(groupName(11), sizeof(variant_alternative_t<11, metaf::Group>));
	printSize(groupName(12), sizeof(variant_alternative_t<12, metaf::Group>));
	printSize(groupName(13), sizeof(variant_alternative_t<13, metaf::Group>));
	printSize(groupName(14), sizeof(variant_alternative_t<14, metaf::Group>));
	printSize(groupName(15), sizeof(variant_alternative_t<15, metaf::Group>));
	printSize(groupName(16), sizeof(variant_alternative_t<16, metaf::Group>));
	printSize(groupName(17), sizeof(variant_alternative_t<17, metaf::Group>));
	printSize(groupName(18), sizeof(variant_alternative_t<18, metaf::Group>));
	printSize(groupName(19), sizeof(variant_alternative_t<19, metaf::Group>));
	printSize(groupName(20), sizeof(variant_alternative_t<20, metaf::Group>));
	printSize(groupName(21), sizeof(variant_alternative_t<21, metaf::Group>));
	cout << endl;
}

int main(int argc, char ** argv) {
	(void) argc; (void) argv;

	static const size_t testDataRealRepetitions = 300;

	PerformanceStats stats;
	Archiver archiver;
	
	{
		cout << "Checking overall parser performance\n";
		ParserPerformanceTester tester(testDataRealRepetitions);
		cout << "Test set size is " << tester.getTestSetSize() << '\n';
		tester.setStats(stats);
		tester.setArchiver(archiver);
		tester.test();
		cout << '\n';

		stats.print("report");
		stats.print("group");
		cout << '\n';
	}

	{
		cout << "Reports not recognised by the parser:\n";
		if (!archiver.printErrorReports(cout)) 
			cout << "no report errors, all reports parsed successfully\n";
		cout << '\n';
	}
	
	{
		cout << "Groups not recognised by the parser:\n";
		const auto unknowGroupIndex = variant_index<metaf::Group, metaf::UnknownGroup>();
		FrequencyStats unknownGroupStats;
		unknownGroupStats.add(archiver.getBatchGroup(unknowGroupIndex, metaf::ReportPart::UNKNOWN));
		unknownGroupStats.add(archiver.getBatchGroup(unknowGroupIndex, metaf::ReportPart::HEADER));
		unknownGroupStats.add(archiver.getBatchGroup(unknowGroupIndex, metaf::ReportPart::METAR));
		unknownGroupStats.add(archiver.getBatchGroup(unknowGroupIndex, metaf::ReportPart::TAF));
		unknownGroupStats.add(archiver.getBatchGroup(unknowGroupIndex, metaf::ReportPart::RMK));
		unknownGroupStats.compensateRepetitions(testDataRealRepetitions);
		unknownGroupStats.printDescending(cout);
	}

	printDataSize();
}