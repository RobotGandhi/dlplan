#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_THESIS_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_THESIS_H_

#include "../concept.h"
#include "../../../elements/concepts/thesis.h"

namespace dlplan::core::parser {

class ThesisConcept : public Concept {
protected:
    std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary_info, Caches& cache) const override {
        if (m_children.size() != 4) {
            throw std::runtime_error("ThesisConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 4).");
        }
        // 1. Parse children
        element::Concept_Ptr l = m_children[0]->parse_concept(vocabulary_info, cache);
        element::Role_Ptr m = m_children[1]->parse_role(vocabulary_info, cache);
        element::Concept_Ptr r = m_children[2]->parse_concept(vocabulary_info, cache);
        element::Concept_Ptr p = m_children[3]->parse_concept(vocabulary_info, cache);
        if (!l || !m || !r || !p) {
            throw std::runtime_error("ThesisConcept::parse_concept_impl - children are not of type Concept, Role, Concept, Concept.");
        }
        // 2. Construct element
        return std::make_unique<element::ThesisConcept>(vocabulary_info, l, m, r, p);
    }

public:
    ThesisConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
