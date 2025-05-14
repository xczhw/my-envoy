#pragma once

#include "source/extensions/load_balancing_policies/common/load_balancer_impl.h"

namespace Envoy {
namespace Upstream {

class CpuAwareP2CLb : public EdfLoadBalancerBase {
public:
  CpuAwareP2CLb(const PrioritySet& priority_set, const PrioritySet* local_priority_set,
                ClusterLbStats& stats, Runtime::Loader& runtime,
                Random::RandomGenerator& random,
                const envoy::config::cluster::v3::Cluster::CommonLbConfig& common_config,
                const envoy::extensions::load_balancing_policies::cpu_aware_p2c::v3::CpuAwareP2C& config,
                TimeSource& time_source);

private:
  void refreshHostSource(const HostsSource&) override {}
  double hostWeight(const Host& host) const override;
  HostConstSharedPtr unweightedHostPeek(const HostVector&,
      const HostsSource&) override;
  HostConstSharedPtr unweightedHostPick(const HostVector& hosts_to_use,
      const HostsSource& source) override;

  HostSharedPtr pickP2C(const HostVector& hosts_to_use) const;

  const uint32_t choice_count_;
};

} // namespace Upstream
} // namespace Envoy
