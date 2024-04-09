#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <filesystem>
#include <vector>
#include <exception>
#include <sstream>
#include <cstdlib>

#define _CRT_SECURE_NO_WARNINGS

// use C++20 or C++17

using namespace std;

namespace fs = filesystem;

static char* _convertWcharToChar(const wchar_t* vIn) {
	char* vOut = new char[wcslen(vIn) + 1];
	wcstombs_s(NULL, vOut, wcslen(vIn) + 1, vIn, wcslen(vIn) + 1);
	return vOut;
}

// Indexes all .txt and c/cpp source files in directory
struct DirectoryIndex {

	// Path of Directory
	fs::path basePath;

	// Stores the paths of text files and their line count
	vector<pair<fs::path, int>> textFiles;

	// Constructor for C-String path
	DirectoryIndex(const char* dirPath) {
		basePath = sanitizeFilePathString(dirPath);
	}

	// Constructor for Wide C-String path
	DirectoryIndex(const wchar_t* dirPath) {
		basePath = sanitizeFilePathString(dirPath);
	}

	// Constructor for filesystem::path
	DirectoryIndex(fs::path dirPath) : basePath(dirPath) {}

	void enumerateDirectory() {
		pair<fs::path, int> tempPair;

		// Check if path exists
		if (!(fs::exists(basePath))) {
			throw invalid_argument("ERROR: Path to directory is invalid");
		}

		// If basePath points to a file, set basePath to its parent directory
		if (fs::is_regular_file(basePath)) {
			basePath = basePath.parent_path();
		}

		// Iterate over Directory
		for (const auto& item : fs::directory_iterator(basePath)) {

			// Skip loop iteration if item is a directory
			if (fs::is_directory(item.path())) {
				continue;
			}

			// If item is a .txt or c/cpp source file, path is saved in textFiles
			if (item.path().extension() == ".txt" ||
				item.path().extension() == ".c"	  || 
				item.path().extension() == ".h"   ||
				item.path().extension() == ".cpp" ||  
				item.path().extension() == ".hpp" || 
				item.path().extension() == ".ino"   )
			{
				tempPair.first = item.path();
				tempPair.second = 0;

				textFiles.push_back(tempPair);
			}

		}
	}

	// Sanitize C String
	const char* sanitizeFilePathString(const char* dirtyString) {
		// Check if string doesnt need to be sanitized
		bool beginClean = false;
		bool endClean = false;

		if ((dirtyString[0] != ' ') && (dirtyString[0] != '	')) {
			// no whitespace at beginning
			beginClean = true;
		}
		if ((dirtyString[strlen(dirtyString) - 1] != ' ') && (dirtyString[strlen(dirtyString) - 1] != '	')) {
			// no whitespace at end
			endClean = true;
		}
		if (beginClean && endClean) {
			// Path doesnt need to be sanitized
			return dirtyString;
		}

		char* tempString = nullptr;
		char* workingCopyString = new char[strlen(dirtyString) + 1];
		strcpy(workingCopyString, dirtyString);

		// Remove Whitespace at the beginning of string
		if (!beginClean) {
			while (1) {
				int i = 0;

				if ((workingCopyString[i] == ' ') || (workingCopyString[i] == '	')) {
					// Initialize string that is one element shorter than current working copy
					tempString = new char[strlen(workingCopyString)];
					// copy string with offset to next character to temp copy
					strcpy(tempString, workingCopyString + i + 1);
					// delete old working copy
					delete[] workingCopyString;
					// Move temp pointer to working copy
					workingCopyString = tempString;
				}
				else break;
			}
		}

		// Remove Whitespace at the end of string
		if (!endClean) {
			// Determine how many characters at the end of string are whitespace
			int loopIndex = strlen(workingCopyString) - 1;
			unsigned int amountOfWhitespace = 0;
			while (1) {

				if ((workingCopyString[loopIndex] == ' ') || (workingCopyString[loopIndex] == '	')) {
					amountOfWhitespace++;
					loopIndex--;
				}
				else break;

			}

			if (amountOfWhitespace > 0) {
				tempString = new char[strlen(workingCopyString) + 1 - amountOfWhitespace];

				int j = 0;
				for (j = 0; j < (strlen(workingCopyString) - amountOfWhitespace); j++) {
					tempString[j] = workingCopyString[j];
				}
				tempString[j] = '\0';

				// Move temp pointer to working copy
				delete[] workingCopyString;
				workingCopyString = tempString;
			}
		}

		return workingCopyString;
	}

