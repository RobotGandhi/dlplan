#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, ConceptOr) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("concept_1", 1);
    auto predicate_1 = vocabulary->add_predicate("concept_2", 1);
    auto instance = std::make_shared<InstanceInfo>(vocabulary, 0);
    auto atom_0 = instance->add_atom("concept_1", {"A"});
    auto atom_1 = instance->add_atom("concept_1", {"B"});
    auto atom_2 = instance->add_atom("concept_2", {"B"});
    auto atom_3 = instance->add_atom("concept_2", {"C"});

    State state_0(instance, {atom_0, atom_1, atom_2, atom_3}, 0);

    SyntacticElementFactory factory(vocabulary);

    auto concept = factory.parse_concept("c_or(c_primitive(concept_1,0),c_primitive(concept_2,0))");
    EXPECT_EQ(concept->evaluate(state_0), create_concept_denotation(*instance, {"A", "B", "C"}));
}

}
