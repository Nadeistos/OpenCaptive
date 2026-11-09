#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "captive_view_window.h"
#include "texture_atlas.h"
#include <stdint.h>

/* Render the first-person dungeon viewport into the framebuffer.
 * Uses the 19-cell view window and hash-verified PL5 panel sheets.
 *
 * The viewport area is CAPTIVE_VIEWPORT_WIDTH x CAPTIVE_VIEWPORT_HEIGHT
 * pixels at position (CAPTIVE_VIEWPORT_X, CAPTIVE_VIEWPORT_Y) in the
 * 320x200 framebuffer. */
void viewport_render(const CaptiveViewWindow *window,
                     const TextureAtlas *atlas,
                     uint32_t *framebuffer, int fb_width, int fb_height);

/* Render the recovered CAPPO panel compositor.  This path uses the original
 * descriptor draw list and verified PL5 sheets; it is intentionally separate
 * from viewport_render(), whose compatibility geometry remains covered by
 * the legacy unit fixtures. */
void viewport_render_original_descriptors(const CaptiveViewWindow *window,
                                          const TextureAtlas *atlas,
                                          uint32_t *framebuffer,
                                          int fb_width, int fb_height);


/* Resolve a recovered DOS descriptor source bank to its content-addressed
 * panel sheet. Bank 4 is the ROOFS sheet in CAPPO's descriptor table. */
int viewport_descriptor_source_sheet(const TextureAtlas *atlas,
                                     uint8_t source_bank);

#include "combat.h"

/* Playable corridor renderer.  Paints far-to-near, outer-to-centre, using the
 * CAPPO descriptor records as a piece catalogue: ceiling, floor, front faces
 * at planes 1..4 and side faces between planes 0..4, with creatures drawn
 * inside their cell before nearer walls.  `parity` alternates the mirrored
 * piece variants as the party moves.  `gs`/`cl` may be NULL (no creatures). */
void viewport_render_playable(const CaptiveViewWindow *window,
                              const TextureAtlas *atlas, int parity,
                              const GameState *gs, const CreatureList *cl,
                              uint32_t *framebuffer, int fb_width, int fb_height);

void viewport_render_creatures(const GameState *gs, const CreatureList *cl,
                               const TextureAtlas *atlas,
                               uint32_t *framebuffer, int fb_width, int fb_height);

#endif
