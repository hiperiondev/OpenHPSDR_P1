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

#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "hpsdr_version.h"
#include "hpsdr_p1.h"

#include "cargs.h"

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

void ep2_cbk (int func, char* name) {
    printf("ep2_cbk: %s (%d)\n", name, func);

}

uint8_t iqtransmitter_init(void) {
    return 0;
}

uint8_t iqtransmitter_deinit(void) {
    return 0;
}

uint8_t iqtransmitter_set_freq(uint64_t freq_hz) {
    return 0;
}

void* iqtransmitter_thread(void *data) {
    return NULL;
}

uint8_t iqreceiver_init(void) {
    return 0;
}

uint8_t iqreceiver_deinit(void) {
    return 0;
}

uint8_t iqreceiver_set_freq(uint64_t freq_hz) {
    return 0;
}

void* iqreceiver_thread(void *data) {
    return NULL;
}

static struct cag_option options[] = {
        {
                .identifier = 'd',
                .access_letters = "d",
                .access_name = NULL,
                .value_name = NULL,
                .description = "Debug"
        }, {
                .identifier = 'e',
                .access_letters = "e",
                .access_name = "emulation_type",
                .value_name = "VALUE",
                .description = "Emulation Type (metis, hermes, griffin, angelia, orion, hermes, hermes_lite, orion2, c25)"
        }, {
                .identifier = 'h',
                .access_letters = "h",
                .access_name = "help",
                .description = "Shows the command help"
        }
};

static void terminate(int num) {
    fprintf(stderr, "Caught signal - Terminating 0x%x/%d(%s)\n", num, num, exit_signal[num]);
    cfg->cb.tx_deinit();
    exit(1);
}

void parse_args(int argc, char *argv[]) {
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

            case 'h':
                printf("Usage: OpenHPSDR_P1 [OPTION]...\n");
                cag_option_print(options, CAG_ARRAY_SIZE(options), stdout);
                exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {
    printf("OpenHPSDR_p1 version: %d.%d.%d\n", HPSDR_VERSION_MAJOR, HPSDR_VERSION_MINOR, HPSDR_VERSION_PATCH);

    cfg = malloc(sizeof(hpsdr_config_t));

    for (int i = 0; i < 64; i++) {
        struct sigaction sa;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = terminate;
        sigaction(i, &sa, NULL);
    }

    parse_args(argc, argv);

    if (cfg->global.debug) {
        hpsdr_dbg_setlevel(1);
    }

    // configure callbacks
        cfg->cb.tx_init = iqtransmitter_init;
      cfg->cb.tx_deinit = iqtransmitter_deinit;
    cfg->cb.tx_set_freq = iqtransmitter_set_freq;
      cfg->cb.tx_thread = iqtransmitter_thread;
        cfg->cb.rx_init = iqreceiver_init;
      cfg->cb.rx_deinit = iqreceiver_deinit;
    cfg->cb.rx_set_freq = iqreceiver_set_freq;
      cfg->cb.rx_thread = iqreceiver_thread;
            cfg->cb.ep2 = ep2_cbk;

    hpsdr_init(&cfg);

    while (1);

    hpsdr_deinit(&cfg);

    return EXIT_SUCCESS;
}
