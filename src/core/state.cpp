#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include "../utils/collections.h"
#include "../utils/logging.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>


namespace dlplan::core {

static AtomIndices compute_sorted_atom_idxs(const AtomIndices& atom_idxs) {
    AtomIndices sorted_atom_idxs(atom_idxs);
    std::sort(sorted_atom_idxs.begin(), sorted_atom_idxs.end());
    return sorted_atom_idxs;
}

State::State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms, StateIndex index)
    : m_instance_info(instance_info), m_index(index) {
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return !atom.is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
    m_atom_indices.reserve(atoms.size());
    for (const auto& atom : atoms) {
        int atom_idx = atom.get_index();
        m_atom_indices.push_back(atom_idx);
    }
}

State::State(std::shared_ptr<const InstanceInfo> instance_info, const AtomIndices& atom_idxs, StateIndex index)
    : m_instance_info(instance_info), m_atom_indices(atom_idxs), m_index(index) {
    const auto& atoms = instance_info->get_atoms();
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, atoms); })) {
        throw std::runtime_error("State::State - atom index out of range.");
    }
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return !atoms[atom_idx].is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
}

State::State(const State&) = default;

State& State::operator=(const State&) = default;

State::State(State&& other) = default;

State& State::operator=(State&& other) = default;

State::~State() = default;

bool State::operator==(const State& other) const {
    return (compute_sorted_atom_idxs(get_atom_indices()) == compute_sorted_atom_idxs(other.get_atom_indices())) && (get_instance_info() == other.get_instance_info());
}

bool State::operator!=(const State& other) const {
    return !(*this == other);
}

std::shared_ptr<const InstanceInfo> State::get_instance_info() const {
    return m_instance_info;
}

const AtomIndices& State::get_atom_indices() const {
    return m_atom_indices;
}

StateIndex State::get_index() const {
    return m_index;
}

std::string State::compute_repr() const {
    std::stringstream ss;
    AtomIndices sorted_atom_indices = m_atom_indices;
    std::sort(sorted_atom_indices.begin(), sorted_atom_indices.end());
    ss << "State("
       << "index=" << m_index << ", "
       << "atom_indices=" << sorted_atom_indices
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const State& state) {
    os << state.compute_repr();
    return os;
}

std::string State::str() const {
    std::stringstream result;
    result << "(instance index=" << get_instance_info()->get_index()
           << ", state index=" << get_index()
           << ", atoms={";
    const auto& atoms = get_instance_info()->get_atoms();
    for (int atom_idx : m_atom_indices) {
        assert(dlplan::utils::in_bounds(atom_idx, atoms));
        const auto& atom = atoms[atom_idx];
        result << atom.get_name();
        if (atom_idx != m_atom_indices.back()) {
            result << ", ";
        }
    }
    result << "})";
    return result.str();
}

size_t State::hash() const {
    AtomIndices sorted_atom_idxs = compute_sorted_atom_idxs(m_atom_indices);
    size_t seed = sorted_atom_idxs.size();
    for (int atom_idx : sorted_atom_idxs) {
        utils::hash_combine(seed, atom_idx);
    }
    utils::hash_combine(seed, m_instance_info.get());
    return seed;
}

void State::set_index(StateIndex index) {
    m_index = index;
}

}
