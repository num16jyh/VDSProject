#include "Manager.h"

namespace ClassProject {

Manager::Manager()
{
    // Initialize table with False and True nodes.
    const TableEntry false_entry = {"False", falseID, falseID, falseID, falseID};
    addTableEntry(false_entry);

    const TableEntry true_entry = {"True", trueID, trueID, trueID, trueID};
    addTableEntry(true_entry);
}

BDD_ID Manager::createVar(const std::string &label)
{
    const BDD_ID id = table_vector.size();
    const TableEntry new_entry = {label, id, trueID, falseID, id};
    addTableEntry(new_entry);

    return id;
}

const BDD_ID &Manager::True()
{
    return trueID;
}

const BDD_ID &Manager::False()
{
    return falseID;
}

bool Manager::isConstant(BDD_ID f)
{
    return (f == trueID || f == falseID);
}

bool Manager::isVariable(BDD_ID x)
{
    if (x == trueID || x == falseID)
        return false;

    return x < table_vector.size();
}

BDD_ID Manager::topVar(BDD_ID f)
{
    if (f < table_vector.size())
        return table_vector[f].top;

    return f;
}

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    if (isConstant(f) || topVar(f) != x)
        return f;

    if (f < table_vector.size())
        return table_vector[f].high;

    return f;
}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{
    if (isConstant(f) || topVar(f) != x)
        return f;

    if (f < table_vector.size())
        return table_vector[f].low;

    return f;
}

BDD_ID Manager::coFactorTrue(BDD_ID f)
{
    return coFactorTrue(f, topVar(f));
}

BDD_ID Manager::coFactorFalse(BDD_ID f)
{
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
