#include <iostream>
#include <unistd.h>
#include <cstring>
#include <chrono>

using namespace std;
using namespace chrono;

int main() {
    // File descriptors for pipe; fd[0] = read and fd[1] = write
    int fd[2];
    char buffer[100]; // Stores data read from pipe, holds 100 chars

    if(pipe(fd) == -1) {
        cerr << "Error: Pipe Creation Failed" << endl;
    }

    pid_t pid = fork(); // Creates a new child process
    if (pid < 0) { // Child process = 0, Parent process = 1
        cerr << "Fork Failed" << endl;
        return 1;
    }

    if (pid == 0) {
        close(fd[1]); // Close write

        auto start = high_resolution_clock::now(); // Starts timer
        ssize_t reading = read(fd[0], buffer, sizeof(buffer)); // Reads from pipe

        // Error handling for reading
        if(reading == -1) {
            if(errno == EPIPE) {
                cerr << "Error: Pipe Broken" << endl;
            } else {
                cerr << "Error: Reading Failed" << endl;
            }
            close(fd[0]);
            return 1;
        }

        auto stop = high_resolution_clock::now(); // Stops timer
        auto duration_time = duration_cast<microseconds>(stop - start); // Duration of timer

        cout << "Time for child to read: " << duration_time.count() << " microsecond(s)" << endl;
        cout << "Child received: " << buffer << endl;

        close(fd[0]);
    } else {
        close(fd[0]); // Close read

        auto start = high_resolution_clock::now(); // Starts timer
        const char* message = "Parent says hello!";
        ssize_t writing = write(fd[1], message, strlen(message) + 1); // Ensure /0 is written so child process can correctly read

        // Error handling for writing
        if(writing == -1) {
            if(errno == EPIPE) {
                cerr << "Error: Pipe Broken" << endl;
            } else {
                cerr << "Error: Writing Failed" << endl;
            }
            close(fd[1]);
            return 1;
        }
        
        auto stop = high_resolution_clock::now(); // Stops timer
        auto duration_time = duration_cast<microseconds>(stop - start);

        cout << "Time for Parent to write: " << duration_time.count() << " microsecond(s)" << endl;

        close(fd[1]);
    }
    return 0;
}