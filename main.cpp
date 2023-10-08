#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

struct Automaton {
    int size{};
    int startState{};
    std::vector<int> finalStates;
    std::unordered_map<int, std::unordered_map<char, int>> transitions;
};

Automaton readAutomatonFromFile(const std::string &filename) {
    Automaton automaton;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Failed to open file." << std::endl;
        exit(1);
    }

    file >> automaton.size;
    file >> automaton.startState;

    int numOfFinalStates;
    file >> numOfFinalStates;
    automaton.finalStates.resize(numOfFinalStates);
    for (int i = 0; i < numOfFinalStates; ++i) {
        file >> automaton.finalStates[i];
    }

    int from, to;
    char transitionChar;
    while (file >> from >> transitionChar >> to) {
        automaton.transitions[from][transitionChar] = to;
    }

    file.close();
    return automaton;
}

bool isWordAccepted(const Automaton &automaton, const std::string &word) {
    int currentState = automaton.startState;
    for (char ch : word) {
        if (automaton.transitions.at(currentState).find(ch) == automaton.transitions.at(currentState).end()) {
            return false;
        }
        currentState = automaton.transitions.at(currentState).at(ch);
    }
    for(int state : automaton.finalStates) {
        if (currentState == state) return true;
    }
    return false;
}

bool checkForWords(const Automaton &automaton, const std::string &w0) {
    for (int len = 1; len <= 10; ++len) {
        for (int i = 0; i < (1 << len); ++i) {
            std::string w1;
            for (int j = 0; j < len; ++j) {
                if (i & (1 << j)) w1 += 'b';
                else w1 += 'a';
            }

            for (int k = 0; k < (1 << len); ++k) {
                std::string w2;
                for (int j = 0; j < len; ++j) {
                    if (k & (1 << j)) w2 += 'b';
                    else w2 += 'a';
                }

                if (isWordAccepted(automaton, w1 + w0 + w2)) return true;
            }
        }
    }
    return false;
}



int main() {
    Automaton automaton = readAutomatonFromFile("D:\\sysProg2\\automation.txt");
    std::string w0;

    std::cout << "Enter w0: ";
    std::cin >> w0;

    if (checkForWords(automaton, w0)) {
        std::cout << "The automaton admits words of the form w = w1 w0 w2." << std::endl;
    } else {
        std::cout << "The automaton does not admit words of the form w = w1 w0 w2." << std::endl;
    }

    return 0;
}
