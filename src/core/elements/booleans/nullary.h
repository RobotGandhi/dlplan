#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class NullaryBoolean : public Boolean {
private:
    void compute_result(const State& state, bool& result) const {
        const auto& atoms = state.get_instance_info()->get_atoms();
        for (int atom_idx : state.get_atom_indices()) {
            const auto& atom = atoms[atom_idx];
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                result = true;
                return;
            }
        }
        for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                result = true;
                return;
            }
        }
        result = false;
    }

    bool evaluate_impl(const State& state, DenotationsCaches&) const override {
        return evaluate(state);
    }

    BooleanDenotations
    evaluate_impl(const States& states, DenotationsCaches&) const override {
        BooleanDenotations denotations;
        for (size_t i = 0; i < states.size(); ++i) {
            denotations.push_back(evaluate(states[i]));
        }
        return denotations;
    }

protected:
    const Predicate m_predicate;

public:
    NullaryBoolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, const Predicate& predicate)
    : Boolean(vocabulary_info, predicate.is_static()), m_predicate(predicate) {
        if (predicate.get_arity() != 0) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
        }
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(" << m_predicate.get_name() << ")";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_LINEAR;
    }

    static std::string get_name() {
        return "b_nullary";
    }
};

}

#endif
