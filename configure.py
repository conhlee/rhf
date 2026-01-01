#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "SOME01",	# 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20250520"
config.dtk_tag = "v1.6.0"
config.objdiff_tag = "v3.0.0-beta.8"
config.sjiswrap_tag = "v1.2.1"
config.wibo_tag = "0.6.16"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-gdwarf-2")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    config.ldflags.append("-listclosure") # For Wii linkers

config.apply_custom_build_rule()

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None


cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-ipa file",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-enc SJIS",
    "-i include",
    "-i include/stl",
    "-i include/revolution",
    "-i src/GameUtil",
    "-i src/Game",
    f"-i build/{config.version}/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym dwarf-2", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Actual game code 
cflags_game = [
    *cflags_base,
    "-RTTI on",
    "-str readonly",
    "-use_lmw_stmw on",
    "-func_align 4",
    "-enc multibyte",
]

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
	"-inline auto",
]

# Game utilities
cflags_gameutil = [
    *cflags_base,
    "-RTTI on",
    "-str readonly",
    "-use_lmw_stmw on",
    "-func_align 4",
]

# Revolution library flags
cflags_rvl = [
    *cflags_base,
    "-func_align 16",
    "-i src/revolution",
]

# HBM library flags
cflags_hbm = [
    *cflags_base,
    "-func_align 16",
    "-i src/revolution",
    "-sdata2 0",
]

config.linker_version = "Wii/1.0"

# Helper function for Revolution libraries
def RevolutionLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.0",
        "cflags": cflags_rvl,
        "progress_category": "sdk",
        "objects": objects,
    }

