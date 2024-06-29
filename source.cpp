#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>
using namespace std;

bool compareStr(char* arr1, char* arr2){
    for(int i = 0; arr1[i] != '\0'; i++){
        if(arr1[i] != arr2[i]){
            return false;
        }
    }
    return true;
}

int lenCh(char str[]){
    int i = 0;
    for(i = 0; str[i] != '\0'; i++);
    return i;
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
        }
        else{
            delete[] confPass;
            break;
        }
    }

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

    char* task_folder = concat("data\\tasks\\", username);
    char* task_folder_command = concat("mkdir ", concat(task_folder, "\\grouped"));

    system(task_folder_command);

    ofstream fout1;
    fout1.open(concat(task_folder, "\\tasks.txt"));
    fout1 << "0";
    fout1.close();

    delete[] task_folder;
    delete[] task_folder_command;


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

void AddTask(char* username){
    char task_title[100];
    cout << "Enter task title (MAX 100 characters): ";
    cin.ignore();
    cin.getline(task_title, 100);

    char add_tags = 'n';
    int total_tags = 0;
    cout << "Do you want to add tags? (y/n): ";
    cin >> add_tags;
    char tag[50] = "\0";

    if(add_tags == 'y' || add_tags == 'Y'){
        cout << "Enter tag (without spaces MAX 50 characters): ";
        cin >> tag;
    }

    char task_description[1000];
    cout << endl << "Enter task description (optional - enter only 'Q' to skip): ";
    cin.ignore();
    cin.getline(task_description, 1000);

    if (compareStr(task_description, "Q")){
        task_description[0] = '\0';
    }

    ofstream fout_title, fout_tag, fout_desc;

    char* path = new char[100];
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

    cout << "Task added successfully!" << endl;
    cout << "Press any key to continue...";
    getch();
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
        system("cls");
        cout << "Welcome " << username;
        if(superuser){
            cout << " - Admin" << endl;
        }
        cout << endl;

        cout << "What you want to do?" << endl;
        cout << "1) Add Task" << endl;
        cout << "2) Display Tasks" << endl;
        cout << "3) Logout" << endl;
        cout << "Enter the number to select the option: ";
        char choice[20];
        cin >> choice;
        if(compareStr(choice, "1")){
            AddTask(username);
        }
        else if(compareStr(choice, "2")){
            // DisplayTasks(username, password, ADMIN_USER_FILE, PROFILES, USER_PASS_SIZE, &login, USERNAMES, &total_users);
        }
        else if(compareStr(choice, "3")){
            login = false;
        }
    }
    else{
        ifstream fin;

        fin.open(ADMIN_USER_FILE);
        if (fin.is_open()){
            system("cls");
            cout << "Total users: " << total_users << endl;
            cout << "Welcome to Task MANAGER" << endl << endl;
            cout << "What you want to do?" << endl;
            cout << "1) Login" << endl;
            cout << "2) Register" << endl;
            cout << "3) ERASE ALL DATA !!" << endl; // delete all data (for testing purposes only)
            cout << "4) Display Names" << endl;

            if(invalid_choice){
                cout << "INVALID OPTION ENTERED! Please Enter from above (numbers)" << endl;
                invalid_choice = false;
            }

            cout << "Enter the number to select the option: ";
            char choice[20];
            cin >> choice;
            if(compareStr(choice, "1"))
            Login(username, password, ADMIN_USER_FILE, PROFILES, &superuser, &login);
            else if(compareStr(choice, "2")){
                Register(username, password, ADMIN_USER_FILE, PROFILES, USER_PASS_SIZE, &login, USERNAMES, &total_users);
            }
            else if(compareStr(choice, "3")){
            fin.close();
            reset_data();
            return 0;
            }
            else if(compareStr(choice, "4")){
                displayUsers();
                cout << "Press any key to continue..." << endl;
                getch();
            }
            else {
                invalid_choice = true;
            }
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