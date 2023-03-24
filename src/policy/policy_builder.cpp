#include "policy_builder.h"

#include "condition.h"
#include "effect.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

std::shared_ptr<const core::Boolean> PolicyBuilderImpl::add_boolean_feature(core::Boolean boolean) {
    return m_caches.m_boolean_cache->insert(std::make_unique<core::Boolean>(boolean)).first;
}

std::shared_ptr<const core::Numerical> PolicyBuilderImpl::add_numerical_feature(core::Numerical numerical) {
    return m_caches.m_numerical_cache->insert(std::make_unique<core::Numerical>(numerical)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(b)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(b)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(n)).first;
}

std::shared_ptr<const BaseCondition> PolicyBuilderImpl::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(n)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(b)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(b)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(b)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(n)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(n)).first;
}

std::shared_ptr<const BaseEffect> PolicyBuilderImpl::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(n)).first;
}

std::shared_ptr<const Rule> PolicyBuilderImpl::add_rule(
    std::set<std::shared_ptr<const BaseCondition>>&& conditions,
    std::set<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_caches.m_rule_cache->insert(std::unique_ptr<Rule>(new Rule(move(conditions), move(effects)))).first;
}

std::shared_ptr<const Policy> PolicyBuilderImpl::add_policy(
    std::set<std::shared_ptr<const Rule>>&& rules) {
    // TODO: add assertion that rules in builder
    return m_caches.m_policy_cache->insert(std::unique_ptr<Policy>(new Policy(move(rules)))).first;
}

}
