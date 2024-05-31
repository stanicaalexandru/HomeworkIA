#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include "search_algorithms.h" // Include fisierul cu algoritmii de cautare

using namespace std;

// Structura pentru un nod ce contine coordonatele (x, y)
struct Node {
    double x;
    double y;
};

// Functie pentru calculul distantelor intre noduri
vector<vector<int>> calculateDistances(const vector<Node>& nodes) {
    // Initializam o matrice de distante cu dimensiunea adecvata
    vector<vector<int>> distances(nodes.size(), vector<int>(nodes.size(), 0));

    // Parcurgem fiecare pereche de noduri pentru a calcula distantele
    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = 0; j < nodes.size(); ++j) {
            // Calculam distanta euclidiana intre nodurile i si j
            distances[i][j] = round(sqrt(pow(nodes[i].x - nodes[j].x, 2) + pow(nodes[i].y - nodes[j].y, 2)));
        }
    }

    return distances; // Returnam matricea de distante
}

// Functie pentru citirea nodurilor din fisier
vector<Node> readNodesFromFile(const string& filename, int numNodesToRead) {
    ifstream file(filename); // Deschidem fisierul pentru citire
    if (!file.is_open()) { // Verificam daca s-a putut deschide fisierul
        cerr << "Failed to open the file." << endl;
        return {}; // In caz negativ, returnam un vector gol
    }

    vector<Node> nodes; // Vector pentru stocarea nodurilor
    string line;
    int linesToRead = numNodesToRead; // Numarul de linii de noduri ce trebuie citite
    bool readNodes = false; // Variabila pentru a indica ca s-a ajuns la sectiunea cu noduri

    // Parcurgem fiecare linie din fisier
    while (getline(file, line)) {
        if (line == "NODE_COORD_SECTION") { // Daca intalnim sectiunea cu noduri
            readNodes = true; // Setam indicativul pentru citirea nodurilor
            continue; // Continuam cu urmatoarea iteratie
        }
        if (readNodes && linesToRead > 0) { // Daca suntem in sectiunea cu noduri si nu am citit suficiente
            istringstream iss(line); // Folosim un istringstream pentru a separa linia in valori
            int id;
            double x, y;
            iss >> id >> x >> y; // Extragem id-ul si coordonatele (x, y) ale nodului
            nodes.push_back({ x, y }); // Adaugam nodul in vectorul de noduri
            --linesToRead; // Scadem numarul de linii ramase de citit
        }
        if (linesToRead == 0) { // Daca am citit suficiente linii
            break; // Iesim din bucla
        }
    }

    file.close(); // Inchidem fisierul

    // Verificam daca am citit suficiente noduri
    if (nodes.size() < numNodesToRead) {
        cerr << "Not enough nodes read from the file." << endl;
        return {}; // In caz negativ, returnam un vector gol
    }

    return nodes; // Returnam vectorul de noduri citite
}

int main() {
    // Solicitam utilizatorului sa introduca numele fisierului pe care doreste sa-l citeasca
    cout << "Introduceti numele fisierului pe care doriti sa-l cititi (lin105.txt sau lin318.txt): ";
    string filename;
    cin >> filename;

    // Specificam calea absoluta catre directorul si fisierul dorit
    string directoryPath = "C:\\Users\\Alejandro\\Desktop\\ConsoleApplication1\\";
    string filePath = directoryPath + filename;

    // Citim nodurile din fisierul specificat
    vector<Node> nodes = readNodesFromFile(filePath, 10);
    if (nodes.empty()) { // Verificam daca nu s-au putut citi nodurile din fisier
        cerr << "Failed to read nodes from " << filename << "." << endl;
        return 1; // In caz negativ, returnam un cod de eroare
    }

    // Calculam distantele intre noduri
    vector<vector<int>> distances = calculateDistances(nodes);

    int startCity = 0; // Specificam orasul de plecare (inceputul turului)

    // Cautarea in latime (Breadth-First Search)
    pair<vector<int>, int> bfsResult = bfs(distances, startCity);
    cout << "Drum optim gasit cu Breadth-First Search: ";
    for (int city : bfsResult.first) { // Afisam drumul optim gasit
        cout << city << " ";
    }
    cout << startCity << endl;
    cout << "Costul drumului gasit cu Breadth-First Search: " << bfsResult.second << endl;

    // Cautarea cu cel mai mic cost (Least-Cost Search)
    pair<vector<int>, int> leastCostResult = leastCostSearch(distances, startCity);
    cout << "Drum optim gasit cu Least-Cost Search: ";
    for (int city : leastCostResult.first) { // Afisam drumul optim gasit
        cout << city << " ";
    }
    cout << startCity << endl;
    cout << "Costul minim gasit cu Least-Cost Search: " << leastCostResult.second << endl;

    // A* Search
    pair<vector<int>, int> aStarResult = aStarSearch(distances, startCity);
    cout << "Drum optim gasit cu A* Search: ";
    for (int city : aStarResult.first) { // Afisam drumul optim gasit
        cout << city << " ";
    }
    cout << startCity << endl;
    cout << "Costul optim gasit cu A* Search: " << aStarResult.second << endl;

    return 0; // Returnam 0 pentru a indica ca programul a fost executat cu succes
}




