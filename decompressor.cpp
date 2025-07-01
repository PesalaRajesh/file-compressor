#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

// Function to decompress the RLE-compressed string
string decompressChunk(const string& compressed) {
    string result;
    int n = compressed.size();

    for (int i = 0; i < n; i++) {
        char ch = compressed[i];
        string countStr;

        // Extract the number following the character
        while (i + 1 < n && isdigit(compressed[i + 1])) {
            countStr += compressed[i + 1];
            i++;
        }

        int count = stoi(countStr);
        result.append(count, ch);
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <compressed file> <output file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    ofstream outputFile(argv[2]);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error opening files." << endl;
        return 1;
    }

    string compressedLine;

    while (getline(inputFile, compressedLine)) {
        string decompressed = decompressChunk(compressedLine);
        outputFile << decompressed << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Decompression complete." << endl;

    return 0;
}
