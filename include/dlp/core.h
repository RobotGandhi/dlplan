#ifndef DLP_INCLUDE_DLP_CORE_H_
#define DLP_INCLUDE_DLP_CORE_H_

#include <memory>
#include <string>
#include <vector>

#include "../../src/core/elements/types.h"
#include "../../src/utils/pimpl.h"


namespace dlp {
namespace core {
class SyntacticElementFactoryImpl;
class InstanceInfoImpl;
class VocabularyInfoImpl;
class PredicateImpl;
class ObjectImpl;
class AtomImpl;
class StateImpl;
class ConceptImpl;
class RoleImpl;
class NumericalImpl;
class BooleanImpl;
class SyntacticElementFactory;
class InstanceInfo;
class VocabularyInfo;

using ConceptDenotation = std::vector<int>;

using RoleDenotation = std::vector<std::pair<int, int>>;

using Name_Vec = std::vector<std::string>;
using Index_Vec = std::vector<int>;


/**
 * A Predicate belongs to a specific vocabulary of a planning domain.
 */
class Predicate {
private:
    pimpl<PredicateImpl> m_pImpl;

    Predicate(const VocabularyInfo& vocabulary_info, const std::string& predicate_name, int predicate_idx, int arity);
    friend class VocabularyInfoImpl;

public:
    Predicate(const Predicate& other);
    ~Predicate();

    bool operator==(const Predicate& other);
    bool operator!=(const Predicate& other);

    /**
     * Getters.
     */
    const VocabularyInfo* get_vocabulary_info() const;
    int get_index() const;
    const std::string& get_name() const;
    int get_arity() const;
};


/**
 * An Object belongs to a specific instance.
 */
class Object {
private:
    pimpl<ObjectImpl> m_pImpl;

    Object(const InstanceInfo& instance_info, const std::string& object_name, int object_idx);
    friend class InstanceInfoImpl;

public:
    Object(const Object& other);
    ~Object();

    bool operator==(const Object& other);
    bool operator!=(const Object& other);

    const InstanceInfo* get_instance_info() const;
    int get_index() const;
    const std::string& get_name() const;
};


/**
 * An Atom belongs to a specific instance.
 */
class Atom {
private:
    pimpl<AtomImpl> m_pImpl;

    Atom(const InstanceInfo& instance_info,
        const std::string& atom_name,
        int atom_idx,
        const Predicate& predicate,
        const std::vector<Object> &objects,
        bool is_static);
    friend class InstanceInfoImpl;

public:
    Atom(const Atom& other);
    ~Atom();

    bool operator==(const Atom& other);
    bool operator!=(const Atom& other);

    /**
     * Getters.
     */
    const InstanceInfo* get_instance_info() const;
    const std::string& get_name() const;
    int get_index() const;
    const Predicate& get_predicate() const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int pos) const;
    bool get_is_static() const;
};


/**
 * A State contains consists of atoms including all static atoms.
 */
class State {
private:
    pimpl<StateImpl> m_pImpl;

public:
    State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms);
    State(std::shared_ptr<const InstanceInfo> instance_info, const Name_Vec& atom_names);
    State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_indices);
    State(const State& other);
    ~State();

    bool operator==(const State& other);
    bool operator!=(const State& other);

    /**
     * Computes string-like representation of the state.
     */
    std::string str() const;
    /**
     * Getters.
     */
    std::shared_ptr<const InstanceInfo> get_instance_info() const;
    const Index_Vec& get_atom_idxs() const;
};


/**
 * VocabularyInfo stores information related to the planning domain.
 */
class VocabularyInfo {
private:
    pimpl<VocabularyInfoImpl> m_pImpl;

public:
    VocabularyInfo();
    ~VocabularyInfo();

    const Predicate& add_predicate(const std::string &predicate_name, int arity);

    bool exists_predicate_name(const std::string& name) const;
    const std::vector<Predicate>& get_predicates() const;
    int get_predicate_idx(const std::string& name) const;
    const Predicate& get_predicate(int predicate_idx) const;
};


/**
 * InstanceInfo stores information related to the planning instance.
 */
class InstanceInfo {
private:
    pimpl<InstanceInfoImpl> m_pImpl;

public:
    InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    InstanceInfo() = delete;
    ~InstanceInfo();

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    const Atom& add_atom(const std::string& predicate_name, const Name_Vec& object_names);

    /**
     * Adds an atom that remains true forever.
     */
    const Atom& add_static_atom(const std::string& predicate_name, const Name_Vec& object_names);

    /**
     * Getters.
     */
    const std::vector<Atom>& get_atoms() const;
    const Atom& get_atom(int atom_idx) const;
    int get_atom_idx(const std::string& name) const;
    const std::vector<Object>& get_objects() const;
    const Object& get_object(int object_idx) const;
    int get_object_idx(const std::string& object_name) const;
    int get_num_objects() const;
    std::shared_ptr<const VocabularyInfo> get_vocabulary_info() const;
    const Index_Vec& get_static_atom_idxs() const;
};


