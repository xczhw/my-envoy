load(
    "//bazel:envoy_build_system.bzl",
    "envoy_package",
)
load(
    "//test/extensions:extensions_build_system.bzl",
    "envoy_extension_cc_test",
)

licenses(["notice"])  # Apache 2

envoy_package()

envoy_extension_cc_test(
    name = "config_test",
    srcs = ["config_test.cc"],
    extension_names = ["envoy.load_balancing_policies.client_side_weighted_round_robin"],
    rbe_pool = "2core",
    deps = [
        "//source/extensions/load_balancing_policies/client_side_weighted_round_robin:config",
        "//test/mocks/server:factory_context_mocks",
        "//test/mocks/upstream:cluster_info_mocks",
        "//test/mocks/upstream:priority_set_mocks",
        "@envoy_api//envoy/config/core/v3:pkg_cc_proto",
    ],
)

envoy_extension_cc_test(
    name = "client_side_weighted_round_robin_lb_test",
    size = "large",
    srcs = ["client_side_weighted_round_robin_lb_test.cc"],
    extension_names = ["envoy.load_balancing_policies.client_side_weighted_round_robin"],
    deps = [
        "//source/common/protobuf",
        "//source/extensions/load_balancing_policies/client_side_weighted_round_robin:config",
        "//test/extensions/load_balancing_policies/common:load_balancer_base_test_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_extension_cc_test(
    name = "integration_test",
    size = "large",
    srcs = ["integration_test.cc"],
    extension_names = ["envoy.load_balancing_policies.client_side_weighted_round_robin"],
    rbe_pool = "2core",
    deps = [
        "//source/common/protobuf",
        "//source/extensions/load_balancing_policies/client_side_weighted_round_robin:config",
        "//test/integration:http_integration_lib",
        "//test/test_common:utility_lib",
        "@envoy_api//envoy/config/endpoint/v3:pkg_cc_proto",
    ],
)
