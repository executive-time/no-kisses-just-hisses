# NO KISSES JUST HISSES — Spec v0.2

Status: **goedgekeurd als richting** (2026-11-15), details per fase. Basis: SLOOP 2.8.5 op de M-VAVE FM-1.

## Ontwerpregel
Elke functie draagt bij aan **groove, agressie of live-manipulatie**. Wat dat niet doet, krijgt geen
prioriteit en geen plek in de snelle bediening. Geen nieuwe hardware: we werken met de draaiknoppen
(KNOB 1–4, PRESETS, SELECT, ALGORITHM), 14 drukknoppen en 27 toetsen.

## Wat SLOOP al heeft (en wat we hergebruiken)
| Wens | In SLOOP 2.8.5 | Wat wij doen |
| --- | --- | --- |
| Saturatie, fuzz, fold, bitcrush per track | INSERT: SOFT, HARD, FOLD, FUZZ, CRUSH | Standaard aan op de juiste tracks; via macro's |
| Stutter, rewind, freeze, tape stop, half speed, loops | 27 punch-in effecten (FX + toets) | Herordenen: de vernielende eerst |
| DJ-filter, sidechain, vinyl/tape | FILT, DUCK, DUST, TEXTURE (VINYL, TAPE, RADIO) | Behouden; DUCK standaard aan |
| P-locks, microtiming, ratchets, ghost notes, fills | aanwezig | Ongewijzigd |
| Probability | alleen in ARP (PROB) | **Nieuw**: per step |
| Pattern mutation, controlled chaos | ontbreekt | **Nieuw** |
| Macro's (één knop, meerdere parameters) | ontbreekt | **Nieuw**: DESTROY |

## 1. Klankidentiteit: vijf families
DIRTY BASS · PERCUSSION · NOISE & TEXTURE · RAVE & ACID · BROKEN MACHINE.
De presetlijst (PRESETS-knop) toont eerst een eigen bank van ±40 NKJH-klanken in deze vijf families.
De bestaande SLOOP-klanken blijven beschikbaar achteraan, maar bepalen niet de identiteit.

**Engines** (voorstel):
- Behouden: ANALOG (acid, saws, reese), DIGITAL en FM6 (FM-bass, metaal), PHASE, LOFI (1-bit, chip noise),
  TRIO (rave stabs, hoover), NOISE (hiss, metal, radio), GRAIN (texturen, drones), SAMPLE (eigen samples).
- Verwijderen: **WHEEL** (orgel), **VOICE** (formantstem) en **PHYS** (fysieke modellen).
- Fabriekssamples van SAMPLE (piano, vibes, hoorns, strijkers, fluit) eruit: dat scheelt ±280 KB
  firmware. Of dat ook geheugen vrijmaakt voor nieuwe effecten, meten we eerst.

**Drumkits**: van 37 naar ±14. Behouden: TECHNO, INDUSTR, HYPER, GLITCH, ELECTRO, 909, 808, JUNGLE
(breakcore), MINIMAL. Nieuw (als data voor de bestaande drumsynth, geen nieuwe code): **GABBER**
(overstuurde 909-kick), **EBM**, **WAREHOUSE**, **NOISE**, **RUST** (metaal, ketting, plaatstaal).

## 2. Vaste rollen voor de vier tracks
| Track | Rol | Standaardklank | Standaardinsert |
| --- | --- | --- | --- |
| 1 | **BASS**: mono, acid, distorted | ANALOG acid-bass, MONO + glide | FUZZ |
| 2 | **RHYTHM SYNTH**: stabs, metaal, synth-percussie | TRIO rave stab | HARD |
| 3 | **DAMAGE**: noise, drones, risers, glitch | NOISE (METAL / RADIO) | CRUSH |
| 4 | **DRUMS** | kit INDUSTR | SOFT (lijm), DUCK aan |

Nieuw project: **165 BPM**, swing 0, deze vier klanken, en DUCK op 40 %. De trackkleuren en
-namen op het scherm worden BASS / SYNTH / DMG / DRUM.

## 3. Effecten in drie groepen
**A. PERMANENT DAMAGE** (per track, de vaste klank): drive (DST), INSERT (SOFT/HARD/FOLD/FUZZ/CRUSH),
trackfilter met resonantie. Bestaat; alleen standaarden en volgorde veranderen.

**B. PERFORMANCE DAMAGE** (FX + toets, zolang je vasthoudt). De witte toetsen krijgen een nieuwe volgorde,
de vernielende effecten op de makkelijkste plekken:

