#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <climits>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <windows.h> 
#include <psapi.h>   

using namespace std;

//Function declarations
void fill_1d(vector<int> &vec, string name);
void fill_2d(vector<vector<int>> &replaceSs, string name);
int costo_sub(char a, char b);
int costo_ins(char a);
int costo_del(char a);
int costo_trans(char a, char b);
pair<string, string> Read_input(const string &filename);
int min_dist_brute(const string &a, const string &b, int i, int j);
int min_dist_dp(const string &a, const string &b);

//Global variables
string cost_insert = "cost_insert.txt";
string cost_delete = "cost_delete.txt";
string cost_replaceSs = "cost_replace.txt";
string cost_transpose = "cost_transpose.txt";
vector<vector<int>> replaceSs, transpose;
vector<int> insert, delet;
map<string, int> index_map = {
    {"a", 1},
    {"b", 2},
    {"c", 3},
    {"d", 4},
    {"e", 5},
    {"f", 6},
    {"g", 7},
    {"h", 8},
    {"i", 9},
    {"j", 10},
    {"k", 11},
    {"l", 12},
    {"m", 13},
    {"n", 14},
    {"o", 15},
    {"p", 16},
    {"q", 17},
    {"r", 18},
    {"s", 19},
    {"t", 20},
    {"u", 21},
    {"v", 22},
    {"w", 23},
    {"x", 24},
    {"y", 25},
    {"z", 26}
};


SIZE_T getCurrentMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

int main() {
    int len = 14;//2048
    fill_2d(replaceSs, cost_replaceSs);
    fill_2d(transpose, cost_transpose);
    fill_1d(insert, cost_insert);
    fill_1d(delet, cost_delete);   
    pair<string, string> input = Read_input("input.txt");
    double total_duration_brute = 0;
    double total_duration_dp = 0;
    SIZE_T total_memory_brute = 0;
    SIZE_T total_memory_dp = 0;
    int brute_result = 0;
    int dp_result = 0;

    for (int i = 0; i < 50; ++i) {
        auto start = chrono::high_resolution_clock::now();
        SIZE_T memory_before = getCurrentMemoryUsage();
        brute_result = min_dist_brute(input.first, input.second, 0, 0);
        auto end = chrono::high_resolution_clock::now();
        SIZE_T memory_after = getCurrentMemoryUsage();
        chrono::duration<double, milli> duration = end - start;
        total_duration_brute += duration.count();
        total_memory_brute += (memory_after - memory_before);

        start = chrono::high_resolution_clock::now();
        memory_before = getCurrentMemoryUsage();
        dp_result = min_dist_dp(input.first, input.second);
        end = chrono::high_resolution_clock::now();
        memory_after = getCurrentMemoryUsage();
        duration = end - start;
        total_duration_dp += duration.count();
        total_memory_dp += (memory_after - memory_before);

if (brute_result != dp_result) {
        cerr << "Error: Distancia minima no coincide en la iteracion " << i + 1 << endl;
}
}
    cout << "Palabra 1: " << input.first <<" \\newline "<< endl;
    cout << "Palabra 2: " << input.second << " \\newline "<< endl;
    cout << "Largo de palabra 1: " << input.first.length() <<" \\newline "<< endl;
    cout << "Largo de palabra 2: " << input.second.length() <<" \\newline "<< endl;
    cout << "Distancia minima: " << brute_result <<" \\newline "<< endl;
    cout << "Distancia minima dp: " << dp_result <<" \\newline "<< endl;
    cout << "Tiempo medio de ejecucion (Fuerza Bruta): " << fixed << setprecision(6) << total_duration_brute / 15 << " ms" <<" \\newline "<< endl;
    cout << "Uso medio de memoria (Fuerza Bruta): " << (total_memory_brute / 15) / 1024 << " KB" <<" \\newline "<< endl;

    cout << "Tiempo medio de ejecucion (DP): " << fixed << setprecision(6) << total_duration_dp / 15 << " ms" <<" \\newline "<< endl;
    cout << "Uso medio de memoria (DP): " << (total_memory_dp / 15) / 1024 << " KB" <<" \\newline "<< endl;

    return 0;
}



