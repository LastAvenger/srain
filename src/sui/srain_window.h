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

#ifndef __SRAIN_WINDOW_H
#define __SRAIN_WINDOW_H

#include <gtk/gtk.h>

#include "srain_app.h"
#include "srain_buffer.h"
#include "srain_server_buffer.h"
#include "srain_connect_popover.h"
#include "srain_join_popover.h"

#define SUI_TYPE_WINDOW (sui_window_get_type())
#define SUI_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST((obj), SUI_TYPE_WINDOW, SuiWindow))
#define SRAIN_IS_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_TYPE((obj), SUI_TYPE_WINDOW))

typedef struct _SuiWindow SuiWindow;
typedef struct _SuiWindowClass SuiWindowClass;

GType sui_window_get_type(void);
SuiWindow* sui_window_new(SuiApplication *app, SuiWindowEvents *events, SuiWindowConfig *cfg);

/* Sui interface */
void* sui_window_get_ctx(SuiWindow *self);
void sui_window_set_ctx(SuiWindow *self, void *ctx);
SuiWindowEvents* sui_window_get_events(SuiWindow *sui);

void sui_window_add_buffer(SuiWindow *self, SuiBuffer *buffer);
void sui_window_rm_buffer(SuiWindow *self, SuiBuffer *buffer);
SuiBuffer *sui_window_get_buffer(SuiWindow *self, const char *name, const char *remark);
SuiBuffer *sui_window_get_cur_buffer(SuiWindow *self);
SrainServerBuffer* sui_window_get_cur_server_buffer(SuiWindow *self);

void sui_window_spinner_toggle(SuiWindow *self, gboolean is_busy);
void sui_window_stack_sidebar_update(SuiWindow *self, SuiBuffer *buffer, const char *nick, const char *msg);
int sui_window_is_active(SuiWindow *self);
void sui_window_tray_icon_stress(SuiWindow *self, int stress);

SrainConnectPopover *sui_window_get_connect_popover(SuiWindow *self);
SrainJoinPopover *sui_window_get_join_popover(SuiWindow *self);

#endif /* __SRAIN_WINDOW_H */
