#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "MarkovChain.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

#define CONTEXT_LENGTH 2



int main(int argc, char* argv[]) {
    if(argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input.txt> <output.txt> <length> [line_length]" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    int textLength = std::stoi(argv[3]);
    int lineLength = 80;

    if(argc > 4) {
        lineLength = std::stoi(argv[4]);
    }

    MarkovChain chain;
    initMarkovChain(&chain, inputFile);

    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Konnte Ausgabedatei nicht öffnen!" << std::endl;
    } else {
        int length = 0;
        for(int i = 0; i < textLength; ++i){
            std::string next = chain.getNextWord();
            length += next.length();
            if(length > lineLength) {
                outFile << "\n";
                length = next.length();
            }
            outFile << next;
        }
        outFile << std::endl;
        outFile.close();
    }

    return 0;
}

void initMarkovChain(MarkovChain* chain, std::string fileName) {
    std::ifstream file(fileName);
    if(!file){
        std::cerr << "Datei konnte nicht geöffnet werden!" << std::endl;
        return;
    }

    std::vector<Element> elements;

    std::string word;
    std::string current;
    int length = 0;
    while(file >> word){
        if(length++ < CONTEXT_LENGTH) {
            current = current + (length == 1 ? "" : " ") + word;
            continue;
        }

        auto it = std::find_if(elements.begin(), elements.end(), 
        [&current](const Element& elem){
            return elem.words == current;
        });

        Element* e = nullptr;

        if(it != elements.end()) {
            e = &(*it);
        } else {
            elements.push_back(Element{current, std::vector<Word>{}});
            e = &elements.back();
        }

        auto it2 = std::find_if(e->successors.begin(), e->successors.end(), 
    [&word](const Word& elem){
            return elem.word == word;
        });

        if(it2 != e->successors.end()) {
            it2->p += 1;
        } else {
            Word w{word, 1};
            e->successors.push_back(w);
        }
        current = removeFirstWord(current) + " " + word;
    }
    file.close();

    for(auto& element : elements) {
        double sum = 0;
        for(auto w : element.successors) {
            sum += w.p;
        }

        double prev = 0;
        for(auto& w : element.successors) {

            w.p = w.p/sum + prev;
            prev = w.p;
        }
    }


    chain->elements = elements;

    int randomIndex = floor(dis(gen) * elements.size());
    chain->currentChain = elements.at(randomIndex).words;
}

std::string MarkovChain::getNextWord(){
    std::string current = this->currentChain;
    double p = dis(gen);
    auto it = std::find_if(elements.begin(), elements.end(), 
        [&current](const Element& elem){
            return elem.words == current;
        });

    if(it != elements.end()){
        for(const auto& w : it->successors) {
            if(w.p > p) {
                currentChain = removeFirstWord(currentChain) + " " + w.word;
                return w.word + " ";
            }
        }
    }
    int randomIndex = floor(dis(gen) * elements.size());
    this->currentChain = elements.at(randomIndex).words;
    return currentChain + " ";
}

 std::string removeFirstWord(const std::string& str) {
    std::istringstream iss(str);
    std::string firstWord;
    iss >> firstWord;
    std::string rest;
    std::getline(iss, rest);
    if (!rest.empty() && rest[0] == ' ') rest.erase(0, 1);
    return rest;
}