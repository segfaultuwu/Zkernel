.global idt_load
.type idt_load, @function

idt_load:
    lidt (%rdi)
    ret
