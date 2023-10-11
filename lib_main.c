//------------------------------------------------------------------------------
/**
 * @file lib_main.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief GPIO control library for ODROID-JIG.
 * @version 0.2
 * @date 2023-10-10
 *
 * @package
 *
 * @copyright Copyright (c) 2022
 *
 */
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <getopt.h>

#include "lib_gpio.h"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#if defined(__LIB_GPIO_APP__)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// gpio list
//
// cat /sys/kernel/debug/gpio
//
static void print_usage (const char *prog)
{
    puts("");
    printf("Usage: %s [-p:gpio no] [-d:out|in] [-o:1|0]\n", prog);
    puts("\n"
         "  -p --gpio number        Control Device node\n"
         "  -d --gpio direction     out:output, in:input (default input)\n"
         "  -o --gpio set value     if direction is out, set data (1 or 0)\n"
         "\n"
         "  e.g) gpio 250 set high.\n"
         "       lib_gpio -p 250 -d out -o 1\n"
    );
    exit(1);
}

//------------------------------------------------------------------------------
/* Control variable */
//------------------------------------------------------------------------------
static char OPT_GPIO_DIR = 0;
static int  OPT_GPIO_VAL = 0;
static int  OPT_GPIO_NUM = 0;

//------------------------------------------------------------------------------
// 문자열 변경 함수. 입력 포인터는 반드시 메모리가 할당되어진 변수여야 함.
//------------------------------------------------------------------------------
static void tolowerstr (char *p)
{
    int i, c = strlen(p);

    for (i = 0; i < c; i++, p++)
        *p = tolower(*p);
}

//------------------------------------------------------------------------------
static void toupperstr (char *p)
{
    int i, c = strlen(p);

    for (i = 0; i < c; i++, p++)
        *p = toupper(*p);
}

//------------------------------------------------------------------------------
static void parse_opts (int argc, char *argv[])
{
    while (1) {
        static const struct option lopts[] = {
            { "gpio_num",  0, 0, 'p' },
            { "gpio_dir",  0, 0, 'd' },
            { "gpio_val",  0, 0, 'o' },
            { NULL, 0, 0, 0 },
        };
        int c;

        c = getopt_long(argc, argv, "p:d:o:h", lopts, NULL);

        if (c == -1)
            break;

        switch (c) {
        case 'p':
            OPT_GPIO_NUM = atoi(optarg);
            break;
        /* direction, 1 = out, 0 = in (default) */
        case 'd':
            tolowerstr (optarg);
            if (!strncmp (optarg, "out", strlen(optarg)))
                OPT_GPIO_DIR = 1;
            break;
        /* output data */
        case 'o':
            OPT_GPIO_VAL = (atoi(optarg) != 0) ? 1: 0;
            break;
        case 'h':
        default:
            print_usage(argv[0]);
            break;
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
    int data = 0;

    parse_opts(argc, argv);

    if (!OPT_GPIO_NUM)
        print_usage(argv[0]);

    if (!gpio_export    (OPT_GPIO_NUM)) exit(1);
    if (!gpio_direction (OPT_GPIO_NUM, OPT_GPIO_DIR)) exit(1);

    if (OPT_GPIO_DIR)
        gpio_set_value (OPT_GPIO_NUM, OPT_GPIO_VAL);
    else
        gpio_get_value (OPT_GPIO_NUM, &data);

    if (!gpio_unexport  (OPT_GPIO_NUM)) exit(1);

    printf ("GPIO = %d, DIR = %s, Data = %d\n",
        OPT_GPIO_NUM,
        OPT_GPIO_DIR ? "out" : "in",
        OPT_GPIO_DIR ? OPT_GPIO_VAL : data);

    return 0;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif  // #if defined(__LIB_GPIO_APP__)
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
