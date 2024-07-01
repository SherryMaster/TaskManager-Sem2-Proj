#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
using namespace std;


int lenCh(char str[]){
    int i = 0;
    for(i = 0; str[i] != '\0'; i++);
    return i;
}

bool compareStr(char* arr1, char* arr2){
    if(lenCh(arr1) != lenCh(arr2)) return false;
    for(int i = 0; arr1[i] != '\0'; i++){
        if(arr1[i] != arr2[i]){
            return false;
        }
    }
    return true;
}

char* concat(char* arr1, char* arr2){
    int len1 = lenCh(arr1);
    int len2 = lenCh(arr2);

    char* result = new char[len1 + len2 + 1];

    for (int i = 0; i < len1; i++){
        result[i] = arr1[i];
    }
    for (int i = 0; i < len2; i++){
        result[len1 + i] = arr2[i];
    }

    result[len1 + len2] = '\0';

    // if(recursive_mode) delete[] arr2;
    return result;

}

bool exists_in_file(char str[], char file[]){
    ifstream fin;
    fin.open(file);
    char line[100];
    while(fin.getline(line, 100)){
        if(compareStr(str, line)){
            return true;
        }
    }
    return false;
}

bool is_super_user(char* user, char* root_user_path, char* password = "",bool check_pass = false){
    ifstream fin;
    char root_user[20] = { "\0" };
    char root_pass[20] = { "\0" };
    fin.open(root_user_path);
    fin >> root_user;
    if(check_pass){
        fin >> root_pass;
        if (compareStr(user, root_user) && compareStr(password, root_pass)){
            return true;
        }
    }
    else{
        if (compareStr(user, root_user)){
            return true;
        }
    }
    
    return false;
}

char** regrow2DchArr(char** arr, int size, char* str){
    char** newarr = new char* [size + 1];
    for(int i = 0; i < size; i++){
        int strlen = lenCh(arr[i]);
        newarr[i] = new char[strlen + 1];
        newarr[i] = arr[i];
        newarr[i][strlen] = '\0';
    }

    int strlen = lenCh(str);
    newarr[size] = new char[strlen + 1];
    newarr[size] = str;
    newarr[size][strlen] = '\0';
    for (int i = 0; i <= size; i++){
        cout << "i = " << i << " " << newarr[i] << endl;
    }
    delete[] arr;
    return newarr;

}

void Register(char* username, char* password, char* ADMIN_USER_FILE, char* PROFILES, const int USER_PASS_SIZE, bool* login, char* USERNAMES, int* total_users,bool root_mode = false){
    bool wrong_pass = false;
    bool already_exists = false;

    // getting username and password
    while(true){
        system("cls");
        cout << "--REGISTRATION--" << endl;
        if(wrong_pass){
            cout << endl << "Passwords don't match, Try Again..." << endl;
        }
        if(already_exists){
            cout << endl << "Username already exists, Choose another..." << endl;
        }
        cout << "Enter username (without spaces MAX " << USER_PASS_SIZE <<" characters): ";
        cin >> username;

        if(exists_in_file(username, USERNAMES)){
            already_exists = true;
            continue;
        }
        cout << "Enter password (without spaces MAX " << USER_PASS_SIZE <<" characters): ";
        cin >> password;

        char* confPass = new char[USER_PASS_SIZE];
        cout << "Confirm password: ";
        cin >> confPass;
        if (!compareStr(password, confPass)){
            cout << "Passwords don't match, Try Again..." << endl;
            wrong_pass = true;
            delete[] confPass;
        }
        else{
            delete[] confPass;
            break;
        }
    }

    // writing username and password to file
    char* path = new char [100];
    path = {"\0"};

    if(root_mode){
        path = ADMIN_USER_FILE;
    }
    else{
        path = concat(PROFILES, concat("users/",concat(username, ".txt")));
    }

    ofstream fout;
    fout.open(path);
    fout << username << endl;
    fout << password;
    fout.close();

    // making the task folder
    char* task_folder = concat("data\\tasks\\", username);
    char* task_folder_command = concat("mkdir ", concat(task_folder, "\\grouped"));

    system(task_folder_command);

    ofstream fout1;
    fout1.open(concat(task_folder, "\\tasks.txt"));
    fout1 << "0";
    fout1.close();

    // making used tags file for keeping track of tags used.
    fout1.open(concat(task_folder, "\\used_tags.txt"));
    fout1 << "0";
    fout1.close();

    delete[] task_folder;
    delete[] task_folder_command;

    // updating total users
    ifstream fin;
    fin.open(USERNAMES);
    fin >> *total_users;
    char** user_names = new char* [*total_users];
    for(int i = 0; i < *total_users; i++){
        user_names[i] = new char[20];
        fin >> user_names[i];
    }
    (*total_users)++;
    fin.close();

    ofstream fout2;
    fout2.open(USERNAMES);
    fout2 << *total_users;
    for(int i = 0; i < *total_users - 1; i++){
        fout2 << endl << user_names[i];
        delete[] user_names[i];
    }
    fout2 << endl << username;
    fout2.close();

    delete[] path;

    delete[] user_names;
}

