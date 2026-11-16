/* SPDX-License-Identifier: GPL-3.0-only */
/* NKJH: GRIT, the DESTROY layer's KNOB 1 (fx.c grit_process, docs/NKJH-SPEC.md 4). A bar of the NKJH default sounds
 * (ACID BITE, WAREHOUSE chords, STATIC) with the INDUSTR kit, rendered through the master at GRIT 0, 16 .. 127:
 *   0     the output bit for bit as without GRIT (the stage is skipped)
 *   <=64  about as loud as at 0 (RMS within +-3 dB): a small move is usable
 *   all   bounded (16 bits), no DC, and more GRIT = more squashed (the crest factor falls)
 *   a project keeps GRIT (G_LOAD's slot, NKJH_MARK); a SLOOP project (no mark) opens at 0
 * Exit status: failed checks. */
#include "editor_host.c"                             /* hostsim, the UI, the projects */
static int gfails;
static void gcheck(int ok, const char *what) { printf("grit: %-70s %s\n", what, ok ? "ok" : "FAIL"); gfails += !ok; }
#define NB 2800u                                       /* blocks of CTL: ~2 s */
static int32_t out[NB * CTL * 2];
static void render(int32_t g)
{
    static const uint8_t BASS[8] = {36, 36, 48, 36, 39, 36, 43, 36};
    uint32_t b, i;
    host_tracks_init();
    host_preset(&trk[0], 0, 25);                       /* ACID BITE */
    host_preset(&trk[1], 6, 15);                       /* WAREHOUSE */
    host_preset(&trk[2], ENGI_NOISE, 13);              /* STATIC */
    TDRUM->p[P_E0] = DRUM_DEFAULT_KIT;
    song.g[G_GRIT] = (int16_t)g;
    memset(&grit, 0, sizeof grit);
    grit.g = g;                                        /* (at once: the glide is not what is measured) */
    for (b = 0; b < NB; b++) {
        uint32_t t = b * CTL, step = FS / 8u;           /* 1/8 notes at ~165 BPM-ish */
        if (t % step < CTL) {
            uint32_t k = t / step;
            if (k % 2u == 0u) drum_on(36, 110);
            if (k % 4u == 2u) drum_on(38, 100);
            drum_on(42, 70);
            trk_all_off(&trk[0]);
            trk_note_on(&trk[0], BASS[k % 8u], 100);
            if (k % 8u == 0u) { trk_all_off(&trk[1]); trk_note_on(&trk[1], 60, 90); trk_note_on(&trk[1], 63, 90); }
            if (k % 16u == 4u) { trk_all_off(&trk[2]); trk_note_on(&trk[2], 72, 80); }
        }
        mix_block(&out[b * CTL * 2u], CTL);
    }
}
static double rms, peak, dc;
static void stats(void)
{
    uint32_t i, n = NB * CTL * 2u, from = n / 4u;      /* (past the first notes' attack) */
    double s2 = 0, s = 0;
    peak = 0;
    for (i = from; i < n; i++) {
        double x = out[i];
        s2 += x * x, s += x;
        if (fabs(x) > peak) peak = fabs(x);
    }
    rms = sqrt(s2 / (n - from)), dc = s / (n - from);
}
int main(void)
{
    static int32_t ref[NB * CTL * 2];
    static const int32_t G[] = {1, 4, 8, 16, 32, 48, 64, 96, 127};
    double r0, c0, crest_prev;
    uint32_t i, same;
    int ok_level = 1, ok_bound = 1, ok_dc = 1, ok_crest = 1, ok_warm = 1;
    song.g[G_GRIT] = 0;
    render(0); memcpy(ref, out, sizeof out);
    stats(); r0 = rms; c0 = peak / rms;
    printf("grit:   0: rms %7.1f  peak %7.0f  crest %.2f\n", rms, peak, c0);
    {   /* GRIT 0: the stage skipped, the buffer untouched (and its filter state cleared) */
        static int32_t bl[CTL], br[CTL], kl[CTL], kr[CTL];
        for (i = 0; i < CTL; i++) bl[i] = kl[i] = ref[2u * i + 4000u] * 3, br[i] = kr[i] = -ref[2u * i + 4001u] * 3;
        grit.g = 0, grit.sl = 1234, song.g[G_GRIT] = 0;
        grit_process(bl, br, CTL);
        for (same = 1, i = 0; i < CTL; i++) same &= bl[i] == kl[i] && br[i] == kr[i];
        gcheck(same && grit.g == 0 && grit.sl == 0, "GRIT 0: the mix bit for bit untouched (the stage skipped)");
    }
    if (getenv("GRIT_CAL")) {   /* tools/gen_grit.py: the level of this mix at every 8th GRIT, dB from GRIT 0 */
        FILE *f = fopen(getenv("GRIT_CAL"), "w");
        uint32_t g;
        for (g = 0; g < 128u; g += g == 120u ? 7u : 8u) {
            render((int32_t)g); stats();
            fprintf(f, "%u %.3f\n", g, 20 * log10(rms / r0));
        }
        fclose(f);
    }
    crest_prev = c0;
    for (i = 0; i < sizeof G / sizeof G[0]; i++) {
        double db, crest;
        render(G[i]); stats();
        db = 20 * log10(rms / r0), crest = peak / rms;
        printf("grit: %3d: rms %7.1f  peak %7.0f  crest %.2f  %+5.1f dB\n", G[i], rms, peak, crest, db);
        if (G[i] <= 64 && fabs(db) > 3.0) ok_level = 0;
        if (peak > 32767) ok_bound = 0;
        if (fabs(dc) > 200) ok_dc = 0;
        if (G[i] > 32 && crest > crest_prev + 0.05) ok_crest = 0;   /* (where it bites: 48 .. 127) */
        if (G[i] == 8 && crest < c0 * 0.85) ok_warm = 0;   /* (a warm start: the peaks barely touched at 6 %) */
        crest_prev = crest;
    }
    gcheck(ok_level, "GRIT up to 64 (50 %): RMS within +-3 dB of GRIT 0");
    gcheck(ok_bound && ok_dc, "every GRIT: bounded (16 bits) and no DC (|mean| < 200)");
    gcheck(ok_crest, "more GRIT, more squashed: the crest factor falls from 32 to 127");
    gcheck(ok_warm, "a warm start: at GRIT 8 (6 %) the crest factor within 15 % of GRIT 0");
    {   /* a project keeps it; a SLOOP project (G_LOAD 0) opens with GRIT 0 */
        static project_t p;
        song.g[G_GRIT] = 77;
        proj_capture(&p);
        song.g[G_GRIT] = 0;
        proj_apply(&p, 1);
        i = (uint32_t)song.g[G_GRIT];
        p.g[G_LOAD] = 0;
        song.g[G_GRIT] = 50;
        proj_apply(&p, 1);
        gcheck(i == 77u && song.g[G_GRIT] == 0, "a project keeps GRIT; a SLOOP project opens at 0");
        song.g[G_GRIT] = 50;
        p.g[G_LOAD] = (int16_t)(NKJH_MARK | 90);
        proj_apply(&p, 0);
        gcheck(song.g[G_GRIT] == 50, "a song section leaves GRIT as it is (as the master bus)");
    }
    printf("grit: %d failed\n", gfails);
    return gfails;
}
