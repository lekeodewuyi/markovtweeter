
#include <iostream>
#include <cstdio>
#include <map>
#include <deque>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <unistd.h>

using namespace std;

class Prefix {
    public:
        string prefix;
        vector<string> prefixState;
        
        Prefix(string word, string suffix) {
            prefix = word;
            prefixState.push_back(suffix);
        }
};


vector<Prefix> MarkovState;
string tweet;

int markovIndex(string prefix){
    for(int i = 0; i < MarkovState.size(); i++){
        if(MarkovState.at(i).prefix.compare(prefix) == 0){
            return i;
        }
    }
    return -1;
}

void newEntry(string prefix, string suffix){
    int index = markovIndex(prefix);
    if(index == -1){
        MarkovState.push_back(Prefix(prefix, suffix));
    }
    else{
        MarkovState.at(index).prefixState.push_back(suffix);
    }
}

void parseLine(string line){
    size_t prefixStart = 0;
    size_t prefixEnd;
    size_t suffixStart;
    size_t suffixEnd;
    size_t prefixLength;
    size_t suffixLength;
    string prefix;
    string suffix;
    
    while(prefixStart != string::npos){
        prefixEnd = line.find(" ", prefixStart);
        prefixLength = (prefixEnd - prefixStart);
        
        while(prefixLength == 0){
            prefixStart++;
            prefixEnd = line.find(" ", prefixStart);
            prefixLength = (prefixEnd - prefixStart);
            
            if(prefixStart >= line.length()){
                prefixStart = line.length();
                break;
            }
        }
        
        prefix = line.substr(prefixStart, prefixLength);
        
        if(prefixEnd != string::npos){
            suffixStart = prefixEnd + 1;
            suffixEnd = line.find(" ", suffixStart);
            suffixLength = suffixEnd - suffixStart;
            
            while(suffixLength == 0){
                suffixStart++;
                suffixEnd = line.find(" ", suffixStart);
                suffixLength = suffixEnd - suffixStart;
                if(suffixStart >= line.length()){
                    suffixStart = line.length();
                    break;
                }
            }
            
            prefix = line.substr(prefixStart, prefixLength);
            suffix = line.substr(suffixStart, suffixLength);
            
            prefixStart = suffixStart;
        }
        else {
            suffix = "";
            prefixStart = string::npos;
        }
        
        newEntry(prefix, suffix);
    }
}

int parseInputFile(string filename){
    string line;
    
    ifstream inputFile(filename);
    if (inputFile.is_open()){
        while(getline (inputFile,line)){
            parseLine(line);
        }
        inputFile.close();
        return 1;
    }
    else{
        return 0;
    }
}


int selectPrefixIndex(int options){
    return rand() % options;
}

void printTweet(){
    int prefixIndex = selectPrefixIndex(MarkovState.size());
    int suffixIndex = 0;
    int suffixNumber = 0;
    
    bool tweetFinished = false;
    string tweetEnd;
    
    tweet = "";
    tweet = MarkovState.at(prefixIndex).prefix;
    tweetEnd = tweet;
    
    while(!tweetFinished){
        prefixIndex = markovIndex(tweetEnd);
        Prefix temp = MarkovState.at(prefixIndex);
        suffixNumber = temp.prefixState.size();
        suffixIndex = selectPrefixIndex(suffixNumber);
        tweetEnd = temp.prefixState.at(suffixIndex);
        if(tweetEnd.length() == 0){
            tweetFinished = true;
        }
        else{
            if(tweet.length() + tweetEnd.length() > 279){
                tweetFinished = true;
            }
            else{
                tweet = tweet + " " + tweetEnd;
            }
        }
    }
    cout << tweet << endl;
    cout << "- this tweet has " << tweet.length() << " characters" << endl;
}


void generateTweets(){
    int numberOfTweets;
    cout << "\nHow many tweets will you like to generate? (input a number between 1 and any value really)\n-you can generate 1000000 tweets just for kicks if you feel your computer has the computing power to do so.\n" << endl;
    cin >> numberOfTweets;
    while((cin.fail()) || (numberOfTweets == 0)) {
        cout << "Error: Please enter a valid [non-zero] number." << endl;
        cin.clear();
        cin.ignore(256,'\n');
        cin >> numberOfTweets;
    }
    cout << "\nNow generating " << numberOfTweets;
    if (numberOfTweets == 1) {
        cout << " tweet...\n" << endl;
    } else {
        cout <<" tweets...\n" << endl;
    }
    for(int i = 0; i < numberOfTweets; i++){
        printTweet();
        cout << endl;
    }
}

int main(){

    string textFile;

    cout << "\nHelp:\n1. This program generates tweets by using words from a source text file." << endl;
    cout << "2. Make sure your source text file is in the same directory as the executable." << endl;
    cout << "3. To run the program, input the name of your text file e.g. 'usconst.txt'- without the quotes.\n" << endl;
    cout << "p.s. this program is actually able to parse just about any input file, so you could try parsing non-text files just for kicks and see the weird tweets you're able to generate.\n" << endl;
    cout << "Type the name of your source text file below and press enter (e.g. usconst.txt):\n" << endl;
    cin >> textFile;

    cout << "\nParsing " << textFile << " ...\n" << endl;
    cout << "Note: If your file is VERYYY large, this might take a while," << endl;
    cout << "don't panic, you'll be notified once parsing is complete.\n" << endl;

    if(parseInputFile(textFile)){
        cout << "Parsing complete, markov states created." << endl;
        generateTweets();
        string decision;

        while (true) {
            cout << "Will you like to generate more tweets? (y/n)\n" << endl;
            cin >> decision;
            if (((decision == "y") || (decision == "Y")) || ((decision == "n") || (decision == "N"))) {
                if ((decision == "y") || (decision == "Y")) {
                    generateTweets();
                } else {
                    return 0;
                }
            } else {
                cout << "\nPlease input y to generate more tweets or input n to exit the program." << endl;
            }
        }
        
    } else {
        cout <<  textFile << " cannot be located. Please ensure it's in the same folder as the executable." << endl;
    }
    return 0;

}
