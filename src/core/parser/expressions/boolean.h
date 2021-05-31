#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_H_

#include "expression.h"

#include "../../elements/boolean.h"


namespace dlp {
namespace core {
namespace parser {

class BooleanExpression : public Expression {
protected:
    /**
     * Construct the Boolean.
     */
    virtual element::Boolean_Ptr parse_boolean_element_impl(ElementCache &cache) const = 0;

public:
    BooleanExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Expression(name, std::move(children)) { }

    /**
     * Construct or retrieve the Boolean.
     */
    virtual element::Boolean_Ptr parse_boolean_element(ElementCache &cache) const {
        element::Boolean_Ptr value = parse_boolean_element_impl(cache);
        return cache.boolean_element_cache().insert(std::make_pair(value->compute_repr(), std::move(value))).first->second;
    }
};

}
}
}

#endif