void Login(char* username, char* password, char* ADMIN_USER_FILE, char* PROFILES, bool* superuser, bool* login){
    system("cls");

    cout << "--LOGIN--" << endl;
    cout << "Enter username (without spaces): ";
    cin >> username;
    cout << "Enter password (without spaces): ";
    cin >> password;

    if (is_super_user(username, ADMIN_USER_FILE, password, true)){
        cout << "Welcome SuperUser " << username << "!" << endl;
        *superuser = true;
        *login = true;
        cout << "Press any key to continue...";
        getch();
    }
    else{
        ifstream fin;
        char* path = new char[100];
        path = concat(PROFILES, concat("users/", concat(username, ".txt")));
        fin.open(path);
        if(fin.is_open()){
            char name[20];
            char pass[20];

            fin >> name;
            fin >> pass;

            if (compareStr(username, name) && compareStr(password, pass)){
                cout << "Welcome " << username << "!" << endl;
                *login = true;
                *superuser = false;
                cout << "Press any key to continue...";
                getch();
            }
            
        }
        else{
            
        }

        delete[] path;
    }
}

void reset_data(){
    system("rmdir /q /s data");
    system("mkdir data\\profiles\\admin");
    system("mkdir data\\profiles\\users");
    system("mkdir data\\tasks");

    ofstream fout;
    fout.open("data\\profiles\\users.txt");
    fout << "0";
    fout.close();

    ofstream fout1;
    fout1.open("data\\tasks\\total_tasks.txt");
    fout1 << "0";
    fout1.close();
}

int getSelection(int* selection_var, int max, char exit_key = '\0'){
    bool arrow_key_mode = false;
    int mode = 0;
    while(true){
        int key = getch();
        if(key == 13){
            mode = 5;
            return mode;
        }
        if(key == 27){
            exit(0);
        }
        if(key == exit_key){
            mode = 6;
            return mode;
        }
        if(arrow_key_mode){
            switch (key)
            {
            case 72:    // Up
                mode = 1;
                arrow_key_mode = false;
                break;
            
            case 80:    // Down
                mode = 2;
                arrow_key_mode = false;
                break;
            
            case 77:    // Right
                mode = 3;
                arrow_key_mode = false;
                break;
            
            case 75:    // Left
                mode = 4;
                arrow_key_mode = false;
                break;
            
            default:
                mode = 0;
                break;
            }
            if (mode != 0){
                if (mode == 1){
                    *selection_var -= 1;
                    if (*selection_var < 0){
                        *selection_var = max - 1;
                    }
                }
                if (mode == 2){
                    *selection_var += 1;
                    if (*selection_var > max - 1){
                        *selection_var = 0;
                    }
                }
                return mode;
            }
        }
        if(key == 224){
            arrow_key_mode = true;
        }
    }
}

