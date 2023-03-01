/*
MIT License

Copyright (c) 2019 Marcin Borowicz
Copyright (c) 2022 Srikavin Ramkumar <srikavinramkumar@gmail.com>

        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"), to deal
        in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                                                                      copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be included in all
        copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
                SOFTWARE.
                        */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "../src/cat.h"

static char const *input_text;
static size_t input_index;

static int common_var_read_handler(const struct cat_variable *var)
{
        return 0;
}

static int8_t var_int;
static uint8_t var_uint;
static uint8_t var_hex8;
static uint16_t var_hex16;
static uint32_t var_hex32;
static uint8_t var_buf[4];
static char var_string[16];

static struct cat_variable vars[] = {
        {
                .type = CAT_VAR_INT_DEC,
                .data = &var_int,
                .data_size = sizeof(var_int),
                .read = common_var_read_handler
        },
        {
                .type = CAT_VAR_UINT_DEC,
                .data = &var_uint,
                .data_size = sizeof(var_uint),
                .read = common_var_read_handler
        },
        {
                .type = CAT_VAR_NUM_HEX,
                .data = &var_hex8,
                .data_size = sizeof(var_hex8),
                .read = common_var_read_handler
        },
        {
                .type = CAT_VAR_NUM_HEX,
                .data = &var_hex16,
                .data_size = sizeof(var_hex16),
                .read = common_var_read_handler
        },
        {
                .type = CAT_VAR_NUM_HEX,
                .data = &var_hex32,
                .data_size = sizeof(var_hex32),
                .read = common_var_read_handler
        },
        {
                .type = CAT_VAR_BUF_HEX,
                .data = &var_buf,
                .data_size = sizeof(var_buf),
                .read = common_var_read_handler
        },
        {
                .type = CAT_VAR_BUF_STRING,
                .data = &var_string,
                .data_size = sizeof(var_string),
                .read = common_var_read_handler
        }
};

static int a_run(const struct cat_command *cmd)
{
        return 0;
}

static int ap_run(const struct cat_command *cmd)
{
        return 0;
}

static int test_run(const struct cat_command *cmd)
{
        return 0;
}

static int cmd_read(const struct cat_command *cmd, uint8_t *data, size_t *data_size, const size_t max_data_size)
{
        return 0;
}


static struct cat_command cmds[] = {
        {
                .name = "A",
                .run = a_run,
                .disable = false,
        },
        {
                .name = "AP",
                .run = ap_run,
                .disable = false,
        },
        {
                .name = "+TEST",
                .run = test_run,
                .disable = false,
        },
        {
                .name = "+SET",
                .read = cmd_read,

                .var = vars,
                .var_num = sizeof(vars) / sizeof(vars[0])
        },
        {
                .name = "+TEST",
                .read = cmd_read,

                .var = vars,
                .var_num = sizeof(vars) / sizeof(vars[0])
        },
};

static char buf[128];

static struct cat_command_group cmd_group = {
        .cmd = cmds,
        .cmd_num = sizeof(cmds) / sizeof(cmds[0]),
};

static struct cat_command_group *cmd_desc[] = {
        &cmd_group
};

static struct cat_descriptor desc = {
        .cmd_group = cmd_desc,
        .cmd_group_num = sizeof(cmd_desc) / sizeof(cmd_desc[0]),

        .buf = (uint8_t *) buf,
        .buf_size = sizeof(buf),
};

static int write_char(char ch)
{
        return 1;
}

static int read_char(char *ch)
{
        if (input_index >= strlen(input_text))
                return 0;

        *ch = input_text[input_index];
        input_index++;
        return 1;
}

static struct cat_io_interface iface = {
        .read = read_char,
        .write = write_char
};

static void prepare_input(const char *text)
{
        input_text = text;
        input_index = 0;
}

int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
        // ensure null-termination to avoid false positives
        if (size == 0 || data[size - 1] != 0) return 0;

        struct cat_object at;

        cat_init(&at, &desc, &iface, NULL);
        prepare_input((const char*) data);

        while (cat_service(&at) != 0) {};

        return 0;
}
