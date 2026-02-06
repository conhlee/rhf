#include <internal/va_list.h>

void *__va_arg(_va_list_struct *v_list, int type) {
    char *addr;
    char *reg = &(v_list->gpr);
    int g_reg = v_list->gpr;
    int maxsize = 8;
    int size = 4;
    int increment = 1;
    int even = 0;
    int fpr_offset = 0;
    int regsize = 4;

    if (type == arg_ARGREAL) {
        reg = &(v_list->fpr);
        g_reg = v_list->fpr;
        size = 8;
        fpr_offset = 32;
        regsize = 8;
    }
    if (type == arg_DOUBLEWORD) {
        size = 8;
        maxsize--;
        if (g_reg & 1) {
            even = 1;
        }
        increment = 2;
    }

    if (g_reg < maxsize) {
        g_reg += even;
        addr = v_list->reg_save_area + fpr_offset + (g_reg * regsize);
        *reg = g_reg + increment;
    }
    else {
        *reg = 8;
        addr = v_list->input_arg_area;
        addr = (char *)(((unsigned long)(addr) + (size - 1)) & ~(size - 1));
        v_list->input_arg_area = addr + size;
    }

    if (type == arg_ARGPOINTER) {
        addr = *(char **)addr;
    }

    return addr;
}
