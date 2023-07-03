#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_THESIS_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_THESIS_H_

#include "../../../../include/dlplan/core.h"
#include "../utils.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {

class ThesisConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& concept_from_denot, const RoleDenotation& connection_denot, const ConceptDenotation& concept_to_denot, const ConceptDenotation& concept_painted_denot, const ConceptDenotation& concept_robot_denot, ConceptDenotation& denotation, int& num_objects) const {
        int best_cell = -1;
        int best_distance = -1;

        for (auto potential_cell_id : concept_from_denot.to_vector()) {
            ConceptDenotation potential_cell(num_objects);
            potential_cell.insert(potential_cell_id);
            utils::Distances source_distances = utils::compute_multi_source_multi_target_shortest_distances(potential_cell, connection_denot, concept_to_denot);

            int result = 0;

            for (const auto target : concept_to_denot.to_vector()) {
                result = utils::path_addition(result, source_distances[target]);
            }

            if (result < INF && result >= best_distance) {
                best_distance = result;
                best_cell = potential_cell_id;
            }
        }

        if(best_cell != -1) {
            ConceptDenotation potential_cell_denot(num_objects);
            ConceptDenotation best_cell_denot(num_objects);
            best_cell_denot.insert(best_cell);
            utils::Distances source_distances = utils::compute_multi_source_multi_target_shortest_distances(best_cell_denot, connection_denot, concept_painted_denot);

            for (auto potential_cell_id : concept_painted_denot.to_vector()) {
                if (source_distances[potential_cell_id] == 1){
                    potential_cell_denot.insert(potential_cell_id);
                    break;
                }
            }
            utils::Distances potential_distances = utils::compute_multi_source_multi_target_shortest_distances(concept_robot_denot, connection_denot, potential_cell_denot);
            int best_adjacent_cell = -1;
            int best_distance = INF;
            for (auto potential_cell_id : potential_cell_denot.to_vector()) {
                int result = source_distances[potential_cell_id];
                if (result < best_distance) {
                    best_distance = result;
                    best_adjacent_cell = potential_cell_id;
                }
            }

            denotation.insert(best_adjacent_cell);
        }

    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& cache) const override { //TODO: Change to 3 inputs similar to distance, use sum_concept_distance to get the distance to all sources, return concept predicate of cell with smallest sum.
        int num_objects = state.get_instance_info()->get_objects().size();
        ConceptDenotation denotation(num_objects);

        auto concept_from_denot = *m_from_concept->evaluate(state, cache);
        auto connection_denot = *m_connection->evaluate(state, cache);
        auto concept_to_denot = *m_to_concept->evaluate(state, cache);
        auto concept_painted_denot = *m_painted_concept->evaluate(state, cache);
        auto concept_robot_denot = *m_robot_concept->evaluate(state, cache);

        compute_result(concept_from_denot, connection_denot, concept_to_denot, concept_painted_denot, concept_robot_denot, denotation, num_objects);

        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            auto denotation = evaluate_impl(states[i], caches);
            denotations.push_back(caches.get_concept_denotation_cache().insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

protected:
    const std::shared_ptr<const Concept> m_from_concept;
    const std::shared_ptr<const Role> m_connection;
    const std::shared_ptr<const Concept> m_to_concept;
    const std::shared_ptr<const Concept> m_painted_concept;
    const std::shared_ptr<const Concept> m_robot_concept;

public:
    ThesisConcept(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> from_concept, std::shared_ptr<const Role> connection, std::shared_ptr<const Concept> to_concept, std::shared_ptr<const Concept> painted_concept, std::shared_ptr<const Concept> robot_concept)
    : Concept(vocabulary_info, from_concept->is_static() && connection->is_static() && to_concept->is_static() && painted_concept->is_static() && robot_concept->is_static()), m_from_concept(from_concept), m_connection(connection), m_to_concept(to_concept), m_painted_concept(painted_concept), m_robot_concept(robot_concept) {
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
        if (!robot_concept) {
            throw std::runtime_error("ThesisConcept::ThesisConcept - robot_concept is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        int num_objects = state.get_instance_info()->get_objects().size();
        ConceptDenotation denotation(num_objects);
        auto concept_from_denot = m_from_concept->evaluate(state);
        auto concept_to_denot = m_to_concept->evaluate(state);
        auto connection_denot = m_connection->evaluate(state);
        auto concept_painted_denot = m_painted_concept->evaluate(state);
        auto concept_robot_denot = m_robot_concept->evaluate(state);
        compute_result(concept_from_denot, connection_denot, concept_to_denot, concept_painted_denot, concept_robot_denot, denotation, num_objects);
        return denotation;
    }

    int compute_complexity() const override {
        return m_from_concept->compute_complexity() + m_connection->compute_complexity() + m_to_concept->compute_complexity() + m_painted_concept->compute_complexity() + m_robot_concept->compute_complexity() + 1;
    }

    int compute_evaluate_time_score() const override {
        return m_from_concept->compute_evaluate_time_score() + m_connection->compute_evaluate_time_score() + m_to_concept->compute_evaluate_time_score() + m_painted_concept->compute_evaluate_time_score() + m_robot_concept->compute_evaluate_time_score() + 1;
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
