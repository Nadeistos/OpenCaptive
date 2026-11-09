# Playable Captive dungeon (fork: playable first, fidelity later)

This fork makes the native Captive dungeon playable end to end before every
rule is recovered from CAPPO.EXE. Anything below marked *approximation* is a
known deviation from the original, to be replaced as the reverse engineering
progresses.

## How to run

```sh
./build/opencaptive --data gamedata/captive-dos --game captive --captive-dungeon --lang fr
```

From the holomap, `L` also enters the dungeon.

| Key | Action |
| --- | --- |
| Up / W, Down / S | step forward / back |
| Left / A, Right / D | turn |
| Q / E | strafe |
| Space | selected droid attacks |
| F / Enter | interact: open door, use shop, destroy generator |
| Keypad 1 / Keypad 3 | climb up / down a ladder (stairs cell) |
| 1-4 | select droid |
| I | inventory, T terminal, M map overlay |

## What changed

- **Dungeon source.** `--captive-dungeon` now uses the legacy Architect
  generator (`map_gen.c`: landing zone + base with doors, locked doors,
  stairs, shop and generator). The byte-exact GM.EXE port stays available
  with `--captive-gm-map`, but its cell values are not decoded yet (walls
  only).
- **Corridor renderer** (`viewport_render_playable`). Uses the CAPPO
  descriptor records as a catalogue of wall pieces: front faces on planes
  1-4 (IDs 14/16/18, 8/10/12, 2/4/6, 40-48), side faces between planes
  (IDs 20-35), with the mirrored variant chosen by cell parity.
- **Gameplay loop.** The native Captive path had its input and tick cut off.
  The playable dungeon now routes keys to the full legacy handler and runs a
  small tick (movement cooldown, energy/HP regeneration, creature AI and
  attacks, game over). Creatures and puzzles are spawned on entry.
- **Starting party.** The roster was empty (0 HP); four default droids are
  created as in `game_state_init()`.
- **Test aid.** `--input-script "Right,Up,Space"` replays SDL key names, one
  every 12 frames; with `--capture-frame` the capture waits for the script
  and prints the end state (position, creatures, droid HP).

## Known approximations

- Floor and ceiling copy the two 144x44 halves of `ROOFS.PL5` (floor
  mirrored). The descriptor bank-4 offsets (floor bands, IDs 50-105) do not
  address ROOFS as stored on disk.
- Doors are the closed iris from `DOORS1.PL5` scaled onto the front face;
  locked doors are tinted red. The original per-depth door pieces are not
  mapped yet.
- Stairs, shop, generator and terminal cells are drawn as open floor.
- Creature sprites come from the author's earlier renderer and are drawn in
  pieces.
- The landing zone uses the base wall set instead of the planet surface.
- Levels are not Captive's real levels (see `--captive-gm-map`).
