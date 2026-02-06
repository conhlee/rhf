#ifndef RUNTIME_PTMF_H
#define RUNTIME_PTMF_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct __ptmf {
	long this_delta; // self-explanatory
	long v_offset;   // vtable offset
	union {
		void *f_addr;   // function address
		long ve_offset; // virtual function entry offset (of vtable)
	} f_data;
} __ptmf;

#ifdef __cplusplus
}
#endif
#endif
