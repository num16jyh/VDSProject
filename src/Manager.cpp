#include "Manager.h"
#include <algorithm>
#include <fstream>

namespace ClassProject {

Manager::Manager()
{
    // Initialize table with False and True nodes.
    const TableEntry false_entry = {"False", FALSE_ID, FALSE_ID, FALSE_ID, FALSE_ID};
    addTableEntry(false_entry);

    const TableEntry true_entry = {"True", TRUE_ID, TRUE_ID, TRUE_ID, TRUE_ID};
    addTableEntry(true_entry);
}

BDD_ID Manager::createVar(const std::string &label)
{
    const BDD_ID id = table_vector.size();
    const TableEntry new_entry = {label, id, TRUE_ID, FALSE_ID, id};
    addTableEntry(new_entry);

    return id;
}

const BDD_ID &Manager::True()
{
    return TRUE_ID;
}

const BDD_ID &Manager::False()
{
    return FALSE_ID;
}

bool Manager::isConstant(BDD_ID f)
{
    return (f == TRUE_ID || f == FALSE_ID);
}

bool Manager::isVariable(BDD_ID x)
{
    if (x == TRUE_ID || x == FALSE_ID)
        return false;

    return (x < table_vector.size()) && (x == topVar(x));
}

BDD_ID Manager::topVar(BDD_ID f)
{
    if (f < table_vector.size())
        return table_vector[f].top;

    return f;
}

BDD_ID Manager::ite(BDD_ID i, BDD_ID t, BDD_ID e)
{
    if (i == TRUE_ID)
        return t;

    if (i == FALSE_ID)
        return e;

    if (!isConstant(t) && (topVar(t) < topVar(i)))
        return ite(t, ite(i, coFactorTrue(t), coFactorTrue(e, topVar(t))),
                   ite(i, coFactorFalse(t), coFactorFalse(e, topVar(t))));

    if (!isConstant(e) && (topVar(e) < topVar(i)))
        return ite(e, ite(i, coFactorTrue(t, topVar(e)), coFactorTrue(e)),
                   ite(i, coFactorFalse(t, topVar(e)), coFactorFalse(e)));

    BDD_ID x = topVar(i);
    BDD_ID high = ite(coFactorTrue(i), coFactorTrue(t, x), coFactorTrue(e, x));
    BDD_ID low = ite(coFactorFalse(i), coFactorFalse(t, x), coFactorFalse(e, x));

    if (high == low) // reduce
        return high;

    const BDD_ID id = table_vector.size();

    const TableEntry entry = {.id = id, .high = high, .low = low, .top = x};

    auto ret = table_set.insert(entry); // will insert only if top/high/low triple does not exist

    if (ret.second) // was inserted
    {
        table_vector.push_back(entry);
        return id;
    }

    return ret.first->id; // was not inserted, already existed
}

BDD_ID Manager::coFactorTrue(BDD_ID f, BDD_ID x)
{
    //if (isConstant(f) || (topVar(x) < topVar(f)))
    if (isConstant(f))
        return f;

    if (f < table_vector.size()) // if f's ID exists in table
    {
        if (topVar(f) == x)
            return table_vector[f].high;

        return ite(topVar(f), coFactorTrue(table_vector[f].high, x),
                   coFactorTrue(table_vector[f].low, x));
    }

    return f;
}

BDD_ID Manager::coFactorFalse(BDD_ID f, BDD_ID x)
{
    //if (isConstant(f) || (topVar(x) < topVar(f)))
    if (isConstant(f))
        return f;

    if (f < table_vector.size()) // if f's ID exists in table
    {
        if (topVar(f) == x)
            return table_vector[f].low;

        return ite(topVar(f), coFactorFalse(table_vector[f].high, x),
                   coFactorFalse(table_vector[f].low, x));
    }

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
{
    return ite(a, b, FALSE_ID);
}

BDD_ID Manager::or2(BDD_ID a, BDD_ID b)
{
    return ite(a, TRUE_ID, b);
}

BDD_ID Manager::xor2(BDD_ID a, BDD_ID b)
{
    return ite(a, neg(b), b);
}

BDD_ID Manager::neg(BDD_ID a)
{
    return ite(a, FALSE_ID, TRUE_ID);
}

BDD_ID Manager::nand2(BDD_ID a, BDD_ID b)
{
    return neg(and2(a, b));
}

BDD_ID Manager::nor2(BDD_ID a, BDD_ID b)
{
    return neg(or2(a, b));
}

BDD_ID Manager::xnor2(BDD_ID a, BDD_ID b)
{
    return neg(xor2(a, b));
}

std::string Manager::getTopVarName(const BDD_ID &root)
{
    const BDD_ID top_id = topVar(root);

    if (top_id >= table_vector.size())
        return "UNKNOWN";

    return table_vector[top_id].label;
}

void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
{
    nodes_of_root.insert(root);

    if (!isConstant(root))
    {
        findNodes(coFactorTrue(root), nodes_of_root);
        findNodes(coFactorFalse(root), nodes_of_root);
    }
}

void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
{
    if (!isConstant(root))
    {
        vars_of_root.insert(topVar(root));

        if (root != topVar(root))
        {
            findVars(coFactorTrue(root), vars_of_root);
            findVars(coFactorFalse(root), vars_of_root);
        }
    }
}

size_t Manager::uniqueTableSize()
{
    return table_vector.size();
}

void Manager::visualizeBDD(std::string filepath, BDD_ID &root)
{
    std::ofstream file(filepath, std::ios::trunc);
    const std::string indent = "    ";

    file << "graph BDD {\n\n"
         << indent << "node[shape=circle]\n\n"
         << indent << "False [shape=rectangle]\n"
         << indent << "True [shape=rectangle]\n\n";

    // For fetching the ID of each node
    std::set<BDD_ID> nodes;
    findNodes(root, nodes);

    for (auto node : nodes)
    {
        if (!isConstant(node))
        {
            file << indent << getTopVarName(node) << " -- " << getTopVarName(coFactorTrue(node))
                 << "\n";
            file << indent << getTopVarName(node) << " -- " << getTopVarName(coFactorFalse(node))
                 << " [style=dashed]\n";
        }
    }

    file << "\n}\n";
}

} // namespace ClassProject
