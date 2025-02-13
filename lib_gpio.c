//------------------------------------------------------------------------------
/**
 * @file lib_gpio.c
 * @author charles-park (charles.park@hardkernel.com)
 * @brief GPIO control library for ODROID-JIG.
 * @version 0.2
 * @date 2023-10-10
 *
 * @package apt install minicom
 *
 * @copyright Copyright (c) 2022
 *
 */
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
#include <pthread.h>

#include "lib_gpio.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int gpio_export (int gpio)
{
    char fname[256];
    FILE *fp;

    memset (fname, 0x00, sizeof(fname));
    sprintf (fname, "%s/export", GPIO_CONTROL_PATH);
    if ((fp = fopen (fname, "w")) != NULL) {
        char gpio_num[4];
        memset (gpio_num, 0x00, sizeof(gpio_num));
        sprintf (gpio_num, "%d", gpio);
        fwrite (gpio_num, strlen(gpio_num), 1, fp);
        fclose (fp);
        return 1;
    }
    printf ("%s error : gpio = %d\n", __func__, gpio);
    return 0;
}

//------------------------------------------------------------------------------
int gpio_direction (int gpio, int status)
{
    char fname [256], gpio_dir [4];
    FILE *fp;

    memset  (gpio_dir, 0, sizeof(gpio_dir));
    memset  (fname, 0x00, sizeof(fname));
    sprintf (fname, "%s/gpio%d/direction", GPIO_CONTROL_PATH, gpio);
    if ((fp = fopen (fname, "w")) != NULL) {
        sprintf(gpio_dir, "%s", status ? "out" : "in");
        fwrite (gpio_dir, strlen(gpio_dir), 1, fp);
        fclose (fp);
        return 1;
    }
    printf ("%s : error!(gpio = %d)\n", __func__, gpio);
    return 0;
}

//------------------------------------------------------------------------------
int gpio_set_direction (int gpio, int status)
{
    return gpio_direction (gpio, status);
}

//------------------------------------------------------------------------------
int gpio_get_direction (int gpio)
{
    char fname [256], gpio_dir [4];
    FILE *fp;

    memset  (gpio_dir, 0, sizeof(gpio_dir));
    memset  (fname, 0x00, sizeof(fname));
    sprintf (fname, "%s/gpio%d/direction", GPIO_CONTROL_PATH, gpio);
    if ((fp = fopen (fname, "r")) != NULL) {
        fread (gpio_dir, sizeof(gpio_dir), 1, fp);
        fclose (fp);
    } else {
        printf ("%s : error!(gpio = %d)\n", __func__, gpio);
        return 0;
    }

    return (strncmp (gpio_dir, "out", sizeof("out")) == 0) ? 1 : 0;
}

//------------------------------------------------------------------------------
int gpio_set_value (int gpio, int s_value)
{
    char fname[256];
    FILE *fp;

    memset (fname, 0x00, sizeof(fname));
    sprintf (fname, "%s/gpio%d/value", GPIO_CONTROL_PATH, gpio);
    if ((fp = fopen (fname, "w")) != NULL) {
        fputc (s_value ? '1' : '0', fp);
        fclose (fp);
        return 1;
    }
    printf ("%s error : gpio = %d\n", __func__, gpio);
    return 0;
}

//------------------------------------------------------------------------------
int gpio_get_value (int gpio, int *g_value)
{
    char fname[256];
    FILE *fp;

    memset (fname, 0x00, sizeof(fname));
    sprintf (fname, "%s/gpio%d/value", GPIO_CONTROL_PATH, gpio);
    if ((fp = fopen (fname, "r")) != NULL) {
        *g_value = (fgetc (fp) - '0');
        fclose (fp);
        return 1;
    }
    printf ("%s error : gpio = %d\n", __func__, gpio);
    return 0;
}

//------------------------------------------------------------------------------
int gpio_unexport (int gpio)
{
    char fname[256];
    FILE *fp;

    memset (fname, 0x00, sizeof(fname));
    sprintf (fname, "%s/unexport", GPIO_CONTROL_PATH);
    if ((fp = fopen (fname, "w")) != NULL) {
        char gpio_num[4];
        memset (gpio_num, 0x00, sizeof(gpio_num));
        sprintf (gpio_num, "%d", gpio);
        fwrite (gpio_num, strlen(gpio_num), 1, fp);
        fclose (fp);
        return 1;
    }
    printf ("%s error : gpio = %d\n", __func__, gpio);
    return 0;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
