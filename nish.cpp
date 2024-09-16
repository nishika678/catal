#include <iostream>
#include <fstream>
#include "json.hpp" 
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <cmath>
using namespace std;
using json = nlohmann::json;
unsigned long long decode1(int base, const string& value) {
    unsigned long long ans = 0;
    int power = 0;
    for (int i = value.size() - 1; i >= 0; i--) {
        char digit = value[i];
        int num = (isdigit(digit)) ? (digit - '0') : (toupper(digit) - 'A' + 10);
        ans += num * pow(base, power);
        power++;
    }
    return ans;
}
vector<pair<int, unsigned long long>> extract(map<int, pair<int, string>>& data, int k) {
    vector<pair<int, unsigned long long>> points;   
    int count = 0;
    for (auto& entry : data) {
        if (count == k) break; 
        int x = entry.first;
        int base = entry.second.first;
        string value = entry.second.second;
        unsigned long long y = decode1(base, value);
        points.push_back({x, y});
        count++;
    }
    return points;
}
double solvelagrange(const vector<pair<int, unsigned long long>>& points) {
    int k = points.size(); 
    double constantTerm = 0.0;
    for (int i = 0; i < k; i++) {
        double li = 1.0;
        unsigned long long yi = points[i].second;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                li *= points[j].first / (double)(points[j].first - points[i].first);
            }
        }
        constantTerm += yi * li;
    }
    return constantTerm;
}
int main() {
    ifstream inputFile("input.json");
    json jsonData;
    inputFile >> jsonData;
    int n = jsonData["keys"]["n"];
    int k = jsonData["keys"]["k"];
    map<int, pair<int, string>> rootData;
    for (auto& el : jsonData.items()) {
        if (el.key() == "keys") continue; 
        int x = stoi(el.key());            
        int base = stoi(el.value()["base"].get<string>());
        string encodedY = el.value()["value"];
        rootData[x] = {base, encodedY};
    }
    vector<pair<int, unsigned long long>> points = extract(rootData, k);   
    double constantTerm = solvelagrange(points);   
    cout << "The constant term 'c' is: " << constantTerm << endl;

    return 0;
}


