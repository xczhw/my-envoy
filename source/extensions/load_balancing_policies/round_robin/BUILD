load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_extension_package",
)

licenses(["notice"])  # Apache 2

envoy_extension_package()

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    # previously considered core code and used by mobile.
    visibility = ["//visibility:public"],
    deps = [
        ":round_robin_lb_lib",
        "//source/common/common:minimal_logger_lib",
        "//source/common/upstream:load_balancer_context_base_lib",
        "//source/extensions/load_balancing_policies/common:factory_base",
        "@envoy_api//envoy/extensions/load_balancing_policies/round_robin/v3:pkg_cc_proto",
    ],
)

envoy_cc_library(
    name = "round_robin_lb_lib",
    srcs = ["round_robin_lb.cc"],
    hdrs = ["round_robin_lb.h"],
    deps = [
        "//source/extensions/load_balancing_policies/common:load_balancer_lib",
        "//source/common/common:minimal_logger_lib",
    ],
)
