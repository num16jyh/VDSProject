// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Created by Markus Wedler 2014

#ifndef VDSPROJECT_MANAGER_H
#define VDSPROJECT_MANAGER_H

#include "ManagerInterface.h"

#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>

namespace ClassProject {

using namespace std;

class Manager : public ManagerInterface
{
public:
    Manager();

    BDD_ID createVar(const std::string &label) override;

    const BDD_ID &True() override;

    const BDD_ID &False() override;

    bool isConstant(BDD_ID f) override;

    bool isVariable(BDD_ID x) override;

    BDD_ID topVar(BDD_ID f) override;

    BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

    BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;

    BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;

    BDD_ID coFactorTrue(BDD_ID f) override;

    BDD_ID coFactorFalse(BDD_ID f) override;

    BDD_ID and2(BDD_ID a, BDD_ID b) override;

    BDD_ID or2(BDD_ID a, BDD_ID b) override;

    BDD_ID xor2(BDD_ID a, BDD_ID b) override;

    BDD_ID neg(BDD_ID a) override;

    BDD_ID nand2(BDD_ID a, BDD_ID b) override;

    BDD_ID nor2(BDD_ID a, BDD_ID b) override;

    BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

    std::string getTopVarName(const BDD_ID &root) override;

    void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

    void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

    size_t uniqueTableSize() override;

    void visualizeBDD(std::string filepath, BDD_ID &root) override;

private:
    struct UniqueTableEntry
    {
        string label;
        BDD_ID id = 0;
        BDD_ID high;
        BDD_ID low;
        BDD_ID top;

        // For lookup by triple.
        bool operator==(const UniqueTableEntry &other) const
        {
            return (high == other.high) && (low == other.low) && (top == other.top);
        }
    };

    struct UniqueTableEntryHash
    {
        size_t operator()(const UniqueTableEntry& entry) const noexcept
        {
            // Combine the 21 least significant bits of high, low, and top IDs
            // into a single 64-bit unsigned integer in order to only hash once.
            // This causes ID values >= 2 ^ 21 (2097152) to break functionality.
            // In the presence of such IDs, the OBDD will not be properly
            // reduced.
            const size_t shift = 21;
            const uint64_t mask_lsb = 0x01FFFFFu;

            uint64_t three_in_one = entry.top & mask_lsb;
            three_in_one |= (entry.high & mask_lsb) << shift;
            three_in_one |= (entry.low & mask_lsb) << (2 * shift);

            return hash<uint64_t>{}(three_in_one);
        }
    };

    struct ComputedTableEntry
    {
        BDD_ID i;
        BDD_ID t;
        BDD_ID e;

        // For lookup by triple.
        bool operator==(const ComputedTableEntry &other) const
        {
            return (i == other.i) && (t == other.t) && (e == other.e);
        }
    };

    struct ComputedTableEntryHash
    {
        size_t operator()(const ComputedTableEntry& entry) const noexcept
        {
            // Same logic as for the unique table set hashing.
            const size_t shift = 21;
            const uint64_t mask_lsb = 0x01FFFFFu;

            uint64_t three_in_one = entry.i & mask_lsb;
            three_in_one |= (entry.t & mask_lsb) << shift;
            three_in_one |= (entry.e & mask_lsb) << (2 * shift);

            return hash<uint64_t>{}(three_in_one);
        }
    };

protected:
    vector<UniqueTableEntry> unique_table_vector; // for lookup by ID
    unordered_set<UniqueTableEntry, UniqueTableEntryHash> unique_table_set; // for lookup by triple
    unordered_map<ComputedTableEntry, BDD_ID, ComputedTableEntryHash> computed_table_map; // for caching ite results

    void addTableEntry(const UniqueTableEntry &new_entry)
    {
        unique_table_vector.push_back(new_entry);
        unique_table_set.insert(new_entry);
    }

    const BDD_ID FALSE_ID = 0;
    const BDD_ID TRUE_ID = 1;
};

} // namespace ClassProject

#endif
