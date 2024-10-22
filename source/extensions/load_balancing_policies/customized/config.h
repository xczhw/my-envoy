#pragma once

#include "envoy/extensions/load_balancing_policies/customized/v3/customized.pb.h"
#include "envoy/extensions/load_balancing_policies/customized/v3/customized.pb.validate.h"
#include "envoy/upstream/load_balancer.h"

#include "source/common/common/logger.h"
#include "source/extensions/load_balancing_policies/common/factory_base.h"

namespace Envoy {
namespace Extensions {
namespace LoadBalancingPolices {
namespace Customized {

using CustomizedLbProto =
    envoy::extensions::load_balancing_policies::customized::v3::Customized;
using ClusterProto = envoy::config::cluster::v3::Cluster;
using LegacyCustomizedLbProto = ClusterProto::CustomizedLbConfig;

/**
 * Load balancer config that used to wrap the legacy least request config.
 */
class LegacyCustomizedLbConfig : public Upstream::LoadBalancerConfig {
public:
  LegacyCustomizedLbConfig(const ClusterProto& cluster);

  OptRef<const LegacyCustomizedLbProto> lbConfig() const {
    if (lb_config_.has_value()) {
      return lb_config_.value();
    }
    return {};
  };

private:
  absl::optional<LegacyCustomizedLbProto> lb_config_;
};

/**
 * Load balancer config that used to wrap the least request config.
 */
class TypedCustomizedLbConfig : public Upstream::LoadBalancerConfig {
public:
  TypedCustomizedLbConfig(const CustomizedLbProto& lb_config);

  const CustomizedLbProto lb_config_;
};

struct CustomizedCreator : public Logger::Loggable<Logger::Id::upstream> {
  Upstream::LoadBalancerPtr operator()(Upstream::LoadBalancerParams params,
                                       OptRef<const Upstream::LoadBalancerConfig> lb_config,
                                       const Upstream::ClusterInfo& cluster_info,
                                       const Upstream::PrioritySet& priority_set,
                                       Runtime::Loader& runtime, Random::RandomGenerator& random,
                                       TimeSource& time_source);
};

class Factory : public Common::FactoryBase<CustomizedLbProto, CustomizedCreator> {
public:
  Factory() : FactoryBase("envoy.load_balancing_policies.customized") {}

  Upstream::LoadBalancerConfigPtr loadConfig(Upstream::LoadBalancerFactoryContext&,
                                             const Protobuf::Message& config,
                                             ProtobufMessage::ValidationVisitor&) override {
    auto active_or_legacy = Common::ActiveOrLegacy<CustomizedLbProto, ClusterProto>::get(&config);
    ASSERT(active_or_legacy.hasLegacy() || active_or_legacy.hasActive());

    return active_or_legacy.hasLegacy()
               ? Upstream::LoadBalancerConfigPtr{new LegacyCustomizedLbConfig(
                     *active_or_legacy.legacy())}
               : Upstream::LoadBalancerConfigPtr{
                     new TypedCustomizedLbConfig(*active_or_legacy.active())};
  }
};

DECLARE_FACTORY(Factory);

} // namespace Customized
} // namespace LoadBalancingPolices
} // namespace Extensions
} // namespace Envoy
