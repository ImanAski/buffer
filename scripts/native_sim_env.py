Import("env")

import os
import platform
import subprocess
import shutil


def _append_pkg_config_flags(package_name: str) -> bool:
    try:
        cflags = subprocess.check_output(
            ["pkg-config", "--cflags", package_name], text=True
        ).strip().split()
        libs = subprocess.check_output(
            ["pkg-config", "--libs", package_name], text=True
        ).strip().split()
    except Exception:
        return False

    for token in cflags:
        if token.startswith("-I"):
            env.Append(CPPPATH=[token[2:]])
        else:
            env.Append(CCFLAGS=[token])

    for token in libs:
        if token.startswith("-L"):
            env.Append(LIBPATH=[token[2:]])
        elif token.startswith("-l"):
            env.Append(LIBS=[token[2:]])
        else:
            env.Append(LINKFLAGS=[token])

    return True


system_name = platform.system()

if system_name == "Windows":
    default_root = os.path.join(os.path.expanduser("~"), "libs", "SDL2")
    sdl_root = os.environ.get("SDL2_DIR", default_root)
    sdl_include = os.path.join(sdl_root, "include")
    sdl_lib_x64 = os.path.join(sdl_root, "lib", "x64")
    sdl_lib = sdl_lib_x64 if os.path.isdir(sdl_lib_x64) else os.path.join(sdl_root, "lib")

    env.Append(CPPPATH=[sdl_include])
    env.Append(LIBPATH=[sdl_lib])
    env.Append(LIBS=["mingw32", "SDL2main", "SDL2"])

    sdl_dll = os.path.join(sdl_root, "bin", "SDL2.dll")
    if os.path.isfile(sdl_dll):
        def _copy_sdl2_dll(target, source, env):
            out_dir = env.subst("$BUILD_DIR")
            shutil.copy2(sdl_dll, os.path.join(out_dir, "SDL2.dll"))

        env.AddPostAction("$PROGPATH", _copy_sdl2_dll)
        env.AddPostAction("buildprog", _copy_sdl2_dll)
elif system_name == "Linux":
    if not _append_pkg_config_flags("sdl2"):
        env.Append(LIBS=["SDL2"])
else:
    env.Append(LIBS=["SDL2"])
