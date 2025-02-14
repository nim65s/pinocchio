#!/usr/bin/env python

from dataclasses import dataclass, field
from os import environ
from pathlib import Path

import httpx
from tomllib import load


@dataclass
class Project:
    name: str
    owner: str
    dependencies: [str] = field(default_factory=list)
    flags: [str] = field(default_factory=list)
    main: bool = False
    tests: bool = True

    def url(self):
        api = f"https://api.github.com/repos/{self.owner}/{self.name}/releases/latest"
        headers = {}
        if "GITHUB_TOKEN" in environ:
            headers["Authorization"] = f"Bearer {environ['GITHUB_TOKEN']}"
        return httpx.get(api, headers=headers).json()["tarball_url"]


JOBS = 4
HEAD = f"""
FROM ubuntu:24.04 AS base

WORKDIR /src

SHELL ["/bin/bash", "-eux", "-c"]

RUN --mount=type=cache,sharing=locked,target=/var/cache/apt \\
    --mount=type=cache,sharing=locked,target=/var/lib/apt \\
    apt update \\
    && DEBIAN_FRONTEND=noninteractive apt install -qqy --no-install-recommends \\
    build-essential \\
    cmake \\
    git \\
    libboost-all-dev \\
    libeigen3-dev \\
    libpython3-dev \\
    libqhull-dev\
    liburdfdom-dev \\
    libtinyxml-dev \\
    python-is-python3 \\
    python3-numpy \\
    python3-scipy

ENV JOBS={JOBS} \\
    CMAKE_BUILD_TYPE=Release \\
    CTEST_PARALLEL_LEVEL={JOBS} \\
    CTEST_OUTPUT_ON_FAILURE=ON

"""


def install(prj: Project) -> [str]:
    add = f"{prj.url()} /src.tar.gz" if not prj.main else ". ."
    extract = "tar xf /src.tar.gz --strip-components=1" if not prj.main else "echo"
    return (
        [
            "",
            f"FROM base AS {prj.name}",
            f"ADD {add}",
            "",
        ]
        + [f"COPY --from={dep} /usr/local /usr/local" for dep in prj.dependencies]
        + [
            "",
            f"RUN {extract} \\",
            " && ldconfig \\",
            " && cmake -B build \\",
            "        -DCMAKE_INSTALL_LIBDIR=lib \\",
        ]
        + [f"        {flag} \\" for flag in prj.flags]
        + [
            "        -DPYTHON_DEB_LAYOUT=ON \\",
            "        -Wno-dev \\",
            " && cmake --build build -j $JOBS \\",
        ]
        + ([" && cmake --build build -t test \\"] if prj.tests else [])
        + [
            " && cmake --build build -t install \\",
            " && rm -rf ./*",
        ]
    )


def generate_dockerfile():
    deps = []
    with (Path(__file__).parent / "docker.toml").open("rb") as f:
        for k, v in load(f).items():
            deps.append(Project(name=k, **v))
    with Path("Dockerfile").open("w") as f:
        f.write(HEAD)
        for dep in deps:
            f.writelines(f"{line}\n" for line in install(dep))


if __name__ == "__main__":
    generate_dockerfile()