void AddTask(char* username){
    system("cls");
    char Choices[2][4] = {"Yes", "No"};
    int mode = 0;

    // for inputs
    char task_title[100] = { '\0' };
    char *tag = nullptr;
    char task_description[1000] = { '\0' };
    int total_tags = 0;
    bool add_tags = false;
    bool add_discription = false;
    int selection_pos = 0;

    // utils
    char **Tags = nullptr;

    ifstream fin_tags;
    fin_tags.open(concat("data\\tasks\\", concat(username, "\\used_tags.txt")));
    fin_tags >> total_tags;
    Tags = new char*[total_tags];
    for(int i = 0; i < total_tags; i++){
        Tags[i] = new char[50];
        fin_tags >> Tags[i];
    }
    fin_tags.close();


    // Title
    cout << "-- Add Task --" << endl << endl;
    cout << "Enter task title (MAX 100 characters): ";
    cin.ignore();
    cin.getline(task_title, 100);

    while (true) // add tag yes/no loop
    {
        system("cls");
        cout << "-- Add Task --" << endl << endl;
        cout << "Title: " << task_title << endl;
        cout << "Do you want to add a Tag to this task?" << endl;
        for(int i = 0; i < 2; i++){
            if(i == selection_pos){
                cout << ">> " << Choices[i] << endl;
            }
            else{
                cout << "-- " << Choices[i] << endl;
            }
        }

        mode = getSelection(&selection_pos, 2);

        if(mode == 5){
            if(selection_pos == 0){
                while (true)
                {
                    if (total_tags == 0){
                        break;
                    }
                    system("cls");
                    cout << "-- Add Task --" << endl << endl;
                    cout << "Title: " << task_title << endl;
                    cout << "Select the tag from below or press 'N' for new tag:" << endl;

                    for(int i = 0; i < total_tags; i++){
                        if(i == selection_pos){
                            cout << ">> " << Tags[i] << endl;
                        }
                        else{
                            cout << "-- " << Tags[i] << endl;
                        }
                    }

                    mode = getSelection(&selection_pos, total_tags, 'n');
                    if(mode == 5){
                        tag = Tags[selection_pos];
                        add_tags = true;
                        break;
                    }
                    else if(mode == 6){
                        break;
                    }
                }
                
                if(!add_tags){    
                    system("cls");
                    cout << "-- Add Task --" << endl << endl;
                    cout << "Title: " << task_title << endl;
                    cout << "Enter tag (without spaces MAX 50 characters): ";
                    tag = new char[50];
                    cin >> tag;
                    add_tags = true;
                }
                break;
            }
            if(selection_pos == 1){
                add_tags = false;
                break;
            }
        }

    }

    while (true) // add Discription yes/no loop
    {
        system("cls");
        cout << "-- Add Task --" << endl << endl;
        cout << "Title: " << task_title << endl;
        cout << "Tag: " << ((add_tags)?tag:"(No Tag)") << endl;
        cout << "Add description for this Task?" << endl;
        for(int i = 0; i < 2; i++){
            if(i == selection_pos){
                cout << ">> " << Choices[i] << endl;
            }
            else{
                cout << "-- " << Choices[i] << endl;
            }
        }

        mode = getSelection(&selection_pos, 2);

        if(mode == 5){
            if(selection_pos == 0){
                system("cls");
                cout << "-- Add Task --" << endl << endl;
                cout << "Title: " << task_title << endl;
                cout << "Tag: " << ((add_tags)?tag:"(No Tag)") << endl;
                cout << "Enter the description (MAX 1000 characters): ";
                cin.ignore();
                cin.getline(task_description, 1000);
                add_discription = true;
                break;
            }
            if(selection_pos == 1){
                add_discription = false;
                break;
            }
        }
        else if(mode == 6){

        }

    }
    
    system("cls");
    cout << "-- Add Task --" << endl << endl;
    cout << "Title: " << task_title << endl;
    cout << "Tag: " << ((add_tags)?tag:"(No Tag)") << endl;
    cout << "Discription: " << ((add_discription)?task_description:"(No Discription)") << endl;
 

    if(add_tags){
        if(!exists_in_file(tag, concat("data\\tasks\\", concat(username, "\\used_tags.txt")))){
            ofstream fout;
            fout.open(concat("data\\tasks\\", concat(username, "\\used_tags.txt")));
            fout << total_tags + 1;
            for(int i = 0; i < total_tags; i++){
                fout << endl << Tags[i];
            }
            fout << endl << tag;
            fout.close();
        }
    }

    ofstream fout_title, fout_tag, fout_desc;

    char* path = nullptr;
    path = new char[200];
    path = concat("data\\tasks\\", concat(username, concat("\\", task_title)));
    system(concat("mkdir \"", concat(path, "\"")));
    
    fout_title.open(concat(path, "\\task_title.txt"));
    fout_tag.open(concat(path, "\\task_tag.txt"));
    fout_desc.open(concat(path, "\\task_description.txt"));
    
    fout_title << task_title;
    fout_tag << tag;
    fout_desc << task_description;
    fout_title.close();
    fout_tag.close();
    fout_desc.close();


    int total_tasks = 0;
    ifstream fin;
    fin.open("data\\tasks\\total_tasks.txt");
    fin >> total_tasks;
    fin.close();
    ofstream fout;
    fout.open("data\\tasks\\total_tasks.txt");
    fout << total_tasks + 1;
    fout.close();

    int total_lines = 0;
    // cout << concat("data\\tasks\\", concat(username, "\\tasks.txt")) << endl;
    fin.open(concat("data\\tasks\\", concat(username, "\\tasks.txt")));
    fin >> total_lines;
    fin.close();
    fin.open(concat("data\\tasks\\", concat(username, "\\tasks.txt")));
    char** lines = new char* [total_lines];
    for(int i = 0; i < total_lines + 1; i++){
        lines[i] = new char[100];
        fin.getline(lines[i], 100);
    }
    fin.close();
    fout.open(concat("data\\tasks\\", concat(username, "\\tasks.txt")));
    fout << total_lines + 1;
    for(int i = 1; i < total_lines + 1; i++){
        fout << endl << lines[i];
    }
    fout << endl << task_title;
    fout.close();

    delete[] path;
    delete[] lines;

    cout << endl << "Task added successfully!" << endl;
    cout << "Press any key to continue...";
    getch();
}

