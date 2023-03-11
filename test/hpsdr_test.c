/*
 * Copyright 2022 Emiliano Gonzalez LU3VEA (lu3vea @ gmail . com))
 * * Project Site: https://github.com/hiperiondev/OpenHPSDR_P1 *
 *
 * This is based on other projects:
 *    HPSDR simulator (https://github.com/g0orx/pihpsdr)
 *    Others: see individual files
 *
 *    please contact their authors for more information.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <complex.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "hpsdr_version.h"
#include "hpsdr_p1.h"
#include "hpsdr_hardware_fake.h"

#include "cargs.h"

hpsdr_config_t *cfg;

pthread_t iqtransmitter_thread_id;
pthread_t iqreceiver_thread_id;

char exit_signal[33][17] = {
        "NOSIGNAL",
        "SIGHUP",
        "SIGINT",
        "SIGQUIT",
        "SIGILL",
        "SIGTRAP",
        "SIGABRT",
        "SIGIOT",
        "SIGBUS",
        "SIGFPE",
        "SIGKILL",
        "SIGUSR1",
        "SIGSEGV",
        "SIGUSR2",
        "SIGPIPE",
        "SIGALRM",
        "SIGTERM",
        "SIGSTKFLT",
        "SIGCHLD",
        "SIGCONT",
        "SIGSTOP",
        "SIGTSTP",
        "SIGTTIN",
        "SIGTTOU",
        "SIGURG",
        "SIGXCPU",
        "SIGXFSZ",
        "SIGVTALRM",
        "SIGPROF",
        "SIGWINCH",
        "SIGIO",
        "SIGPWR",
        "SIGSYS/SIGUNUSED",
};

void ep2_cbk (uint32_t *ep2_value, int func, char* name) {
    printf("ep2_cbk: (%d) %s = %d\n", func, name, (int) ep2_value[func]);
}

static struct cag_option options[] = {
        {
                    .identifier = 'd',
                .access_letters = "d",
                   .access_name = NULL,
                    .value_name = NULL,
                   .description = "Debug"
        }, {
                    .identifier = 'n',
                .access_letters = "n",
                   .access_name = NULL,
                    .value_name = NULL,
                   .description = "Receiver noise"
        }, {
                    .identifier = 'e',
                .access_letters = "e",
                   .access_name = "emulation_type",
                    .value_name = "VALUE",
                   .description = "Emulation Type (metis, hermes, griffin, angelia, orion, hermes, hermes_lite, orion2, c25)"
        }, {
                    .identifier = 'i',
                .access_letters = "i",
                   .access_name = "ifilename",
                    .value_name = "VALUE",
                   .description = "Read I/Q samples from file and return as receiver"
        }, {
                    .identifier = 'o',
                .access_letters = "o",
                   .access_name = "ofilename",
                    .value_name = "VALUE",
                   .description = "Write I/Q samples to file"
        }, {
                    .identifier = 'h',
                .access_letters = "h",
                   .access_name = "help",
                   .description = "Shows the command help"
        }
};

static void terminate(int num) {
    fprintf(stderr, "Caught signal - Terminating 0x%x/%d(%s)\n", num, num, exit_signal[num]);
    exit(1);
}

void parse_args(hpsdr_config_t *cfg, int argc, char *argv[]) {
    char identifier;
    const char *value;
    cag_option_context context;

    cag_option_prepare(&context, options, CAG_ARRAY_SIZE(options), argc, argv);
    while (cag_option_fetch(&context)) {
        identifier = cag_option_get(&context);
        switch (identifier) {

            case 'd':
                cfg->global.debug = true;
                break;

            case 'n':
                cfg->global.replay = true;
                break;

            case 'e':
                value = cag_option_get_value(&context);
                if (!strcmp(value, "metis")) {
                    cfg->global.emulation = DEVICE_METIS;
                    break;
                } else if (!strcmp(value, "hermes")) {
                    cfg->global.emulation = DEVICE_HERMES;
                    break;
                } else if (!strcmp(value, "griffin")) {
                    cfg->global.emulation = DEVICE_GRIFFIN;
                    break;
                } else if (!strcmp(value, "angelia")) {
                    cfg->global.emulation = DEVICE_ANGELIA;
                    break;
                } else if (!strcmp(value, "orion")) {
                    cfg->global.emulation = DEVICE_ORION;
                    break;
                } else if (!strcmp(value, "hermes_lite")) {
                    cfg->global.emulation = DEVICE_HERMES_LITE;
                    break;
                } else if (!strcmp(value, "hermes_lite2")) {
                    cfg->global.emulation = DEVICE_HERMES_LITE2;
                    break;
                } else if (!strcmp(value, "orion2")) {
                    cfg->global.emulation = DEVICE_ORION2;
                    break;
                } else if (!strcmp(value, "c25")) {
                    cfg->global.emulation = DEVICE_C25;
                    break;
                } else {
                    printf("ERROR: Unknown emulation type\n");
                    exit(EXIT_SUCCESS);
                }
                break;

            case 'i':
                ifile = true;
                value = cag_option_get_value(&context);
                strcpy(ifilename, value);
                printf("< input file iq: %s >\n", ifilename);
                break;

            case 'o':
                ofile = true;
                value = cag_option_get_value(&context);
                strcpy(ofilename, value);
                printf("< output file iq: %s >\n", ofilename);
                break;

            case 'h':
                printf("Usage: OpenHPSDR_P1 [OPTION]...\n");
                cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
                exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {
    printf("OpenHPSDR_p1 version: %d.%d.%d\n", HPSDR_VERSION_MAJOR, HPSDR_VERSION_MINOR, HPSDR_VERSION_PATCH);
    uint8_t res = 0;
    cfg = malloc(sizeof(hpsdr_config_t));
    hpsdr_clear_config(&cfg);

    for (int i = 0; i < 64; i++) {
        struct sigaction sa;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = terminate;
        sigaction(i, &sa, NULL);
    }

    parse_args(cfg, argc, argv);

    if (cfg->global.debug) {
        hpsdr_dbg_setlevel(1);
    }

    cfg->ep2_cb = ep2_cbk;

    hpsdr_init(&cfg);
    hpsdr_start(&cfg);

    res = iqtransmitter_init();
    if (res == -1)
        printf("WARNING: tx_init failed\n");
    res = iqreceiver_init();
    if (res == -1)
        printf("WARNING: rx_init failed\n");

    pthread_create(&iqtransmitter_thread_id, NULL, &iqtransmitter_thread, (void*) cfg);
    //pthread_detach(iqtransmitter_thread_id);

    pthread_create(&iqreceiver_thread_id, NULL, &iqreceiver_thread, (void*) cfg);
    //pthread_detach(iqreceiver_thread_id);

    while (1)
        usleep(1000); // this time... do nothing

    pthread_cancel(iqreceiver_thread_id);
    pthread_cancel(iqtransmitter_thread_id);
    hpsdr_stop();
    hpsdr_deinit(&cfg);

    return EXIT_SUCCESS;
}
