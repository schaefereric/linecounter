#include "linecounter.cpp"

int main(int argc, char* argv[]) {
    
    // Path of directory to be scanned
    char* path = nullptr;

    if (argc == 1) {
        // parent path of executable
        path = argv[0];
    }

    if (argc == 2) {
        // given path
        path = argv[1];
    }

    if (argc > 2) {
        std::cout << "Too many arguments!" << std::endl;
        return -1;
    }

	Linecounter linecounter(path);

	linecounter.run();

	return 0;
}
