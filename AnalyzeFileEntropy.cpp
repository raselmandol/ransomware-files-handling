#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

double calculateEntropy(const std::vector<char>& data) {
    std::vector<int> frequency(256, 0);
    for (char ch : data) {
        frequency[static_cast<unsigned char>(ch)]++;
    }

    double entropy = 0.0;
    for (int freq : frequency) {
        if (freq > 0) {
            double probability = static_cast<double>(freq) / data.size();
            entropy -= probability * log2(probability);
        }
    }

    return entropy;
}

bool isFileEncryptedByEntropy(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (file) {
        //reading the entire file into a vector
        std::vector<char> fileData(std::istreambuf_iterator<char>(file), {});

        //calculating entropy of the file data
        double entropy = calculateEntropy(fileData);
        double entropyThreshold = /*Set an appropriate threshold*/;

        file.close();
        return entropy > entropyThreshold;
    }
    return false;
}

int main() {
    std::string filePath = "example.txt";
    //encrypted by entropy?
    if (isFileEncryptedByEntropy(filePath)) {
        std::cout << "File appears to be encrypted based on entropy analysis.\n";
    } else {
        std::cout << "File does not appear to be encrypted based on entropy analysis.\n";
    }
    return 0;
}
