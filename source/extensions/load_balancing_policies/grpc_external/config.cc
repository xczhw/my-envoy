class GrpcExternalLoadBalancerFactory : public Upstream::TypedLoadBalancerFactory {
    public:
      std::string name() const override { return "grpc_external"; }

      Upstream::LoadBalancerFactorySharedPtr create(
          const Protobuf::Message&,
          Upstream::ClusterInfoConstSharedPtr cluster_info,
          const envoy::config::cluster::v3::Cluster::CommonLbConfig&,
          Server::Configuration::TransportSocketMatcher&,
          Runtime::Loader&,
          Random::RandomGenerator&,
          TimeSource&,
          Upstream::LoadBalancerStats&,
          Stats::Scope&,
          const envoy::config::core::v3::TypedExtensionConfig*) override {

        return std::make_shared<Upstream::SimpleLoadBalancerFactory>(
            [cluster_info](const Upstream::HostSet& host_set) -> Upstream::LoadBalancerSharedPtr {
              return std::make_shared<GrpcExternalLoadBalancer>(host_set);
            });
      }

      ProtobufTypes::MessagePtr createEmptyConfigProto() override {
        return std::make_unique<ProtobufWkt::Empty>();
      }
    };

    REGISTER_FACTORY(GrpcExternalLoadBalancerFactory, Upstream::TypedLoadBalancerFactory);
