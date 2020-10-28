/*
** Copyright (C) 2009-2020 Quadrant Information Security <quadrantsec.com>
** Copyright (C) 2009-2020 Champ Clark III <cclark@quadrantsec.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License Version 2 as
** published by the Free Software Foundation.  You may not use, modify or
** distribute this program under any other version of the GNU General
** Public License.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* eve.c
 *
 * Write alerts in a JSON/Suricata like format
 *
 */


#ifdef HAVE_CONFIG_H
#include "config.h"             /* From autoconf */
#endif

#ifdef HAVE_LIBFASTJSON

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "sagan.h"
#include "sagan-defs.h"
#include "json-handler.h"
#include "output-plugins/eve.h"

#include "sagan-config.h"

struct _SaganConfig *config;

void Alert_JSON( _Sagan_Event *Event )
{

    char alert_data[MAX_SYSLOGMSG+1024] = { 0 };
    FILE *eve_stream;
    int eve_stream_int = 0;

    if ( config->eve_alerts == true )
        {

            if (( eve_stream = fopen( config->eve_filename, "a" )) == NULL )
                {
                    Sagan_Log(ERROR, "[%s, line %d] Cannot open %s (%s). Abort", __FILE__, __LINE__, config->eve_filename, strerror(errno));
                }

            eve_stream_int = fileno( eve_stream );

            File_Lock( eve_stream_int );

            Format_JSON_Alert_EVE( Event, alert_data, sizeof(alert_data) );
            fprintf(eve_stream, "%s\n", alert_data);
            fflush(eve_stream);

            File_Unlock( eve_stream_int );
            fclose(eve_stream);

        }
}

void Log_JSON ( _Sagan_Proc_Syslog *SaganProcSyslog_LOCAL, struct timeval tp )
{

    char log_data[MAX_SYSLOGMSG+1024] = { 0 };
    FILE *eve_stream;
    int eve_stream_int = 0;

    if (( eve_stream = fopen( config->eve_filename, "a" )) == NULL )
        {
            Sagan_Log(ERROR, "[%s, line %d] Cannot open %s (%s). Abort", __FILE__, __LINE__, config->eve_filename, strerror(errno));
        }


    eve_stream_int = fileno( eve_stream );

    File_Lock( eve_stream_int );

    Format_JSON_Log_EVE( SaganProcSyslog_LOCAL, tp, log_data, sizeof(log_data) );
    fprintf(eve_stream, "%s\n", log_data);
    fflush(eve_stream);

    File_Unlock( eve_stream_int );


}

#endif
