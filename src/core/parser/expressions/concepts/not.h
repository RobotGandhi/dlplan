#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_NOT_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_NOT_H_

#include "../concept.h"
#include "../../../elements/concepts/not.h"

namespace dlp {
namespace core {
namespace parser {

class NotConcept : public Concept {
protected:
    virtual element::Concept_Ptr parse_concept_impl(const VocabularyInfoImpl& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NotConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        element::Concept_Ptr l = m_children[0]->parse_concept(vocabulary, cache);
        if (!l) {
            throw std::runtime_error("NotConcept::parse_concept_impl - children are not of type Concept.");
        }
        // 2. Construct element
        return std::make_shared<element::NotConcept>(vocabulary, l);
    }

public:
    NotConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}
}
}

#endif