	// Sanitize Wide C String
	const wchar_t* sanitizeFilePathString(const wchar_t* dirtyString) {
		// Check if string doesnt need to be sanitized
		bool beginClean = false;
		bool endClean = false;

		if ((dirtyString[0] != L' ') && (dirtyString[0] != L'	')) {
			// no whitespace at beginning
			beginClean = true;
		}
		if ((dirtyString[wcslen(dirtyString) - 1] != L' ') && (dirtyString[wcslen(dirtyString) - 1] != L'	')) {
			// no whitespace at end
			endClean = true;
		}	
		if (beginClean && endClean) {
			// Path doesnt need to be sanitized
			return dirtyString;
		}

		wchar_t* tempString = nullptr;
		wchar_t* workingCopyString = new wchar_t[wcslen(dirtyString) + 1];
		wcscpy(workingCopyString, dirtyString);

		// Remove Whitespace at the beginning of string
		if (!beginClean) {
			while (1) {
				int i = 0;

				if ((workingCopyString[i] == L' ') || (workingCopyString[i] == L'	')) {
					// Initialize string that is one element shorter than current working copy
					tempString = new wchar_t[wcslen(workingCopyString)];
					// copy string with offset to next character to temp copy
					wcscpy(tempString, workingCopyString + i + 1);
					// delete old working copy
					delete[] workingCopyString;
					// Move temp pointer to working copy
					workingCopyString = tempString;
				}
				else break;
			}
		}
		// Remove Whitespace at the end of string
		if (!endClean) {
			// Determine how many characters at the end of string are whitespace
			int loopIndex = wcslen(workingCopyString) - 1;
			unsigned int amountOfWhitespace = 0;
			while (1) {

				if ((workingCopyString[loopIndex] == L' ') || (workingCopyString[loopIndex] == L'	')) {
					amountOfWhitespace++;
					loopIndex--;
				}
				else break;

			}

			if (amountOfWhitespace > 0) {
				tempString = new wchar_t[wcslen(workingCopyString) + 1 - amountOfWhitespace];

				int j = 0;
				for (j = 0; j < (wcslen(workingCopyString) - amountOfWhitespace); j++) {
					tempString[j] = workingCopyString[j];
				}
				tempString[j] = L'\0';

				// Move temp pointer to working copy
				delete[] workingCopyString;
				workingCopyString = tempString;
			}
		}

		return workingCopyString;
		
	}

};


class Linecounter {

public:
	Linecounter(const char* dirPath) : directoryIndex(dirPath) {
		totalLineCount = 0;
		longestFileNameCount = 0;
	}

	Linecounter(const wchar_t* dirPath) : directoryIndex(dirPath) {
		totalLineCount = 0;
		longestFileNameCount = 0;
	}

	Linecounter(fs::path dirPath) : directoryIndex(dirPath) {
		totalLineCount = 0;
		longestFileNameCount = 0;
	}

	string run() {
		try {
			directoryIndex.enumerateDirectory();
		}
		catch (std::invalid_argument const& ex) {
			std::cout << ex.what() << std::endl;
			return string("error");
		}
		
		evaluateAllFiles();
		format_output();
		print_output();
		return formattedConsoleOut;
	}

	void print_output() const {
		cout << formattedConsoleOut << endl;
	} 

	int getTotalLineCount() const {
		return this->totalLineCount;
	}

private:
	int			   totalLineCount;			// Total lines counted
	string		   formattedConsoleOut;     // Formatted Results for console output
	DirectoryIndex directoryIndex;
	int			   longestFileNameCount;	// Number of Letters of longest file name, needed for console output formatting. "arsch.txt" == 9

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

			// Update total line count
			this->totalLineCount += tempLinecount;

			// If length of filename is higher than any previous file counted, update counter
			if (static_cast<int>(wcslen(i.first.filename().c_str())) > this->longestFileNameCount) {
				this->longestFileNameCount = static_cast<int>(wcslen(i.first.filename().c_str()));
			}

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
		stringstream sstr;
		char * tempCastingPointer;

		sstr << "Path: ";
		tempCastingPointer = _convertWcharToChar(directoryIndex.basePath.c_str());
		sstr << tempCastingPointer;

		sstr << "\n";
		sstr << "\n";
		delete[] tempCastingPointer; // Deallocate temporary typecast

		for (auto& i : directoryIndex.textFiles) {
			tempCastingPointer = _convertWcharToChar(i.first.filename().c_str()); // Filename typecast 

			sstr << left << "Lines in " << tempCastingPointer << ": ";
			
			// Calculate how much whitespace is needed for proper column formatting
			int amountOfWhitespace = this->longestFileNameCount - static_cast<int>(wcslen(i.first.filename().c_str()));
			for (int n = 0; n <= amountOfWhitespace; n++) {
				sstr << " ";
			}

			sstr << to_string(i.second) << " Lines";

			sstr << "\n";
			delete[] tempCastingPointer; // Deallocate temporary typecast
		}

		sstr << "-----------------------------";
		sstr << "\n";
		sstr << "Total Lines: ";
		sstr << to_string(this->totalLineCount);

		this->formattedConsoleOut = sstr.str();
		return sstr.str();
	}

};