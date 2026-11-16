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
de punch-ins (FX + toets), met de andere de macro's. **SEL + SELECT** draaien wisselt tussen pagina A (links) en B
(rechts); het tempo blijft dan staan. De toetsen in de
SEL-laag zijn vrij voor later (bijvoorbeeld macro-snapshots). Toonsoort, toonladder en akkoorden blijven bereikbaar
via **SEL tikken** (de SEL-pagina's); de oude SEL-laag (vasthouden) vervalt. FX blijft zoals in SLOOP.

| Pagina | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 |
| --- | --- | --- | --- | --- |
| **A** | **GRIT** | **BITE** | **CRUSH** | **FOLD** |
| **B** | **PITCH** | **GRAIN** | **CHAOS** | **RING** |

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
blijft ongeveer gelijk (±3 dB) tot 50 %. De knop glijdt (geen zipper-ruis). De waarden worden per project
bewaard, zoals SLOOP's masterbus (DUST, DUCK, FILT): een sectiewissel zet je knopstand live niet terug.
Per step locken komt later (het zijn globale waarden, geen trackparameters).

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

- Fase 2, stap 1: **GRIT** op SEL + KNOB 1 (de laag heet nu *destroy*). De hele mix na het DJ-filter, /4 de
  tanh in (drive x1 .. x28,6), gecompenseerd op de NKJH-startmix (tests/nkjh_grit_test.c, tools/grit_cal.txt):
  0 = bit voor bit onaangeroerd; tot 25 % warm (crest 2,84 → 2,7); 38–50 % bijt; 75–100 % bijna blokgolf
  (crest 1,2), niveau 0 dB tot 50 % en +2,5 dB op 100 %. KNOB 2–4 houden voorlopig SCALE / KEYS / TRANSP en
  de toetsen kiezen nog de toonsoort, tot BITE, CRUSH en SPACE er zijn. CHORD staat op de SEL-tikpagina.
  Opslag: g[G_LOAD] van het projectbestand (een GO-knop, nooit geladen) met merkbit 0x4000.

- Fase 2, stap 2 (2026-11-15): de andere zes macro's. Keten van de master: DUST → punch-in → DJ-filter → **BITE**
  (resonante banddoorlaat erbij, 430 Hz → 2,9 kHz) → **GRIT** → **CRUSH** (sample-and-hold tot ~2,8 kHz, tot 12 bits
  minder, afronding naar 0) → MASTER → limiter. **SPACE** stuurt de droge mix naar delay en reverb, feedback tot
  1,015 (de int16-lijn en zijn laagdoorlaat begrenzen het zingen). **REPEAT** (zones: uit < 16, 1/4, 1/8, 1/16,
  1/32) en **GRAIN** (FREEZE, half tot 50 %, vol daarboven) gaan via de punch-in machine; een FX-toets wint.
  **CHAOS**: velocity ±CHAOS/5, vanaf 40 ghost-hats en -snares op lege steps, vanaf 80 vallen niet-kick-hits weg
  (tot ~28 %); kicks, toonhoogtes en patroonlengte nooit. Nog niet: microtiming en extra ratchets.
  **STOP is stilte**: SPACE, REPEAT en GRAIN werken alleen terwijl de transport loopt (de stresstest eist het).
  Opslag: g[G_LOAD] merk | GRIT | BITE, g[G_CLRSEQ] CRUSH | SPACE, g[G_INITSND] REPEAT | GRAIN, g[G_NEWPRJ] CHAOS;
  die drie GO-plekken worden niet meer geladen. Tests: tests/nkjh_destroy_test.c (26 controles).

- Fase 2, stap 3 (2026-11-16, na het luisteren): GRIT, BITE, CRUSH, CHAOS en GRAIN goed. CRUSH herschaald (100 % =
  het oude 80 %). SPACE, REPEAT en de DJ-filterknop eruit. Nieuw: **PITCH** (B1) ±12 halve tonen, een per klik, op de
  drie synthtracks samen via de pitchbend-route (ook klinkende noten, glijdt als de bend; drums ongemoeid).
  **FOLD** (A4) sinus-wavefolder, drive x1..x12 kwadratisch, niveau gekalibreerd (tools/fold_cal.txt).
  **RING** (B4) ringmodulator: tot 50 % mengt hij in op 20..110 Hz, daarna vol, toon tot 1,5 kHz.
  Masterketen: DUST → punch-in → DJ-filter → BITE → GRIT → FOLD → CRUSH → RING → MASTER → limiter.
  Opslag: g[G_CLRSEQ] CRUSH | FOLD, g[G_INITSND] PITCH+64 | GRAIN, g[G_NEWPRJ] CHAOS | RING.

## Ideeën voor later
- **Live boetseren** (gebruiker, 2026-11-15): tijdens REC een witte toets vasthouden en aan een knop draaien,
  zodat de klank van die noot live wordt opgenomen en vervormd. Bouwt voort op SLOOP's parameter locks.
  Te plannen na fase 2, samen met een basisarsenaal klanken dat daarvoor gemaakt is.
- **STEAM-engine** (gebruiker, 2026-11-15): een noisesynth die ritmisch "stoom" blaast: ruisstoten met eigen
  ritme, druk en sissende uitlaat. Kandidaat voor de eerste eigen NKJH-engine (in de plaats van een geschrapte).
