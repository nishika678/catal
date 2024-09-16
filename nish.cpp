#include<iostream>
#include <fstream>
#include "json.hpp" 
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <algorithm> 
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
vector<pair<int, unsigned long long>> extract(map<int, pair<int, string>>& data) {
    vector<pair<int, unsigned long long>> points;    
    for (auto& entry : data) {
        int x = entry.first;
        int base = entry.second.first;
        string value = entry.second.second;
        unsigned long long y = decode1(base, value);
        points.push_back({x, y});
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
bool ConsistentPoint(const vector<pair<int, unsigned long long>>& points, int x, unsigned long long y, double constantTerm) {
    double interpolatedY = 0.0;
    for (int i = 0; i < points.size(); i++) {
        double li = 1.0;
        for (int j = 0; j < points.size(); j++) {
            if (i != j) {
                li *= (x - points[j].first) / (double)(points[i].first - points[j].first);
            }
        }
        interpolatedY += points[i].second * li;
    }
    return fabs(interpolatedY - y) < 1e-6;  
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
    vector<pair<int, unsigned long long>> allPoints = extract(rootData);
    vector<int> indices(n, 0);
    fill(indices.begin(), indices.begin() + k, 1); 
    bool foundSolution = false;
    vector<int> incorrectRoots;
    do {
        vector<pair<int, unsigned long long>> selectedPoints;
        vector<pair<int, unsigned long long>> remainingPoints;

        for (int i = 0; i < n; i++) {
            if (indices[i] == 1) {
                selectedPoints.push_back(allPoints[i]);
            } else {
                remainingPoints.push_back(allPoints[i]);
            }
        }
        double constantTerm = solvelagrange(selectedPoints);
        vector<int> currentIncorrectRoots;
        for (auto& point : remainingPoints) {
            if (!ConsistentPoint(selectedPoints, point.first, point.second, constantTerm)) {
                currentIncorrectRoots.push_back(point.first);
            }
        }
        if (currentIncorrectRoots.size() <= 3) {  
            foundSolution = true;
            incorrectRoots = currentIncorrectRoots;
            break;
        }
    } while (prev_permutation(indices.begin(), indices.end()));
    if (foundSolution) {
        cout << "Incorrect roots are: ";
        for (int root : incorrectRoots) {
            cout << root << " ";
        }
        cout << endl;
    } else {
        cout << "No valid solution found!" << endl;
    }
    return 0;
}
