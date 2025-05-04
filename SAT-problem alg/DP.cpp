// USE C++ 17

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

typedef set<int> Clause;
typedef vector<Clause> CNFFormula;

// Citire formula de la tastatură
CNFFormula read_formula_from_user() {
    CNFFormula formula;
    Clause clause;
    int literal;
    int num_clauses;

    cout << "Introdu numarul de clauze: ";
    cin >> num_clauses;
    cout << "Introdu fiecare clauza (literali intregi), terminata cu 0:\n";

    for (int i = 0; i < num_clauses; ++i) {
        clause.clear();
        while (cin >> literal) {
            if (literal == 0) break;
            clause.insert(literal);
        }
        formula.push_back(clause);
    }

    return formula;
}

// Elimină literalul specific din toate clauzele
void remove_literal(CNFFormula& formula, int literal) {
    for (auto& clause : formula) {
        clause.erase(literal);
    }
}

// Elimina clauzele care conțin literalul specific
void remove_clauses_with_literal(CNFFormula& formula, int literal) {
    formula.erase(
        remove_if(formula.begin(), formula.end(), [literal](const Clause& c) {
            return c.count(literal) > 0;
        }),
        formula.end()
    );
}

// Algoritmul Davis–Putnam (DP)
bool davis_putnam(CNFFormula formula) {
    while (true) {
        // 1. Eliminare clauze unitare (pur simplificare)
        bool simplified = false;
        map<int, int> literal_count;

        for (const auto& clause : formula) {
            for (int lit : clause) {
                literal_count[lit]++;
            }
        }

        // 2. Eliminare literali puri (care apar doar cu un semn)
        for (const auto& [lit, _] : literal_count) {
            if (literal_count.count(-lit) == 0) {
                // Literal pur gasit
                remove_clauses_with_literal(formula, lit);
                simplified = true;
                break;
            }
        }

        if (simplified) continue;

        // 3. Clauză vidă => formula e nesatisfiabilă
        for (const auto& clause : formula) {
            if (clause.empty()) {
                cout << "\nFormula este NESATISFIABILA (clauza vida).\n";
                return false;
            }
        }

        // 4. Dacă nu mai sunt clauze => formula satisfiabilă
        if (formula.empty()) {
            cout << "\nFormula este SATISFIABILA.\n";
            return true;
        }

        // 5. Alegem un literal arbitrar
        int chosen_lit = *(formula[0].begin());

        // 6. Ramura A: presupunem literalul e adevărat
        CNFFormula formula_true = formula;
        remove_clauses_with_literal(formula_true, chosen_lit);
        remove_literal(formula_true, -chosen_lit);
        if (davis_putnam(formula_true)) return true;

        // 7. Ramura B: presupunem literalul e fals
        CNFFormula formula_false = formula;
        remove_clauses_with_literal(formula_false, -chosen_lit);
        remove_literal(formula_false, chosen_lit);
        return davis_putnam(formula_false);
    }
}

int main() {
    CNFFormula formula = read_formula_from_user();
    davis_putnam(formula);
    return 0;
}
