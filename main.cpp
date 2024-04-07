#include "linecounter.cpp"

#define _CRT_SECURE_NO_WARNINGS

// if windows -> wmain with wchar_t, else main with char

#ifdef _WIN32

int wmain(int argc, wchar_t* argv[]) {

	// Path of directory to be scanned
	wchar_t* path = nullptr;

	// no argument given
	if (argc == 1) {
		// path of executable
		path = argv[0];
	}

	// one argument given
	if (argc == 2) {

		// Help argument
		if (wcscmp(argv[1], L"--help") == 0 ||
			wcscmp(argv[1], L"/?") == 0)


#else

int main(int argc, char* argv[]) {

	// Path of directory to be scanned
	char* path = nullptr;

	// no argument given
	if (argc == 1) {
		// path of executable
		path = argv[0];
	}

	// one argument given
	if (argc == 2) {

		// Help argument
		if (strcmp(argv[1], "--help") == 0 ||
			strcmp(argv[1], "/?") == 0)


#endif


		{
			std::cout << "Linecounter" << "\n"
					  << "github.com/schaefereric" << "\n" 
					  << "\n"
					  << "Usage: linecounter.exe [PATH]" << "\n"
					  << "[PATH] is optional." << "\n"
					  << "If no [PATH] is given, linecounter scans the directory where the executable is stored" << "\n"
					  << std::endl;
			return 0;
		}

		else {
			// given path
			path = argv[1];
		} 
	}

	// more than one argument given
	if (argc > 2) {
		std::cout << "Too many arguments!" << std::endl;
		return -1;
	}

	Linecounter linecounter(path);

	linecounter.run();

	return 0;
}