# Helper function for NW4R libraries
def Nw4rLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "Wii/1.0",
        "cflags": [*cflags_base, "-fp_contract off", "-func_align 16"],
        "progress_category": "nw4r",
        "objects": objects,
    }

Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Game", 
        "mw_version": config.linker_version,
        "cflags": cflags_game,
        "shiftjis": False,
        "progress_category": "game",
        "objects": [
            Object(NonMatching, "Game/Menu/SceneMenu.cpp"),
            Object(Matching, "Game/Global.cpp"),
            Object(Matching, "Game/data_802E57E0.cpp"),
            Object(Matching, "Game/FaderFlash.cpp"),
            Object(NonMatching, "Game/ExScene.cpp"),
            Object(NonMatching, "Game/ExChecker.cpp"),
            Object(Matching, "Game/Prologue/MyFlow.cpp"),
            Object(Matching, "Game/Prologue/MyLayout.cpp"),
            Object(Matching, "Game/Prologue/ScenePrologue.cpp"),
            Object(Matching, "Game/Script/Prologue.cpp"),
            Object(NonMatching, "Game/MyTagProcessor.cpp"),
            Object(NonMatching, "Game/Rocket/SceneRocket.cpp"),
            Object(NonMatching, "Game/main.cpp"),
            Object(NonMatching, "Game/BalloonLayout.cpp"),
            Object(NonMatching, "Game/CursorLayout.cpp"),
            Object(Matching, "Game/PauseLayout.cpp"),
            Object(NonMatching, "Game/Batting/MyChecker.cpp"),
            Object(NonMatching, "Game/Batting/MyFlow.cpp"),
            Object(NonMatching, "Game/Batting/SceneBatting.cpp"),
            Object(NonMatching, "Game/Boat/MyChecker.cpp"),
            Object(NonMatching, "Game/Boat/MyFlow.cpp"),
            Object(NonMatching, "Game/Boat/SceneBoat.cpp"),
            Object(NonMatching, "Game/Fishing/MyChecker.cpp"),
            Object(NonMatching, "Game/Fishing/MyFlow.cpp"),
            Object(NonMatching, "Game/Fishing/SceneFishing.cpp"),
            Object(NonMatching, "Game/Flea/MyChecker.cpp"),
            Object(NonMatching, "Game/Flea/MyFlow.cpp"),
            Object(NonMatching, "Game/Flea/SceneFlea.cpp"),
            Object(NonMatching, "Game/Fork/MyChecker.cpp"),
            Object(NonMatching, "Game/Fork/MyFlow.cpp"),
            Object(NonMatching, "Game/Fork/SceneFork.cpp"),
            Object(NonMatching, "Game/Goma/MyChecker.cpp"),
            Object(NonMatching, "Game/Goma/MyFlow.cpp"),
            Object(NonMatching, "Game/Goma/SceneGoma.cpp"),
            Object(NonMatching, "Game/Rap/MyChecker.cpp"),
            Object(NonMatching, "Game/Rap/MyFlow.cpp"),
            Object(NonMatching, "Game/Rap/SceneRap.cpp"),
            Object(NonMatching, "Game/Receive/MyChecker.cpp"),
            Object(NonMatching, "Game/Receive/MyFlow.cpp"),
            Object(NonMatching, "Game/Receive/SceneReceive.cpp"),
            Object(NonMatching, "Game/Rotation/MyChecker.cpp"),
            Object(NonMatching, "Game/Rotation/MyFlow.cpp"),
            Object(NonMatching, "Game/Rotation/SceneRotation.cpp"),
            Object(NonMatching, "Game/Seal/SceneSeal.cpp"),
            Object(NonMatching, "Game/Seal/MyChecker.cpp"),
            Object(NonMatching, "Game/Seal/MyFlow.cpp"),
            Object(NonMatching, "Game/Sort/SceneSort.cpp"),
            Object(NonMatching, "Game/Sort/MyChecker.cpp"),
            Object(NonMatching, "Game/Sort/MyFlow.cpp"),
            Object(NonMatching, "Game/Tap/SceneTap.cpp"),
            Object(NonMatching, "Game/Tap/MyChecker.cpp"),
            Object(NonMatching, "Game/Tap/MyFlow.cpp"),
            Object(NonMatching, "Game/Badminton/MyChecker.cpp"),
            Object(NonMatching, "Game/Badminton/MyFlow.cpp"),
            Object(NonMatching, "Game/Badminton/SceneBadminton.cpp"),
            Object(NonMatching, "Game/Baseball/MyChecker.cpp"),
            Object(NonMatching, "Game/Baseball/MyFlow.cpp"),
            Object(NonMatching, "Game/Baseball/SceneBaseball.cpp"),
            Object(NonMatching, "Game/Karateman/MyChecker.cpp"),
            Object(NonMatching, "Game/Karateman/MyFlow.cpp"),
            Object(NonMatching, "Game/Karateman/SceneKarateman.cpp"),
            Object(NonMatching, "Game/Baseball/MyLayout.cpp"),
            Object(Matching, "Game/Error/SceneError.cpp"),
            Object(Matching, "Game/Error/MyChecker.cpp"),
            Object(Matching, "Game/Error/MyFlow.cpp"),
            Object(NonMatching, "Game/Error/MyLayout.cpp"),
            Object(Matching, "Game/Strap/SceneStrap.cpp"),
            Object(Matching, "Game/Strap/MyChecker.cpp"),
            Object(Matching, "Game/Strap/MyFlow.cpp"),
            Object(Matching, "Game/MyCanvas.cpp"),
            Object(Matching, "Game/DynTex.cpp"),
            Object(Matching, "Game/Perfect/MyChecker.cpp"),
            Object(Matching, "Game/Perfect/MyFlow.cpp"),
            Object(Matching, "Game/Perfect/MyLayout.cpp"),
            Object(Matching, "Game/Perfect/ScenePerfect.cpp"),
            Object(Matching, "Game/ModeChangeLayout.cpp"),
            Object(Matching, "Game/SNDHeap.cpp"),
            Object(NonMatching, "Game/EndlessCounter.cpp"),
            Object(NonMatching, "Game/HBMManager.cpp"),
            Object(NonMatching, "Game/MessageManager.cpp"),
            Object(Matching, "Game/SceneManager.cpp"),
            Object(NonMatching, "Game/ExFlow.cpp"),
            Object(Matching, "Game/CellAnimRenderEff.cpp"),
            Object(Matching, "Game/Logo/MyChecker.cpp"),
            Object(Matching, "Game/Logo/MyFlow.cpp"),
            Object(Matching, "Game/Logo/SceneLogo.cpp"),
            Object(Matching, "Game/Script/AgbTap.cpp"),
            Object(Matching, "Game/Script/AgbGhost.cpp"),
            Object(NonMatching, "Game/AgbGhost/MyFlow.cpp"),
            Object(NonMatching, "Game/AgbGhost/SceneAgbGhost.cpp"),
            Object(Matching, "Game/AgbTap/MyChecker.cpp"),
            Object(Matching, "Game/AgbTap/MyFlow.cpp"),
            Object(Matching, "Game/AgbTap/SceneAgbTap.cpp"),
            Object(Matching, "Game/Pseudo3D.cpp"),
            Object(NonMatching, "Game/Rap/MyLayout.cpp"),
            Object(Matching, "Game/RFLManager.cpp"),
            Object(NonMatching, "Game/WindParticle.cpp"),
            Object(Matching, "Game/Script/CharacterIntro.cpp"),
            Object(Matching, "Game/Credit/MyChecker.cpp"),
            Object(Matching, "Game/Credit/MyFlow.cpp"),
            Object(Matching, "Game/Credit/MyLayout.cpp"),
            Object(Matching, "Game/Credit/SceneCredit.cpp"),
            Object(NonMatching, "Game/Metroman/MyChecker.cpp"),
            Object(NonMatching, "Game/Metroman/MyFlow.cpp"),
            Object(NonMatching, "Game/Metroman/SceneMetroman.cpp"),
        ],
    },
    RevolutionLib(
        "base",
        [
            Object(Matching, "revolution/base/PPCArch.c"),
        ]
    ),
    RevolutionLib(
        "os",
        [
            Object(NonMatching, "revolution/os/OS.c"),
            Object(NonMatching, "revolution/os/OSAlarm.c"),
            Object(NonMatching, "revolution/os/OSAlloc.c"),
            Object(NonMatching, "revolution/os/OSArena.c"),
            Object(NonMatching, "revolution/os/OSAudioSystem.c"),
            Object(NonMatching, "revolution/os/OSCache.c"),
            Object(NonMatching, "revolution/os/OSContext.c"),
            Object(NonMatching, "revolution/os/OSError.c"),
            Object(NonMatching, "revolution/os/OSExec.c"),
            Object(NonMatching, "revolution/os/OSFatal.c"),
            Object(NonMatching, "revolution/os/OSFont.c"),
            Object(NonMatching, "revolution/os/OSInterrupt.c"),
            Object(NonMatching, "revolution/os/OSLink.c"),
            Object(NonMatching, "revolution/os/OSMessage.c"),
            Object(NonMatching, "revolution/os/OSMemory.c"),
            Object(NonMatching, "revolution/os/OSMutex.c"),
            Object(NonMatching, "revolution/os/OSReboot.c"),
            Object(NonMatching, "revolution/os/OSReset.c"),
            Object(NonMatching, "revolution/os/OSRtc.c"),
            Object(NonMatching, "revolution/os/OSSync.c"),
            Object(NonMatching, "revolution/os/OSThread.c"),
            Object(NonMatching, "revolution/os/OSTime.c"),
            Object(NonMatching, "revolution/os/OSUtf.c"),
            Object(NonMatching, "revolution/os/OSIpc.c"),
            Object(NonMatching, "revolution/os/OSStateTM.c"),
            Object(NonMatching, "revolution/os/__start.c"),
            Object(NonMatching, "revolution/os/OSPlayRecord.c"),
            Object(NonMatching, "revolution/os/OSStateFlags.c"),
            Object(NonMatching, "revolution/os/OSNet.c"),
            Object(NonMatching, "revolution/os/OSNandbootInfo.c"),
            Object(NonMatching, "revolution/os/OSPlayTime.c"),
            Object(NonMatching, "revolution/os/OSCrc.c"),
            Object(NonMatching, "revolution/os/OSLaunch.c"),
            Object(NonMatching, "revolution/os/__ppc_eabi_init.c"),
        ]
    ),
    RevolutionLib(
        "mtx",
        [
            Object(NonMatching, "revolution/mtx/mtx.c"),
            Object(NonMatching, "revolution/mtx/mtxvec.c"),
            Object(NonMatching, "revolution/mtx/mtx44.c"),
            Object(NonMatching, "revolution/mtx/vec.c"),
        ]
    ),
    RevolutionLib(
        "dvd",
        [
            Object(NonMatching, "revolution/dvd/dvdfs.c"),
            Object(NonMatching, "revolution/dvd/dvd.c"),
            Object(NonMatching, "revolution/dvd/dvdqueue.c"),
            Object(NonMatching, "revolution/dvd/dvderror.c"),
            Object(NonMatching, "revolution/dvd/dvdidutils.c"),
            Object(NonMatching, "revolution/dvd/dvdFatal.c"),
            Object(NonMatching, "revolution/dvd/dvdDeviceError.c"),
            Object(NonMatching, "revolution/dvd/dvd_broadway.c"),
        ]
    ),
    RevolutionLib(
        "vi",
        [
            Object(NonMatching, "revolution/vi/vi.c"),
            Object(NonMatching, "revolution/vi/i2c.c"),
            Object(NonMatching, "revolution/vi/vi3in1.c"),
        ]
    ),
    RevolutionLib(
        "pad",
        [
            Object(NonMatching, "revolution/pad/Pad.c"),
        ]
    ),
    RevolutionLib(
        "ai",
        [
            Object(NonMatching, "revolution/ai/ai.c"),
        ]
    ),
    RevolutionLib(
        "ax",
        [
            Object(NonMatching, "revolution/ax/AX.c"),
            Object(NonMatching, "revolution/ax/AXAlloc.c"),
            Object(NonMatching, "revolution/ax/AXAux.c"),
            Object(NonMatching, "revolution/ax/AXCL.c"),
            Object(NonMatching, "revolution/ax/AXOut.c"),
            Object(NonMatching, "revolution/ax/AXSPB.c"),
            Object(NonMatching, "revolution/ax/AXVPB.c"),
            Object(NonMatching, "revolution/ax/AXComp.c"),
            Object(NonMatching, "revolution/ax/DSPCode.c"),
            Object(NonMatching, "revolution/ax/AXProf.c"),
            
        ]
    ),
    RevolutionLib(
        "axfx",
        [
            Object(NonMatching, "revolution/axfx/AXFXReverbHi.c"),
            Object(NonMatching, "revolution/axfx/AXFXReverbHiExp.c"),
            Object(NonMatching, "revolution/axfx/AXFXHooks.c"),
            
        ]
    ),
    RevolutionLib(
        "dsp",
        [
            Object(NonMatching, "revolution/dsp/dsp.c"),
            Object(NonMatching, "revolution/dsp/dsp_debug.c"),
            Object(NonMatching, "revolution/dsp/dsp_task.c"),
            
        ]
    ),
    RevolutionLib(
        "gx",
        [
            Object(NonMatching, "revolution/gx/GXInit.c"),
            Object(NonMatching, "revolution/gx/GXFifo.c"),
            Object(NonMatching, "revolution/gx/GXAttr.c"),
            Object(NonMatching, "revolution/gx/GXMisc.c"),
            Object(NonMatching, "revolution/gx/GXGeometry.c"),
            Object(NonMatching, "revolution/gx/GXFrameBuf.c"),
            Object(NonMatching, "revolution/gx/GXLight.c"),
            Object(NonMatching, "revolution/gx/GXTexture.c"),
            Object(NonMatching, "revolution/gx/GXBump.c"),
            Object(NonMatching, "revolution/gx/GXTev.c"),
            Object(NonMatching, "revolution/gx/GXPixel.c"),
            Object(NonMatching, "revolution/gx/GXDisplayList.c"),
            Object(NonMatching, "revolution/gx/GXTransform.c"),
            Object(NonMatching, "revolution/gx/GXPerf.c"),
            
        ]
    ),
    {
        "lib": "PowerPC_EABI_Support/Runtime",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "runtime",  # str | List[str]
        "objects": [
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/New.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/__mem.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/__va_arg.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/global_destructor_chain.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/NMWException.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/ptmf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/MWRTTI.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/runtime.c"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/__init_cpp_exceptions.cpp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/Gecko_ExceptionPPC.cp"),
            Object(NonMatching, "PowerPC_EABI_Support/Runtime/Src/GCN_mem_alloc.c"),
        ],
    },
    {
        "lib": "PowerPC_EABI_Support/MSL",
        "mw_version": config.linker_version,
        "cflags": cflags_base,
        "progress_category": "msl",  # str | List[str]
        "objects": [
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/alloc.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/errno.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ansi_files.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Src/ansi_fp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/ctype.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/locale.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/buffer_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/direct_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/file_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/file_pos.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mbstring.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/mem_funcs.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/math_api.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/misc_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/printf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/float.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wctype.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/scanf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/signal.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/string.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/strtoul.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wmem.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wprintf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wstring.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/wchar_io.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/SRC/uart_console_io_gcn.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/PPC_EABI/SRC/abort_exit_ppc_eabi.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/secure_error.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Src/math_sun.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_atan2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_log10.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_pow.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_rem_pio2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_cos.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_rem_pio2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_sin.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/k_tan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_atan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_copysign.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_cos.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_floor.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_frexp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_ldexp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_sin.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/s_tan.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_atan2.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_log10.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_pow.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/e_sqrt.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common_Embedded/Math/Double_precision/w_sqrt.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MSL/MSL_C/MSL_Common/Src/extras.c"),
        ],
    },
    RevolutionLib(
        "exi",
        [
            Object(NonMatching, "revolution/exi/EXIBios.c"),
            Object(NonMatching, "revolution/exi/EXIUart.c"),
            Object(NonMatching, "revolution/exi/EXICommon.c"),
        ]
    ),
    RevolutionLib(
        "si",
        [
            Object(NonMatching, "revolution/si/SIBios.c"),
            Object(NonMatching, "revolution/si/SISamplingRate.c"),
        ]
    ),
    {
        "lib": "PowerPC_EABI_Support/MetroTRK",
        "mw_version": config.linker_version,
        "cflags": cflags_base,
        "progress_category": "metrotrk",  # str | List[str]
        "objects": [
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/targsupp.s"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/custconn/cc_gdev.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/custconn/MWCriticalSection_gc.cpp"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/custconn/CircleBuffer.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/flush_cache.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/main_TRK.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/mainloop.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/mem_TRK.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/dispatch.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/dolphin_trk.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/dolphin_trk_glue.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/notify.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/nubevent.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/nubinit.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/serpoll.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/string_TRK.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/support.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/targcont.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/mpc_7xx_603e.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/msg.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/msgbuf.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/msghndlr.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/mslsupp.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/targimpl.c"),
            Object(NonMatching, "PowerPC_EABI_Support/MetroTRK/target_options.c"),
        ],
    },
    RevolutionLib( # not actually rvl sdk
        "ndevexi2a",
        [
            Object(NonMatching, "NdevExi2A/DebuggerDriver.c"),
            Object(NonMatching, "NdevExi2A/exi.c"),
        ]
    ),
    RevolutionLib(
        "mem",
        [
            Object(Matching, "revolution/mem/mem_heapCommon.c"),
            Object(NonMatching, "revolution/mem/mem_expHeap.c"),
            Object(Matching, "revolution/mem/mem_frameHeap.c"),
            Object(Matching, "revolution/mem/mem_allocator.c"),
            Object(Matching, "revolution/mem/mem_list.c"),
        ]
    ),
    RevolutionLib(
        "euart",
        [
            Object(NonMatching, "revolution/euart/euart.c"),
        ]
    ),
    RevolutionLib(
        "fs",
        [
            Object(NonMatching, "revolution/fs/fs.c"),
        ]
    ),
    RevolutionLib(
        "ipc",
        [
            Object(NonMatching, "revolution/ipc/ipcMain.c"),
            Object(NonMatching, "revolution/ipc/ipcclt.c"),
            Object(NonMatching, "revolution/ipc/memory.c"),
            Object(NonMatching, "revolution/ipc/ipcProfile.c"),
        ]
    ),
    RevolutionLib(
        "nand",
        [
            Object(NonMatching, "revolution/nand/nand.c"),
            Object(NonMatching, "revolution/nand/NANDOpenClose.c"),
            Object(NonMatching, "revolution/nand/NANDCore.c"),
            Object(NonMatching, "revolution/nand/NANDCheck.c"),
            Object(NonMatching, "revolution/nand/NANDLogging.c"),
            Object(NonMatching, "revolution/nand/NANDErrorMessage.c"),
            
        ]
    ),
    RevolutionLib(
        "sc",
        [
            Object(NonMatching, "revolution/sc/scsystem.c"),
            Object(NonMatching, "revolution/sc/scapi.c"),
            Object(NonMatching, "revolution/sc/scapi_prdinfo.c"),
        ]
    ),
    RevolutionLib(
        "wenc",
        [
            Object(NonMatching, "revolution/wenc/wenc.c"),
        ]
    ),
    RevolutionLib(
        "arc",
        [
            Object(NonMatching, "revolution/arc/arc.c"),
        ]
    ),
    RevolutionLib(
        "cx",
        [
            Object(NonMatching, "revolution/cx/CXStreamingUncompression.c"),
            Object(NonMatching, "revolution/cx/CXUncompression.c"),
            Object(NonMatching, "revolution/cx/CXSecureUncompression.c"),
        ]
    ),
    RevolutionLib(
        "wpad",
        [
            Object(NonMatching, "revolution/wpad/WPAD.c"),
            Object(NonMatching, "revolution/wpad/WPADHIDParser.c"),
            Object(NonMatching, "revolution/wpad/WPADEncrypt.c"),
            Object(NonMatching, "revolution/wpad/WPADMem.c"),
            Object(NonMatching, "revolution/wpad/lint.c"),
            Object(NonMatching, "revolution/wpad/WUD.c"),
            Object(NonMatching, "revolution/wpad/WUDHidHost.c"),
            
        ]
    ),
    RevolutionLib(
        "bte",
        [
            Object(NonMatching, "revolution/bte/gki/gki_buffer.c"),
            Object(NonMatching, "revolution/bte/gki/gki_time.c"),
            Object(NonMatching, "revolution/bte/gki/gki_ppc.c"),
            Object(NonMatching, "revolution/bte/hci/hcisu_h2.c"),
            Object(NonMatching, "revolution/bte/hci/uusb_ppc.c"),
            Object(NonMatching, "revolution/bte/bta/dm/bta_dm_cfg.c"),
            Object(NonMatching, "revolution/bte/bta/hh/bta_hh_cfg.c"),
            Object(NonMatching, "revolution/bte/bta/sys/bta_sys_cfg.c"),
            Object(NonMatching, "revolution/bte/main/bte_hcisu.c"),
            Object(NonMatching, "revolution/bte/main/bte_init.c"),
            Object(NonMatching, "revolution/bte/main/bte_logmsg.c"),
            Object(NonMatching, "revolution/bte/main/bte_main.c"),
            Object(NonMatching, "revolution/bte/main/btu_task1.c"),
            Object(NonMatching, "revolution/bte/bta/sys/bd.c"),
            Object(NonMatching, "revolution/bte/bta/sys/bta_sys_conn.c"),
            Object(NonMatching, "revolution/bte/bta/sys/bta_sys_main.c"),
            Object(NonMatching, "revolution/bte/bta/sys/ptim.c"),
            Object(NonMatching, "revolution/bte/bta/sys/utl.c"),
            Object(NonMatching, "revolution/bte/bta/dm/bta_dm_act.c"),
            Object(NonMatching, "revolution/bte/bta/dm/bta_dm_api.c"),
            Object(NonMatching, "revolution/bte/bta/dm/bta_dm_main.c"),
            Object(NonMatching, "revolution/bte/bta/dm/bta_dm_pm.c"),
            Object(NonMatching, "revolution/bte/bta/hh/bta_hh_act.c"),
            Object(NonMatching, "revolution/bte/bta/hh/bta_hh_api.c"),
            Object(NonMatching, "revolution/bte/bta/hh/bta_hh_main.c"),
            Object(NonMatching, "revolution/bte/bta/hh/bta_hh_utils.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_acl.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_dev.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_devctl.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_discovery.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_inq.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_main.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_pm.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_sco.c"),
            Object(NonMatching, "revolution/bte/stack/btm/btm_sec.c"),
            Object(NonMatching, "revolution/bte/stack/btu/btu_hcif.c"),
            Object(NonMatching, "revolution/bte/stack/btu/btu_init.c"),
            Object(NonMatching, "revolution/bte/stack/wbt/wbt_ext.c"),
            Object(NonMatching, "revolution/bte/stack/gap/gap_api.c"),
            Object(NonMatching, "revolution/bte/stack/gap/gap_conn.c"),
            Object(NonMatching, "revolution/bte/stack/gap/gap_utils.c"),
            Object(NonMatching, "revolution/bte/stack/hcic/hcicmds.c"),
            Object(NonMatching, "revolution/bte/stack/hid/hidd_api.c"),
            Object(NonMatching, "revolution/bte/stack/hid/hidd_conn.c"),
            Object(NonMatching, "revolution/bte/stack/hid/hidd_mgmt.c"),
            Object(NonMatching, "revolution/bte/stack/hid/hidd_pm.c"),
            Object(NonMatching, "revolution/bte/stack/hid/hidh_api.c"),
            Object(NonMatching, "revolution/bte/stack/hid/hidh_conn.c"),
            Object(NonMatching, "revolution/bte/stack/l2cap/l2c_api.c"),
            Object(NonMatching, "revolution/bte/stack/l2cap/l2c_csm.c"),
            Object(NonMatching, "revolution/bte/stack/l2cap/l2c_link.c"),
            Object(NonMatching, "revolution/bte/stack/l2cap/l2c_main.c"),
            Object(NonMatching, "revolution/bte/stack/l2cap/l2c_utils.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/port_api.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/port_rfc.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/port_utils.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/rfc_l2cap_if.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/rfc_mx_fsm.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/rfc_port_fsm.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/rfc_port_if.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/rfc_ts_frames.c"),
            Object(NonMatching, "revolution/bte/stack/rfcomm/rfc_utils.c"),
            Object(NonMatching, "revolution/bte/stack/sdp/sdp_api.c"),
            Object(NonMatching, "revolution/bte/stack/sdp/sdp_db.c"),
            Object(NonMatching, "revolution/bte/stack/sdp/sdp_discovery.c"),
            Object(NonMatching, "revolution/bte/stack/sdp/sdp_main.c"),
            Object(NonMatching, "revolution/bte/stack/sdp/sdp_server.c"),
            Object(NonMatching, "revolution/bte/stack/sdp/sdp_utils.c"),
        ]
    ),
    RevolutionLib(
        "usb",
        [
            Object(NonMatching, "revolution/usb/usb.c"),
        ]
    ),
    RevolutionLib(
        "kpad",
        [
            Object(NonMatching, "revolution/kpad/KPAD.c"),
            Object(NonMatching, "revolution/kpad/KMPLS.c"),
            Object(NonMatching, "revolution/kpad/KZMplsTestSub.c"),
        ]
    ),
    RevolutionLib(
        "tpl",
        [
            Object(Matching, "revolution/tpl/TPL.c"),
        ]
    ),
    RevolutionLib(
        "esp",
        [
            Object(NonMatching, "revolution/esp/esp.c"),
        ]
    ),
    {
        "lib": "hbm",
        "mw_version": "Wii/1.0",
        "cflags": cflags_hbm,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "revolution/hbm/HBMFrameController.cpp"),
            Object(NonMatching, "revolution/hbm/HBMAnmController.cpp"),
            Object(NonMatching, "revolution/hbm/HBMGUIManager.cpp"),
            Object(NonMatching, "revolution/hbm/HBMController.cpp"),
            Object(NonMatching, "revolution/hbm/HBMRemoteSpk.cpp"),
            Object(NonMatching, "revolution/hbm/HBMAxSound.cpp"),
            Object(NonMatching, "revolution/hbm/HBMCommon.cpp"),
            Object(NonMatching, "revolution/hbm/HBMBase.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_animation.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_arcResourceAccessor.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_bounding.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_common.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_drawInfo.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_group.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_layout.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_material.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_pane.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_picture.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_resourceAccessor.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_textBox.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/lyt/lyt_window.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/math/math_triangular.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_binaryFileFormat.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_CharStrmReader.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_CharWriter.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_Font.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_LinkList.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_list.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_ResFont.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_ResFontBase.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_TagProcessorBase.cpp"),
            Object(NonMatching, "revolution/hbm/nw4hbm/ut/ut_TextWriterBase.cpp"),
            Object(NonMatching, "revolution/hbm/mix.cpp"),
            Object(NonMatching, "revolution/hbm/syn.cpp"),
            Object(NonMatching, "revolution/hbm/synctrl.cpp"),
            Object(NonMatching, "revolution/hbm/synenv.cpp"),
            Object(NonMatching, "revolution/hbm/synmix.cpp"),
            Object(NonMatching, "revolution/hbm/synpitch.cpp"),
            Object(NonMatching, "revolution/hbm/synsample.cpp"),
            Object(NonMatching, "revolution/hbm/synvoice.cpp"),
            Object(NonMatching, "revolution/hbm/seq.cpp"), 
        ],
    },
    Nw4rLib(
        "db",
        [
            Object(NonMatching, "nw4r/db/db_console.cpp"),
            Object(NonMatching, "nw4r/db/db_DbgPrintBase.cpp"),
            Object(NonMatching, "nw4r/db/db_assert.cpp"),
        ]
    ),
    Nw4rLib(
        "g3d",
        [
            Object(NonMatching, "nw4r/g3d/platform/g3d_cpu.cpp"),
            Object(NonMatching, "nw4r/g3d/g3d_state.cpp"),
            Object(NonMatching, "nw4r/g3d/g3d_init.cpp"),
            Object(NonMatching, "nw4r/g3d/g3d_fog.cpp"),
            Object(NonMatching, "nw4r/g3d/g3d_light.cpp"),
        ]
    ),
    Nw4rLib(
        "lyt",
        [
            Object(NonMatching, "nw4r/lyt/lyt_init.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_pane.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_group.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_layout.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_picture.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_textBox.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_window.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_bounding.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_material.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_texMap.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_drawInfo.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_animation.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_resourceAccessor.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_arcResourceAccessor.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_common.cpp"),
            Object(NonMatching, "nw4r/lyt/lyt_util.cpp"),
        ]
    ),
    Nw4rLib(
        "math",
        [
            Object(NonMatching, "nw4r/math/math_arithmetic.cpp"),
            Object(NonMatching, "nw4r/math/math_triangular.cpp"),
            Object(NonMatching, "nw4r/math/math_types.cpp"),
        ]
    ),
    Nw4rLib(
        "snd",
        [
            Object(NonMatching, "nw4r/snd/snd_AxManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxVoice.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxVoiceManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_AxfxImpl.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Bank.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BankFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BasicPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BasicSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_BiquadFilterPreset.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Channel.cpp"),
            Object(NonMatching, "nw4r/snd/snd_DisposeCallbackManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_DvdSoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_EnvGenerator.cpp"),
            Object(NonMatching, "nw4r/snd/snd_ExternalSoundPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FrameHeap.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxBase.cpp"),
            Object(NonMatching, "nw4r/snd/snd_FxReverbHi.cpp"),
            Object(NonMatching, "nw4r/snd/snd_InstancePool.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Lfo.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MemorySoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MidiSeqPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MmlParser.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MmlSeqTrack.cpp"),
            Object(NonMatching, "nw4r/snd/snd_MmlSeqTrackAllocator.cpp"),
            Object(NonMatching, "nw4r/snd/snd_NandSoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_PlayerHeap.cpp"),
            Object(NonMatching, "nw4r/snd/snd_RemoteSpeaker.cpp"),
            Object(NonMatching, "nw4r/snd/snd_RemoteSpeakerManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqSoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SeqTrack.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundActor.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchiveFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchiveLoader.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundArchivePlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundHeap.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundStartable.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundSystem.cpp"),
            Object(NonMatching, "nw4r/snd/snd_SoundThread.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmChannel.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_StrmSoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Task.cpp"),
            Object(NonMatching, "nw4r/snd/snd_TaskManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_TaskThread.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Voice.cpp"),
            Object(NonMatching, "nw4r/snd/snd_VoiceManager.cpp"),
            Object(NonMatching, "nw4r/snd/snd_Util.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveArchive.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveSound.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WaveSoundHandle.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WsdFile.cpp"),
            Object(NonMatching, "nw4r/snd/snd_WsdPlayer.cpp"),
            Object(NonMatching, "nw4r/snd/snd_adpcm.cpp"),
        ]
    ),
    Nw4rLib(
        "ut",
        [
            Object(NonMatching, "nw4r/ut/ut_list.cpp"),
            Object(NonMatching, "nw4r/ut/ut_LinkList.cpp"),
            Object(NonMatching, "nw4r/ut/ut_binaryFileFormat.cpp"),
            Object(NonMatching, "nw4r/ut/ut_CharStrmReader.cpp"),
            Object(NonMatching, "nw4r/ut/ut_TagProcessorBase.cpp"),
            Object(NonMatching, "nw4r/ut/ut_IOStream.cpp"),
            Object(NonMatching, "nw4r/ut/ut_FileStream.cpp"),
            Object(NonMatching, "nw4r/ut/ut_DvdFileStream.cpp"),
            Object(NonMatching, "nw4r/ut/ut_DvdLockedFileStream.cpp"),
            Object(NonMatching, "nw4r/ut/ut_LockedCache.cpp"),
            Object(NonMatching, "nw4r/ut/ut_Font.cpp"),
            Object(NonMatching, "nw4r/ut/ut_RomFont.cpp"),
            Object(NonMatching, "nw4r/ut/ut_ResFontBase.cpp"),
            Object(NonMatching, "nw4r/ut/ut_ResFont.cpp"),
            Object(NonMatching, "nw4r/ut/ut_ArchiveFontBase.cpp"),
            Object(NonMatching, "nw4r/ut/ut_ArchiveFont.cpp"),
            Object(NonMatching, "nw4r/ut/ut_CharWriter.cpp"),
            Object(NonMatching, "nw4r/ut/ut_TextWriterBase.cpp"),
        ]
    ),
    {
        "lib": "RVLFaceLibrary",
        "mw_version": config.linker_version,
        "cflags": cflags_base,
        "progress_category": "rfl",
        "objects": [
            Object(NonMatching, "RVLFaceLibrary/RFL_System.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_NANDLoader.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_NANDAccess.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_Model.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_MakeTex.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_Icon.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_HiddenDatabase.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_Database.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_Controller.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_MiddleDatabase.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_DefaultDatabase.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_DataUtility.c"),
            Object(NonMatching, "RVLFaceLibrary/RFL_Format.c"),
        ],
    },
    {
        "lib": "GameUtil",
        "mw_version": config.linker_version,
        "cflags": cflags_gameutil,
        "progress_category": "gameutil",
        "objects": [
            Object(Matching, "GameUtil/Mem.cpp"),
            Object(Matching, "GameUtil/FileManager.cpp"),
            Object(Matching, "GameUtil/Controller.cpp"),
            Object(Matching, "GameUtil/GraphicManager.cpp"),
            Object(NonMatching, "GameUtil/LayoutManager.cpp"),
            Object(Matching, "GameUtil/GameManager.cpp"),
            Object(Matching, "GameUtil/Scene.cpp"),
            Object(Matching, "GameUtil/FaderFlash.cpp"),
            Object(NonMatching, "GameUtil/Layout.cpp"),
            Object(Matching, "GameUtil/LayoutAnimation.cpp"),
            Object(Matching, "GameUtil/Random.cpp"),
            Object(Matching, "GameUtil/DiscMessages.cpp"),
            Object(NonMatching, "GameUtil/CellAnimManager.cpp"),
            Object(Matching, "GameUtil/CellAnim.cpp"),
            Object(NonMatching, "GameUtil/TickFlow.cpp"),
            Object(NonMatching, "GameUtil/TickFlowManager.cpp"),
            Object(Matching, "GameUtil/SoundManager.cpp"),
            Object(NonMatching, "GameUtil/InputChecker.cpp"),
            Object(NonMatching, "GameUtil/InputCheckManager.cpp"),
            Object(NonMatching, "GameUtil/TimeRatio.cpp"),
            Object(NonMatching, "GameUtil/List.cpp"), # matching; not linked because weak RTTI
            Object(NonMatching, "GameUtil/CheckPointManager.cpp"),
            Object(Matching, "GameUtil/SDManager.cpp"),
            Object(NonMatching, "GameUtil/DebugPrint.cpp"),
            Object(NonMatching, "GameUtil/VarParamManager.cpp"),
            Object(Matching, "GameUtil/code_801ED6A8.cpp"),
            Object(Matching, "GameUtil/code_801ED7D4.cpp"),
            Object(Matching, "GameUtil/DebugFont.cpp"),
        ],
    },
]

# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects

# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
    ProgressCategory("runtime", "Runtime Code"),
    ProgressCategory("msl", "MSL C/C++ Code"),
    ProgressCategory("metrotrk", "MetroTRK Code"),
    ProgressCategory("nw4r", "NW4R Code"),
    ProgressCategory("rfl", "RVLFaceLibrary Code"),
    ProgressCategory("gameutil", "Game Utility Code"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
