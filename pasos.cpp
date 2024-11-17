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

void print_operations_and_cost(const string &a, const string &b);


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

    
    cout << "Cadena 1: " << input.first <<" \\ \\newline "<< endl;
    cout << "Cadena 2: " << input.second << " \\ \\newline "<< endl;
    print_operations_and_cost(input.first, input.second);
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


void print_operations_and_cost(const string &a, const string &b) {
    int len_a = a.length();
    int len_b = b.length();

    vector<vector<int>> dp(len_a + 1, vector<int>(len_b + 1, INT_MAX));
    vector<vector<tuple<int, string, char, char, int>>> ops(len_a + 1, vector<tuple<int, string, char, char, int>>(len_b + 1));

    dp[0][0] = 0;

    for (int i = 1; i <= len_b; i++) {
        dp[0][i] = dp[0][i - 1] + costo_ins(b[i - 1]);
        ops[0][i] = make_tuple(costo_ins(b[i - 1]), "Insert", b[i - 1], 0, i - 1);
    }

    for (int i = 1; i <= len_a; i++) {
        dp[i][0] = dp[i - 1][0] + costo_del(a[i - 1]);
        ops[i][0] = make_tuple(costo_del(a[i - 1]), "Delete", a[i - 1], 0, i - 1);
    }

    for (int i = 1; i <= len_a; i++) {
        for (int j = 1; j <= len_b; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
                ops[i][j] = make_tuple(0, "Match", a[i - 1], b[j - 1], j - 1);
            } else {
                if (dp[i][j] > dp[i - 1][j - 1] + costo_sub(a[i - 1], b[j - 1])) {
                    dp[i][j] = dp[i - 1][j - 1] + costo_sub(a[i - 1], b[j - 1]);
                    ops[i][j] = make_tuple(costo_sub(a[i - 1], b[j - 1]), "Substitute", a[i - 1], b[j - 1], j - 1);
                }
                if (dp[i][j] > dp[i - 1][j] + costo_del(a[i - 1])) {
                    dp[i][j] = dp[i - 1][j] + costo_del(a[i - 1]);
                    ops[i][j] = make_tuple(costo_del(a[i - 1]), "Delete", a[i - 1], 0, i - 1);
                }
                if (dp[i][j] > dp[i][j - 1] + costo_ins(b[j - 1])) {
                    dp[i][j] = dp[i][j - 1] + costo_ins(b[j - 1]);
                    ops[i][j] = make_tuple(costo_ins(b[j - 1]), "Insert", b[j - 1], 0, j - 1);
                }
            }
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                if (dp[i][j] > dp[i - 2][j - 2] + costo_trans(a[i - 1], a[i - 2])) {
                    dp[i][j] = dp[i - 2][j - 2] + costo_trans(a[i - 1], a[i - 2]);
                    ops[i][j] = make_tuple(costo_trans(a[i - 1], a[i - 2]), "Transpose", a[i - 2], a[i - 1], j - 2);
                }
            }
        }
    }

    vector<tuple<int, string, char, char, int>> trace;
    int i = len_a, j = len_b;

    while (i > 0 || j > 0) {
        trace.push_back(ops[i][j]);
        string op_type = get<1>(ops[i][j]);
        if (op_type == "Match" || op_type == "Substitute") {
            i--;
            j--;
        } else if (op_type == "Delete") {
            i--;
        } else if (op_type == "Insert") {
            j--;
        } else if (op_type == "Transpose") {
            i -= 2;
            j -= 2;
        }
    }

    reverse(trace.begin(), trace.end());

    cout << "Caso de prueba: '" << a << "' -> '" << b << "'" << " \\newline"  << endl;
    cout << "----------------------------------------" << " \\newline"  << endl;
    cout << "Secuencia de operaciones:" <<  " \\newline"  <<endl;

    string current = a;
    for (const auto &step : trace) {
        int cost = get<0>(step);
        string op = get<1>(step);
        char c1 = get<2>(step);
        char c2 = get<3>(step);
        int pos = get<4>(step);

        if (op == "Substitute") {
            current[pos] = c2;
            cout << "- Sustituir '" << c1 << "' por '" << c2 << "' en posicion " << pos << " (costo: " << cost << ")" << " \\newline"  << endl;
        } else if (op == "Delete") {
            current.erase(pos, 1);
            cout << "- Eliminar '" << c1 << "' en posicion " << pos << " (costo: " << cost << ")" << " \\newline"  << endl;
        } else if (op == "Insert") {
            current.insert(pos, 1, c1);
            cout << "- Insertar '" << c1 << "' en posicion " << pos << " (costo: " << cost << ")" << " \\newline"  << endl;
        } else if (op == "Transpose") {
            swap(current[pos], current[pos + 1]);
            cout << "- Transponer '" << c1 << "' y '" << c2 << "' en posicion " << pos << " (costo: " << cost << ")" << " \\newline"  << endl;
        }
        cout << "  Resultado parcial: " << current << "  \\newline"  << endl;
    }

    cout << endl;
    cout << "Costo total: " << dp[len_a][len_b] << "  \\newline"  << endl;
    cout << "Resultado final: " << b <<  "  \\newline"  <<endl;
}