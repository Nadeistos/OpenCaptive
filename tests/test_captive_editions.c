/* The French HRVG translation is accepted as an alternative identity for the
 * four DOS v1.06 sources that carry text, and for nothing else. */
#include "captive_scene_assets.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int is_sha256_hex(const char *h) {
    if (!h || strlen(h) != 64) return 0;
    for (const char *c = h; *c; ++c)
        if (!((*c >= '0' && *c <= '9') || (*c >= 'a' && *c <= 'f'))) return 0;
    return 1;
}

int main(void) {
    const size_t with_alt[] = {
        0,                              /* CAPPO.EXE */
        1,                              /* TEST0.ANM */
        CAPTIVE_BOOT_SOURCE_COUNT + 18, /* GAMESCRN.PL5 */
        CAPTIVE_BOOT_SOURCE_COUNT + 19, /* ICONS.PL5 */
    };
    int alt_count = 0;
    for (size_t i = 0; i < CAPTIVE_REQUIRED_SOURCE_COUNT; ++i) {
        const char *alt = captive_required_source_alt_hash(i);
        int expected = 0;
        for (size_t k = 0; k < sizeof(with_alt) / sizeof(with_alt[0]); ++k)
            if (with_alt[k] == i) expected = 1;
        if (!expected) {
            assert(alt == NULL);
            continue;
        }
        assert(is_sha256_hex(alt));
        assert(strcmp(alt, captive_required_source_hash(i)) != 0);
        /* An alternative must never collide with any primary identity. */
        for (size_t j = 0; j < CAPTIVE_REQUIRED_SOURCE_COUNT; ++j)
            assert(strcmp(alt, captive_required_source_hash(j)) != 0);
        alt_count++;
    }
    assert(alt_count == 4);
    assert(captive_required_source_alt_hash(CAPTIVE_REQUIRED_SOURCE_COUNT) == NULL);
    assert(captive_view_source_alt_hash(-1) == NULL);
    assert(captive_view_source_alt_hash(CAPTIVE_VIEW_SOURCE_COUNT) == NULL);
    assert(captive_find_required_source(NULL, 0, NULL) == NULL);
    printf("captive editions: %d alternative identities OK\n", alt_count);
    return 0;
}
