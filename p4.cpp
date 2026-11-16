#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief Rozwiązuje problem plecaka metodą dynamiczną (0/1 Knapsack)
 * @param weights Tablica wag przedmiotów
 * @param values Tablica wartości przedmiotów
 * @param capacity Pojemność plecaka
 * @param n Liczba przedmiotów
 * @param dp Tablica programowania dynamicznego przekazywana przez referencję (do późniejszego odtworzenia wyniku)
 * @return Maksymalna wartość, którą można osiągnąć
 */
long long knapsack(const vector<int>& weights, const vector<long long>& values, int capacity, int n, vector<vector<long long>>& dp) {
    // Tworzymy tablicę DP o wymiarach (n + 1) x (capacity + 1) wypełnioną zerami
    dp.assign(n + 1, vector<long long>(capacity + 1, 0));
    
    // Wypełniamy tabelę dynamicznie
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            // Nie bierzemy przedmiotu
            dp[i][w] = dp[i - 1][w];
            
            // Sprawdzamy, czy możemy dodać przedmiot (jeśli ma wagę <= pojemności)
            if (weights[i - 1] <= w) {
                // Bierzemy maksimum z niebrania lub wzięcia przedmiotu
                dp[i][w] = max(dp[i][w], dp[i - 1][w - weights[i - 1]] + values[i - 1]);
            }
        }
    }
    
    return dp[n][capacity];
}

/**
 * @brief Wypisuje przedmioty wchodzące do optymalnego rozwiązania na podstawie gotowej tablicy dp
 * @param weights Tablica wag przedmiotów
 * @param values Tablica wartości przedmiotów
 * @param capacity Pojemność plecaka
 * @param n Liczba przedmiotów
 * @param dp Wypełniona wcześniej tablica programowania dynamicznego
 */
void printItems(const vector<int>& weights, const vector<long long>& values, int capacity, int n, const vector<vector<long long>>& dp) {
    int w = capacity;
    vector<int> selectedItems;
    
    // Odtwarzamy wybrane przedmioty idąc od prawego dolnego rogu w górę
    for (int i = n; i > 0 && w > 0; i--) {
        // Jeśli wartość się różni od poprzedniego wiersza, przedmiot został wybrany
        if (dp[i][w] != dp[i - 1][w]) {
            selectedItems.push_back(i - 1); // Dodajemy indeks przedmiotu (0-based)
            w -= weights[i - 1];            // Odejmujemy wagę przedmiotu
        }
    }
    
    cout << "\nZawartosc plecaka:\n";
    if (selectedItems.empty()) {
        cout << "  Brak przedmiotow (plecak pusty)\n";
        return;
    }
    
    long long totalWeight = 0;
    cout << "  Wybrane przedmioty:\n";
    for (int i = static_cast<int>(selectedItems.size()) - 1; i >= 0; i--) {
        int idx = selectedItems[i];
        cout << "    " << idx + 1 << ". Waga: " << weights[idx] 
             << ", Wartosc: " << values[idx] << "\n";
        totalWeight += weights[idx];
    }
    cout << "  Laczna waga: " << totalWeight << " / " << capacity << "\n";
}

/**
 * @brief Wczytuje dane od użytkownika i sprawdza ich poprawność
 * @param weights Tablica wag przedmiotów
 * @param values Tablica wartości przedmiotów
 * @param capacity Pojemność plecaka
 * @param n Liczba przedmiotów
 * @return true jeśli wczytywanie powiodło się, false w przypadku błędu
 */
bool readInput(vector<int>& weights, vector<long long>& values, int& capacity, int& n) {
    cout << "Podaj liczbe przedmiotow: ";
    if (!(cin >> n) || n < 0) {
        cout << "Nieprawidlowa liczba przedmiotow (musi byc >= 0)!\n";
        return false;
    }
    
    weights.resize(n);
    values.resize(n);
    
    if (n > 0) {
        cout << "Podaj wagi i wartosci przedmiotow:\n";
        for (int i = 0; i < n; i++) {
            cout << "Przedmiot " << i + 1 << ": ";
            if (!(cin >> weights[i] >> values[i]) || weights[i] <= 0 || values[i] < 0) {
                cout << "Nieprawidlowe dane! Waga musi byc dodatnia (> 0), a wartosc nieujemna (>= 0).\n";
                return false;
            }
        }
    }
    
    cout << "Podaj pojemnosc plecaka: ";
    if (!(cin >> capacity) || capacity < 0) {
        cout << "Nieprawidlowa pojemnosc plecaka (musi byc >= 0)!\n";
        return false;
    }
    
    return true;
}

int main() {
    int n = 0;
    int capacity = 0;
    vector<int> weights;
    vector<long long> values;
    
    cout << "=== Problem plecakowy ===\n\n";
    
    // Wczytujemy dane i przerywamy program w razie błędu
    if (!readInput(weights, values, capacity, n)) {
        return 1;
    }
    
    // Obliczamy maksymalną wartość i zapisujemy stan w tablicy dp
    vector<vector<long long>> dp;
    long long maxValue = knapsack(weights, values, capacity, n, dp);
    
    // Wypisujemy wynik
    cout << "\n=== Wynik ===\n";
    cout << "Maksymalna wartosc: " << maxValue << "\n";
    cout << "Pojemnosc plecaka: " << capacity << "\n";
    
    // Wypisujemy zawartość plecaka korzystając z już policzonej tablicy dp
    printItems(weights, values, capacity, n, dp);
    
    return 0;
}