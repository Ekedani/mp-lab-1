#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string FILE_ADDRESS = "task2_input.txt";

const int LINES_PER_PAGE = 45;

int main() {
    ifstream file;
    file.open(FILE_ADDRESS);
    int wordsArraySize = 10;
    int wordsNum = 0;
    string *wordsArray = new string[wordsArraySize];
    int *wordsFrequency = new int[wordsArraySize];

    int **wordsPages = new int *[wordsArraySize];
    int *pagesNums = new int[wordsArraySize];
    int *pagesCapacities = new int[wordsArraySize];

    int wordStartIdx;
    int wordEndIdx;

    string currentLine;
    int lineSize;

    int currentPageNum = 0;
    int currentLineNum = 0;


    PROCESS_FILE:
    if (currentLineNum % LINES_PER_PAGE == 0) {
        currentPageNum++;
    }
    currentLineNum++;
    getline(file, currentLine);
    lineSize = 0;
    wordStartIdx = 0;

    CALCULATE_LINE_LENGTH:
    if (currentLine[lineSize] != '\0') {
        lineSize++;
        goto CALCULATE_LINE_LENGTH;
    }

    PROCESS_LINE:
    FIND_WORD_START:
    if (!((currentLine[wordStartIdx] >= '0' && currentLine[wordStartIdx] <= '9') ||
          ((currentLine[wordStartIdx] >= 'A' && currentLine[wordStartIdx] <= 'Z')) ||
          ((currentLine[wordStartIdx] >= 'a' && currentLine[wordStartIdx] <= 'z')))) {
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

    if (wordSize > 0) {
        char currentWord[wordSize + 1];
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
            if (wordsFrequency[comparedExWordIdx] < 100) {
                if (wordsPages[comparedExWordIdx][pagesNums[comparedExWordIdx] - 1] != currentPageNum) {
                    wordsPages[comparedExWordIdx][pagesNums[comparedExWordIdx]] = currentPageNum;
                    pagesNums[comparedExWordIdx]++;
                }
                if (pagesNums[comparedExWordIdx] >= 0.8 * pagesCapacities[comparedExWordIdx]) {
                    int *oldPages = wordsPages[comparedExWordIdx];
                    pagesCapacities[comparedExWordIdx] *= 2;
                    wordsPages[comparedExWordIdx] = new int[pagesCapacities[comparedExWordIdx]];
                    int wordIndex = 0;
                    COPY_PAGES_TO_NEW:
                    if (wordIndex < pagesNums[comparedExWordIdx]) {
                        wordsPages[comparedExWordIdx][wordIndex] = oldPages[wordIndex];
                        wordIndex++;
                        goto COPY_PAGES_TO_NEW;
                    }
                    delete[] oldPages;
                }
            }
        } else {
            wordsArray[wordsNum] = currentWord;
            wordsFrequency[wordsNum] = 1;
            wordsPages[wordsNum] = new int[10];
            wordsPages[wordsNum][0] = currentPageNum;
            pagesCapacities[wordsNum] = 10;
            pagesNums[wordsNum] = 1;
            wordsNum++;
        }
        if (wordsNum >= 0.8 * wordsArraySize) {
            string *oldWords = wordsArray;
            int *oldFrequency = wordsFrequency;
            int **oldPages = wordsPages;
            int *oldPagesNums = pagesNums;
            int *oldPagesCapacities = pagesCapacities;

            wordsArraySize *= 2;
            wordsArray = new string[wordsArraySize];
            wordsFrequency = new int[wordsArraySize];
            wordsPages = new int *[wordsArraySize];
            pagesNums = new int[wordsArraySize];
            pagesCapacities = new int[wordsArraySize];

            int wordIndex = 0;
            COPY_TO_NEW:
            wordsArray[wordIndex] = oldWords[wordIndex];
            wordsFrequency[wordIndex] = oldFrequency[wordIndex];
            wordsPages[wordIndex] = oldPages[wordIndex];
            pagesNums[wordIndex] = oldPagesNums[wordIndex];
            pagesCapacities[wordIndex] = oldPagesCapacities[wordIndex];
            wordIndex++;

            if (wordIndex < wordsNum) {
                goto COPY_TO_NEW;
            }
            delete[] oldWords;
            delete[] oldFrequency;
            delete[] oldPages;
            delete[] oldPagesCapacities;
            delete[] oldPagesNums;
        }
    } else {
        wordStartIdx = wordEndIdx;
    }

    if (wordEndIdx < lineSize) {
        goto PROCESS_LINE;
    }

    if (!file.eof()) {
        goto PROCESS_FILE;
    }

    string *oldWords = wordsArray;
    int *oldFrequency = wordsFrequency;
    int **oldPages = wordsPages;
    int *oldPagesNums = pagesNums;
    int *oldPagesCapacities = pagesCapacities;

    wordsArray = new string[wordsArraySize];
    wordsFrequency = new int[wordsArraySize];
    wordsPages = new int *[wordsArraySize];
    pagesNums = new int[wordsArraySize];
    pagesCapacities = new int[wordsArraySize];

    int wordIndex = 0;
    COPY_INFREQUENT_TO_NEW:
    wordsArray[wordIndex] = oldWords[wordIndex];
    wordsFrequency[wordIndex] = oldFrequency[wordIndex];
    wordsPages[wordIndex] = oldPages[wordIndex];
    pagesNums[wordIndex] = oldPagesNums[wordIndex];
    pagesCapacities[wordIndex] = oldPagesCapacities[wordIndex];
    wordIndex++;

    if (wordIndex < wordsNum) {
        goto COPY_INFREQUENT_TO_NEW;
    }

    cout << wordsArray[0];
    for (int i = 0; i < pagesNums[0]; ++i) {
        cout << wordsPages[0][i] << ' ';
    }
    //TODO: Delete old stats
    cout << "Total words: " << wordsNum << '\n' << "Total pages: " << currentPageNum << '\n' << "Total lines: "
         << currentLineNum;
    file.close();
}