void ViewTaskList(char* username){
    char* path_to_task_titles = concat("data\\tasks\\", concat(username, "\\tasks.txt"));
    int selection_pos = 0;
    int mode = 0;
    bool task_view_mode = false;

    int total_tasks = 0;
    ifstream fin;
    fin.open(path_to_task_titles);
    fin >> total_tasks;
    char** Titles = new char*[total_tasks];
    char** Tags = new char*[total_tasks];
    char** path_to_tasks = new char*[total_tasks];

    for(int i = 0; i < total_tasks + 1; i++){
        Titles[i] = new char[100];
        fin.getline(Titles[i], 100);
        path_to_tasks[i] = new char[250];
        path_to_tasks[i] = concat("data\\tasks\\", concat(username, concat("\\", Titles[i])));
    }
    fin.close();

    // Adding Tags
    for(int i = 0; i < total_tasks; i++){
        fin.open(concat(path_to_tasks[i + 1], "\\task_tag.txt"));
        Tags[i] = new char[50];
        fin >> Tags[i];
        fin.close();
    }

    while(true){ // Main Task View mode
        system("cls");
        cout << "--Task Viewer--" << endl << endl;

        if(task_view_mode){ // when enter pressed on title
            cout << "Title: " << Titles[selection_pos + 1] << endl;
            cout << "It's tag: " << Tags[selection_pos] << endl << endl;
            task_view_mode = false;
        }

        cout << "Select the task using arrowkeys and select using enter." << endl << endl;
        for(int i = 1; i < total_tasks + 1; i++){
            if(selection_pos == i - 1){
                cout << ">> " << i << "= " << Titles[i] << endl;
            }
            else{
                cout << "-- " << i << "- " << Titles[i] << endl;
            }
        }

        cout << endl << "B - Go Back" << endl;

        mode = getSelection(&selection_pos, total_tasks, 'b');

        if(mode == 5){
            task_view_mode = true;
        }

        if(mode == 6){
            return;
        }
    }

    delete[] path_to_task_titles;
    for(int i = 0; i < total_tasks; i++){
        delete[] path_to_tasks[i];
        delete[] Titles[i];
        delete[] Tags[i];
    }
    delete[] path_to_tasks;
    delete[] Titles;
    delete[] Tags;

}

