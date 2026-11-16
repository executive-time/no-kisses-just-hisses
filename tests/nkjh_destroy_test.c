/* SPDX-License-Identifier: GPL-3.0-only */
/* NKJH: the DESTROY macros after GRIT (tests/nkjh_grit_test.c): BITE, CRUSH, FOLD (page A), PITCH, GRAIN, CHAOS,
 * RING (page B), on a bar of the NKJH default sounds through the master (docs/NKJH-SPEC.md 4):
 *   BITE, CRUSH, FOLD, RING   0: the mix untouched; up to 50 % about as loud (+-3 dB); every value bounded, no DC
 *   PITCH         the synth parts a semitone a step (+12: an octave up), held notes too; the drums untouched
 *   GRAIN         holds FREEZE (the reverb held), only while the transport plays (STOP is silence); a key wins
 *   CHAOS         0: no random draw (the sequencer as SLOOP's); kicks never dropped; other hits drop out and ghosts
 *                 come in at the top, in sensible amounts
 *   the page: SEL held + SELECT; a project keeps all seven, a SLOOP project opens with all at 0
 * Exit status: failed checks. */
#include "editor_host.c"                             /* hostsim, the UI, the projects */
static int dfails;
static void dcheck(int ok, const char *what) { printf("destroy: %-74s %s\n", what, ok ? "ok" : "FAIL"); dfails += !ok; }
#define NB 2800u
static int32_t out[NB * CTL * 2];
static void macros_zero(void)
{
    song.g[G_GRIT] = song.g[G_BITE] = song.g[G_CRUSH] = song.g[G_FOLD] = 0;
    song.g[G_PITCH] = song.g[G_GRAIN] = song.g[G_CHAOS] = song.g[G_RING] = 0;
    grit.g = bite.b = crsh.c = fold.f = ring.r = 0, nkjh_pitch_q = 0;
}
/* a bar of the default sounds; notes stop at stop_at (blocks), the rest is the tail */
static void render(uint32_t gid, int32_t v, uint32_t stop_at)
{
    static const uint8_t BASS[8] = {36, 36, 48, 36, 39, 36, 43, 36};
    uint32_t b;
    host_tracks_init();
    host_preset(&trk[0], 0, 25);
    host_preset(&trk[1], 6, 15);
    host_preset(&trk[2], ENGI_NOISE, 13);
    TDRUM->p[P_E0] = DRUM_DEFAULT_KIT;
    macros_zero();
    if (gid)
        song.g[gid] = (int16_t)v;
    bite.b = song.g[G_BITE], crsh.c = song.g[G_CRUSH], fold.f = song.g[G_FOLD], ring.r = song.g[G_RING];   /* (at once) */
    song.playing = 1;                                  /* (GRAIN: while the transport plays) */
    memset(dly_buf, 0, sizeof dly_buf);
    for (b = 0; b < NB; b++) {
        uint32_t t = b * CTL, step = FS / 8u;
        if (b < stop_at && t % step < CTL) {
            uint32_t k = t / step;
            if (k % 2u == 0u) drum_on(36, 110);
            if (k % 4u == 2u) drum_on(38, 100);
            drum_on(42, 70);
            trk_all_off(&trk[0]);
            trk_note_on(&trk[0], BASS[k % 8u], 100);
            if (k % 8u == 0u) { trk_all_off(&trk[1]); trk_note_on(&trk[1], 60, 90); trk_note_on(&trk[1], 63, 90); }
        }
        if (b == stop_at) { trk_all_off(&trk[0]); trk_all_off(&trk[1]); trk_all_off(&trk[2]); }
        mix_block(&out[b * CTL * 2u], CTL);
    }
}
/* every voice let go and rung out (host_tracks_init keeps what sounds) */
static void silence(void)
{
    uint32_t k, b;
    macros_zero();
    for (k = 0; k < NTRK; k++) trk_all_off(&trk[k]);
    for (b = 0; b < 3000u; b++) mix_block(out, CTL);
}
static double rms_of(uint32_t from, uint32_t to, double *peak, double *dc)
{
    double s2 = 0, s = 0;
    uint32_t i;
    *peak = 0;
    for (i = from; i < to; i++) {
        double x = out[i];
        s2 += x * x, s += x;
        if (fabs(x) > *peak) *peak = fabs(x);
    }
    *dc = s / (to - from);
    return sqrt(s2 / (to - from));
}
static void level_sweep(uint32_t gid, const char *name)
{
    static const int32_t V[] = {16, 32, 48, 64, 96, 127};
    double r0, p, dc;
    uint32_t i, n = NB * CTL * 2u;
    int ok_lvl = 1, ok_b = 1;
    char b[96];
    render(0, 0, NB); r0 = rms_of(n / 4u, n, &p, &dc);
    for (i = 0; i < sizeof V / sizeof V[0]; i++) {
        double r, db;
        render(gid, V[i], NB); r = rms_of(n / 4u, n, &p, &dc);
        db = 20 * log10(r / r0);
        printf("destroy: %-5s %3d: rms %7.1f peak %6.0f dc %6.1f  %+5.1f dB\n", name, V[i], r, p, dc, db);
        if (V[i] <= 64 && fabs(db) > 3.0) ok_lvl = 0;
        if (p > 32767 || fabs(dc) > 200) ok_b = 0;
    }
    snprintf(b, sizeof b, "%s up to 50 %%: within +-3 dB of 0", name); dcheck(ok_lvl, b);
    snprintf(b, sizeof b, "%s every value: bounded (16 bits), no DC (|mean| < 200)", name); dcheck(ok_b, b);
}
int main(void)
{
    uint32_t i, n = NB * CTL * 2u;
    double p, dc;
    {   /* 0: the stages skipped, the buffer untouched */
        static int32_t bl[CTL], br[CTL], kl[CTL], kr[CTL];
        int same = 1;
        for (i = 0; i < CTL; i++) bl[i] = kl[i] = (int32_t)(i * 2731u % 60000u) - 30000, br[i] = kr[i] = -bl[i] / 3;
        macros_zero(); bite.z[0] = 77; crsh.hl = 5;
        bite_process(bl, br, CTL); crush_process(bl, br, CTL); fold_process(bl, br, CTL); ring_process(bl, br, CTL);
        for (i = 0; i < CTL; i++) same &= bl[i] == kl[i] && br[i] == kr[i];
        dcheck(same && bite.z[0] == 0, "BITE, CRUSH, FOLD, RING at 0: the mix bit for bit untouched (the stages skipped)");
    }
    level_sweep(G_BITE, "BITE");
    level_sweep(G_CRUSH, "CRUSH");
    if (getenv("FOLD_CAL")) {   /* tools/gen_grit.py --fold: the level at every 8th FOLD, dB from 0 */
        FILE *f = fopen(getenv("FOLD_CAL"), "w");
        double r0, r;
        uint32_t g;
        render(0, 0, NB); r0 = rms_of(n / 4u, n, &p, &dc);
        for (g = 8; g < 128u; g += g == 120u ? 7u : 8u) {
            render(G_FOLD, (int32_t)g, NB); r = rms_of(n / 4u, n, &p, &dc);
            fprintf(f, "%u %.3f\n", g, 20 * log10(r / r0));
        }
        fprintf(f, "0 0\n");
        fclose(f);
    }
    level_sweep(G_FOLD, "FOLD");
    level_sweep(G_RING, "RING");
    {   /* PITCH: a sine bass held on track 1, its zero crossings; +12 doubles them, -12 halves them; held notes follow;
         * the drums are not the synth parts */
        static const int32_t P[3] = {0, 12, -12};
        static float w[16384];
        double zc[3];
        uint32_t k, b;
        for (k = 0; k < 3u; k++) {
            int32_t prev = 0;
            uint32_t z = 0;
            silence();
            host_tracks_init(); macros_zero(); song.playing = 1;
            host_preset(&trk[0], 0, 2);                  /* SUB BASS: a sine */
            trk[0].p[P_DIST] = trk[0].p[P_DLY] = trk[0].p[P_REV] = trk[0].p[P_CHOR] = 0;
            trk[0].p[P_ED_PIT] = 0;                     /* (no pitch envelope: one steady tone) */
            trk_note_on(&trk[0], 72, 100);              /* held from before the turn (C3 after its TRN -24) */
            for (b = 0; b < 40u; b++) mix_block(out, CTL);
            song.g[G_PITCH] = (int16_t)P[k];
            for (b = 0; b < 60u; b++) mix_block(out, CTL);   /* (the glide) */
            for (b = 0; b < 512u; b++) {                /* 16384 samples, then the period: autocorrelation, 30 .. 1500 Hz */
                uint32_t j;
                mix_block(out, CTL);
                for (j = 0; j < CTL; j++) w[b * CTL + j] = (float)out[2u * j];
            }
            {
                static double cr[1470];
                uint32_t lag, best = 0;
                double bv = -1e30;
                for (lag = 29; lag < 1470u; lag++) {
                    double c = 0;
                    uint32_t j;
                    for (j = 0; j < 8192u; j++) c += (double)w[j] * w[j + lag];
                    cr[lag] = c;
                    if (c > bv) bv = c;
                }
                for (lag = 30; lag < 1469u && !best; lag++)   /* the first peak near the top: the period, not a multiple */
                    if (cr[lag] > 0.85 * bv && cr[lag] >= cr[lag - 1] && cr[lag] >= cr[lag + 1])
                        best = lag;
                zc[k] = 44100.0 / best;                   /* (the fundamental, Hz) */
                if (getenv("PITCH_DEBUG")) {
                    double e = 0; uint32_t j; for (j = 0; j < 8192u; j++) e += (double)w[j] * w[j];
                    printf("  pitch %d: rms %.0f best lag %u, corr %.3g max %.3g, corr at 84 %.3g\n", P[k], sqrt(e / 8192), best, cr[best], bv, cr[84]);
                }
            }
            (void)prev; (void)z;
        }
        printf("destroy: PITCH held note: 0 %.1f Hz, +12 %.1f Hz, -12 %.1f Hz\n", zc[0], zc[1], zc[2]);
        dcheck(fabs(zc[1] / zc[0] - 2.0) < 0.06 && fabs(zc[2] / zc[0] - 0.5) < 0.03,
               "PITCH +12 / -12: a held note an octave up / down (the synth parts)");
        {   /* the drums: the 808 kick's crossings over 0.3 s with PITCH 0 and +12, about the same (an octave would double
             * them; its click is noise, so not bit for bit) */
            uint32_t zk[2];
            for (k = 0; k < 2u; k++) {
                int32_t prev = 0;
                silence();
                host_tracks_init(); macros_zero(); song.g[G_PITCH] = (int16_t)(k ? 12 : 0);
                nkjh_pitch_q = k ? 12 * 4096 : 0;
                TDRUM->p[P_E0] = DRUM_SAMPLED;           /* (808: a sine kick) */
                drum_on(36, 110);
                zk[k] = 0;
                for (b = 0; b < 400u; b++) {
                    uint32_t j;
                    mix_block(out, CTL);
                    for (j = 0; j < CTL; j++) { int32_t x = out[2u * j]; zk[k] += (x >= 0) != (prev >= 0); prev = x; }
                }
            }
            printf("destroy: PITCH and the 808 kick: crossings 0 %u, +12 %u\n", zk[0], zk[1]);
            dcheck(zk[1] < zk[0] * 13u / 10u && zk[1] * 13u > zk[0] * 10u, "PITCH +12: the drums untouched (only the synth parts move)");
        }
        macros_zero();
    }
    {   /* GRAIN through the punch-in machine */
        static int32_t l[CTL], r[CTL];
        render(0, 0, 0);                              /* (a quiet mix in the ring) */
        macros_zero(); punch.req = -1; punch.cur = -1; song.playing = 1;
        song.g[G_GRAIN] = 127;
        for (i = 0; i < 8u; i++) { punch_freeze_feed(acc[0].sr, l, r, CTL); punch_process(l, r, CTL); }
        dcheck(punch.cur == PX_FREEZE && rev_frz, "GRAIN 127: FREEZE held (the reverb holds)");
        punch.req = PX_REV;
        for (i = 0; i < 200u; i++) { punch_freeze_feed(acc[0].sr, l, r, CTL); punch_process(l, r, CTL); }
        dcheck(punch.cur == PX_REV, "GRAIN on, FX + a key: the key's effect wins");
        punch.req = -1;
        { int32_t w = -1, d = 0; song.playing = 0; nkjh_macro_fx(&w, &d); song.playing = 1;
          dcheck(w == -1, "GRAIN: nothing while stopped (STOP is silence)"); }
        { int32_t w = -1, d = 0; song.g[G_GRAIN] = 32; nkjh_macro_fx(&w, &d);
          dcheck(w == PX_FREEZE && d > 0 && d < 32767, "GRAIN 32: FREEZE mixed in part way"); }
        macros_zero();
        for (i = 0; i < 400u; i++) { punch_freeze_feed(acc[0].sr, l, r, CTL); punch_process(l, r, CTL); }
    }
    {   /* CHAOS: on the drum track's step (vis_note: the last note a track started) */
        dstep_t s;
        uint32_t st, k, kick = 0, clap = 0, ghost = 0, N = 2000u;
        int32_t v, vmin = 127, vmax = 0;
        host_tracks_init(); macros_zero();
        memset(&s, 0, sizeof s); dstep_set(&s, 0, 2, 0);   /* the kick alone */
        st = chaos_st;
        for (k = 0; k < N; k++) { vis_note[3] = 0; drum_step(TDRUM, &s, 0); kick += vis_note[3] == LANE_NOTE[0]; }
        dcheck(chaos_st == st && kick == N, "CHAOS 0: every hit plays, no random draw (the sequencer as SLOOP's)");
        song.g[G_CHAOS] = 127; kick = 0;
        for (k = 0; k < N; k++) { vis_note[3] = 0; drum_step(TDRUM, &s, 0); kick += vis_note[3] == LANE_NOTE[0]; }
        dcheck(kick == N, "CHAOS 127: the kick never dropped (it plays last, after any ghost)");
        memset(&s, 0, sizeof s); dstep_set(&s, 3, 2, 0);    /* the clap alone */
        for (k = 0; k < N; k++) { vis_note[3] = 0; drum_step(TDRUM, &s, 0); clap += vis_note[3] == LANE_NOTE[3]; }
        memset(&s, 0, sizeof s);                            /* an empty step */
        for (k = 0; k < N; k++) { vis_hit = 0; drum_step(TDRUM, &s, 0); ghost += (vis_hit >> 3) & 1u; }
        printf("destroy: CHAOS 127: clap played %u / %u, ghosts on an empty step %u / %u\n", clap, N, ghost, N);
        dcheck(clap > N * 6u / 10u && clap < N * 85u / 100u, "CHAOS 127: other hits drop out now and then (60 .. 85 % play)");
        dcheck(ghost > N * 3u / 10u && ghost < N * 7u / 10u, "CHAOS 127: ghosts on empty steps (30 .. 70 %)");
        for (k = 0; k < N; k++) { v = (int32_t)chaos_vel(100, 127); if (v < vmin) vmin = v; if (v > vmax) vmax = v; }
        dcheck(vmin >= 75 && vmax <= 125 && vmax - vmin >= 30, "CHAOS 127: the velocities move, +-25 around the step's");
        song.g[G_CHAOS] = 0;
    }
    {   /* the page and the knobs; a project */
        static project_t pr;
        song.g[G_GRIT] = 11, song.g[G_BITE] = 22, song.g[G_CRUSH] = 33, song.g[G_FOLD] = 44;
        song.g[G_PITCH] = -7, song.g[G_GRAIN] = 66, song.g[G_CHAOS] = 77, song.g[G_RING] = 88;
        proj_capture(&pr);
        macros_zero();
        proj_apply(&pr, 1);
        dcheck(song.g[G_GRIT] == 11 && song.g[G_BITE] == 22 && song.g[G_CRUSH] == 33 && song.g[G_FOLD] == 44 &&
               song.g[G_PITCH] == -7 && song.g[G_GRAIN] == 66 && song.g[G_CHAOS] == 77 && song.g[G_RING] == 88,
               "a project keeps all eight (PITCH -7 too)");
        dcheck(song.g[G_CLRSEQ] == GP[G_CLRSEQ].def && song.g[G_NEWPRJ] == GP[G_NEWPRJ].def,
               "... their slots never load as the GO buttons they were (CLEAR, NEW)");
        pr.g[G_LOAD] = 0;
        proj_apply(&pr, 1);
        dcheck(!song.g[G_GRIT] && !song.g[G_BITE] && !song.g[G_CRUSH] && !song.g[G_FOLD] && !song.g[G_PITCH] &&
               !song.g[G_GRAIN] && !song.g[G_CHAOS] && !song.g[G_RING], "a SLOOP project (no mark): all eight at 0");
        macros_zero();
    }
    printf("destroy: %d failed\n", dfails);
    return dfails;
}