//filling functions
void fill_1d(vector<int> &vec, string name)
{
    ifstream file(name);
    if (file.is_open())
    {
        string line;
        if (getline(file, line))
        {
            int value;
            istringstream iss(line);
            while (iss >> value)
            {
                vec.push_back(value);
            }
            if (vec.size() != 26)
            {
                cerr << "Invalid format in file " << name << ": expected 26 values, got " << vec.size() << endl;
                vec.clear();
            }
        }
        file.close();
    }
    else
    {
        cerr << "Unable to open file " << name << endl;
    }
}

void fill_2d(vector<vector<int>> &replaceSs, string name)
{
    ifstream file(name);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            vector<int> row;
            int value;
            istringstream iss(line);
            while (iss >> value)
            {
                row.push_back(value);
            }
            replaceSs.push_back(row);
        }
        file.close();

        if (replaceSs.size() != 26)
        {
            cerr << "Invalid format in file " << name << ": expected 26 rows, got " << replaceSs.size() << endl;
            replaceSs.clear();
        }
        else
        {
            for (const auto &row : replaceSs)
            {
                if (row.size() != 26)
                {
                    cerr << "Invalid format in file " << name << ": expected 26 columns, got " << row.size() << endl;
                    replaceSs.clear();
                    break;
                }
            }
        }
    }
    else
    {
        cerr << "Unable to open file " << name << endl;
    }
}
pair<string, string> Read_input(const string &filename) {
    ifstream file(filename);
    pair<string, string> result;
    if (file.is_open()) {
        getline(file, result.first);
        getline(file, result.second);
        file.close();
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
    return result;
}


// Cost functions 
int costo_ins(char a) {
    return insert[index_map[string(1, a)] - 1];
}

int costo_del(char a) {
    return delet[index_map[string(1, a)] - 1];
}

int costo_sub(char a, char b) {
    return replaceSs[index_map[string(1, a)] - 1][index_map[string(1, b)] - 1];
}

int costo_trans(char a, char b) {
    return transpose[index_map[string(1, a)] - 1][index_map[string(1, b)] - 1];
}

//alg functions
int min_dist_brute(const string &a, const string &b, int i, int j) {
    if (i == a.length()) {
        int costo = 0;
        while (j < b.length()){
            costo += costo_ins(b[j]);
            j++;
        }
        return costo;
    }
    if (j == b.length()){
        int costo = 0;
        while (i < a.length()){
            costo += costo_del(a[i]);
            i++;
        }
        return costo;
    }

    int costo = INT_MAX;
    
    costo = min(costo, costo_sub(a[i], b[j]) + min_dist_brute(a, b, i + 1, j + 1));
    costo = min(costo, costo_ins(b[j]) + min_dist_brute(a, b, i, j + 1));
    costo = min(costo, costo_del(a[i]) + min_dist_brute(a, b, i + 1, j));
    
    if(i > 0 && j > 0 && i + 1 < a.length() && j + 1 < b.length() && 
       a[i] == b[j + 1] && a[i + 1] == b[j]) {
        costo = min(
            costo,
            costo_trans(a[i], a[i + 1]) + min_dist_brute(a, b, i + 2, j + 2)
        );
    }
    
    return costo;
}

int min_dist_dp(const string &a, const string &b){
    int len_a = a.length();
    int len_b = b.length();

    vector<vector<int>> dp(len_a + 1, vector<int>(len_b + 1, -1));

    dp[0][0] = 0;

    for(int i = 1; i <= len_b; i++){
        dp[0][i] = dp[0][i - 1] + costo_ins(b[i - 1]);
    }
    for(int i = 1; i <= len_a; i++){
        dp[i][0] = dp[i - 1][0] + costo_del(a[i-1]);
    }

    for(int i = 1; i <= len_a; i++){
        for(int j = 1; j <= len_b; j++){

            if (a[i - 1] == b[j - 1]){
                dp[i][j] = dp[i - 1][j - 1];
                continue;
            }

            dp[i][j] = min({
                dp[i - 1][j] + costo_del(a[i - 1]),
                dp[i][j - 1] + costo_ins(b[j - 1]),
                dp[i - 1][j - 1] + costo_sub(a[i - 1], b[j - 1])
            });

            if(i > 0 && j > 0 && i + 1 < a.length() && j + 1 < b.length() && 
       a[i] == b[j + 1] && a[i + 1] == b[j]){
                dp[i][j] = min(
                    dp[i][j],
                    dp[i - 2][j - 2] + costo_trans(a[i - 2], a[i - 1])
                );
            }
        }
    }
    return dp[len_a][len_b];
}