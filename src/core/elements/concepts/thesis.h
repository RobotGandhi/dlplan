#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_THESIS_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_THESIS_H_

#include "../../../../include/dlplan/core.h"
#include "../utils.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core::element {

class ThesisConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& concept_from_denot, const RoleDenotation& connection_denot, const ConceptDenotation& concept_to_denot, const ConceptDenotation& concept_painted_denot, ConceptDenotation& denotation, int& num_objects) const {
        int best_cell = -1;
        int best_distance = -1;

        for (auto potential_cell_id : concept_from_denot) {
            ConceptDenotation potential_cell(num_objects);
            potential_cell.insert(potential_cell_id);
            utils::Distances source_distances = utils::compute_multi_source_multi_target_shortest_distances(potential_cell, connection_denot, concept_to_denot);

            int result = 0;

            for (const auto target : concept_to_denot) {
                result = utils::path_addition(result, source_distances[target]);
            }

            if (result < INF && result >= best_distance) {
                best_distance = result;
                best_cell = potential_cell_id;
            }
        }

        if(best_cell != -1) {
            ConceptDenotation best_cell_denot(num_objects);
            best_cell_denot.insert(best_cell);
            utils::Distances source_distances = utils::compute_multi_source_multi_target_shortest_distances(best_cell_denot, connection_denot, concept_painted_denot);

            for (auto potential_cell_id : concept_painted_denot) {
                if (source_distances[potential_cell_id] == 1){
                    denotation.insert(potential_cell_id);
                    break;
                }
            }
        }

    }

    std::unique_ptr<ConceptDenotation> evaluate_impl(const State& state, DenotationsCaches& cache) const override { //TODO: Change to 3 inputs similar to distance, use sum_concept_distance to get the distance to all sources, return concept predicate of cell with smallest sum.
        int num_objects = state.get_instance_info()->get_num_objects();
        auto denotation = std::make_unique<ConceptDenotation>(ConceptDenotation(num_objects));

        auto concept_from_denot = *m_from_concept->evaluate(state, cache);
        auto connection_denot = *m_connection->evaluate(state, cache);
        auto concept_to_denot = *m_to_concept->evaluate(state, cache);
        auto concept_painted_denot = *m_painted_concept->evaluate(state, cache);

        compute_result(concept_from_denot, connection_denot, concept_to_denot, concept_painted_denot, *denotation, num_objects);

        return denotation;
    }

    std::unique_ptr<ConceptDenotations> evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        auto denotations = std::make_unique<ConceptDenotations>();
        denotations->reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = evaluate_impl(states[i], caches);
            denotations->push_back(caches.m_c_denot_cache.insert(std::move(denotation)).first->get());
        }
        return denotations;
    }

protected:
    const Concept_Ptr m_from_concept;
    const Role_Ptr m_connection;
    const Concept_Ptr m_to_concept;
    const Concept_Ptr m_painted_concept;

public:
    ThesisConcept(const VocabularyInfo& vocabulary_info, Concept_Ptr from_concept, Role_Ptr connection, Concept_Ptr to_concept, Concept_Ptr painted_concept)
    : Concept(vocabulary_info, from_concept->get_is_static() && connection->get_is_static() && to_concept->get_is_static() && painted_concept->get_is_static()), m_from_concept(from_concept), m_connection(connection), m_to_concept(to_concept), m_painted_concept(painted_concept) {
        if (!from_concept) {
            throw std::runtime_error("ThesisConcept::ThesisConcept - from_concept is a nullptr");
        }        
        if (!connection) {
            throw std::runtime_error("ThesisConcept::ThesisConcept - connection is a nullptr");
        }        
        if (!to_concept) {
            throw std::runtime_error("ThesisConcept::ThesisConcept - to_concept is a nullptr");
        }        
        if (!painted_concept) {
            throw std::runtime_error("ThesisConcept::ThesisConcept - painted_concept is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_num_objects();
        ConceptDenotation denotation(num_objects);
        auto concept_from_denot = m_from_concept->evaluate(state);
        auto concept_to_denot = m_to_concept->evaluate(state);
        auto connection_denot = m_connection->evaluate(state);
        auto concept_painted_denot = m_painted_concept->evaluate(state);
        compute_result(concept_from_denot, connection_denot, concept_to_denot, concept_painted_denot, denotation, num_objects);
        return denotation;
    }

    int compute_complexity() const override {
        return m_from_concept->compute_complexity() + m_connection->compute_complexity() + m_to_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_from_concept->compute_repr(out);
        out << ",";
        m_connection->compute_repr(out);
        out << ",";
        m_to_concept->compute_repr(out);
        out << ",";
        m_painted_concept->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "c_thesis";
    }
};

}

#endif
