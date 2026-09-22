/******************************************************************************
 *  @file         haclog_console_handler.h
 *  @author       Muggle Wei
 *  @email        mugglewei@gmail.com
 *  @date         2023-10-08
 *  @copyright    Copyright 2023 Muggle Wei
 *  @license      MIT License
 *  @brief        haclog console handler
 *****************************************************************************/

#ifndef HACLOG_CONSOLE_HANDLER_H_
#define HACLOG_CONSOLE_HANDLER_H_

#include "haclog/haclog_macro.h"
#include "haclog/handler/haclog_handler.h"
#include <stdio.h>

HACLOG_EXTERN_C_BEGIN


// enum color define by the user 
typedef enum {
	HACLOG_COLOR_RST, //reset (default color)
	HACLOG_COLOR_RED, //red
	HACLOG_COLOR_GRN, //green
	HACLOG_COLOR_YEL, //yellow
	HACLOG_COLOR_BLU, //blue
	HACLOG_COLOR_MAG, //magenta
	HACLOG_COLOR_CYN, //cyan
	HACLOG_COLOR_WHT  //white
} haclog_color_t;


typedef struct haclog_console_handler {
	haclog_handler_t base; //!< base log handler
	FILE *fp; //!< current file handle
	int enable_color; //!< enable color

	haclog_color_t trace;
	haclog_color_t debug;
	haclog_color_t info;
	haclog_color_t warning;
	haclog_color_t error;
	haclog_color_t fatal;
	haclog_color_t max;

#if HACLOG_PLATFORM_WINDOWS
	WORD sb_attrs;
#endif
} haclog_console_handler_t;

/**
 * @brief initialize a console log handler
 *
 * @param handler       console log handler pointer
 * @param enable_color  enable console log handler color
 *
 * @return
 *   - on success, return 0
 *   - otherwise return error code
 */
HACLOG_EXPORT
int haclog_console_handler_init(haclog_console_handler_t *handler,
								int enable_color);

/**
 * @brief Set the colors used by the console log handler.
 *
 * The colors are applied in the following order:
 * trace, debug, info, warning, error, fatal, max.
 *
 * If @p colors_count is less than the number of available levels,
 * only the first @p colors_count colors are updated.
 *
 * @param handler      Console log handler pointer.
 * @param colors       Array of colors to apply to the log levels.
 * @param colors_count Number of colors in the @p colors array.
 *
 * @return
 *   - 0 on success.
 *   - 1 if @p handler or @p colors is NULL, or if @p colors_count is less than 1.
 */
HACLOG_EXPORT
int haclog_console_handler_set_color(haclog_handler_t *handler,
									 haclog_color_t *colors,
									 int colors_count);

HACLOG_EXTERN_C_END

#endif // !HACLOG_CONSOLE_HANDLER_H_
