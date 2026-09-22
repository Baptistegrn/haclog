#include "haclog_console_handler.h"
#include "haclog/haclog_vsprintf.h"
#include "haclog/handler/haclog_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*
 * unix terminal colors
 *
 * output colored text, need print or echo -e the control characters for 
 * required color, then output text, and then reset the output back to default.
 *
 * "\x1B[${code}m" or "\033[${code}m", ${code} represent color code
 *
 * reset codes: 0
 *
 * color codes:
 * | color        | foreground | background |
 * | ----         | ----       | ----       |
 * | default      | 39         | 49         |
 * | black        | 30         | 40         |
 * | dark red     | 31         | 41         |
 * | dark green   | 32         | 42         |
 * | dark yellow  | 33         | 43         |
 * | dark blue    | 34         | 44         |
 * | dark magenta | 35         | 45         |
 * | dark cyan    | 36         | 46         |
 * | light gray   | 37         | 47         |
 * | dark gray    | 90         | 100        |
 * | red          | 91         | 101        |
 * | green        | 92         | 102        |
 * | orange       | 93         | 103        |
 * | blue         | 94         | 104        |
 * | magenta      | 95         | 105        |
 * | cyan         | 96         | 106        |
 * | white        | 97         | 107        |
 *
 * format for foreground color is:
 * "\x1B[" + "<0 or 1, meaning nromal or bold>;" + "<color code>" + "m"
 *
 * format for background:
 * "\x1B[" + "<color code>" + "m"
 * */

#define UNIX_TERMINAL_COLOR(code) "\x1B[" #code "m"

// terminal color for *nix
#define UNIX_TERMINAL_COLOR_RST UNIX_TERMINAL_COLOR(0)
#define UNIX_TERMINAL_COLOR_RED UNIX_TERMINAL_COLOR(31)
#define UNIX_TERMINAL_COLOR_GRN UNIX_TERMINAL_COLOR(32)
#define UNIX_TERMINAL_COLOR_YEL UNIX_TERMINAL_COLOR(33)
#define UNIX_TERMINAL_COLOR_BLU UNIX_TERMINAL_COLOR(34)
#define UNIX_TERMINAL_COLOR_MAG UNIX_TERMINAL_COLOR(35)
#define UNIX_TERMINAL_COLOR_CYN UNIX_TERMINAL_COLOR(36)
#define UNIX_TERMINAL_COLOR_WHT UNIX_TERMINAL_COLOR(37)

/*TODO : remove the switch
while keeping the color to be written to memory
*/
static int haclog_console_handler_before_write(haclog_handler_t *base_handler,
											   haclog_meta_info_t *meta)
{
	haclog_console_handler_t *handler =
		(haclog_console_handler_t *)base_handler;

	if (handler->enable_color &&
		meta->loc->level >= HACLOG_LEVEL_TRACE &&
		meta->loc->level < HACLOG_LEVEL_MAX) {

		handler->fp = stderr;

#if HACLOG_PLATFORM_WINDOWS

		const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		// Get the current text color
		CONSOLE_SCREEN_BUFFER_INFO sb_info;
		GetConsoleScreenBufferInfo(stdout_handle, &sb_info);
		handler->sb_attrs = sb_info.wAttributes;

		haclog_color_t color;

		switch (meta->loc->level) {
		case HACLOG_LEVEL_TRACE:
			color = handler->trace;
			break;

		case HACLOG_LEVEL_DEBUG:
			color = handler->debug;
			break;

		case HACLOG_LEVEL_INFO:
			color = handler->info;
			break;

		case HACLOG_LEVEL_WARNING:
			color = handler->warning;
			break;

		case HACLOG_LEVEL_ERROR:
			color = handler->error;
			break;

		case HACLOG_LEVEL_FATAL:
			color = handler->fatal;
			break;

		default:
			color = handler->max;
			break;
		}

		switch (color) {
		case HACLOG_COLOR_RED:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_RED | FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_GRN:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_GREEN | FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_YEL:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_RED |
				FOREGROUND_GREEN |
				FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_BLU:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_BLUE | FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_MAG:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_RED |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_CYN:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_GREEN |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_WHT:
			SetConsoleTextAttribute(
				stdout_handle,
				FOREGROUND_RED |
				FOREGROUND_GREEN |
				FOREGROUND_BLUE |
				FOREGROUND_INTENSITY
			);
			break;

		case HACLOG_COLOR_RST:
		default:
			SetConsoleTextAttribute(
				stdout_handle,
				handler->sb_attrs
			);
			break;
		}

#else

		haclog_color_t color;

		switch (meta->loc->level) {
		case HACLOG_LEVEL_TRACE:
			color = handler->trace;
			break;

		case HACLOG_LEVEL_DEBUG:
			color = handler->debug;
			break;

		case HACLOG_LEVEL_INFO:
			color = handler->info;
			break;

		case HACLOG_LEVEL_WARNING:
			color = handler->warning;
			break;

		case HACLOG_LEVEL_ERROR:
			color = handler->error;
			break;

		case HACLOG_LEVEL_FATAL:
			color = handler->fatal;
			break;

		default:
			color = handler->max;
			break;
		}

		switch (color) {
		case HACLOG_COLOR_RED:
			fwrite(UNIX_TERMINAL_COLOR_RED,
				   1, strlen(UNIX_TERMINAL_COLOR_RED), handler->fp);
			break;

		case HACLOG_COLOR_GRN:
			fwrite(UNIX_TERMINAL_COLOR_GRN,
				   1, strlen(UNIX_TERMINAL_COLOR_GRN), handler->fp);
			break;

		case HACLOG_COLOR_YEL:
			fwrite(UNIX_TERMINAL_COLOR_YEL,
				   1, strlen(UNIX_TERMINAL_COLOR_YEL), handler->fp);
			break;

		case HACLOG_COLOR_BLU:
			fwrite(UNIX_TERMINAL_COLOR_BLU,
				   1, strlen(UNIX_TERMINAL_COLOR_BLU), handler->fp);
			break;

		case HACLOG_COLOR_MAG:
			fwrite(UNIX_TERMINAL_COLOR_MAG,
				   1, strlen(UNIX_TERMINAL_COLOR_MAG), handler->fp);
			break;

		case HACLOG_COLOR_CYN:
			fwrite(UNIX_TERMINAL_COLOR_CYN,
				   1, strlen(UNIX_TERMINAL_COLOR_CYN), handler->fp);
			break;

		case HACLOG_COLOR_WHT:
			fwrite(UNIX_TERMINAL_COLOR_WHT,
				   1, strlen(UNIX_TERMINAL_COLOR_WHT), handler->fp);
			break;

		case HACLOG_COLOR_RST:
		default:
			fwrite(UNIX_TERMINAL_COLOR_RST,
				   1, strlen(UNIX_TERMINAL_COLOR_RST), handler->fp);
			break;
		}

#endif
	} else {
		handler->fp = stdout;
	}

	return 0;
}

