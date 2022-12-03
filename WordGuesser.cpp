#include <iostream>
#include <ctime>
#include <string>
#include <vector>

#define MAX_ACTIONS 10

using namespace std;

class GuessWord;

class Action {
    public:
        virtual void run(GuessWord* guess_word) {
        };
        static int countActions;
        static int countPowerups;
};
int Action::countActions = 0;
int Action::countPowerups = 2; // define how many power-ups to give there

class GuessWord : public Action {
        std::string word;
        std::string hidden_word;
        std::string unrevealed;
    public:
        GuessWord(const std::string& word);
        void guess_char(char chr);
        bool have_won();
        void reveal_letter(int index);
        void reveal_random();
        void set_word(const std::string& word);
        void show_hidden_word();
        bool over_limit();
};

class GuessLetter : public Action {
    private:
        char letter;
    public:
        GuessLetter() : Action() {
        }
        GuessLetter(char let) {
            letter = let;
        }
        ~GuessLetter() {
        }
        void setLetter(char let) {
            letter = let;
        }
        char getLetter() {
            return letter;
        }
        virtual void run(GuessWord* guess_word) {
            guess_word->guess_char(letter);
        };
};

class RevealLetter : public Action {
    private:
        int index;
    public:
        RevealLetter() : Action() {
        }
        RevealLetter(int ind) {
            index = ind;
        }
        ~RevealLetter() {
        }
        void setIndex(int ind) {
            index = ind;
        }
        int getIndex() {
            return index;
        }
        virtual void run(GuessWord* guess_word) {
            guess_word->reveal_letter(index);
        };
};

GuessWord::GuessWord(const std::string& word) {
    set_word(word);
}
void GuessWord::guess_char(char chr) {
    for (int i = 0; i < word.size(); ++i) {
        if (word[i] == chr && hidden_word[i] == '*') {
            reveal_letter(i);
        }
    }
}
bool GuessWord::have_won() {
    return word == hidden_word;
}
void GuessWord::reveal_letter(int index) {
    if (index >= 0 && index < word.size()) {
        hidden_word[index] = word[index];
    }
}
int getRandomNumber(int length) {
    srand(time(NULL));
    return rand() % length;
}

void GuessWord::reveal_random() {
    vector<int> unrevealedIndexes;
    for (int i = 0; i < hidden_word.length(); ++i) {
        if (hidden_word[i] == '*') {
            unrevealedIndexes.push_back(i);
        }
    }
    int randomIndex = getRandomNumber(int(unrevealedIndexes.size()));
    int unrevealedIndex = unrevealedIndexes[randomIndex];
    reveal_letter(unrevealedIndex);
}
void GuessWord::set_word(const string& word) {
    this->word = word;
    this->hidden_word.clear();
    for (int i = 0; i < word.size(); ++i) {
        hidden_word.push_back('*');
    }
}
void GuessWord::show_hidden_word() {
    cout << "WORD: " << hidden_word << endl;
}
bool GuessWord::over_limit() {
    if (countActions == 11) {
        return true;
    } else return false;
}
bool is_number(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main(int argc, char** agrv) {
    cout << "-------------------------------------------------------\n";
    cout << "\tWelcome to Word Guesser game!\n";
    cout << "-------------------------------------------------------\n";
    cout << "\nGame rules:";
    cout << "\nTo guess, enter a letter (both uppercase and lowercase are accepted)\n";
    cout << "To reveal a letter, enter an index of position (1st letter's index is 0)";
    cout << "To reveal a random letter, enter symbol '?'";
    cout << "\nIf you use more than 10 commands, you lose";
    cout << "You have 2 helps (reveal a position and reveal a random letter)\n\n";

    GuessWord *gw = new GuessWord("HELLO"); // set the word there

    string input;
    do {
        cout << "Commands left: " << MAX_ACTIONS - Action::countActions << "\t Power-ups left: " << Action::countPowerups << endl;
        gw->show_hidden_word();
        cout << "> ";
        cin >> input;
        if (input.compare("?") == 0) { // compare returns 0 if both the strings are the same
            if (Action::countPowerups > 0) {
                gw->reveal_random();
                Action::countPowerups--;
                Action::countActions++;
            } else {
                cout << "\nLimit of helps is used!\n" << endl;
            }
        } else if (is_number(input)) {
            if (Action::countPowerups > 0) {
                RevealLetter b1;
                b1.setIndex(atoi( input.c_str()));
                b1.run(gw);
                gw->reveal_letter(b1.getIndex());
                Action::countPowerups--;
                Action::countActions++;
            } else {
                cout << "\nLimit of helps is used!\n" << endl;
            }
        } else {
            GuessLetter a1;
            if (input.length() <= 1) {
                a1.setLetter(toupper(input[0]));
                a1.run(gw);
                gw->guess_char(a1.getLetter());
                Action::countActions++;
            } else {
                cout << "\nGuess only one letter a time!\n" << endl;
            }
        }
        if (gw->have_won() == 1) {
            cout << "You have won!" << endl;
            break;
        }
        if (Action::countActions == MAX_ACTIONS) {
            cout << "\n" << MAX_ACTIONS << " commands (the limit) used. You lose." << endl;
            break;
        }
    } while (1);

    delete gw;
    return 0;
}
