/* SPDX-License-Identifier: GPL-3.0-only */
/* NKJH: the DESTROY macros after GRIT (tests/nkjh_grit_test.c): BITE, CRUSH, SPACE (page A), REPEAT, GRAIN, CHAOS
 * (page B), on a bar of the NKJH default sounds through the master (docs/NKJH-SPEC.md 4):
 *   BITE, CRUSH   0: the mix untouched; up to 50 % about as loud (+-3 dB); every value bounded, no DC
 *   SPACE         0: the delay as SLOOP's; up: a longer tail after the notes stop, bounded even singing; STOP: dies
 *   REPEAT, GRAIN, SPACE act while the transport plays (STOP is silence, as in SLOOP)
 *   REPEAT        holds the loop of its zone (1/4 .. 1/32); a key (FX + key) wins
 *   GRAIN         holds FREEZE (the reverb held)
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
    song.g[G_GRIT] = song.g[G_BITE] = song.g[G_CRUSH] = song.g[G_SPACE] = 0;
    song.g[G_REPEAT] = song.g[G_GRAIN] = song.g[G_CHAOS] = 0;
    grit.g = bite.b = crsh.c = space_s = 0;
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
    bite.b = song.g[G_BITE], crsh.c = song.g[G_CRUSH], space_s = song.g[G_SPACE];   /* (at once) */
    song.playing = 1;                                  /* (SPACE, REPEAT, GRAIN: while the transport plays) */
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
        bite_process(bl, br, CTL); crush_process(bl, br, CTL);
        for (i = 0; i < CTL; i++) same &= bl[i] == kl[i] && br[i] == kr[i];
        dcheck(same && bite.z[0] == 0, "BITE 0, CRUSH 0: the mix bit for bit untouched (the stages skipped)");
    }
    level_sweep(G_BITE, "BITE");
    level_sweep(G_CRUSH, "CRUSH");
    {   /* SPACE: the tail after the notes stop (1 s on), and it stays bounded singing at the top */
        double t0, t64, t127, pk;
        render(0, 0, NB / 2u); t0 = rms_of(n * 5u / 8u, n, &pk, &dc);
        render(G_SPACE, 64, NB / 2u); t64 = rms_of(n * 5u / 8u, n, &pk, &dc);
        render(G_SPACE, 127, NB / 2u); t127 = rms_of(n * 5u / 8u, n, &pk, &dc);
        printf("destroy: SPACE tail rms: 0 %.1f, 64 %.1f, 127 %.1f (peak %.0f, dc %.1f)\n", t0, t64, t127, pk, dc);
        dcheck(t64 > t0 * 2 && t127 > t64, "SPACE: a longer tail as it turns (the delay and reverb fed)");
        dcheck(pk <= 32767 && fabs(dc) < 400, "SPACE at 127 (feedback past 1): bounded, no DC");
        macros_zero(); space_in(acc[0].sd, acc[0].sr, acc[0].l, acc[0].r, CTL);
        dcheck(nkjh_sp_fb == 0 && nkjh_sp_mix == 0, "SPACE 0: the delay's own feedback and return (none added)");
        song.g[G_SPACE] = 127; space_s = 127; song.playing = 0;
        for (i = 0; i < 80u; i++) space_in(acc[0].sd, acc[0].sr, acc[0].l, acc[0].r, CTL);
        dcheck(space_s == 0 && nkjh_sp_fb == 0, "SPACE 127, STOP: no more fed, the delay back to its own feedback");
        song.playing = 1; macros_zero();
    }
    {   /* REPEAT and GRAIN through the punch-in machine */
        static int32_t l[CTL], r[CTL];
        int z_ok = 1;
        static const struct { int32_t v, fx; } Z[] = {{10, -1}, {20, PX_LOOP4}, {60, PX_LOOP8}, {90, PX_LOOP16}, {120, PX_LOOP32}};
        for (i = 0; i < 5u; i++) {
            int32_t w = -1, d = 0;
            macros_zero(); song.g[G_REPEAT] = (int16_t)Z[i].v;
            nkjh_macro_fx(&w, &d);
            z_ok &= w == Z[i].fx;
        }
        dcheck(z_ok, "REPEAT: off below 16, then loop 1/4, 1/8, 1/16, 1/32 as it turns");
        render(0, 0, 0);                              /* (a quiet mix in the ring) */
        macros_zero(); punch.req = -1; punch.cur = -1; song.g[G_REPEAT] = 90;
        for (i = 0; i < 8u; i++) punch_process(l, r, CTL);
        dcheck(punch.cur == PX_LOOP16, "REPEAT 90: the punch-in machine holds LOOP 16");
        punch.req = PX_REV;
        for (i = 0; i < 200u; i++) punch_process(l, r, CTL);
        dcheck(punch.cur == PX_REV, "REPEAT on, FX + a key: the key's effect wins");
        punch.req = -1; macros_zero();
        for (i = 0; i < 400u; i++) punch_process(l, r, CTL);
        dcheck(punch.cur < 0, "REPEAT back to 0, no key: the punch-in machine idle again");
        song.g[G_GRAIN] = 127;
        for (i = 0; i < 8u; i++) { punch_freeze_feed(acc[0].sr, l, r, CTL); punch_process(l, r, CTL); }
        dcheck(punch.cur == PX_FREEZE && rev_frz, "GRAIN 127: FREEZE held (the reverb holds)");
        { int32_t w = -1, d = 0; song.playing = 0; nkjh_macro_fx(&w, &d); song.playing = 1;
          dcheck(w == -1, "REPEAT, GRAIN: nothing while stopped (STOP is silence)"); }
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
        song.g[G_GRIT] = 11, song.g[G_BITE] = 22, song.g[G_CRUSH] = 33, song.g[G_SPACE] = 44;
        song.g[G_REPEAT] = 55, song.g[G_GRAIN] = 66, song.g[G_CHAOS] = 77;
        proj_capture(&pr);
        macros_zero();
        proj_apply(&pr, 1);
        dcheck(song.g[G_GRIT] == 11 && song.g[G_BITE] == 22 && song.g[G_CRUSH] == 33 && song.g[G_SPACE] == 44 &&
               song.g[G_REPEAT] == 55 && song.g[G_GRAIN] == 66 && song.g[G_CHAOS] == 77, "a project keeps all seven macros");
        dcheck(song.g[G_CLRSEQ] == GP[G_CLRSEQ].def && song.g[G_NEWPRJ] == GP[G_NEWPRJ].def,
               "... their slots never load as the GO buttons they were (CLEAR, NEW)");
        pr.g[G_LOAD] = 0;
        proj_apply(&pr, 1);
        dcheck(!song.g[G_GRIT] && !song.g[G_BITE] && !song.g[G_CRUSH] && !song.g[G_SPACE] && !song.g[G_REPEAT] &&
               !song.g[G_GRAIN] && !song.g[G_CHAOS], "a SLOOP project (no mark): all seven at 0");
        macros_zero();
    }
    printf("destroy: %d failed\n", dfails);
    return dfails;
}
