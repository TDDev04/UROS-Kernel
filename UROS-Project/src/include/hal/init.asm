global irq_stub_0, irq_stub_1, irq_stub_nothing, load_idt
extern irq0, irq1, idt_r

%macro irq_stub_n 1
	cli
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, %1
    call eax
	mov al, 0x20
	out 0x20, al
    pop gs
    pop fs
    pop es
    pop ds
    popa
    iret
%endmacro

irq_stub_0: irq_stub_n irq0
irq_stub_1: irq_stub_n irq1
	
irq_stub_nothing:
	cli
    iret
	
load_idt:
	lidt [idt_r]
	ret