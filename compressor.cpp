// Project 1: Multi-threaded File Compressor (Simple RLE Compression)

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

mutex writeMutex;

// Function to compress a chunk using Run-Length Encoding (RLE)
string compressChunk(const string& chunk) {
    string result;
    int n = chunk.size();
    for (int i = 0; i < n; i++) {
        int count = 1;
        while (i + 1 < n && chunk[i] == chunk[i + 1]) {
            count++;
            i++;
        }
        result += chunk[i] + to_string(count);
    }
    return result;
}

// Thread worker to compress a file chunk
void compressWorker(const string& chunk, ofstream& outputFile) {
    string compressed = compressChunk(chunk);
    lock_guard<mutex> lock(writeMutex);
    outputFile << compressed;
}

int main() {
    ifstream inputFile("input.txt");
    ofstream outputFile("compressed.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening files." << endl;
        return 1;
    }

    vector<thread> threads;
    string chunk;
    const int CHUNK_SIZE = 1024;

    while (getline(inputFile, chunk)) {
        // If chunk is large, split further
        if (chunk.length() > CHUNK_SIZE) {
            for (size_t i = 0; i < chunk.length(); i += CHUNK_SIZE) {
                string subChunk = chunk.substr(i, CHUNK_SIZE);
                threads.emplace_back(compressWorker, subChunk, ref(outputFile));
            }
        } else {
            threads.emplace_back(compressWorker, chunk, ref(outputFile));
        }
    }

    for (auto& t : threads)
        t.join();

    inputFile.close();
    outputFile.close();

    cout << "Compression complete." << endl;
    return 0;
}