/**
 * Abstract base class of any Element.
 */
template<typename T>
class Element {
protected:
    Element() = default;

public:
    virtual ~Element() = default;

    /**
     * Evaluates the element for a state given as a vector of atom indices.
     */
    virtual T evaluate(const State& state) = 0;

    /**
     * Returns the complexity of the element
     * measured in the size of the abstract syntax tree.
     */
    virtual int compute_complexity() const = 0;

    /**
     * Returns a canonical string representation.
     */
    virtual std::string compute_repr() const = 0;
};


/**
 * Concept evaluates to ConceptDenotation.
 */
class Concept : public Element<ConceptDenotation> {
protected:
    pimpl<ConceptImpl> m_pImpl;

    Concept(const VocabularyInfo& vocabulary_info, element::Concept_Ptr&& concept);
    friend class SyntacticElementFactoryImpl;

public:
    Concept(const Concept& other);
    virtual ~Concept();

    virtual ConceptDenotation evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Concept evaluates to RoleDenotation.
 */
class Role : public Element<RoleDenotation> {
protected:
    pimpl<RoleImpl> m_pImpl;

    Role(const VocabularyInfo& vocabulary_info, element::Role_Ptr&& role);
    friend class SyntacticElementFactoryImpl;

public:
    Role(const Role& other);
    virtual ~Role();

    virtual RoleDenotation evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Numerical evaluates to int.
 */
class Numerical : public Element<int> {
protected:
    pimpl<NumericalImpl> m_pImpl;

    Numerical(const VocabularyInfo& vocabulary_info, element::Numerical_Ptr&& numerical);
    friend class SyntacticElementFactoryImpl;

public:
    Numerical(const Numerical& other);
    virtual ~Numerical();

    virtual int evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * Boolean evaluates to bool.
 */
class Boolean : public Element<bool> {
protected:
    pimpl<BooleanImpl> m_pImpl;

    Boolean(const VocabularyInfo& vocabulary_info, element::Boolean_Ptr&& boolean);
    friend class SyntacticElementFactoryImpl;

public:
    Boolean(const Boolean& other);
    virtual ~Boolean();

    virtual bool evaluate(const State& state) override;

    virtual int compute_complexity() const override;

    virtual std::string compute_repr() const override;
};


/**
 * The SyntacticElementFactory for creation of syntactically unique elements.
 */
class SyntacticElementFactory {
private:
    pimpl<SyntacticElementFactoryImpl> m_pImpl;

public:
    SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info);
    SyntacticElementFactory(const SyntacticElementFactory& other);
    ~SyntacticElementFactory();

    const VocabularyInfo* get_vocabulary_info() const;

    /**
     * Returns a Concept if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Concept parse_concept(const std::string &description);

    /**
     * Returns a Role if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Role parse_role(const std::string &description);

    /**
     * Returns a Numerical if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Numerical parse_numerical(const std::string &description);

    /**
     * Returns a Boolean if the description is correct.
     * If description is incorrect, throw an error with human readable information.
     */
    Boolean parse_boolean(const std::string &description);


    Boolean make_empty_boolean(const Concept& concept);
    Boolean make_empty_boolean(const Role& role);

    Concept make_all_concept(const Role& role, const Concept& concept);
    Concept make_and_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_bot_concept();
    Concept make_diff_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_not_concept(const Concept& concept);
    Concept make_one_of_concept(const std::string& object_name);
    Concept make_or_concept(const Concept& concept_left, const Concept& concept_right);
    Concept make_primitive_concept(const std::string& name, int pos);
    Concept make_some_concept(const Role& role, const Concept& concept);
    Concept make_subset_concept(const Role& role_left, const Role& role_right);
    Concept make_top_concept();

    Numerical make_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_count(const Concept& concept);
    Numerical make_count(const Role& role);
    Numerical make_role_distance(const Role& role_from, const Role& role, const Role& role_to);
    Numerical make_sum_concept_distance(const Concept& concept_from, const Role& role, const Concept& concept_to);
    Numerical make_sum_role_distance(const Role& role_from, const Role& role, const Role& role_to);

    Role make_and_role(const Role& role_left, const Role& role_right);
    Role make_compose_role(const Role& role_left, const Role& role_right);
    Role make_diff_role(const Role& role_left, const Role& role_right);
    Role make_identity_role(const Concept& concept);
    Role make_inverse_role(const Role& role);
    Role make_not_role(const Role& role);
    Role make_or_role(const Role& role_left, const Role& role_right);
    Role make_primitive_role(const std::string& name, int pos_1, int pos_2);
    Role make_restrict_role(const Role& role, const Concept& concept);
    Role make_top_role();
    Role make_transitive_closure(const Role& role);
    Role make_transitive_reflexive_closure(const Role& role);
};


}
}

#endif
