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
| F / Enter | interact: open door, enter shop, destroy generator |
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
- **Creatures** are drawn whole, inside their cell (behind nearer walls),
  scaled between the two planes of the cell.
- **Droid HUD.** The four top monitors show each droid's name, HP (green, red
  when low) and energy (blue) bars and numbers; the selected droid has a
  yellow frame. The text panel under the viewport shows the last messages.
- **French catalogue** (`po/fr.po`) is complete; the 5x7 font folds accents.
- **Cell features.** Ladders (stairs), the generator and the shop door are
  drawn standing in their cell.
- **Shop.** Facing the shop door, Enter opens the stock list inside the
  viewport (the game screen stays visible): Up/Down select, Enter buys for the
  selected droid (1-4 to change), R repairs, Escape leaves. Purchases are
  reported in the message panel.
- **Mission flow.** Destroying the last generator (Enter in front of it)
  completes the base, advances the mission and returns to the holomap; `L`
  lands on the next base with the same party.
- **Test aids.** `--input-script "Right,Up,Space"` replays SDL key names, one
  every 12 frames; with `--capture-frame` the capture waits for the script
  and prints the end state (position, creatures, droid HP).
  `--place-party LEVEL,X,Y,DIR` starts the dungeon at a given cell.

## Known approximations

- Floor and ceiling copy the two 144x44 halves of `ROOFS.PL5` (floor
  mirrored). The descriptor bank-4 offsets (floor bands, IDs 50-105) do not
  address ROOFS as stored on disk.
- Doors are the closed iris from `DOORS1.PL5` scaled onto the front face;
  locked doors are tinted red. The original per-depth door pieces are not
  mapped yet.
- Ladder (WALLA), generator and shop door (DOORS1) are hand-picked pictures
  scaled per depth; terminals, pits and pressure plates are drawn as floor.
  SHOP1/SHOP2 are scenery pieces, not a shop screen, so the shop is a text
  list.
- The holomap between missions is the author's English reference capture
  and always reads "MISSION 0001".
- One hand-picked near-size sprite per ALIEN sheet (and per CAPPO frame
  byte on ALIEN2) is scaled for every depth; CAPPO's per-type, per-depth
  creature pieces are not mapped, so several types share a picture and
  there is no animation.
- The top monitors show a simple status panel, not the original device
  views; the right-hand control panel is not drawn yet.
- The landing zone uses the base wall set instead of the planet surface.
- Levels are not Captive's real levels (see `--captive-gm-map`).
