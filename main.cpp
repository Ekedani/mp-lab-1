#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const string FILE_ADDRESS = "D:\\Programming\\mp-lab-1\\data.txt";
const int NUM_OF_DISPLAYED_WORDS = 25;

const int NUM_OF_STOP_WORDS = 3;
const char* STOP_WORDS[] = { "for", "in", "the"};

int main() {
    ifstream file;
    file.open(FILE_ADDRESS);

    int wordsArraySize = 10;
    int wordsNum = 0;
    string* wordsArray = new string[wordsArraySize];
    int* wordsFrequency = new int[wordsArraySize];

    int wordStartIdx = 0;
    int wordEndIdx = 0;

    string line;
    int lineSize;

    PROCESS_FILE:
        getline(file, line);
        lineSize = 0;
        wordStartIdx = 0;

        CALCULATE_LINE_LENGTH:
        if(line[lineSize] != '\0'){
            lineSize++;
            goto CALCULATE_LINE_LENGTH;
        }

        PROCESS_LINE:
            // I really just wanna die
            FIND_WORD_START:
                if (!((line[wordStartIdx] >= '0' && line[wordStartIdx] <= '9') ||
                      ((line[wordStartIdx] >= 'A' && line[wordStartIdx] <= 'Z')) ||
                      ((line[wordStartIdx] >= 'a' && line[wordStartIdx] <= 'z')))) {
                    wordStartIdx++;
                    goto FIND_WORD_START;
                }
                wordEndIdx = wordStartIdx;
            FIND_WORD_END:
                if(wordEndIdx < lineSize && (((line[wordEndIdx] >= '0' && line[wordEndIdx] <= '9') ||
                                              ((line[wordEndIdx] >= 'A' && line[wordEndIdx] <= 'Z')) ||
                                              ((line[wordEndIdx] >= 'a' && line[wordEndIdx] <= 'z'))) ||
                                              line[wordEndIdx] == '-')){
                    wordEndIdx++;
                    goto FIND_WORD_END;
                }
            PROCESS_WORD:
                int wordSize = wordEndIdx - wordStartIdx;
                // Ignoring noise words (size less than 1)
                if(wordSize > 1){
                    string test;
                    COPY_SUBSTR:
                    if(wordStartIdx < wordEndIdx){
                        if(line[wordStartIdx] >= 65 && line[wordStartIdx] <= 90){
                            test += line[wordStartIdx] + 32;
                        }
                        else{
                            test += line[wordStartIdx];
                        }
                        wordStartIdx++;
                        goto COPY_SUBSTR;
                    }
                    int comparedWordIdx = 0;
                    CHECK_EXISTING_WORDS:
                    bool stringsEqual = true;
                    int comparedCharIdx = 0;
                    CHECK_IF_EQUAL:
                    stringsEqual &= (test[comparedCharIdx] == wordsArray[comparedWordIdx][comparedCharIdx]);
                    if(stringsEqual && comparedCharIdx < wordSize){
                        comparedCharIdx++;
                        goto CHECK_IF_EQUAL;
                    }
                    if(!stringsEqual && comparedWordIdx < wordsNum){
                        comparedWordIdx++;
                        goto CHECK_EXISTING_WORDS;
                    }
                    if(stringsEqual){
                        wordsFrequency[comparedWordIdx]++;
                    }
                    else{
                        wordsArray[wordsNum] = test;
                        wordsFrequency[wordsNum] = 1;
                        wordsNum++;
                    }
                    if(wordsNum >= 0.8 * wordsArraySize){
                        string* oldWords = wordsArray;
                        int* oldFrequency = wordsFrequency;
                        wordsArraySize *= 2;
                        wordsArray = new string[wordsArraySize];
                        wordsFrequency = new int[wordsArraySize];
                        int wordIndex = 0;
                        COPY_TO_NEW:
                        wordsArray[wordIndex] = oldWords[wordIndex];
                        wordsFrequency[wordIndex] = oldFrequency[wordIndex];
                        wordIndex++;
                        if(wordIndex < wordsNum){
                            goto COPY_TO_NEW;
                        }
                        delete[] oldWords;
                        delete [] oldFrequency;
                    }
                }
                else{
                    wordStartIdx = wordEndIdx;
                }

                if(wordEndIdx < lineSize){
                    goto PROCESS_LINE;
                }

            if(!file.eof()){
                goto PROCESS_FILE;
            }

    SORT_BY_FREQUENCY:
    bool swapped = false;
        int j = 0;
        SORT_INNER:
        if(wordsFrequency[j] < wordsFrequency[j + 1]){
            int freqTmp = wordsFrequency[j];
            string wordTmp = wordsArray[j];
            wordsFrequency[j] = wordsFrequency[j + 1];
            wordsArray[j] = wordsArray[j + 1];
            wordsFrequency[j + 1] = freqTmp;
            wordsArray[j + 1] = wordTmp;
            swapped = true;
        }
        j++;
        if(j < wordsNum - 1){
            goto SORT_INNER;
        }
        if(swapped){
            goto SORT_BY_FREQUENCY;
        }
    for (int i = 0; i < wordsNum; ++i) {
        cout << wordsArray[i] << " : " << wordsFrequency[i] << " #" << i << '\n';

    }
    cout << wordsNum;
    file.close();
    return 0;
}
