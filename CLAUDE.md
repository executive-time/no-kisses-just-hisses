# No Kisses Just Hisses (NKJH) — rules for Claude

Custom firmware for the M-VAVE FM-1: a dark, filthy, aggressive live techno instrument
(industrial, distorted electro, gabber, EBM, noise, breakcore, warehouse techno).
Every control has an immediate musical purpose. Maximum aggression, minimum menu diving:
everything important is playable by hand while a track runs.

Based on SLOOP 2.8.5 (isod89/sloop-fm1), which is based on Felucca (Hügelton Instruments).
GPL-3.0-only: keep every licence and credit; new code is GPL-3.0 too.

## Where things are
- `firmware/src/` app: engines `eng_*.c`, drums `drums.c` / `drum_synth.c`, effects `fx.c`,
  sequencer `seq.c`, UI `ui*.c`, presets `params.c`, mix `audio.c`.
- `firmware/hal/` hardware layer. `firmware/loader/` update loader. `firmware/src/recovery.c` USB rescue.
- `tests/` host tests. `web/` installer and editor. Docs: `SLOOP.md`, `GUIDE.md`, `RAM.md`, `DUALCORE.md`.
- Spec for this firmware: `docs/NKJH-SPEC.md` (once written). It decides what we build.

## How work runs
Claude edits files; the user runs commands in Terminal on the Mac (Claude's shell has no
network and no Docker). From `~/NKJH`:
- build: `cd fw && bash build.sh` (Docker + Rosetta, toolchain in `~/.jieli/toolchain`)
- test: `bash test.sh` (logs to `~/NKJH/test-log.txt`; must end `ALL HOST TESTS PASSED`)
Give the user exact commands to paste, one block at a time, and read the logs afterwards.
Claude's shell cannot delete files in this folder, so git leaves lock files behind: Claude runs git only as
`git --no-optional-locks --no-pager` with read-only commands (log, show, diff), and never commits itself.

## Hard rules
1. Never flash the FM-1 unless the build passed, all host tests passed, and the user says go.
   Before a first flash: the user has a backup plan (SLOOP USB RESCUE: hold OCT− while powering on;
   last resort: FM-1-transporter with an RP2040).
2. Do not touch `firmware/hal/`, `firmware/loader/`, `recovery.c`, `ota.c`, flash layout or
   update identity unless the task is about them and the user agreed. These decide whether the
   FM-1 can still be rescued.
3. Register access stays in `hal/` (the build checks this).
4. One small change at a time on branch `nkjh`: change → build → test → commit.
   Commit messages say what changed musically and why.
5. Sound changes: `GOLDEN_UPDATE=1` only after an *intended* change, and say which renders changed.
   CPU: `BUDGET_UPDATE=1` only when a cost increase is intended and explained; never to silence a test.
6. Memory is tight (build output at 2.8.5: RAM .data+.bss 59 KB of 96 KB, pool 334 KB of 344 KB).
   Removing unused engines or samples is how we pay for new DSP. Report sizes after each build.
7. Audio code runs in real time: no allocation, no unbounded loops, no float divides per sample
   where a table or multiply works. Watch clipping: aggression is intended, crashes and DC are not.
8. Bring upstream SLOOP fixes in only on purpose (`git fetch upstream`), never a blind merge.

## Baseline (2026-11-15)
SLOOP v2.8.5 builds and all host tests pass on macOS (Apple silicon). Local fixes vs upstream:
`tests/editor_host.c` includes `fm1_flash_id.h`; `tests/target_budget.txt` and
re-recorded on the JieLi toolchain jieli-linux-toolchains-20260730.1. `tests/cpu_baseline.txt` is SLOOP's own
(a re-recording on 2026-11-15 was an outlier: the Mac was busy). Rule: re-record the host CPU baseline only on an idle Mac,
and compare the new `cpu/mix/idle` with the old one first.
