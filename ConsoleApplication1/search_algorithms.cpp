#include "search_algorithms.h" 
#include <queue> /
#include <algorithm> 
#include <climits> 
#include <cmath> 
#include <functional> 

using namespace std;

// Functia pentru calculul costului total al unui drum dat
int calculateCost(const vector<vector<int>>& distances, const vector<int>& path) {
    int cost = 0; // Costul initial este 0
    // Parcurgem fiecare pereche de noduri consecutive din drum
    for (int i = 0; i < path.size() - 1; ++i) {
        cost += distances[path[i]][path[i + 1]]; // Adaugam distanta intre nodurile consecutive la cost
    }
    cost += distances[path.back()][path.front()]; // Adaugam distanta intre ultimul si primul nod pentru a inchide circuitul
    return cost; // Returnam costul total calculat
}

// Functia pentru calculul euristic al unui drum dat
int heuristic(const vector<vector<int>>& distances, const vector<int>& path) {
    int maxDistance = 0; // Initializam distanta maxima cu 0
    // Parcurgem fiecare pereche de noduri consecutive din drum
    for (int i = 0; i < path.size() - 1; ++i) {
        // Actualizam valoarea maxima cu distanta maxima intre nodurile consecutive
        maxDistance = max(maxDistance, distances[path[i]][path[i + 1]]);
    }
    return maxDistance; // Returnam distanta maxima calculata
}

// Functia pentru cautarea in latime (Breadth-First Search)
pair<vector<int>, int> bfs(const vector<vector<int>>& distances, int start) {
    int n = distances.size(); // Numarul total de noduri
    queue<State> q; // Coada pentru stocarea starilor
    State startState; // Starea de pornire
    startState.path.push_back(start); // Adaugam nodul de pornire in drum
    startState.cost = 0; // Costul initial este 0
    q.push(startState); // Adaugam starea de pornire in coada

    vector<int> minPath; // Drumul minim gasit
    int minCost = INT_MAX; // Costul minim gasit

    // Parcurgem toate starile din coada pana cand coada devine goala
    while (!q.empty()) {
        State currentState = q.front(); // Extragem starea curenta din fata cozii
        q.pop(); // Stergem starea curenta din coada

        // Verificam daca drumul curent formeaza un circuit Hamiltonian si a fost finalizat
        if (currentState.path.size() == n && currentState.path[0] == start) {
            // Calculam costul drumului curent
            int currentCost = calculateCost(distances, currentState.path);
            // Verificam daca costul curent este mai mic decat cel minim gasit pana acum
            if (currentCost < minCost) {
                minCost = currentCost; // Actualizam costul minim
                minPath = currentState.path; // Actualizam drumul minim
            }
        }
        else {
            // Parcurgem toate nodurile pentru a determina urmatoarele stari posibile
            for (int i = 0; i < n; ++i) {
                // Verificam daca nodul i nu a fost vizitat inca
                if (find(currentState.path.begin(), currentState.path.end(), i) == currentState.path.end()) {
                    State nextState = currentState; // Copiem starea curenta in starea urmatoare
                    nextState.path.push_back(i); // Adaugam nodul i la drumul urmator
                    nextState.cost = calculateCost(distances, nextState.path); // Calculam costul drumului urmator
                    q.push(nextState); // Adaugam starea urmatoare in coada
                }
            }
        }
    }

    // Returnam drumul minim si costul minim gasite
    return make_pair(minPath, minCost);
}

