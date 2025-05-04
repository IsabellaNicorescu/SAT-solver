// USE C++ 17

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

typedef set<int> Clause;
typedef vector<Clause> CNFFormula;

// === CITIREA ===
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

// === FUNCTII AJUTATOARE ===
void remove_literal(CNFFormula& formula, int literal) {
	for (auto& clause : formula) {
		clause.erase(literal);
	}
}

void remove_clauses_with_literal(CNFFormula& formula, int literal) {
	formula.erase(
	remove_if(formula.begin(), formula.end(), [literal](const Clause& c) {
		return c.count(literal) > 0;
	}),
	formula.end()
	);
}

// GDseHte o clauzD unitarD (cu un singur literal)
int find_unit_clause(const CNFFormula& formula) {
	for (const auto& clause : formula) {
		if (clause.size() == 1) {
			return *clause.begin();
		}
	}
	return 0; // 0 C.nseamnD cD nu existD
}

// GDseHte un literal pur (apare doar cu un semn)
int find_pure_literal(const CNFFormula& formula) {
	map<int, bool> seen;

	for (const auto& clause : formula) {
		for (int lit : clause) {
			seen[lit] = true;
		}
	}

	for (auto it = seen.begin(); it != seen.end(); ++it) {
		int lit = it->first;
		if (seen.count(-lit) == 0) {
			return lit;
		}
	}

	return 0; // nu existD literal pur
}

// === DPLL ===
bool dpll(CNFFormula formula) {
	// 1. DacD existD clauzD vidD => NESAT
	for (const auto& clause : formula) {
		if (clause.empty()) {
			return false;
		}
	}

	// 2. DacD nu mai sunt clauze => SAT
	if (formula.empty()) {
		return true;
	}

	// 3. Eliminare clauze unitare
	int unit = find_unit_clause(formula);
	if (unit != 0) {
		remove_clauses_with_literal(formula, unit);
		remove_literal(formula, -unit);
		return dpll(formula);
	}

	// 4. Eliminare literali puri
	int pure = find_pure_literal(formula);
	if (pure != 0) {
		remove_clauses_with_literal(formula, pure);
		return dpll(formula);
	}

	// 5. Alegere literal arbitrar Hi recursiv
	int chosen = *formula[0].begin();

	CNFFormula formula_true = formula;
	remove_clauses_with_literal(formula_true, chosen);
	remove_literal(formula_true, -chosen);
	if (dpll(formula_true)) return true;

	CNFFormula formula_false = formula;
	remove_clauses_with_literal(formula_false, -chosen);
	remove_literal(formula_false, chosen);
	return dpll(formula_false);
}

int main() {
	CNFFormula formula = read_formula_from_user();
	if (dpll(formula)) {
		cout << "\nFormula este SATISFIABILA.\n";
	} else {
		cout << "\nFormula este NESATISFIABILA.\n";
	}
	return 0;
}