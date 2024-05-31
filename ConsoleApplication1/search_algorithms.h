#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include <vector>
#include <utility>

// Structura care reprezinta o stare a algoritmului de cautare
struct State {
    std::vector<int> path; // Traseul parcurs pana in acest moment
    int cost; // Costul total asociat traseului
};

// Functie pentru calculul costului total al unui traseu
int calculateCost(const std::vector<std::vector<int>>& distances, const std::vector<int>& path);

// Functie euristica pentru estimarea costului optim dintr-un nod la altul
int heuristic(const std::vector<std::vector<int>>& distances, const std::vector<int>& path);

// Functie pentru cautarea Breadth-First Search (BFS)
std::pair<std::vector<int>, int> bfs(const std::vector<std::vector<int>>& distances, int start);

// Functie pentru cautarea cu cel mai mic cost
std::pair<std::vector<int>, int> leastCostSearch(const std::vector<std::vector<int>>& distances, int start);

// Functie pentru cautarea A* Search
std::pair<std::vector<int>, int> aStarSearch(const std::vector<std::vector<int>>& distances, int start);

#endif // SEARCH_ALGORITHMS_H

