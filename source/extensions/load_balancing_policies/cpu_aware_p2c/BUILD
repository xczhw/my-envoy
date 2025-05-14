load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_extension_package",
)

licenses(["notice"])

envoy_extension_package()

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    deps = [
        ":cpu_aware_p2c_lb_lib",
        "//source/common/common:minimal_logger_lib",
        "//source/common/upstream:load_balancer_context_base_lib",
        "//source/extensions/load_balancing_policies/common:factory_base",
        "@envoy_api//envoy/extensions/load_balancing_policies/cpu_aware_p2c/v3:pkg_cc_proto",
    ],
)

envoy_cc_library(
    name = "cpu_aware_p2c_lb_lib",
    srcs = ["cpu_aware_p2c_lb.cc"],
    hdrs = ["cpu_aware_p2c_lb.h"],
    deps = [
        "//source/extensions/load_balancing_policies/common:load_balancer_lib",
    ],
)
