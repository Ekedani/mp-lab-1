// Used for input and output data
#include <fstream>
// Used only as a container, without methods
#include <string>

using namespace std;

const string INPUT_FILE_ADDRESS = "task1_input.txt";
const string OUTPUT_FILE_ADDRESS = "task1_output.txt";

const int NUM_OF_DISPLAYED_WORDS = 25;

// Just as example of stop words handling
const int NUM_OF_STOP_WORDS = 8;
const char *STOP_WORDS[] = {"in", "on", "out", "of", "the", "an", "and", "for"};

int main() {
    ifstream input_file;
    input_file.open(INPUT_FILE_ADDRESS);

    int wordsArraySize = 10;
    int wordsNum = 0;
    string *wordsArray = new string[wordsArraySize];
    int *wordsFrequency = new int[wordsArraySize];

    int wordStartIdx;
    int wordEndIdx;

    string currentLine;
    int lineSize;

    PROCESS_FILE:
    getline(input_file, currentLine);
    lineSize = 0;
    wordStartIdx = 0;

    CALCULATE_LINE_LENGTH:
    if (currentLine[lineSize] != '\0') {
        lineSize++;
        goto CALCULATE_LINE_LENGTH;
    }

    PROCESS_LINE:
    FIND_WORD_START:
    if (!(((currentLine[wordStartIdx] >= 'A' && currentLine[wordStartIdx] <= 'Z')) ||
          ((currentLine[wordStartIdx] >= 'a' && currentLine[wordStartIdx] <= 'z')))
        && wordStartIdx < lineSize) {
        wordStartIdx++;
        goto FIND_WORD_START;
    }
    wordEndIdx = wordStartIdx;
    FIND_WORD_END:
    if (wordEndIdx < lineSize && (((currentLine[wordEndIdx] >= '0' && currentLine[wordEndIdx] <= '9') ||
                                   ((currentLine[wordEndIdx] >= 'A' && currentLine[wordEndIdx] <= 'Z')) ||
                                   ((currentLine[wordEndIdx] >= 'a' && currentLine[wordEndIdx] <= 'z'))) ||
                                  currentLine[wordEndIdx] == '-' || currentLine[wordEndIdx] == '\'')) {
        wordEndIdx++;
        goto FIND_WORD_END;
    }

    int wordSize = wordEndIdx - wordStartIdx;
    // Ignoring noise words (size must be greater than 1)
    if (wordSize > 1) {
        char* currentWord = new char[wordSize + 1];
        currentWord[wordSize] = '\0';
        int lastCharIdx = 0;
        COPY_SUBSTR:
        if (wordStartIdx < wordEndIdx) {
            if (currentLine[wordStartIdx] >= 65 && currentLine[wordStartIdx] <= 90) {
                currentWord[lastCharIdx] = currentLine[wordStartIdx] + 32;
                lastCharIdx++;
            } else {
                currentWord[lastCharIdx] = currentLine[wordStartIdx];
                lastCharIdx++;
            }
            wordStartIdx++;
            goto COPY_SUBSTR;
        }

        // Check if stop word
        int comparedStopWordIdx = 0;
        CHECK_STOP_WORDS:
        if (comparedStopWordIdx < NUM_OF_STOP_WORDS) {
            bool stringsStopEqual = true;
            int comparedStopCharIdx = 0;
            CHECK_IF_EQUAL_TO_STOP:
            stringsStopEqual &= (currentWord[comparedStopCharIdx] ==
                                 STOP_WORDS[comparedStopWordIdx][comparedStopCharIdx]);
            if (stringsStopEqual && comparedStopCharIdx < wordSize) {
                comparedStopCharIdx++;
                goto CHECK_IF_EQUAL_TO_STOP;
            }
            if (!stringsStopEqual) {
                comparedStopWordIdx++;
                goto CHECK_STOP_WORDS;
            } else {
                goto PROCESS_LINE;
            }
        }

        // Check if new word
        int comparedExWordIdx = 0;
        bool stringsExEqual = false;
        CHECK_EXISTING_WORDS:
        if (comparedExWordIdx < wordsNum) {
            stringsExEqual = true;
            int comparedExCharIdx = 0;
            CHECK_IF_EQUAL_TO_EX:
            stringsExEqual &= (currentWord[comparedExCharIdx] == wordsArray[comparedExWordIdx][comparedExCharIdx]);
            if (stringsExEqual && comparedExCharIdx < wordSize) {
                comparedExCharIdx++;
                goto CHECK_IF_EQUAL_TO_EX;
            }
            if (!stringsExEqual) {
                comparedExWordIdx++;
                goto CHECK_EXISTING_WORDS;
            }
        }
        if (stringsExEqual) {
            wordsFrequency[comparedExWordIdx]++;
        } else {
            wordsArray[wordsNum] = currentWord;
            wordsFrequency[wordsNum] = 1;
            wordsNum++;
        }
        if (wordsNum >= 0.8 * wordsArraySize) {
            string *oldWords = wordsArray;
            int *oldFrequency = wordsFrequency;
            wordsArraySize *= 2;
            wordsArray = new string[wordsArraySize];
            wordsFrequency = new int[wordsArraySize];
            int wordIndex = 0;
            COPY_TO_NEW:
            wordsArray[wordIndex] = oldWords[wordIndex];
            wordsFrequency[wordIndex] = oldFrequency[wordIndex];
            wordIndex++;
            if (wordIndex < wordsNum) {
                goto COPY_TO_NEW;
            }
            delete[] oldWords;
            delete[] oldFrequency;
        }
    } else {
        wordStartIdx = wordEndIdx;
    }

    if (wordEndIdx < lineSize) {
        goto PROCESS_LINE;
    }

    if (!input_file.eof()) {
        goto PROCESS_FILE;
    }

    input_file.close();
    SORT_BY_FREQUENCY:
    bool swapped = false;
    int j = 0;
    SORT_INNER:
    if (wordsFrequency[j] < wordsFrequency[j + 1]) {
        int freqTmp = wordsFrequency[j];
        string wordTmp = wordsArray[j];
        wordsFrequency[j] = wordsFrequency[j + 1];
        wordsArray[j] = wordsArray[j + 1];
        wordsFrequency[j + 1] = freqTmp;
        wordsArray[j + 1] = wordTmp;
        swapped = true;
    }
    j++;
    if (j < wordsNum - 1) {
        goto SORT_INNER;
    }
    if (swapped) {
        goto SORT_BY_FREQUENCY;
    }

    int outputWordIdx = 0;
    ofstream output_file;
    output_file.open(OUTPUT_FILE_ADDRESS);
    OUTPUT_WORDS:
    if (outputWordIdx < wordsNum && outputWordIdx < NUM_OF_DISPLAYED_WORDS) {
        output_file << wordsArray[outputWordIdx] << " - " << wordsFrequency[outputWordIdx] << '\n';
        outputWordIdx++;
        goto OUTPUT_WORDS;
    }
    output_file.close();
    return 0;
}