| Toets | Effect | Toets | Effect |
| --- | --- | --- | --- |
| 1 | loop 1/4 | 9 | tape stop |
| 2 | loop 1/8 | 10 | rewind |
| 3 | loop 1/16 | 11 | half speed (pitch drop) |
| 4 | loop 1/32 | 12 | freeze |
| 5 | stutter 1/16T | 13 | bit crush |
| 6 | gate 1/16 | 14 | alias (sample-rate drop) |
| 7 | reverse | 15 | kill low |
| 8 | echo 1/8. | 16 | fade out |

De zwarte toetsen houden de rest (kill mid/high, flanger, oct up, mono, auto-pan, loop 8T, LP/HP sweep, phone, wobble).

**C. MASTER DAMAGE**: master-saturatie (**nieuw**, via GRIT), DJ-filter (FILT), DUST, TEXTURE, DUCK,
delay met feedback (via SPACE). Alles vóór de bestaande limiter, die clipping op de uitgang blijft voorkomen.

## 4. DESTROY: zeven macro's
**Bediening:** houd **SEL** vast: de vier draaiknoppen worden macro's. Een eigen laag, los van FX: met de ene hand
de punch-ins (FX + toets), met de andere de macro's. **SEL + OCT+** wisselt tussen pagina A en B. De toetsen in de
SEL-laag zijn vrij voor later (bijvoorbeeld macro-snapshots). Toonsoort, toonladder en akkoorden blijven bereikbaar
via **SEL tikken** (de SEL-pagina's); de oude SEL-laag (vasthouden) vervalt. FX blijft zoals in SLOOP.

| Pagina | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 |
| --- | --- | --- | --- | --- |
| **A** | **GRIT** | **BITE** | **CRUSH** | **SPACE** |
| **B** | **REPEAT** | **GRAIN** | **CHAOS** | **FILT** (DJ-filter) |

| Macro | 0 → 50 % (bruikbaar) | 50 → 100 % (ontsporen) | Stuurt intern |
| --- | --- | --- | --- |
| GRIT | warme saturatie | brute clipping | master-drive, toon kantelt donkerder, volumecompensatie |
| BITE | meer resonantie | piepende, bijtende filters | resonantie van alle trackfilters + cutoff-verschuiving |
| CRUSH | lichte degradatie | 4-bit, aliasing | bits en sample rate van de mix (DUST-code, zonder lowpass/hiss) |
| SPACE | korte slapback | zelfoscillerende feedback | delay-sends, delay-feedback (begrensd, met saturatie in de lus), reverb |
| REPEAT | uit tot loop 1/4 | 1/8 → 1/16 → 1/32 | de bestaande loop-punch-in, met vaste trappen |
| GRAIN | mix gaat deels in freeze | volledige freeze | de bestaande FREEZE als mengbaar effect |
| CHAOS | kleine variaties | breakcore | zie §5 (sequencer) |

Regels voor elke macro: positie 0 = exact het geluid zonder macro (de test vergelijkt het). Het niveau
blijft ongeveer gelijk (±3 dB) tot 50 %. De knop glijdt (geen zipper-ruis). De waarden worden per
song-sectie (A–D) bewaard en zijn per step te locken.

## 5. Groove engine
- **PROBABILITY per step**: in het bestaande step-blad (SEQ + step + OCT+ vasthouden) komt een rij
  **PROB**: 100 / 87 / 75 / 50 / 25 / 12 %. Werkt op synth- en drumsteps; een overgeslagen step is helemaal stil
  (zoals een fill-conditie). Herhaalbaar per patroon-doorloop, zodat het niet "zwemt".
- **MUTATE**: **GLO + toets 11** (vasthouden = tijdelijk, loslaten = terug). Het patroon krijgt verplaatste hits,
  extra ratchets, andere velocities en nudges, binnen de maat. **GLO + toets 12** houdt de mutatie vast;
  EDIT + OCT− maakt dat ongedaan.
- **CHAOS** (macro, pagina B): de hoeveelheid variatie bij elke doorloop. Laag: velocity en microtiming wiebelen.
  Midden: ghost notes en ratchets erbij. Hoog: hits verspringen. Altijd vast: de kick op de tellen, de
  toonhoogte binnen de toonladder, en de patroonlengte. Je groove blijft herkenbaar.

## 6. Wat we niet doen
Geen nieuwe synth-engine. Geen nieuwe effecten voordat de bestaande goed speelbaar zijn. Geen
menu per functie. Geen onbegrensde randomisatie. Geen DSP die de CPU-budgettest laat falen of
stemmen doet wegvallen bij een vol patroon op vier tracks.

## 7. Fases
| Fase | Inhoud | Eerste stap |
| --- | --- | --- |
| **1. Strippen en cureren** | standaardproject, trackrollen, kits, presetbank, WHEEL/VOICE/PHYS eruit, punch-in volgorde | standaardproject (165 BPM, 4 rollen, INDUSTR) |
| **2. DESTROY** | de 7 macro's op SEL | GRIT + master-saturatie |
| **3. Groove engine** | PROB, MUTATE, CHAOS | PROB per step |
| **4. Eigen effecten** | betere tape stop, stutter met eigen buffer, echte audio-freeze | na meting van vrij geheugen |
| **5. Afwerking** | scherm, kleuren, NKJH-logo, scenes, MIDI, live-test van een uur | |

Elke stap: bouwen → alle tests groen → commit → pas dan flashen.

## Beslissingen
- 2026-11-15: VOICE en PHYS geschrapt; standaardtempo 165 BPM; DESTROY op de SEL-laag.
- Fase 1, stap 1: standaardproject = ACID 303 / RAVE STAB / NZ METAL / kit INDUSTR, 165 BPM, DUCK 40 %.
  De standaardinserts per track komen met de NKJH-presetbank (een preset zet zijn eigen INSERT).
- Fase 1, stap 2: de eerste 18 NKJH-presets, vooraan in de PRESETS-bank, elk met eigen INSERT en volume-afstemming
  (−15 LUFS; korte percussie zit op het limiterplafond, zoals SLOOP's eigen NZ SNARE).
  DIRT: HISS BASS, SCRAPE, GABBA SUB, FM GRIND · PERC: CLANG, STEAM HIT, SHRAPNEL, ANVIL ·
  NOIS: STATIC, FURNACE, 1BIT HISS · ACID: ACID BITE, ACID RIP, HOOVER X, WAREHOUSE · BRKN: GLITCH, DEAD MODEM, PIXEL WIRE.
  Achteraan elke enginetabel toegevoegd: SLOOP-presets houden hun nummer. Klankontwerp nog niet beluisterd.
- Les: CRUSH-insert + snelle toonhoogte-sweep (LOFI SWP) geeft DC-offset. Vermijden in presets.
- Fase 1, stap 3: vijf NKJH-drumkits op de plaats van vijf wereldkits (het aantal blijft 32, dus de USR- en
  SYN-kits houden hun nummer): DEMBOW → **GABBER**, AMAPIANO → **EBM**, AFRO → **WAREHSE** (warehouse),
  LATIN → **NOISE** (gecrusht), JAZZ → **RUST** (metaal). Volume gemeten; de 27 SLOOP-kits bit voor bit gelijk.
- Fase 1, stap 4 (2026-11-15): de PRESETS-lijst bevat alleen NKJH-klanken; SLOOP's 153 blijven in de firmware
  (oude projecten openen, de tests renderen ze) maar zijn niet meer te kiezen. Een engine kiezen laadt zijn eerste
  NKJH-klank. Nieuw project: ACID BITE / WAREHOUSE / STATIC. De klankbibliotheek en engines breiden we later
  doelbewust uit.
- Fase 1, stap 5: punch-ins op de NKJH-volgorde (§3B). Zwarte toetsen: KILL MID, KILL HIGH, LP-sweep, HP-sweep,
  OCT UP, FLANGER, LOOP 8T, WOBBLE, PHONE, AUTO-PAN, MONO. De effecten houden hun nummer (editor, MIDI FX CH).
- Fase 1, stap 6: VOICE, WHEEL en PHYS onbereikbaar (de engineknop slaat ze over). De code fysiek verwijderen
  (±4,4 KB werkgeheugen: PHYS is het grootste deel van de engine-arena) gebeurt aan het begin van fase 4.

## Ideeën voor later
- **Live boetseren** (gebruiker, 2026-11-15): tijdens REC een witte toets vasthouden en aan een knop draaien,
  zodat de klank van die noot live wordt opgenomen en vervormd. Bouwt voort op SLOOP's parameter locks.
  Te plannen na fase 2, samen met een basisarsenaal klanken dat daarvoor gemaakt is.
- **STEAM-engine** (gebruiker, 2026-11-15): een noisesynth die ritmisch "stoom" blaast: ruisstoten met eigen
  ritme, druk en sissende uitlaat. Kandidaat voor de eerste eigen NKJH-engine (in de plaats van een geschrapte).
