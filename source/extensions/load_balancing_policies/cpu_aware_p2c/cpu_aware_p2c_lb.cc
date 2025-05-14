#include "source/extensions/load_balancing_policies/cpu_aware_p2c/cpu_aware_p2c_lb.h"

namespace Envoy {
namespace Upstream {

CpuAwareP2CLb::CpuAwareP2CLb(const PrioritySet& priority_set, const PrioritySet* local_priority_set,
                             ClusterLbStats& stats, Runtime::Loader& runtime,
                             Random::RandomGenerator& random,
                             const envoy::config::cluster::v3::Cluster::CommonLbConfig& common_config,
                             const envoy::extensions::load_balancing_policies::cpu_aware_p2c::v3::CpuAwareP2C& config,
                             TimeSource& time_source)
    : EdfLoadBalancerBase(priority_set, local_priority_set, stats, runtime, random,
                          PROTOBUF_PERCENT_TO_ROUNDED_INTEGER_OR_DEFAULT(common_config,
                                                                         healthy_panic_threshold, 100, 50),
                          LoadBalancerConfigHelper::localityLbConfigFromCommonLbConfig(common_config),
                          absl::nullopt, time_source),
      choice_count_(PROTOBUF_GET_WRAPPED_OR_DEFAULT(config, choice_count, 2)) {}

double CpuAwareP2CLb::hostWeight(const Host& host) const {
    return static_cast<double>(host.weight());
}

HostConstSharedPtr CpuAwareP2CLb::unweightedHostPeek(const HostVector&, const HostsSource&) {
  return nullptr;
}

HostConstSharedPtr CpuAwareP2CLb::unweightedHostPick(const HostVector& hosts_to_use,
                                                     const HostsSource&) {
  return pickP2C(hosts_to_use);
}

HostSharedPtr CpuAwareP2CLb::pickP2C(const HostVector& hosts_to_use) const {
  HostSharedPtr candidate = nullptr;

  for (uint32_t i = 0; i < choice_count_; ++i) {
    const HostSharedPtr& sampled_host = hosts_to_use[random_.random() % hosts_to_use.size()];

    if (!candidate ||
        sampled_host->stats().rq_active_.value() < candidate->stats().rq_active_.value()) {
      candidate = sampled_host;
    }
  }

  return candidate;
}

} // namespace Upstream
} // namespace Envoy
