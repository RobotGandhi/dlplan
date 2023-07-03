#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_THESIS_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_THESIS_H_

#include "../concept.h"
#include "../../../elements/concepts/thesis.h"

namespace dlplan::core::parser {

class ThesisConcept : public Concept {
protected:
    std::unique_ptr<dlplan::core::Concept> parse_concept_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches& cache) const override {
        if (m_children.size() != 5) {
            throw std::runtime_error("ThesisConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 5).");
        }
        // 1. Parse children
        auto l = m_children[0]->parse_concept(vocabulary_info, cache);
        auto m = m_children[1]->parse_role(vocabulary_info, cache);
        auto r = m_children[2]->parse_concept(vocabulary_info, cache);
        auto p = m_children[3]->parse_concept(vocabulary_info, cache);
        auto robot = m_children[4]->parse_concept(vocabulary_info, cache);
        if (!l || !m || !r || !p || !robot) {
            throw std::runtime_error("ThesisConcept::parse_concept_impl - children are not of type Concept, Role, Concept, Concept.");
        }
        // 2. Construct element
        return std::make_unique<dlplan::core::ThesisConcept>(vocabulary_info, l, m, r, p, robot);
    }

public:
    ThesisConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
