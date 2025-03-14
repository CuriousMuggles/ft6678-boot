/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef    __UTILIB_H__
#define    __UTILIB_H__

#include <stdarg.h>

#ifdef    __cplusplus
extern "C"{
#endif

/* ref from apps.hi.baidu.com/share/detail/32193425 */
#if 0
#ifndef va_list
typedef    char * va_list;
#endif

#ifndef    va_start
    #define    va_start(ap, v)    (ap = (va_list)&v + _INTSIZEOF(v))
    #define    va_arg(ap, t)    (*(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)))
    #define    va_end(ap)        (ap = (va_list)0)
#endif

#ifndef    _INTSIZEOF
    #define    _INTSIZEOF(n)    ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#endif
#endif

#if 1    /* prototype defined in stdin.h */
    #ifndef    uint32_t
    typedef    unsigned int    uint32_t1;
    typedef    unsigned short    uint16_t;
    typedef    unsigned char    uint8_t;
    #endif

    #ifndef    int32_t
    typedef        int            int32_t1;
    typedef        short        int16_t;
    typedef        char        int8_t1;
    #endif
#endif

#define    BIN    (2)
#define    OCT    (8)
#define    DEC    (10)
#define    HEX    (16)

#define ZEROPAD    1        /* pad with zero */
#define SIGN    2        /* unsigned/signed long */
#define PLUS    4        /* show plus */
#define SPACE    8        /* space if plus */
#define LEFT    16        /* left justified */
#define SPECIAL    32        /* 0x */
#define LARGE    64        /* use 'ABCDEF' instead of 'abcdef' */
#define    PRINT_BUFSIZE        (2048)
#define    UART_PARA_NUM_MAX    20//(8)
#define    UART_PARA_LEN_MAX    128//(64)
#define    UART_CMD_LOG_NUM    30//(10)/* max number of uart command log list */

/*-----------------------------------------------*/
/*RETURN CODES*/
/*-----------------------------------------------*/
typedef enum
{ 
  INPUT_COMPLETE = 0,
  INPUT_BUSY = 1, 
  INPUT_CANCEL,
  INPUT_EMPTY,
  INPUT_ERROR,
  INPUT_ESC,
  INPUT_TIMEOUT
}INPUT_Status;

#define    is_digit(c)    ((c) >= '0' && (c) <= '9')

extern int8_t1 cmd_buf[256];

/*-----------------------------------------------*/
/*PROTOTYPES*/
/*-----------------------------------------------*/
void SystemInit_ExtMemCtl(void);
static int32_t1 skip_atoi(const char **s);
static int32_t1 atoik(const char *s);
INPUT_Status scan_input(void);
int32_t1 vsprintfk(int8_t1 * buf, const int8_t1 * fmt, va_list args);
void printfk(const int8_t1 *fmt, ...);
int8_t1 itoak(int32_t1 num, int8_t1 *str, int32_t1 radix, int32_t1 len);
int strnlenk(const char * s, int count);
static char * number(char * str,unsigned long num, int base, int size, int precision, int type);
void TSK_UartCmd();
void polling_uart_cmd(void);
uint32_t1 resolve_cmd_buf(int8_t1 * cmd_str);
void resolve_cmd(int8_t1 * cmd_str);
int32_t1 strcmpk(const int8_t1 * str1, const int8_t1 * str2);
void strcpyk(int8_t1 * dest_str, const int8_t1 * src_str);
void program_bar(unsigned int percent);
extern char getck(void);
#ifdef    __cplusplus
}
#endif

#endif    /*__UTILIB_H__ */

