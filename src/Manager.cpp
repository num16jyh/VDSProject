#include "Manager.h"

namespace ClassProject {
    Manager::Manager() {
        // Initialize the uniqueTable with False and True nodes
        uniqueTable.insert({falseID, 0, 0, falseID, "False"}); // Correct label
        uniqueTable.insert({trueID, 1, 1, trueID, "True"});    // Correct label
    }
    BDD_ID Manager::createVar(const std::string &label) {
        // Check if the variable name (label) already exists
        for (const auto &node : uniqueTable) {
            if (node.varName == label) { // Use 'varname' instead of 'label'
                return node.id; // Return the existing ID
            }
        }

        // Create a new variable
        BDD_ID id = uniqueTable.size();
        uniqueTable.insert({id, trueID, falseID, id, label}); // Use 'label' to initialize 'varname'
        return id;
    }


    const BDD_ID &Manager::True() {
        return trueID;
    }

    const BDD_ID &Manager::False() {
        return falseID;
    }

    bool Manager::isConstant(BDD_ID f) {
        return (f == trueID || f == falseID);
    }

    bool Manager::isVariable(BDD_ID x) {
        return (x != trueID && x != falseID);
    }

    BDD_ID Manager::topVar(BDD_ID f) {
        for (const auto &node : uniqueTable) {
            if (node.id == f) {
                return node.topVar;
            }
        }
        return falseID;
    }

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{}

    BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x) {
        if (isConstant(f) || topVar(f) != x) return f;

        for (const auto &node : uniqueTable) {
            if (node.id == f) {
                return node.high;
            }
        }
        return falseID;
    }

    BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x) {
        if (isConstant(f) || topVar(f) != x) return f;

        for (const auto &node : uniqueTable) {
            if (node.id == f) {
                return node.low;
            }
        }
        return falseID;
    }

    BDD_ID Manager::coFactorTrue(BDD_ID f) {
        return coFactorTrue(f, topVar(f));
    }

    BDD_ID Manager::coFactorFalse(BDD_ID f) {
        return coFactorFalse(f, topVar(f));
    }

BDD_ID Manager::and2(BDD_ID a, BDD_ID b)
{}

BDD_ID Manager::or2(BDD_ID a, BDD_ID b)
{}

BDD_ID Manager::xor2(BDD_ID a, BDD_ID b)
{}

BDD_ID Manager::neg(BDD_ID a)
{}

BDD_ID Manager::nand2(BDD_ID a, BDD_ID b)
{}

BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
{}

BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{}

std::string Manager::getTopVarName(const BDD_ID &root)
{}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{}

size_t Manager::uniqueTableSize()
{}

void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{}

} // namespace ClassProject
