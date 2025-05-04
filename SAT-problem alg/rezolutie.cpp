// USE C++ 17

#include <iostream>
#include <vector>
#include <set>

using namespace std;

typedef set<int> Clause;
typedef vector<Clause> CNFFormula;

// Citire de la tastatură, cu număr fix de clauze
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

// Aplică rezoluția pe două clauze
bool resolve(const Clause& c1, const Clause& c2, Clause& resolvent) {
    int count = 0;
    int pivot = 0;

    for (int lit : c1) {
        if (c2.count(-lit)) {
            ++count;
            pivot = lit;
        }
    }

    if (count != 1) return false;

    resolvent = c1;
    resolvent.insert(c2.begin(), c2.end());
    resolvent.erase(pivot);
    resolvent.erase(-pivot);

    return true;
}

// Algoritmul de rezoluție
bool resolution(CNFFormula formula) {
    set<Clause> clauses(formula.begin(), formula.end());
    set<Clause> new_clauses;

    while (true) {
        vector<Clause> clause_list(clauses.begin(), clauses.end());
        int n = clause_list.size();

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                Clause resolvent;
                if (resolve(clause_list[i], clause_list[j], resolvent)) {
                    if (resolvent.empty()) {
                        cout << "\nFormula este NESATISFIABILA (clauza vida gasita).\n";
                        return false;
                    }
                    if (!clauses.count(resolvent)) {
                        new_clauses.insert(resolvent);
                    }
                }
            }
        }

        if (new_clauses.empty()) {
            cout << "\nFormula este SATISFIABILA (nu s-a putut deduce clauza vida).\n";
            return true;
        }

        for (const auto& c : new_clauses)
            clauses.insert(c);

        new_clauses.clear();
    }
}

// Punctul de intrare
int main() {
    CNFFormula formula = read_formula_from_user();
    resolution(formula);
    return 0;
}