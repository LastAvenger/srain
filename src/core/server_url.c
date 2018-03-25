/* Copyright (C) 2016-2017 Shengyu Zhang <i@silverrainz.me>
 *
 * This file is part of Srain.
 *
 * Srain is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file server_url.c
 * @brief IRC URL parse and perfrom
 * @author Shengyu Zhang <i@silverrainz.me>
 * @version 0.06.2
 * @date 2017-08-06
 */

#include <glib.h>
#include <libsoup/soup.h>

#include "core/core.h"
#include "config/reader.h"
#include "ret.h"
#include "i18n.h"
#include "log.h"
#include "utils.h"

static void join_comma_separated_chans(SrnServer *srv, const char *comma_chans);

SrnRet srn_server_url_open(const char *url){
    const char *scheme;
    const char *user;
    const char *passwd;
    const char *host;
    int port;
    const char *path;
    const char *fragment;
    bool new_cfg = FALSE;
    SoupURI *suri = NULL;
    SrnRet ret = SRN_ERR;
    SrnServerConfig *cfg = NULL;
    SrnServer *srv = NULL;

    suri = soup_uri_new(url);
    if (!suri){
        ret = RET_ERR(_("Failed to parse \"%1$s\" as a URL"), url);
        goto fin;
    }

    scheme = soup_uri_get_scheme(suri);
    if (g_ascii_strcasecmp(scheme, "irc") != 0
            && g_ascii_strcasecmp(scheme, "ircs") != 0){
        ret = RET_ERR(_("Unsupported protocol: %1$s"), scheme);
        goto fin;
    }

    user = soup_uri_get_user(suri);
    passwd = soup_uri_get_password(suri);
    host = soup_uri_get_host(suri);
    port = soup_uri_get_port(suri);
    path = soup_uri_get_path(suri);
    fragment = soup_uri_get_fragment(suri);

    if (str_is_empty(host)){
        ret = RET_ERR(_("Host is empty in URL \"%1$s\""), url);
        goto fin;
    }

    return SRN_ERR; // FIXME: config
    /* Got SrnServerConfig */
    cfg = NULL;
    if (!cfg){
        // If no such SrnServerConfig, create one
        cfg = srn_server_config_new(host);
        if (!cfg){
            ret =  RET_ERR(_("Failed to create server \"%1$s\""), host);
            goto fin;
        }
        new_cfg = TRUE;
    }
    /* Instantiate SrnServer */
    if (!cfg->srv){
        ret = srn_config_manager_read_server_config(
                srn_application_get_default()->cfg_mgr,
                cfg,
                cfg->name);
        if (!RET_IS_OK(ret)){
            goto fin;
        }
        // FIXME: config
        // if (!str_is_empty(host)){
        //     str_assign(&cfg->host, host);
        // }
        // if (port){
        //     cfg->port = port;
        // }
        if (!str_is_empty(passwd)){
            str_assign(&cfg->passwd, passwd);
        }
        if (!str_is_empty(user)){
            str_assign(&cfg->nickname, user);
        }
        if (g_ascii_strcasecmp(scheme, "ircs") == 0){
            cfg->irc->tls = TRUE;
        }

        ret = srn_server_config_check(cfg);
        if (!RET_IS_OK(ret)){
            goto fin;
        }

        // If no such SrnServer, create one
        srv = srn_server_new(cfg);
        if (!srv) {
            ret =  RET_ERR(_("Failed to instantiate server \"%1$s\""), host);
            goto fin;
        }
    } else {
        srv = cfg->srv;
    }

    DBG_FR("Server instance: %p", srv);

    if (srv->state == SRN_SERVER_STATE_DISCONNECTED){
        srn_server_connect(srv);
    }

    srn_server_wait_until_registered(srv);
    if (!srn_server_is_registered(srv)){
        ret =  RET_ERR(_("Failed to register on server \"%1$s\""), cfg->name);
        goto fin;
    }

    /*  Join channels in URL */
    if (!str_is_empty(path)){
        if (path[0] == '/') {
            path++;    // Skip root of URL path
        }
        join_comma_separated_chans(srv, path);
    }

    if (!str_is_empty(fragment)){
        join_comma_separated_chans(srv, fragment);
    }

    return SRN_OK;

fin:
    if (suri){
        soup_uri_free(suri);
    }
    if (new_cfg && cfg){
        srn_server_config_free(cfg);
    }

    return ret;
}

static void join_comma_separated_chans(SrnServer *srv, const char *comma_chans){
    char *chans;
    const char *chan;

    /* TODO: how to distinguish between channel and password if channel doen't
     * start with a '#'?
     */

    chans = g_strdup(comma_chans);
    chan = strtok(chans, ",");

    do {
        if (!str_is_empty(chan)){
            DBG_FR("Get channnel: %s", chan);
            if (!sirc_is_chan(chan)){
                char *chan2 = g_strdup_printf("#%s", chan);
                sirc_cmd_join(srv->irc, chan2, NULL);
                g_free(chan2);
            } else {
                sirc_cmd_join(srv->irc, chan, NULL);
            }
        }
        chan = strtok(NULL, ",");
    } while (chan);

    g_free(chans);
}
