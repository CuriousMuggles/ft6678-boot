/*-------------------------------------------------------------------*/
/*! \file utilib.c
    \brief Interface for printf and shell.

COPYRIGHT(c)    AVIC, Dep6  2012  All Rights Reserved.

This file is interface for shell commands and printfk(like printf()).
*/

#include <stdio.h>
#include <stdint.h>
#include "symTbl.h"
#include "utilib.h"
#include "cmd_table.h"

/* buffer for uart inputed user's command */
int8_t1 cmd_buf[256] = "";
int8_t1 last_cmd[256] = "";
uint8_t cmd_buf_head = 0;
uint8_t cmd_buf_print = 0;
int poll_cmd_delay = 0;
int8_t cmd_param_arry[UART_PARA_NUM_MAX][UART_PARA_LEN_MAX];

uint32_t cmd_log_count = 0;
uint32_t cmd_log_up_cnt = 0;
int8_t cmd_log_arry[UART_CMD_LOG_NUM][256] = {""};    /* command log save */
int8_t cmd_buf_latest[256] = "";
uint8_t spec_cmd_stat = 0;    /* special command status for key "up / down / left /right" */
int8_t printbuffer[PRINT_BUFSIZE];

/*----------------------------------------------------*/
/*! \fn printf
\brief 
\param fmt -- Pass ptr for format string.

\retval 

This function is called like printf, and print string to uart.
*/
void printfk(const int8_t1 *fmt, ...)
{
    va_list args;
    
    va_start(args, fmt);
    /* get buffer */
    vsprintfk(printbuffer, fmt, args);
    va_end(args);
    
    putbufk(printbuffer);
}

/* original source from linux/lib/vsprintf.c,
 Copyright (C) 1991, 1992  Linus Torvalds */

