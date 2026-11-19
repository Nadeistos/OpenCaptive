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
| Enter (on or facing a ladder), Keypad 1/3, Page Up/Down | climb the ladder, whichever way it goes |
| 1-4 | select droid |
| I | inventory, T terminal, M map overlay |
| F5 / F9 | save / load (`opencaptive.sav` in the working directory) |
| Z | rest |

The right-hand control panel is clickable (left button):

| Panel button | Action |
| --- | --- |
| top-left / top-right arrow | turn left / right |
| up / down arrow | step forward / back |
| left / right arrow | strafe |
| ladder icons (row 3, left / right) | climb the ladder (either icon) |
| coloured droid bars | select droid 1-4 |
| four-colour button | inventory |
| coloured hands | that droid attacks with that hand (left / right) |
| disk | save the game (F9 loads) |
| zZ | rest: fast HP/energy regeneration until woken (creature within 2 cells, attack, or any key) |
| red hand (stop) | pause |

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
- **Droid inventory.** `I` (or the four-colour panel button) opens the
  selected droid's sheet inside the viewport: EQUIPMENT page (head, chest,
  arm, leg, foot, hand parts, both weapon hands, shield) and BAG page (10
  slots). Tab switches page, Up/Down or a click selects, Enter or a second
  click equips, uses (batteries recharge energy) or removes, 1-4 changes
  droid, Escape returns to the dungeon.
- **Saves, rest, pause.** The disk button (or F5) saves, F9 loads; both
  report in the message panel. Rest regenerates 1 HP and 1 energy per droid
  every 15 ticks. The pause screen dims the game screen once instead of
  fading it to black.
- **Hands and ranged weapons.** Each panel hand attacks with that hand's
  weapon only ("No weapon in that hand" otherwise); Space uses the best
  weapon in range. Ranged weapons (pistol, rifle, laser…) use the engine's
  per-weapon range and line of sight.
- **Wall devices and pits.** Buttons and levers show the DOORS1 hand panel on
  their wall (dimmed once used), power sockets the socket picture; pits are
  dark holes on the floor ahead.
- **Holomap mission number** is repainted over the reference capture from
  mission 2 on.
- **Messages** are cut to the 23 glyphs of the panel. The panel shows the
  two newest messages (5 s each) above a standing hint line: what Enter does
  with the cell the party stands on or faces (ladder, door, locked door,
  shop, generator), otherwise the objective ("Destroy the generator" when
  the level has one, else "Find a ladder").
- **Accents.** The 5x7 font draws acute, grave, circumflex, diaeresis and
  cedilla marks over the base letter, and descenders (g, p, q, comma) on
  an eighth row.
- **Creature damage** is a linear approximation, `base+4 .. 2*base+4` with
  `base = 2 + category + level` (capped at 20), before armour. The former
  `lo*hi` product hit for about 40 per blow on the first base.
- **Energy.** Walking costs each droid one energy every fourth step (it
  was one per step, which emptied the party on a three-level base; attacks
  need 3 energy). Passive regeneration and rest are unchanged.
- **Creature alert.** Besides the close `range+2` radius, a creature that
  sees the party within 6 cells starts chasing it.
- **Shop cell.** The Architect can wall a generator in behind the shop, so
  Enter on the shop cell uses the generator or door ahead before opening
  the shop.
- **Mission flow.** Destroying the last generator (Enter in front of it)
  completes the base, advances the mission and returns to the holomap; `L`
  lands on the next base with the same party.
- **Test aids.** `--input-script "Right,Up,Space"` replays SDL key names,
  one every 12 frames (no length limit; `--input-script @file` reads the
  list from a file, commas or newlines). With `--capture-frame` the capture
  waits for the script and prints the end state: position, creatures,
  droid HP and energy, mission number and the last 8 messages.
  `--place-party LEVEL,X,Y,DIR` starts the dungeon at a given cell.
  A `Click:X:Y` token clicks the control panel at canvas coordinates and
  `Wait:N` idles N frames without input.
  `CAPTIVE_DUMP_MAP=1` prints every level of each landed base as ASCII
  (`#` wall, `+` door, `L` locked, `U`/`D` ladder, `S` shop, `G`
  generator; the party cell is on a `PARTY level x,y` line), and again at
  the end of a scripted capture. Add `--fps 0` to scripted captures: the
  simulation is tick-based, so the result is identical and several times
  faster.

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
- The holomap between missions is the author's English reference capture;
  only the mission digits are repainted (5x7 font).
- One hand-picked near-size sprite per ALIEN sheet (and per CAPPO frame
  byte on ALIEN2) is scaled for every depth; CAPPO's per-type, per-depth
  creature pieces are not mapped, so several types share a picture and
  there is no animation.
- The top monitors show a simple status panel, not the original device
  views.
- The control panel is copied from the original landed-dungeon capture
  (`assets/captive/landed-dungeon-reference.png`), so it is static: the
  greyed "climb down" icon of that capture is always shown.
- The disk saves straight away; there is no save/load menu or slot choice.
- Levers, triple levers and buttons share one picture; hidden buttons and
  electric wall traps stay invisible; pressure plates are not drawn.
- No shot or muzzle-flash effect is drawn for ranged attacks.
- The landing zone uses the base wall set instead of the planet surface.
- Levels are not Captive's real levels (see `--captive-gm-map`).

## Status (2026-11-19)

- Playable end to end by script: an autoplay run (headless, `--fps 0`,
  `CAPTIVE_DUMP_MAP=1`, path-finding on the dumped map, fighting adjacent
  creatures, resting below 45 HP or 12 energy) completed missions 1, 2 and
  3 in a row with the same party (mission 3: five levels, four
  generators), then stopped on its own time limit inside mission 4 after
  two of its generators. Still not played by hand over a full mission.
- Fixed on the way: creature damage, the message panel (standing hint
  line, objective), accents, walking energy drain, creatures that never
  noticed the party, Enter on a shop cell hiding the generator behind it,
  and the 256-key cap of `--input-script`.
- Still to look at: the droids rarely meet creatures (9 fights over
  mission 3); "Le robot 1 tire !" is printed for melee blows too; levels
  and art remain the approximations listed above.
- Suggested next step: a real hand-played mission (window, mouse and
  keyboard) to judge pace and difficulty, then decide whether to tune the
  balance further or return to decoding CAPPO's real rules.
