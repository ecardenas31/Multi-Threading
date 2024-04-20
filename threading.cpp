#include <iostream>

#include <vector>

#include <string>

#include <sstream>

#include <pthread.h>

#include <atomic>

#include <ctype.h>



using namespace std;



vector<string> inputString;

atomic<int> nextWord(0); //calling it as atomic will make each thread have to take turns to access it. they cannot access at same time



void* checkString(void* arg);



int main(int argc, char* argv[]) {

    string x;

    if (argc > 1) {

        x = argv[1];

    } else {

        cout << "Enter string: ";

        getline(cin, x);

    }



    istringstream iss(x);

    string word;

    while (iss >> word) {

        words.push_back(word);

    }



    pthread_t alphaThread, numericThread; //thread IDs



    pthread_create(&alphaThread, NULL, checkString, (void*)"alpha");

    

    pthread_create(&numericThread, NULL, checkString, (void*)"numeric");

    

    // Wait for threads to complete

    pthread_join(alphaThread, NULL);

    pthread_join(numericThread, NULL);



    return 0;

}



void* checkString(void* arg) {

    char* type = (char*) arg;

    

    while (true) {

        int index = nextWord.load();  //prevents race condition. it is a shared index but since it is atomic the other thread cannot access it untill the other is done with it

        

        if (index >= inputString.size()) {

            break;

        }



        const string& word = inputString[index];  // Get a reference to the current word



        // Determine whether to process the word based on the thread's type

        if (type[0] == 'a') {  // For the thread handling alphabetic words

            if (isalpha(word[0])) {  // Check if the word starts with an alphabet letter

                cout << "alpha: " << word << endl;

                nextWord++;  // Increment index after processing

            }

        } else if (type[0] == 'n') {  // For the thread handling numeric words

            if (isdigit(word[0]) || word.find(':') != string::npos) {  // Check if the word starts with a digit or contains a colon

                cout << "numeric: " << word << endl;

                nextWord++;  // Increment index after processing

            }

        }

    }

    return NULL;

}