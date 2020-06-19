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
const string F_NAME_CLASSICAL_MECHANICS = "classical_mechanics.txt";
const string F_NAME_EM = "EM.txt";
const string F_NAME_WAVE_OPTICS = "wave_optics.txt";
const string LOG_NAME = "PS_log.log";
const int MAX_N_CHOICE = 5;

void write_log(int &n_correct, int &n_solved, string &subject){
    ofstream log_file;
    log_file.open(DIR+LOG_NAME,ios::app);
    if (log_file){
    
        time_t raw_time;
        time (&raw_time);
        log_file << ctime(&raw_time) << endl;
        log_file << "Subject: " << subject << endl;
        log_file << "Score: " << 100* static_cast<float>(n_correct) / n_solved << "(" << n_correct << "/" << n_solved << ")" << endl;
        log_file << setfill('-') << setw(20) << '-' << endl;
        
        log_file.close();
    }
    else{
        cout << "Log file opening failed" << endl;
    }
    return;
}

bool read_PS(string &subject, vector<string> &problems, vector<string> &choices, vector<string> &answers, int &n_total){
    bool _open = false;
    string f_name, problem_type;
    int subject_no, type_no;
    bool valid_input = false;
    bool memorization_only = false;
    bool problem_solving_only = false;
    
    cout << setfill('-') << setw(10) << "-";
    cout << "Choose subject";
    cout << setfill('-') << setw(8) << "-" << endl;
    cout << "1. Classical Mechanics" << endl;
    cout << "2. E&M" << endl;
    cout << "3. Wave and Optics" << endl;
    
    do{
        getline(cin, subject);
        stringstream(subject) >> subject_no;
        
        if ((subject_no > 3) || (subject_no < 1))
            cout << "Choose correct choice" << endl;
        else
            valid_input = true;
    }while(!valid_input);
    
    switch(subject_no){
        case 1:
            subject = "Classical mechanics";
            f_name = F_NAME_CLASSICAL_MECHANICS;
            break;
        case 2:
            subject = "E&M";
            f_name = F_NAME_EM;
            cout << "Note: Electric dipoles, matter effects, EM waves have not been included yet. It will be included in future" << endl;
            break;
        case 3:
            subject = "Wave & Optics";
            f_name = F_NAME_WAVE_OPTICS;
            break;
    }
    valid_input = false;
    
    cout << setfill('-') << setw(10) << "-";
    cout << "Select type";
    cout << setfill('-') << setw(11) << "-" << endl;
    cout << "1. Memorization only" << endl;
    cout << "2. Problem solving only" << endl;
    cout << "3. Both" << endl;
    cout << "Unimplemented yet!" << endl;
    
    do{
        getline(cin,problem_type);
        stringstream(problem_type) >> type_no;
        if ((type_no > 3) || (type_no < 1))
            cout << "Choose correct choice" << endl;
        else
            valid_input = true;
    
    }while(!valid_input);
    
    switch(type_no){
        case 1:
            memorization_only = true;
            problem_solving_only = false;
            break;
        case 2:
            memorization_only = false;
            problem_solving_only = true;
            break;
        case 3:
            memorization_only = true;
            problem_solving_only = true;
            break;
    }
    
    
    
    cout << setfill('-') << setw(10) << "-";
    cout << "Opening file";
    cout << setfill('-') << setw(10) << "-" << endl;
    cout << "File directory: " << DIR << f_name << endl;
    
    ifstream PS;
    PS.open(DIR+f_name, ios::in);
    
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
                    
                    if(write){
                        total_choice = "None";
                    }
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
    string cont_str, subject;
    char cont_char;
    int n_correct(0), n_total(0), n_solved_total(0);
    vector <string> problems, choices, answers;

    //Open files
    open = read_PS(subject,problems, choices, answers, n_total);
    
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
            cout << (n_solved_total + 1) << "/" << n_total << ": " << problems.at(problem_no.at(problem_idx)) << endl;
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
                bool invalid = true;
                do{
                    string dummy;
                    getline(cin,dummy);
                    
                    if (dummy == "Y" || dummy == "y"){
                        correct = true;
                        invalid = false;
                    }
                    else if (dummy == "N" || dummy == "n"){
                        correct = false;
                        invalid = false;
                    }
                    else{
                        cout << "Type either Y or N" << endl;
                        invalid = true;
                    }
                }while(invalid);
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
        
        cout << setfill('-') << setw(10) << '-';
        cout << "Statistics";
        cout << setfill('-') << setw(11) << '-' << endl;
        cout << "Correct/Solved: " << n_correct << "/" << n_solved_total << endl;
        cout << fixed << showpoint << setprecision(2) << 100* static_cast<float>(n_correct)/n_solved_total << "%" << endl;
        
        
        string write_log_str;
        char write_log_char;
    
        
        cout << setfill('-') << setw(10) << '-';
        cout << "Writing log";
        cout << setfill('-') << setw(10) << '-' << endl;
        cout << "Write log? [Y/N]" << endl;
        getline(cin, write_log_str);
        stringstream(write_log_str) >> write_log_char;
        if (write_log_char == 'Y' || write_log_char == 'y')
            write_log(n_correct,n_solved_total, subject);
    }
    
    return 0;
}
