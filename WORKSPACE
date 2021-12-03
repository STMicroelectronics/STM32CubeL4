workspace(name = "com_github_stmicroelectronics_stm32cubewl")

load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
)

# Configure bazel_embedded toolchains for ARM Cortex-M devices.
# Used in modules: All.
git_repository(
    name = "bazel_embedded",
    commit = "d3cbe4eff9a63d3dee63067d61096d681daca33b",
    remote = "https://github.com/silvergasp/bazel-embedded.git",
    shallow_since = "1585022166 +0800",
)

load(
    "@bazel_embedded//:bazel_embedded_deps.bzl",
    "bazel_embedded_deps",
)

# Fetch third-party dependencies for bazel_embedded toolchains.
bazel_embedded_deps()

load(
    "@bazel_embedded//platforms:execution_platforms.bzl",
    "register_platforms",
)

# Register basic platforms, e.g. ARM Cortex-M devices.
register_platforms()

load(
    "@bazel_embedded//toolchains/compilers/gcc_arm_none_eabi:gcc_arm_none_repository.bzl",
    "gcc_arm_none_compiler",
)

# Fetch gcc arm none compiler.
gcc_arm_none_compiler()

load(
    "@bazel_embedded//toolchains/gcc_arm_none_eabi:gcc_arm_none_toolchain.bzl",
    "register_gcc_arm_none_toolchain",
)

# Register gcc arm none toolchains.
register_gcc_arm_none_toolchain()
