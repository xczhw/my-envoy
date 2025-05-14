#pragma once

#include "envoy/extensions/load_balancing_policies/cpu_aware_p2c/v3/cpu_aware_p2c.pb.h"
#include "envoy/upstream/load_balancer.h"

#include "source/extensions/load_balancing_policies/common/factory_base.h"

namespace Envoy {
namespace Extensions {
namespace LoadBalancingPolices {
namespace CpuAwareP2C {

using CpuAwareP2CProto = envoy::extensions::load_balancing_policies::cpu_aware_p2c::v3::CpuAwareP2C;

class CpuAwareP2CConfig : public Upstream::LoadBalancerConfig {
public:
  explicit CpuAwareP2CConfig(const CpuAwareP2CProto& config) : config_(config) {}
  const CpuAwareP2CProto& config() const { return config_; }

private:
  const CpuAwareP2CProto config_;
};

struct CpuAwareP2CCreator {
  Upstream::LoadBalancerPtr operator()(Upstream::LoadBalancerParams params,
                                       OptRef<const Upstream::LoadBalancerConfig> lb_config,
                                       const Upstream::ClusterInfo& cluster_info,
                                       const Upstream::PrioritySet& priority_set,
                                       Runtime::Loader& runtime,
                                       Random::RandomGenerator& random,
                                       TimeSource& time_source);
};

class Factory : public Common::FactoryBase<CpuAwareP2CProto, CpuAwareP2CCreator> {
public:
  Factory() : FactoryBase("envoy.load_balancing_policies.cpu_aware_p2c") {}

  Upstream::LoadBalancerConfigPtr loadConfig(Upstream::LoadBalancerFactoryContext&,
                                             const Protobuf::Message& config,
                                             ProtobufMessage::ValidationVisitor&) override {
    return Upstream::LoadBalancerConfigPtr{new TypedCustomizedLbConfig(*active_or_legacy.active())};
  }
};

DECLARE_FACTORY(Factory);

} // namespace CpuAwareP2C
} // namespace LoadBalancingPolices
} // namespace Extensions
} // namespace Envoy
