#include "source/extensions/load_balancing_policies/cpu_aware_p2c/config.h"
#include "source/extensions/load_balancing_policies/cpu_aware_p2c/cpu_aware_p2c_lb.h"

namespace Envoy {
namespace Extensions {
namespace LoadBalancingPolices {
namespace CpuAwareP2C {

Upstream::LoadBalancerPtr CpuAwareP2CCreator::operator()(Upstream::LoadBalancerParams params,
    OptRef<const Upstream::LoadBalancerConfig> lb_config, const Upstream::ClusterInfo& cluster_info,
    const Upstream::PrioritySet&, Runtime::Loader& runtime, Random::RandomGenerator& random,
    TimeSource& time_source) {

  const auto* config = dynamic_cast<const CpuAwareP2CConfig*>(lb_config.ptr());
  return std::make_unique<Upstream::CpuAwareP2CLb>(
      params.priority_set, params.local_priority_set, cluster_info.lbStats(), runtime, random,
      cluster_info.lbConfig(), config->config(), time_source);
}

REGISTER_FACTORY(Factory, Upstream::TypedLoadBalancerFactory);

} // namespace CpuAwareP2C
} // namespace LoadBalancingPolices
} // namespace Extensions
} // namespace Envoy
