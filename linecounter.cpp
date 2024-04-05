#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>

// use C++20 or C++17

using namespace std;

namespace fs = filesystem;

static char* _convertWcharToChar(const wchar_t* vIn) {
	char* vOut = new char[wcslen(vIn) + 1];
	wcstombs_s(NULL, vOut, wcslen(vIn) + 1, vIn, wcslen(vIn) + 1);
	return vOut;
}

// Indexes all .txt files in directory
struct DirectoryIndex {

	// Path of Directory
	fs::path basePath;

	// Stores the paths of text files (*.txt) in directory and their line count
	vector<pair<fs::path, int>> textFiles;

	DirectoryIndex(const char * dirPath) : basePath(dirPath) {}

	void enumerateDirectory() {
		pair<fs::path, int> tempPair;

		// Check if path exists
		if (!(fs::exists(basePath))) {
			std::cout << "path does not exist" << std::endl;
			throw 1;
		}

		// Iterate over Directory
		for (const auto& item : fs::directory_iterator(basePath)) {

			// Skip loop iteration if item is a directory
			if (fs::is_directory(item.path())) {
				continue;
			}

			// If item is a .txt file, path is saved in textFiles
			if (item.path().extension() == ".txt") {
				tempPair.first = item.path();
				tempPair.second = 0;

				textFiles.push_back(tempPair);
			}

		}


	}
};


class Linecounter {

public:

	Linecounter(const char* dirPath) : directoryIndex(dirPath) {
		totalLineCount = 0;
	}

	string run() {
		directoryIndex.enumerateDirectory();
		evaluateAllFiles();
		format_output();
		print_output();
		return formattedConsoleOut;
	}

	void print_output() const {
		cout << formattedConsoleOut << endl;
	}

private:
	int			   totalLineCount;			// Total lines counted
	string		   formattedConsoleOut;     // Formatted Results for console output
	DirectoryIndex directoryIndex;

	// Iterates over directoryIndex.textFiles, counts all lines and saves the line count in the corresponding pair
	void evaluateAllFiles() {
		string tempFilestreamAsString;
		int tempLinecount = 0;

		for (auto & i : directoryIndex.textFiles) {

			// Get text from filestream (pair.first) and save to string
			tempFilestreamAsString = read_file(i.first);

			// Get linecount
			tempLinecount = count_lines(tempFilestreamAsString);

			// Save linecount to pair (pair.second)
			i.second = tempLinecount;

		}
	}

	// Extracts text from fstream and returns string
	string read_file(const fs::path& path) {
		string line;
		fstream file;
		char tempchar;

		file.open(path, std::ios::in);

		if (file.is_open()) {
			while (!file.eof()) {
				tempchar = file.get();

				if (tempchar == '\xFF') continue;

				line += tempchar;
			}
			file.close();
		}

		return line;
	}

	int count_lines(string str) {
		int counter = 0;

		for (auto i : str) {
			if (i == '\n' || i == '\r' || i == '\xFF') {
				counter++;
			}

		}

		counter++;
		return counter;
	}

	string format_output() {
		string str;
		unsigned totalLines = 0;
		char * tempCastingPointer;

		for (auto& i : directoryIndex.textFiles) {
			//str += '\t';
			str += "Lines in ";
			
			tempCastingPointer = _convertWcharToChar(i.first.filename().c_str()); // Filename
			str += tempCastingPointer;
			str += ": ";

			str += to_string(i.second); // Linecount
			str += " Lines";
			
			str += "\n";
			delete tempCastingPointer; // Deallocate temporary typecast
			totalLines += i.second;
		}

		str += "-----------------------------";
		str += "\n";
		str += "Total Lines: ";
		str += to_string(totalLines);

		this->totalLineCount = totalLines;
		this->formattedConsoleOut = str;
		return str;
	}

};


