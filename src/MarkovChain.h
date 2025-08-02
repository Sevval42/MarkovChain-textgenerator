#include <vector>
#include <string>
#include <sstream>

struct Word {
    std::string word;
    double p;
};

struct Element {
    std::string words;
    std::vector<Word> successors;
};

struct MarkovChain {
    std::string currentChain;
    std::vector<Element> elements;
    std::string getNextWord();
};

void initMarkovChain(MarkovChain*, std::string);

std::string removeFirstWord(const std::string& str);