static int haclog_console_handler_after_write(haclog_handler_t *base_handler,
											  haclog_meta_info_t *meta)
{
	haclog_console_handler_t *handler =
		(haclog_console_handler_t *)base_handler;

	fwrite("\n", 1, 1, handler->fp);

	if (handler->enable_color && meta->loc->level >= HACLOG_LEVEL_WARNING) {
#if HACLOG_PLATFORM_WINDOWS
		const HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(stdout_handle, handler->sb_attrs);
#else
		fwrite(UNIX_TERMINAL_COLOR_RST, 1, strlen(UNIX_TERMINAL_COLOR_RST),
			   stderr);
#endif
	}

	fflush(handler->fp);

	return 0;
}

static int haclog_console_handler_write(haclog_handler_t *base_handler,
										const char *msg, int msglen)
{
	haclog_console_handler_t *handler =
		(haclog_console_handler_t *)base_handler;
	return (int)fwrite(msg, 1, msglen, handler->fp);
}

static int haclog_console_handler_writev(haclog_handler_t *base_handler,
										 const char *fmt_str, ...)
{
	haclog_console_handler_t *handler =
		(haclog_console_handler_t *)base_handler;

	va_list args;
	va_start(args, fmt_str);
	int n = vfprintf(handler->fp, fmt_str, args);
	va_end(args);

	return n;
}

static void haclog_console_handler_destroy(struct haclog_handler *handler)
{
	HACLOG_UNUSED(handler);
}

int haclog_console_handler_init(haclog_console_handler_t *handler,
								int enable_color)
{
	memset(handler, 0, sizeof(*handler));
	handler->enable_color = enable_color;

	handler->base.before_write = haclog_console_handler_before_write;
	handler->base.write_meta = haclog_handler_default_write_meta;
	handler->base.write = haclog_console_handler_write;
	handler->base.writev = haclog_console_handler_writev;
	handler->base.after_write = haclog_console_handler_after_write;
	handler->base.destroy = haclog_console_handler_destroy;
	handler->base.level = HACLOG_LEVEL_INFO;

	if(enable_color){
		// default colors
		haclog_color_t colors[] = {
			HACLOG_COLOR_WHT, // trace
			HACLOG_COLOR_CYN, // debug
			HACLOG_COLOR_GRN, // info
			HACLOG_COLOR_YEL, // warning
			HACLOG_COLOR_RED, // error
			HACLOG_COLOR_MAG,  // fatal
			HACLOG_COLOR_MAG   // max
		};

		haclog_console_handler_set_color(
			(haclog_handler_t *)handler,
			colors,
			sizeof(colors) / sizeof(colors[0])
		);
	}

	return 0;
}


int haclog_console_handler_set_color(haclog_handler_t *base_handler,
									 haclog_color_t *colors,
									 int colors_count)
{
	haclog_console_handler_t *handler =
		(haclog_console_handler_t *)base_handler;

	if (colors == NULL || colors_count <= 0) {
		return 1;
	}

	/*
	 * You don't have to change all colors.
	 * For example, color_count = 2 changes only
	 * TRACE and DEBUG.
	 */

	if (colors_count >= 1) {
		handler->trace = colors[0];
	}

	if (colors_count >= 2) {
		handler->debug = colors[1];
	}

	if (colors_count >= 3) {
		handler->info = colors[2];
	}

	if (colors_count >= 4) {
		handler->warning = colors[3];
	}

	if (colors_count >= 5) {
		handler->error = colors[4];
	}

	if (colors_count >= 6) {
		handler->fatal = colors[5];
	}

	else {
		handler->max = colors[6];
	}

	return 0;
}
