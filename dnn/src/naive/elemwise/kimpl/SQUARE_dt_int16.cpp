// generated by gen_elemwise_kern_impls.py
#define KERN_IMPL_MODE(cb) MEGDNN_ELEMWISE_MODE_ENABLE(SQUARE, cb)
#define KERN_IMPL_ARITY    1
#define KERN_IMPL_CTYPE    dt_int16
#include "../kern_impl.inl"