void displayUsers(){
    ifstream fin;
    fin.open("data\\profiles\\users.txt");
    int total_users = 0;
    fin >> total_users;
    for(int i = 0; i < total_users; i++){
        char name[20];
        fin >> name;
        cout << name << endl;
    }
}

int main(){
    char NotLoginedOptions[4][30] = { "Login", "Register", "EXPERIMENTAL ERASE DATA!!","Exit" };
    char UserOption[3][15] = { "Add Task", "View My Tasks", "Logout" };
    system("cls");
    int selection_pos = 0;
    int select_mode = 0;
    bool login = false;
    bool superuser = false;
    const int USER_PASS_SIZE = 20;

    char username[USER_PASS_SIZE] = { "\0" };
    char password[USER_PASS_SIZE] = { "\0" };
    // Constant paths
    char* PROFILES = "data/profiles/";
    char* ADMIN_USER_FILE = concat(PROFILES, "admin/user.txt");
    char* USERNAMES = concat(PROFILES, "users.txt");

    // dynamic vars
    int total_users = 0;
    int total_tasks = 0;
    
    ifstream fin;
    fin.open(USERNAMES);
    fin >> total_users;
    fin.close();

    system("cls");


    bool invalid_choice = false;

    while(true){
    if(login){
        while (true) // Main Logined Loop
        {
            system("cls");
            cout << "Welcome " << username;
            if(superuser){
                cout << " - Admin" << endl;
            }
            cout << endl;

            cout << "What you want to do?" << endl;
            for (int i = 0; i < 3; i++){
                if(i == selection_pos){
                    cout << ">> " << UserOption[i] << endl;
                }
                else{
                    cout << "-- " << UserOption[i] << endl;
                }
            }

            select_mode = getSelection(&selection_pos, 3);
            if(select_mode == 5){
                if (selection_pos == 0){
                    AddTask(username);
                }
                else if (selection_pos == 1){
                    ViewTaskList(username);
                }
                else if (selection_pos == 2){
                    login = false;
                    break;
                }
            }
        }
        selection_pos = 0;
    }
    else{
        ifstream fin;

        fin.open(ADMIN_USER_FILE);
        if (fin.is_open()){
            while(true) // Main Generic Loop
            {
            system("cls");
            cout << "Welcome to Task MANAGER" << endl << endl;
            cout << "What you want to do?" << endl;

            for (int i = 0; i < 3; i++){
                if(i == selection_pos){
                    cout << ">> " << NotLoginedOptions[i] << endl;
                }
                else{
                    cout << "-- " << NotLoginedOptions[i] << endl;
                }
            }

            select_mode = getSelection(&selection_pos, 3);
            if(select_mode == 5){
                if (selection_pos == 0){
                    Login(username, password, ADMIN_USER_FILE, PROFILES, &superuser, &login);
                    break;
                }
                else if (selection_pos == 1){
                    Register(username, password, ADMIN_USER_FILE, PROFILES, USER_PASS_SIZE, &login, USERNAMES, &total_users);
                }
                else if (selection_pos == 2){
                    fin.close();
                    reset_data();
                    return 0;
                }
                else if (selection_pos == 3){
                    exit(0);
                }
            }    
            }
            selection_pos = 0;
        }
        else{
            cout << "Admin User is not defined.." << endl;
            cout << "The first registration made will be for the Admin user." << endl;
            cout << "After the registration the program will close then login again, or make new registration." << endl << endl;
            cout << "Press any key to continue..." << endl;
            getch();
            reset_data();
            Register(username, password, ADMIN_USER_FILE, PROFILES, USER_PASS_SIZE, &login, USERNAMES, &total_users, true);
            return 0;
        }
        fin.close();
    } 
    }

    // delete all dynamic variables

    delete[] PROFILES;
    delete[] ADMIN_USER_FILE;

    return 0;
}