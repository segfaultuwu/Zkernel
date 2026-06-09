.extern exception_handler

.macro ISR_NOERR vector
.global isr\vector
.type isr\vector, @function
isr\vector:
    pushq $0
    pushq $\vector
    jmp isr_common
.endm

.macro ISR_ERR vector
.global isr\vector
.type isr\vector, @function
isr\vector:
    pushq $\vector
    jmp isr_common
.endm

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_ERR   21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_ERR   29
ISR_ERR   30
ISR_NOERR 31

isr_common:
    /*
     * Stack:
     * [rsp + 0]  = vector
     * [rsp + 8]  = error_code
     *
     * C ABI:
     * rdi = vector
     * rsi = error_code
     */
    movq (%rsp), %rdi
    movq 8(%rsp), %rsi

    call exception_handler

    addq $16, %rsp
    iretq
