#include "captive_scene_assets.h"
#include <stdlib.h>

const char *const captive_boot_source_hashes[CAPTIVE_BOOT_SOURCE_COUNT] = {
    "71bcf404103f1ac2920800a8bc166939bb49a1204cf51bebce8aca7dd5faafde",
    "1ec1f90adbcfcb3b99b64a56cf1c669b409b7d3a76bc09cedb056f503bfb1959",
};

const char *const captive_view_source_hashes[CAPTIVE_VIEW_SOURCE_COUNT] = {
    "47ad15b4a593c37880d0306b6a0f51b7a9f20615cf6a188f23716d5b48315524",
    "43833e4a8df622f84d53698a76c6d18f910c1cca79c6b89cbfacc563f695356c",
    "8b7301fc6c302fd673a81d23e7a99d715aa02d5b404c1e1edea19ceccccc9681",
    "519d3ef4494f0e868479a90c8a47249b840598e382c7ba3272f417ce3daf5936",
    "7edb8ee856a91e835ea86dda00af49fda3dae730d694bd7234b7fa96d711e296",
    "4a2bc840d184ff07657f56e630b0293f1d5d7cdbf1d00e4505a1a69dcf721667",
    "2c8db6bfbec2b463856ab4cd9a313f9fbf20be408a2e278d94d498653562f754",
    "0b0d6ee225493c92b534b50e893d9c27e423ce0a4298e1789682b8cf222b7adc",
    "1f1b89e7692dc7c01f9d649677c820e79076304e8bc79835683e14484d68bb5b",
    "fed16e510697e17123d474c08687de548076b26a55f08f1d00fd17e3fcdf9410",
    "63ffa6901b59d463b050088065503d386ca2f3813ed91d8e0833320f9df2fe11",
    "48df42e6906bfd167981f19e89149aa4c5791297b6e92f3a87470b59e8d0f1f3",
    "303c540f9e88ca9a8e736541b3c6f9a9cb9817b8640b5133ca7721e6db667e1d",
    "4edc60eb7d530ed6a7b11673d26831eb6701f131df3f9e291d882f5b78c2de25",
    "978d18857d5ffcf6fb7b91fb22c02b85079db0171caeac3d290a69b276cf098f",
    "dec7143f063c98459ab2f267ed135204cdee1b521eda9810b219e8c10e05c7e8",
    "d7338db4df839f0b1090234f6b3e30db1ab43c936be5479d007f865a0175cc32",
    "21db7daf64cff3b0cae19c3e7eb2057762df9110055e7253175024ecb146fb6b",
    /* Remaining original surfaces may provide UI overlays, animated work
       buffers or exterior/interior state variants.  Keep them available to
       the compositor rather than guessing their role from archive names. */
    "dfca77f0e219962242226f11f9697f580f92e8ad24786296a5b2571b20c2b707",
    "ce00ba2bc78f160b934486fe101a90264163356e02e7acbea2a41cf5d125b017",
    "4f05bad2a2b5d7474b0dc3735bbec4b5650110b5e79d6902c5536a556f8c27f5",
    "6eb09e17fdd6b97bdef223b3ebab5c94f12010c8cba615536fd40bf1509299c0",
    "70e0b9bfbaa5dfd12643b50cbe10d0b664de2fb1106d8ff0f2fde1ce6f443bbe",
};

/* French translation by HRVG (2022-2023) of the DOS v1.06 release.
 * Identified from the user's media: CAPPO.EXE is LZEXE-packed 73 511 bytes
 * (data segment 0x0E4F instead of 0x0E3F), and the three surfaces carry the
 * translated title, credits and HUD labels. */
static const char *const captive_boot_source_alt_hashes[CAPTIVE_BOOT_SOURCE_COUNT] = {
    "ae3dd438a91f24a66b0062322a7e460fef1ff0de2dd085031a4618dc50fb90ab", /* CAPPO.EXE */
    "10df32ee401079617d3c392a6ed6bb6cdb92476f269c0faaa9ecb51d62fc579d", /* TEST0.ANM */
};

static const char *captive_view_source_alt_hashes[CAPTIVE_VIEW_SOURCE_COUNT] = {
    [18] = "2b92b7d254295dc23f8417d3387c5013c7522d54cac85ac88e74696414a17eb7", /* GAMESCRN.PL5 */
    [19] = "dbe724db6b474dcb455d610972c6f7e73c5ef9f1af21f2f0d98366eb5ca339bf", /* ICONS.PL5 */
};

const char *captive_view_source_alt_hash(int index) {
    if (index < 0 || index >= CAPTIVE_VIEW_SOURCE_COUNT) return NULL;
    return captive_view_source_alt_hashes[index];
}

const char *captive_required_source_alt_hash(size_t index) {
    if (index < CAPTIVE_BOOT_SOURCE_COUNT)
        return captive_boot_source_alt_hashes[index];
    if (index < CAPTIVE_REQUIRED_SOURCE_COUNT)
        return captive_view_source_alt_hash((int)(index - CAPTIVE_BOOT_SOURCE_COUNT));
    return NULL;
}

uint8_t *captive_find_required_source(const DataVFS *vfs, size_t index,
                                      size_t *size) {
    const char *primary = captive_required_source_hash(index);
    if (!vfs || !primary) return NULL;
    uint8_t *data = vfs_find_sha256(vfs, primary, size);
    if (data) return data;
    const char *alt = captive_required_source_alt_hash(index);
    return alt ? vfs_find_sha256(vfs, alt, size) : NULL;
}

bool captive_scene_assets_available(const DataVFS *vfs) {
    if (!vfs || !vfs->initialized) return false;
    for (int i = 0; i < CAPTIVE_VIEW_SOURCE_COUNT; ++i) {
        size_t size = 0;
        uint8_t *data = captive_find_required_source(
            vfs, (size_t)(CAPTIVE_BOOT_SOURCE_COUNT + i), &size);
        if (!data) return false;
        free(data);
    }
    return true;
}

const char *captive_required_source_hash(size_t index) {
    if (index < CAPTIVE_BOOT_SOURCE_COUNT)
        return captive_boot_source_hashes[index];
    if (index < CAPTIVE_REQUIRED_SOURCE_COUNT)
        return captive_view_source_hashes[index - CAPTIVE_BOOT_SOURCE_COUNT];
    return NULL;
}

bool captive_data_available(const DataVFS *vfs) {
    if (!vfs || !vfs->initialized) return false;
    for (size_t i = 0; i < CAPTIVE_REQUIRED_SOURCE_COUNT; ++i) {
        size_t size = 0;
        uint8_t *data = captive_find_required_source(vfs, i, &size);
        if (!data) return false;
        free(data);
    }
    return true;
}
