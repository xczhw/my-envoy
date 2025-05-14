#include "external_lb.grpc.pb.h"
#include "external_lb.pb.h"

#include "envoy/upstream/load_balancer.h"
#include "envoy/upstream/upstream.h"

#include <grpcpp/grpcpp.h>

using namespace Envoy;
using namespace external;

class GrpcExternalLoadBalancer : public Upstream::LoadBalancer {
public:
  GrpcExternalLoadBalancer(const Upstream::HostSet& host_set)
      : host_set_(host_set),
        stub_(ExternalLB::NewStub(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()))) {}

  HostConstSharedPtr chooseHost(LoadBalancerContext* context) override {
    external::LoadBalanceRequest req;
    // 发起 gRPC 请求
    grpc::ClientContext ctx;
    external::LoadBalanceResponse resp;

    for (const auto& host : host_set_.hosts()) {
      external::Node* node = req.add_nodes();
      node->set_hostname(host->hostname());
      node->add_server_local(cpu);
      node->add_server_local(mem);
    }

    auto status = stub_->ChooseHost(&ctx, req, &resp);

    const std::string& target = resp.selected_hostname();
    for (const auto& host : host_set_.hosts()) {
      if (host->hostname() == target) {
        return host;
      }
    }

    ENVOY_LOG(warn, "No host matched the returned hostname: {}", target);
    return nullptr;
  }
private:
  const Upstream::HostSet& host_set_;
  std::unique_ptr<ExternalLB::Stub> stub_;
};
