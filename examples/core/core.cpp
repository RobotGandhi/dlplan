#include "../../include/dlplan/core.h"

#include <iostream>

using namespace dlplan::core;

static std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> v = std::make_shared<VocabularyInfo>();
    // Add predicates and constants of the domain.
    // Note that there are no constants in Blocksworld.
    // v->add_predicate("on", 2);
    // v->add_predicate("on_g", 2);
    // v->add_predicate("ontable", 1);
    // v->add_predicate("holding", 1);
    // v->add_predicate("clear", 1);
    // v->add_predicate("arm-empty", 0);

    v->add_predicate("road", 2);
    v->add_predicate("at", 2);
    v->add_predicate("at_g", 2);
    v->add_predicate("in", 2);
    v->add_predicate("in_g", 2);
    v->add_predicate("capacity", 2);
    v->add_predicate("capacity_predecessor", 2);
    v->add_predicate("is_truck", 1);
    v->add_predicate("is_box", 1);
    return v;
}

static std::shared_ptr<InstanceInfo> construct_instance_info(std::shared_ptr<VocabularyInfo> v) {
    std::shared_ptr<InstanceInfo> i = std::make_shared<InstanceInfo>(v);
    // Add dynamic atoms
    // i->add_atom("on", {"a", "b"});
    // i->add_atom("on", {"b", "a"});
    // i->add_atom("ontable", {"a"});
    // i->add_atom("ontable", {"b"});
    // i->add_atom("holding", {"a"});
    // i->add_atom("holding", {"b"});
    // i->add_atom("clear", {"a"});
    // i->add_atom("clear", {"b"});
    // i->add_atom("arm-empty", {});
    i->add_atom("at", {"box_a", "loc_a"});
    i->add_atom("at", {"truck_a", "loc_a"});
    // Add static goal atoms
    // i->add_static_atom("on_g", {"a", "b"});

    i->add_static_atom("at_g", {"box_a", "loc_b"});
    i->add_static_atom("in_g", {});
    // Add static atoms
    i->add_static_atom("road", {"loc_a", "loc_b"});
    i->add_static_atom("is_truck", {"truck_a"});
    i->add_static_atom("is_box", {"box_a"});
    i->add_static_atom("capacity", {"truck_a", "1"});
    i->add_static_atom("capacity_predecessor", {"0", "1"});
    // Note that there are no static atoms in Blocksworld.
    return i;
}


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 1. Initialize VocabularyInfo
    auto v = construct_vocabulary_info();
    // 2. Initialize InstanceInfo
    auto i = construct_instance_info(v);
    // 3. Initialize SyntacticElementFactory
    SyntacticElementFactory f(v);

    // 4. Construct a state.
    const auto& atoms = i->get_atoms_ref();
    // const Atom& a0 = atoms[0];
    // const Atom& a3 = atoms[3];
    // const Atom& a6 = atoms[6];
    // State state(i, {a0, a3, a6});

    // 5. Parse and evaluate elements.
    // Numerical numerical = f.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    // std::cout << "repr: " << numerical.compute_repr() << std::endl;
    // std::cout << "value: " << numerical.evaluate(state) << std::endl;

    // Boolean boolean = f.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    // std::cout << "repr: " << boolean.compute_repr() << std::endl;
    // std::cout << "value: " << boolean.evaluate(state) << std::endl;

    Boolean feature1 = f.parse_boolean("b_empty(c_primitive(in,0))");

    Numerical feature2 = f.parse_numerical("n_count(c_not(c_and(r_primitive(at_g,0,1),r_primitive(at,0,1))))");

    Numerical feature3 = f.parse_numerical("n_concept_distance(c_some(r_inverse(at),is_truck(0)),r_primitive(road,0,1),c_and(c_all(r_inverse(at_g),c_bot),c_some(r_inverse(at),is_box(0))))");

    Numerical feature4 = f.parse_numerical("n_concept_distance(c_primitive(in,1),r_primitive(road,0,1),c_some(r_primitive(at_g,0,1),c_primitive(in,0)))");

    return 0;
}
