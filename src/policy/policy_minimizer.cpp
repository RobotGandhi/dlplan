#include "condition.h"
#include "effect.h"

#include "../utils/set_operators.h"
#include "../../include/dlplan/utils/hashing.h"

#include "../../include/dlplan/policy.h"

namespace std {
    template<> struct hash<std::vector<const dlplan::policy::Rule*>> {
        size_t operator()(const std::vector<const dlplan::policy::Rule*>& merged_rule_combinations) const noexcept {
            size_t seed = merged_rule_combinations.size();
            for (const auto& rule_ptr : merged_rule_combinations) {
                dlplan::utils::hash_combine(seed, rule_ptr);
            }
            return seed;
        }
    };
}

namespace dlplan::policy {
/**
 * Copies all objects to the given PolicyBuilder and returns newly constructed objects.
 */
template<typename T>
std::set<T> copy_to_builder(
    const std::set<T>& old_objects,
    PolicyBuilder& builder) {
    std::set<T> new_objects;
    std::transform(
        old_objects.begin(),
        old_objects.end(),
        std::inserter(new_objects, new_objects.begin()),
        [&builder](const auto& object){
            return object->copy_to_builder(builder);
        }
    );
    return new_objects;
}


static std::set<std::shared_ptr<const Rule>> compute_dominated_rules(
    const std::set<std::shared_ptr<const Rule>>& rules) {
    std::set<std::shared_ptr<const Rule>> dominated_rules;
    for (const auto& rule_1 : rules) {
        for (const auto& rule_2 : rules) {
            if (rule_1 == rule_2) {
                // Note: there cannot be identical rules in a policy, hence this equality check suffices to not remove all identical rules.
                continue;
            }
            if (utils::is_subset_eq(rule_1->get_conditions(), rule_2->get_conditions()) && utils::is_subset_eq(rule_1->get_effects(), rule_2->get_effects())) {
                dominated_rules.insert(rule_2);
                break;
            }
        }
    }
    return dominated_rules;
}


/**
 * Returns true iff policy classifies true_state_pairs as true and false_state_pairs as false.
 */
static bool check_policy_matches_classification(
    const Policy& policy,
    const core::StatePairs& true_state_pairs,
    const core::StatePairs& false_state_pairs) {
    return std::all_of(true_state_pairs.begin(), true_state_pairs.end(), [&policy](const core::StatePair& state_pair){ return policy.evaluate_lazy(state_pair.first, state_pair.second); }) &&
           std::all_of(false_state_pairs.begin(), false_state_pairs.end(), [&policy](const core::StatePair& state_pair){ return !policy.evaluate_lazy(state_pair.first, state_pair.second); });
}


PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }

Policy PolicyMinimizer::minimize(const Policy& policy) const {
    // successively add simpler rules that are made up of existing rules
    PolicyBuilder builder;
    Policy tmp_policy = policy.copy_to_builder(builder);
    Rules rules = tmp_policy.get_rules();
    Booleans booleans = builder.get_booleans();
    Numericals numerical = builder.get_numericals();
    std::unordered_map<const BaseCondition*, std::vector<const Rule*>> c2r;
    std::unordered_map<const BaseEffect*, std::vector<const Rule*>> e2r;
    bool changed = false;
    do {
        for (const auto& boolean : booleans) {
            const auto c_pos = builder.add_pos_condition(boolean);
            const auto c_neg = builder.add_neg_condition(boolean);
            for (const auto& rule1 : c2r[c_pos.get()]) {
                for (const auto& rule2 : c2r[c_neg.get()]) {
                    // check if rules can be merged, track they have been checked/merged.
                }
            }
        }
        for (const auto& numerical : numerical) {

        }
    } while(changed);
    // remove dominated rules and return policy.
}

Policy PolicyMinimizer::minimize(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    Policy current_policy = policy;
    bool minimization_success;
    do {
        minimization_success = false;
        for (const auto& rule : current_policy.get_rules()) {
            for (const auto& condition : rule->get_conditions()) {
                PolicyBuilder builder;
                builder.add_rule(
                    copy_to_builder(utils::set_difference(rule->get_conditions(), {condition}), builder),
                    copy_to_builder(rule->get_effects(), builder));
                Rules rules = copy_to_builder(utils::set_difference(current_policy.get_rules(), {rule}), builder);
                Policy tmp_policy = *builder.add_policy(std::move(rules)).get();
                if (check_policy_matches_classification(tmp_policy, true_state_pairs, false_state_pairs)) {
                    minimization_success = true;
                    current_policy = tmp_policy;
                    break;
                }
            }
            if (minimization_success) {
                break;
            }
            for (const auto& effect : rule->get_effects()) {
                PolicyBuilder builder;
                builder.add_rule(
                    copy_to_builder(rule->get_conditions(), builder),
                    copy_to_builder(utils::set_difference(rule->get_effects(), {effect}), builder));
                Rules rules = copy_to_builder(utils::set_difference(current_policy.get_rules(), {rule}), builder);
                Policy tmp_policy = *builder.add_policy(std::move(rules)).get();
                if (check_policy_matches_classification(tmp_policy, true_state_pairs, false_state_pairs)) {
                    minimization_success = true;
                    current_policy = tmp_policy;
                    break;
                }
            }
            if (minimization_success) {
                break;
            }
        }
    } while (minimization_success);
    return current_policy;
}

}