/*----------------------------------------------------*/
/*! \fn skip_atoi(const char **s)
\brief change a string to a number.
\param s -- Pass a ptr for a string.

\retval 


*/
static int32_t1 skip_atoi(const char **s)
{
    int32_t i = 0;
    while(is_digit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

/*----------------------------------------------------*/
/*! \fn strnlenk(const char * s, int count)
\brief 
\param 

\retval 


*/
int strnlenk(const char * s, int count)
{
    const char *sc;

    for (sc = s; ((count--) && (*sc != '\0')); ++sc)
        /* nothing */;
    return sc - s;
}


/*----------------------------------------------------*/
/*! \fn number(char * str, long num, 
            int base, int size, int precision,int type)
\brief 
\param 

\retval 


*/
static char * number(char * str, unsigned long num, int base,
                int size, int precision ,int type)
{
    char c,sign,tmp[66];
    const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
    int i;

    if (type & LARGE)
        digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (type & LEFT)
        type &= ~ZEROPAD;
    if (base < 2 || base > 36)
        return 0;
    c = (type & ZEROPAD) ? '0' : ' ';
    sign = 0;
    if (type & SIGN) {
        if (num < 0) {
            sign = '-';
            num = -num;
            size--;
        } else if (type & PLUS) {
            sign = '+';
            size--;
        } else if (type & SPACE) {
            sign = ' ';
            size--;
        }
    }
    if (type & SPECIAL) {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }
    i = 0;
    if (num == 0)
        tmp[i++]='0';
    else
    {
        while (num != 0)
        {
            tmp[i++] = digits[num%base];
            num /= base;

        if (num < base)
        {
            if (num != 0)
                tmp[i++] = digits[num];
            break;
        }
        }
    }
    if (i > precision)
        precision = i;
    size -= precision;
    if (!(type&(ZEROPAD+LEFT)))
        while(size-->0)
            *str++ = ' ';
    if (sign)
        *str++ = sign;
    if (type & SPECIAL) {
        if (base==8)
            *str++ = '0';
        else if (base==16) {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (!(type & LEFT))
        while (size-- > 0)
            *str++ = c;
    while (i < precision--)
        *str++ = '0';
    while (i-- > 0)
        *str++ = tmp[i];
    while (size-- > 0)
        *str++ = ' ';
    return str;
}


/*----------------------------------------------------*/
/*! \fn vsprintfk(int8_t * buf, const int8_t * fmt, va_list args)
\brief 
\param 

\retval 

*/
int32_t1 vsprintfk(int8_t1 * buf, const int8_t1 * fmt, va_list args)
{
    int len;
    unsigned long num;
    int i, base;
    char * str;
    const char *s;

    int flags;        /* flags to number() */

    int field_width;    /* width of output field */
    int precision;        /* min. # of digits for integers; max
                   number of chars for from string */
    int qualifier;        /* 'h', 'l', or 'L' for integer fields */

    for (str=buf ; *fmt ; ++fmt) {
        if (*fmt != '%') {
            *str++ = *fmt;
            continue;
        }

        /* process flags */
        flags = 0;
        repeat:
            ++fmt;        /* this also skips first '%' */
            switch (*fmt) {
                case '-': flags |= LEFT; goto repeat;
                case '+': flags |= PLUS; goto repeat;
                case ' ': flags |= SPACE; goto repeat;
                case '#': flags |= SPECIAL; goto repeat;
                case '0': flags |= ZEROPAD; goto repeat;
                }

        /* get field width */
        field_width = -1;
        if (is_digit(*fmt))
            field_width = skip_atoi(&fmt);
        else if (*fmt == '*') {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if (field_width < 0) {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        /* get the precision */
        precision = -1;
        if (*fmt == '.') {
            ++fmt;
            if (is_digit(*fmt))
                precision = skip_atoi(&fmt);
            else if (*fmt == '*') {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if (precision < 0)
                precision = 0;
        }

        /* get the conversion qualifier */
        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
            qualifier = *fmt;
            ++fmt;
        }

        /* default base */
        base = 10;

        switch (*fmt) {
        case 'c':
            if (!(flags & LEFT))
                while (--field_width > 0)
                    *str++ = ' ';
            *str++ = (unsigned char) va_arg(args, int);
            while (--field_width > 0)
                *str++ = ' ';
            continue;

        case 's':
            s = va_arg(args, char *);
            if (!s)
                s = "<NULL>";

            len = strnlenk(s, precision);

            if (!(flags & LEFT))
                while (len < field_width--)
                    *str++ = ' ';
            for (i = 0; i < len; ++i)
                *str++ = *s++;
            while (len < field_width--)
                *str++ = ' ';
            continue;

        case 'p':
            if (field_width == -1) {
                field_width = 2*sizeof(void *);
                flags |= ZEROPAD;
            }
            str = number(str,
                (unsigned long) va_arg(args, void *), 16,
                field_width, precision, flags);
            continue;


        case 'n':
            if (qualifier == 'l') {
                long * ip = va_arg(args, long *);
                *ip = (str - buf);
            } else {
                int * ip = va_arg(args, int *);
                *ip = (str - buf);
            }
            continue;

        case '%':
            *str++ = '%';
            continue;

        /* integer number formats - set up the flags and "break" */
        case 'o':
            base = 8;
            break;

        case 'X':
            flags |= LARGE;
        case 'x':
            base = 16;
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            break;

        default:
            *str++ = '%';
            if (*fmt)
                *str++ = *fmt;
            else
                --fmt;
            continue;
        }
        if (qualifier == 'l')
            num = va_arg(args, unsigned long);
        else if (qualifier == 'h') {
            num = (unsigned short) va_arg(args, int);
            if (flags & SIGN)
                num = (short) num;
        } else if (flags & SIGN)
            num = va_arg(args, int);
        else
            num = va_arg(args, unsigned int);
        str = number(str, num, base, field_width, precision, flags);
    }
    *str = '\0';
    return str-buf;
}



/*----------------------------------------------------*/
/*! \fn itoa(int32_t num, int8_t *str, int32_t radix, int32_t len)
\brief 
\param 

\retval 


*/
int8_t1 itoak(int32_t1 num, int8_t1 *str, int32_t1 radix, int32_t1 len)
{
    int8_t1 str_p[] = "0123456789ABCDEFG";
    int8_t1 temp = 0, neg_flag = 0, blank_len = 0;
    int8_t1 *ptr = str;
    uint32_t i = 0, j = 0;

    if ((DEC == radix) && (num < 0))
    {
        num = -num;
        neg_flag = 1;
    }
    while(num)
    {
        *ptr++ = str_p[num%radix];
        num /= radix;

        if (num < radix)
        {
            *ptr++ = str_p[num];
            break;
        }
    }
    /* specify format length */
    if (len > 0)
    {
        blank_len = (len - (ptr - str));
        if (blank_len > 0)
        {
            for (j = 0; j < blank_len; j ++)
            {/* pad with zero */
                *ptr++ = '0';
            }
        }
    }
    /* negative decimal format */
    if (neg_flag)
    {
        *ptr++ = '-';
    }

    *ptr = '\0';
    
    j = ptr - str -1;
    for (i = 0; i < ((ptr - str) / 2); i ++)
    {
        temp = str[i];
        str[i] = str[j];
        str[j--] = temp;
    }
    return 1;
}

#ifdef SPM_DEBUG

/*----------------------------------------------------*/
/*! \fn skip_atoi(const char **s)
\brief change a string to a number.
\param s -- Pass a ptr for a string.

\retval 

*/
static int32_t1 atoik(const char *s)
{
    int32_t i = 0, j = 0, k = 0;
    
    j = strnlenk(s, 64);

    if ('x' == s[1] || 'X' == s[1])
    {
        for (k = 2; k < j; k ++)
        {
            if (('0' <= s[k]) && ('9' >= s[k]))
            {/* is number */
                i = i * 16 + s[k] - '0';
            }
            else if (('a' <= s[k]) && ('f' >= s[k]))
            {/* is 'a' to 'f' */
                i = i * 16 + s[k] - 'a' + 10;
            }
            else if (('A' <= s[k]) && ('F' >= s[k]))
            {/* is 'A' to 'F' */
                i = i * 16 + s[k] - 'A' + 10;
            }
            else
            {/* illegal number */
                return 0;
            }
        }
    }
    else if ('h' == s[j -1] || 'H' == s[j -1])
    {
        for (k = 0; k < (j - 1); k ++)
        {
            if (('0' <= s[k]) && ('9' >= s[k]))
            {/* is number */
                i = i * 16 + s[k] - '0';
            }
            else if (('a' <= s[k]) && ('f' >= s[k]))
            {/* is 'a' to 'f' */
                i = i * 16 + s[k] - 'a' + 10;
            }
            else if (('A' <= s[k]) && ('F' >= s[k]))
            {/* is 'A' to 'F' */
                i = i * 16 + s[k] - 'A' + 10;
            }
            else
            {/* illegal number */
                return 0;
            }
        }
    }
    else if ('b' == s[1] || 'B' == s[1])
    {
        for (k = 2; k < j; k ++)
        {
            if ('0' == s[k])
            {/* is bin number */
                i = i * 2;
            }
            else if ('1' == s[k])
            {
                i = i * 2 + 1;
            }
            else
            {/* illegal number */
                return 0;
            }
        }
    }
    else if ('b' == s[j -1] || 'B' == s[j -1])
    {
        for (k = 0; k < (j - 1); k ++)
        {
            if ('0' == s[k])
            {/* is bin number */
                i = i * 2;
            }
            else if ('1' == s[k])
            {
                i = i * 2 + 1;
            }
            else
            {/* illegal number */
                return 0;
            }
        }
    }
    else
    {
        for (k = 0; k < j; k ++)
        {
            if (('0' <= s[k]) && ('9' >= s[k]))
            {/* is number */
                i = i * 10 + s[k] - '0';
            }
            else
            {/* illegal number */
                return 0;
            }
        }
    }
    return i;
}

/*----------------------------------------------------*/
/*! \fn scan_input
\brief 
\param 

\retval 

*/
INPUT_Status scan_input(void)
{
    /* single output buffer to UART */
    int8_t print_buf[8] = "";
    int8_t byt = 0;
    INPUT_Status input_ret = INPUT_BUSY;

    if (byt = getck())
    {
        if (byt > 0x1F)
        {/* real character */
            cmd_buf[cmd_buf_head] = byt;
            cmd_buf_head++;
            cmd_buf[cmd_buf_head] = '\0';
            printfk("%c", byt);
            spec_cmd_stat = 4;    /* indicate that command edited */
        }
        else
        {/* control character */
            switch (byt)
            {
                case 0x08 : /* get "back space" */
                    if (!cmd_buf_head)
                    {
                        printfk("%c", 0x07);    /* alarm for no valid character */
                        spec_cmd_stat = 3;    /* indicate that command empty */
                    }
                    else
                    {
                        cmd_buf[cmd_buf_head - 1] = '\0';
                        cmd_buf_head --;
                        print_buf[0] = 0x08;
                        print_buf[1] = 0x20;
                        print_buf[2] = 0x08;
                        print_buf[3] = '\0';
                        printfk(print_buf);
                        spec_cmd_stat = 4;    /* indicate that command edited */
                    }
                    break;
                case 0x0D :    /*get "Enter" */
                    input_ret = INPUT_COMPLETE;
                    if (!cmd_buf_head)
                    {
                        cmd_buf[0] = '\0';
                    }
                    else
                    {
                        cmd_buf_head = 0;
                    }
                    break;
                case 0x18 :    /* get "Cancel/Abort" */
                    cmd_buf[0] = '\0';
                    cmd_buf_head = 0;
                    input_ret = INPUT_CANCEL;
                    break;
                case 0x1B :    /* get "ESC"*/
                    input_ret = INPUT_ESC;
                default :
                    break;
            }
        }
    }

    return input_ret;
}


/*----------------------------------------------------*/
/*! \fn polling_uart_cmd(void)
\brief get data from uart
\param N/A

\retval N/A

This function is called periodly to get the data from
the uart.
*/
void polling_uart_cmd(void)
{
    int8_t byt = 0, print_buf[131] = "   ";
    uint8_t temp_cmd_head_cnt = 0;
    INPUT_Status cmd_stat = INPUT_BUSY;

    switch (spec_cmd_stat)
    {
        case 1 :
            if (byt = getck())
            {/* next stage of special cmmd */
                spec_cmd_stat = 2;
                if ('[' != byt)
                {
                    spec_cmd_stat = 4;
                }
            }
            break;
        case 2 :
            if (byt = getck())
            {/* next stage of special cmmd */
                spec_cmd_stat = 3;
                switch (byt)
                {
                    case 'A' :    /* key "up" */
                        if (0 == cmd_log_up_cnt || 0 == cmd_log_count)
                        {
                            printfk("%c", 0x7);    /* alarm for no valid log */
                            break;
                        }
                        else
                        {/* re-trace terminal */
                            if (cmd_log_count == cmd_log_up_cnt)
                            {/* save the latest cmd buf */
                                strcpyk(cmd_buf_latest, cmd_buf);
                            }
                            for (temp_cmd_head_cnt = 0; temp_cmd_head_cnt < cmd_buf_head + 3; temp_cmd_head_cnt ++ )
                            {
                                print_buf[temp_cmd_head_cnt] = ' ';
                            }
                            print_buf[temp_cmd_head_cnt] = '\0';
                            printfk("\r%s\r->", print_buf);
                            strcpyk(cmd_buf, cmd_log_arry[cmd_log_up_cnt % UART_CMD_LOG_NUM]);
                            cmd_log_up_cnt --;
                            cmd_buf_head = strnlenk(cmd_buf, 128);
                            printfk("%s", cmd_buf);
                        }
                        break;
                    case 'B' :    /* key "down" */
                        if (cmd_log_count < cmd_log_up_cnt || 0 == cmd_log_count)
                        {
                            printfk("%c", 0x7);    /* alarm for no valid log */
                            break;
                        }
                        else
                        {/* re-trace terminal */
                            for (temp_cmd_head_cnt = 0; temp_cmd_head_cnt < cmd_buf_head + 3; temp_cmd_head_cnt ++ )
                            {
                                print_buf[temp_cmd_head_cnt] = ' ';
                            }
                            print_buf[temp_cmd_head_cnt] = '\0';
                            printfk("\r%s\r->", print_buf);
                            if (cmd_log_count > cmd_log_up_cnt)
                            {/* reload the latest cmd buf */
                                strcpyk(cmd_buf, cmd_log_arry[cmd_log_up_cnt % UART_CMD_LOG_NUM]);
                            }
                            else
                            {
                                strcpyk(cmd_buf, cmd_buf_latest);
                            }
                            cmd_log_up_cnt ++;
                            cmd_buf_head = strnlenk(cmd_buf, 128);
                            printfk("%s", cmd_buf);
                        }
                        break;
                    default :
                        printfk("%c", 0x7);    /* alarm for no valid key */
                        break;
                }
            }
            break;
        default :
            cmd_stat = scan_input();
            break;
    }
    switch (cmd_stat)
    {
        case INPUT_BUSY :
        case INPUT_EMPTY :
            break;
        case INPUT_COMPLETE :
            resolve_cmd(cmd_buf);
            spec_cmd_stat = 3;    /* indicate that command not in special mode */
            cmd_log_up_cnt = cmd_log_count;    /* not command log reload */
            break;
        case INPUT_CANCEL :
            break;
        case INPUT_ESC :
            spec_cmd_stat = 1;
            break;
        case INPUT_ERROR :
            break;
        case INPUT_TIMEOUT :
            break;
        default :
            break;
    }
}



/*----------------------------------------------------*/
/*! \fn resolve_cmd_buf(int8_t * cmd_str)
\brief 
\param cmd_str -- Pass a ptr for command string.

\retval 

Resolve the command.
*/
uint32_t1 resolve_cmd_buf(int8_t1 * cmd_str)
{
    uint32_t param_num = 0, ch_cnt = 0, sub_ch_cnt = 0, blank_cnt = 0;

    if ('\0' == cmd_str[0])
    {
        return 0;
    }
    while (cmd_str[blank_cnt] == ' ')
    {/* ignore front blanks */
        blank_cnt++;
        if ('\0' == cmd_str[blank_cnt])
        {/* no effect command */
            return 0;
        }
    }
    for (ch_cnt = blank_cnt; cmd_str[ch_cnt] != '\0'; ch_cnt ++)
    {/* get real command charaters */
        if ((' ' == cmd_str[ch_cnt]) || (',' == cmd_str[ch_cnt]) || \
                ('(' == cmd_str[ch_cnt]) || (')' == cmd_str[ch_cnt]))
        {/* 'space' character */
            if((' ' == cmd_str[ch_cnt + 1]) || (',' == cmd_str[ch_cnt + 1]) || \
                ('(' == cmd_str[ch_cnt + 1]) || (')' == cmd_str[ch_cnt + 1]))
            {/* duplex blanks, ignore it */
                ;
            }
            else if ('\0' == cmd_str[ch_cnt + 1])
            {/* end of cmd buffer, ignore it */
                ;
            }
            else
            {/* end of one param */
                cmd_param_arry[param_num][sub_ch_cnt] = '\0';
                param_num ++;
                sub_ch_cnt = 0;
                if (!(UART_PARA_NUM_MAX  > param_num))
                {
                    param_num = UART_PARA_NUM_MAX - 1;
                }
            }
        }

        else
        {
            cmd_param_arry[param_num][sub_ch_cnt] = cmd_str[ch_cnt];
            sub_ch_cnt ++;
            if (!(UART_PARA_LEN_MAX  > sub_ch_cnt))
                {
                    sub_ch_cnt = UART_PARA_LEN_MAX - 1;
                }
        }
    }
    cmd_param_arry[param_num][sub_ch_cnt] = '\0';

    return (param_num + 1);
}


/*----------------------------------------------------*/
/*! \fn strcmpk(const int8_t * str1, const int8_t * str2)
\brief compare two string
\param str1 -- a string

\param str2 -- a string

\retval -1 -- the two string are different
\retval 0 -- the two string are same

*/
int32_t1 strcmpk(const int8_t1 * str1, const int8_t1 * str2)
{
    uint32_t cnt = 0;
    for (;((str1[cnt] != '\0') && (str2[cnt] != '\0'))  ; cnt ++)
    {
        if (str1[cnt] > str2[cnt])
        {
            return 1;
        }
        else if (str1[cnt] < str2[cnt])
        {
            return -1;
        }
        else
        {}
    }
    /* last charater compare */
    if (str1[cnt] > str2[cnt])
    {
        return 1;
    }
    else if (str1[cnt] < str2[cnt])
    {
        return -1;
    }
    else
    {
        return 0;
    }

}


/*----------------------------------------------------*/
/*! \fn strcpyk(int8_t * dest_str, const int8_t * src_str)
\brief copy string.
\param dest_str -- the destination address
\param src_str -- the source address

\retval N/A


*/
void strcpyk(int8_t1 * dest_str, const int8_t1 * src_str)
{
    uint32_t temp_cnt = 0;
    for (temp_cnt = 0; src_str[temp_cnt] != '\0'; temp_cnt ++)
    {
        dest_str[temp_cnt] = src_str[temp_cnt];
    }
    dest_str[temp_cnt] = '\0';
}

/*----------------------------------------------------*/
/*! \fn resolve_cmd(int8_t * cmd_str)
\brief resolve the command string get from shell
\param cmd_str -- the string of command

\retval N/A

Resolve the command string, according to the cmmond call
execute the cmmond.
*/
void resolve_cmd(int8_t1 * cmd_str)
{
    uint32_t param_num = 0;
    uint32_t datashow = 0;
    int32_t real_param1 = 0, real_param2 = 0, real_param3 = 0, real_param4 = 0;
    static int32_t last_addr = 0;
    static int32_t last_unit_num = 64;
    static int32_t last_unit_width = 2;
    SYM_TBL *sym_Tbl = NULL;

    if ('\0' == cmd_str[0])
    {
        printfk("\r\n->");
    }
    else
    {
        if ((spec_cmd_stat == 4) && (strcmp(cmd_log_arry[cmd_log_count % UART_CMD_LOG_NUM], cmd_str)))
        {/* save new command line to cmd log */
            cmd_log_count ++;
            strcpyk(cmd_log_arry[cmd_log_count % UART_CMD_LOG_NUM], cmd_str); 
        }
        param_num = resolve_cmd_buf(cmd_str);
        if (!strcmp(cmd_param_arry[0], "d"))
        {
            switch (param_num)
            {
                case 4 :
                    real_param3 = atoik(cmd_param_arry[3]);
                    last_unit_width = real_param3;
                    last_addr = real_param1 + (real_param3 * real_param2);
                case 3 :
                    real_param3 = last_unit_width;
                    real_param2 = atoik(cmd_param_arry[2]);
                    last_unit_num = real_param2;
                    last_addr = real_param1 + (real_param3 * real_param2);
                case 2 :
                    real_param3 = last_unit_width;
                    real_param2 = last_unit_num;
                    real_param1 = atoik(cmd_param_arry[1]);
                    last_addr = real_param1 + (real_param3 * real_param2);
                    break;
                case 1 :
                    real_param3 = last_unit_width;
                    real_param2 = last_unit_num;
                    real_param1 = last_addr;
                    last_addr = real_param1 + (real_param3 * real_param2);
                    break;
                default :
                    break;
            }
            d((uint32_t)real_param1, (uint16_t)real_param2, (uint16_t)real_param3, 0);
            printfk("\r\n->");
            
            return;
        }

        for(sym_Tbl = &symTbl[0]; sym_Tbl->sym_str; sym_Tbl++)
        {
            if(!strcmp(cmd_param_arry[0], sym_Tbl->sym_str))
            {
                if(param_num != sym_Tbl->func_type + 1)
                {
                    printfk("\r\n Parameters are wrong, enter [h] for help");
                    printfk("\r\n->");
                    return;
                }
                printfk("\r\n");
                real_param4 = atoik(cmd_param_arry[4]);
                real_param3 = atoik(cmd_param_arry[3]);
                real_param2 = atoik(cmd_param_arry[2]);
                real_param1 = atoik(cmd_param_arry[1]);
                switch(sym_Tbl->func_type)
                {
                    case 0:
                        (*(sym_Tbl->func_ptr))(0, 0, 0, 0);
                        printfk("\r\n->");
                        return;
                    case 1:
                        (*(sym_Tbl->func_ptr))(real_param1, 0, 0, 0);
                        printfk("\r\n->");
                        return;
                    case 2:
                        (*(sym_Tbl->func_ptr))(real_param1, real_param2, 0, 0);
                        printfk("\r\n->");
                        return;
                    case 3:
                        (*(sym_Tbl->func_ptr))(real_param1, real_param2, real_param3, 0);
                        printfk("\r\n->");
                        return;
                    case 4:
                        (*(sym_Tbl->func_ptr))(real_param1, real_param2, real_param3, real_param4);
                        printfk("\r\n->");
                        return;
                    default:
                        break;
                }
            }
        }
        if (param_num)
        {
            if (0x27 == cmd_param_arry[0][0] && 0x27 == cmd_param_arry[0][2])
            {
                datashow = cmd_param_arry[0][1];
            }
            else
                datashow = atoik(cmd_param_arry[0]);
        
            if (1 == param_num && datashow)
            {    
                printfk("\r\n  Value = %#x, %d",
                    datashow, datashow);
                if(datashow < 0x5 || (datashow > 0xd && datashow < 0x80))
                    printfk(", '%c'", datashow);
            }
            else
                printfk("\r\n Unresolved command or experssion");
        }
        printfk("\r\n->");
    }
}

/*----------------------------------------------------*/
/*!\fn program_bar(unsigned int percent)
\brief percent bar
\param percent -- The guage data, should be between 0~100.

\retval

This function is called for printing guage.
*/
void program_bar(unsigned int percent)
{
    char bar[52] = {
                    "          "
                    "          "
                    "          "
                    "          "
                    "            "};
    int i = 0;
    static unsigned int percent_pre = 255;
    
    if (percent == percent_pre)
    {
        return;
    }
    percent_pre = percent;
    
    if(percent > 100)
        percent = 100;

    for(i = 0; i < percent/2; i++)
        bar[i] = '=';
    bar[i] = '>';
    bar[51] = '\0';
    if(100 == percent)
        bar[i] = '|';
    printfk("\r[|%s %3d%%]", bar, percent);
}

#endif

/*----------------------------------------------------*/
/*!\fn TSK_UartCmd()
\brief Task for shell

\param N/A

\retval

*/
void TSK_UartCmd()
{

#ifdef INCLUDE_SHELL
#ifdef SPM_DEBUG

    ioInit();
    while(1)
    {
        sem_BinaryP(&SEM_200Hz, SEM_FOREVER);
        polling_uart_cmd();
    }
    
#endif
#endif
}

