//
//  main.cpp
//  PGRE_problems
//
//  Created by James Kwon on 6/15/20.
//  Copyright © 2020 James Kwon. All rights reserved.
//
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <numeric>
#include <time.h>
#include <random>
#include <algorithm>
using namespace std;

const string DIR = "/Users/jameskwon/Documents/GRE/";
const string F_NAME = "PS.txt";
const string LOG_NAME = "PS_log.log";
const int MAX_N_CHOICE = 5;

void write_log(int &n_correct, int &n_solved){
    ofstream log_file;
    log_file.open(DIR+LOG_NAME,ios::app);
    if (log_file){
    
        time_t raw_time;
        time (&raw_time);
        log_file << ctime(&raw_time) << endl;
        log_file << "Score: " << 100* static_cast<float>(n_correct) / n_solved << "(" << n_correct << "/" << n_solved << ")" << endl;
        log_file << setfill('-') << setw(20) << '-' << endl;
        
        log_file.close();
    }
    else{
        cout << "Log file opening failed" << endl;
    }
    return;
}

bool read_PS(const string &dir, const string &f_name, vector<string> &problems, vector<string> &choices, vector<string> &answers, int &n_total){
    bool _open = false;
    cout << setfill('-') << setw(10) << "-";
    cout << "Opening file";
    cout << setfill('-') << setw(10) << "-" << endl;
    cout << "File directory: " << DIR << F_NAME << endl;
    
    ifstream PS;
    PS.open(DIR+F_NAME, ios::in);
    
    if(PS){
        string _line;
        
        while(getline(PS, _line)){
            int i = static_cast<int>(_line.find_first_of(':'));
            if(i != string::npos){
                string type = _line.substr(0,i);
                string content = _line.substr(i+2);
                
                if (type == "Question" || type == "question"){
                    problems.push_back(content);
                    n_total++;
                }
                
                else if (type == "Choices" || type == "choices" || type == "choices" || type == "choice"){
                    string choice_idx_str = "A.";
                    string total_choice = "";
                    string each_choice;
                    bool write = false;
                    for (int counter = 0; counter < MAX_N_CHOICE; counter++){
                        size_t pos = content.find(choice_idx_str);
                        if(pos == string::npos){
                            write = true;
                            break;
                        }
                        choice_idx_str[0]++;
                        size_t pos2 = content.find(choice_idx_str);
                        pos2 = (pos2 == string::npos? content.size() : pos2);
                        each_choice = content.substr(pos,pos2-pos);
                        total_choice += each_choice+"\n";
                    }
                    
                    if(write)
                        total_choice = "None";
                    else
                        total_choice.erase(total_choice.find_last_of("\n"));
                    choices.push_back(total_choice);
                }
                else if (type == "Answer") {
                    answers.push_back(content);
                }
            }
        }
        
        PS.close();
        _open = true;
        cout << "File loaded" << endl;
    }
    else
        cout << "File opening failed. Check the directory" << endl;
    
    return _open;
}

int main(){
    //Some running variables
    bool correct,cont, open,valid(false);
    string cont_str;
    char cont_char;
    int n_correct(0), n_total(0), n_solved_total(0);
    vector <string> problems, choices, answers;

    //Open files
    open = read_PS(DIR, F_NAME,problems, choices, answers, n_total);
    
    if(open){
        unsigned short int problem_idx = 0;
        cout << setfill('-') << setw(10) << '-';
        cout << "Shuffling PS";
        cout << setfill('-') << setw(10) << '-' << endl;
        unsigned int size = problems.size();
        vector<int> problem_no(size);
        iota(begin(problem_no),end(problem_no),0);
        
        shuffle(begin(problem_no), end(problem_no),default_random_engine());
        
        do{
            cout << "Question: " << problems.at(problem_no.at(problem_idx)) << endl;
            string choice_str = choices.at(problem_no.at(problem_idx));
            string user_answer;
        
            if (choice_str != "None"){
                cout << choice_str << endl;
                cout << "Choose your answer: ";
                getline(cin, user_answer);
                char true_answer, user_answer_char;
                stringstream(answers.at(problem_no.at(problem_idx))) >> true_answer;
                stringstream(user_answer) >> user_answer_char;
                true_answer = toupper(true_answer);
                user_answer_char = toupper(user_answer_char);
                correct = (user_answer_char == true_answer);
                cout << boolalpha << correct << endl;
            }
            else{
                cout << "Write your answer: ";
                getline(cin, user_answer);
                cout << "Answer: " << answers.at(problem_no.at(problem_idx)) << endl;
                cout << "Correct? [Y/N]" << endl;
                
                string dummy;
                getline(cin,dummy);
                if (dummy == "Y" || dummy == "y"){
                    correct = true;
                }
                else
                    correct = false;
            }
            problem_idx++;
            
            
            
            n_correct += correct;
            n_solved_total++;

            //Options to continue
            while(!valid){
                cout << "Continue? [Y/N]" << endl;
                getline(cin, cont_str);
                stringstream(cont_str) >> cont_char;

                switch(toupper(cont_char)){
                    case 'Y':
                        cont = true;
                        valid = true;
                        break;
                    case 'N':
                        cont = false;
                        valid = true;
                        break;
                    default:
                        cout << "Invalid input. Type again [Y/N]" << endl;
                        valid = false;
                }
            }
            valid = false;
        }while(cont && (n_solved_total < n_total));
    }
    
    cout << setfill('-') << setw(11) << '-';
    cout << "Statistics";
    cout << setfill('-') << setw(10) << '-' << endl;
    cout << "Correct/Solved: " << n_correct << "/" << n_solved_total << endl;
    cout << fixed << showpoint << setprecision(2) << 100* static_cast<float>(n_correct)/n_solved_total << "%" << endl;
    
    write_log(n_correct,n_solved_total);
    
    return 0;
}