// Functia pentru cautarea cu cel mai mic cost (Least-Cost Search)
pair<vector<int>, int> leastCostSearch(const vector<vector<int>>& distances, int start) {
    int n = distances.size(); // Numarul total de noduri
    // Coada de prioritati pentru stocarea starilor in functie de cost
    priority_queue<State, vector<State>, function<bool(State, State)>> pq([&distances](State const& s1, State const& s2) {
        return s1.cost > s2.cost; // Ordonam starile in functie de costul lor
        });

    State startState; // Starea de pornire
    startState.path.push_back(start); // Adaugam nodul de pornire in drum
    startState.cost = 0; // Costul initial este 0
    pq.push(startState); // Adaugam starea de pornire in coada

    vector<int> minPath; // Drumul minim gasit
    int minCost = INT_MAX; // Costul minim gasit

    // Parcurgem toate starile din coada pana cand coada devine goala
    while (!pq.empty()) {
        State currentState = pq.top(); // Extragem starea curenta din varful cozii de prioritati
        pq.pop(); // Stergem starea curenta din coada

        // Verificam daca drumul curent formeaza un circuit Hamiltonian si a fost finalizat
        if (currentState.path.size() == n && currentState.path[0] == start) {
            // Calculam costul drumului curent
            int currentCost = calculateCost(distances, currentState.path);
            // Verificam daca costul curent este mai mic decat cel minim gasit pana acum
            if (currentCost < minCost) {
                minCost = currentCost; // Actualizam costul minim
                minPath = currentState.path; // Actualizam drumul minim
            }
        }
        else {
            // Parcurgem toate nodurile pentru a determina urmatoarele stari posibile
            for (int i = 0; i < n; ++i) {
                // Verificam daca nodul i nu a fost vizitat inca
                if (find(currentState.path.begin(), currentState.path.end(), i) == currentState.path.end()) {
                    State nextState = currentState; // Copiem starea curenta in starea urmatoare
                    nextState.path.push_back(i); // Adaugam nodul i la drumul urmator
                    nextState.cost = calculateCost(distances, nextState.path); // Calculam costul drumului urmator
                    pq.push(nextState); // Adaugam starea urmatoare in coada
                }
            }
        }
    }

    // Returnam drumul minim si costul minim gasite
    return make_pair(minPath, minCost);
}

// Functia pentru cautarea A* (A-Star Search)
pair<vector<int>, int> aStarSearch(const vector<vector<int>>& distances, int start) {
    int n = distances.size(); // Numarul total de noduri
    // Coada de prioritati pentru stocarea starilor in functie de costul total estimat
    priority_queue<State, vector<State>, function<bool(State, State)>> pq([&distances](State const& s1, State const& s2) {
        return s1.cost + heuristic(distances, s1.path) > s2.cost + heuristic(distances, s2.path); // Ordonam starile in functie de costul total estimat
        });

    State startState; // Starea de pornire
    startState.path.push_back(start); // Adaugam nodul de pornire in drum
    startState.cost = 0; // Costul initial este 0
    pq.push(startState); // Adaugam starea de pornire in coada

    vector<int> minPath; // Drumul minim gasit
    int minCost = INT_MAX; // Costul minim gasit

    // Parcurgem toate starile din coada pana cand coada devine goala
    while (!pq.empty()) {
        State currentState = pq.top(); // Extragem starea curenta din varful cozii de prioritati
        pq.pop(); // Stergem starea curenta din coada

        // Verificam daca drumul curent formeaza un circuit Hamiltonian si a fost finalizat
        if (currentState.path.size() == n && currentState.path[0] == start) {
            // Calculam costul drumului curent
            int currentCost = calculateCost(distances, currentState.path);
            // Verificam daca costul curent este mai mic decat cel minim gasit pana acum
            if (currentCost < minCost) {
                minCost = currentCost; // Actualizam costul minim
                minPath = currentState.path; // Actualizam drumul minim
            }
        }
        else {
            // Parcurgem toate nodurile pentru a determina urmatoarele stari posibile
            for (int i = 0; i < n; ++i) {
                // Verificam daca nodul i nu a fost vizitat inca
                if (find(currentState.path.begin(), currentState.path.end(), i) == currentState.path.end()) {
                    State nextState = currentState; // Copiem starea curenta in starea urmatoare
                    nextState.path.push_back(i); // Adaugam nodul i la drumul urmator
                    nextState.cost = calculateCost(distances, nextState.path); // Calculam costul drumului urmator
                    pq.push(nextState); // Adaugam starea urmatoare in coada
                }
            }
        }
    }

    // Returnam drumul minim si costul minim gasite
    return make_pair(minPath, minCost);
}


