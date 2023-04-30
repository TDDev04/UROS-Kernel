[BITS 16]
[ORG 0x7C00]

%DEFINE ELF_SHF_ALLOC  0x02 ; ELFセクションフラグ
%DEFINE ELF_SHT_NOBITS 0x08 ; ELFセクションタイプ

Init:
    XOR AX, AX
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV SP, 0x7C00
    MOV BP, SP

    CLI

    JMP Main

Main:
    CALL LoadKernel
    CALL LoadSettings
    CALL EnableA20
    CALL DisableInt
    CALL SetupGDT
    JMP EnterProtectedMode
    
LoadKernel:
    MOV BYTE [BootDrive], DL ; Keep boot disk value to load boot
	MOV AH, 0x42 ; Extended read sectors function
	MOV DL, BYTE [BootDrive] ; Boot disk
	MOV SI, DAP ; Send DAP parameter (Disk Address Packet)
	INT 0x13    ; Load required sectors ahead of MBR
    JC LoadKernelFailed
    RET

LoadSettings:
    MOV BYTE [Settings], DL ; Keep boot disk value to load boot
 
    mov bl, 02h         ; Will be reading 3 sectors 
    mov di, 7E00h       ; Memory address to read sectors into

    mov dx, 1F6h        ; Head & drive # port
    mov al, [Settings] ; Drive # - hard disk 1
    and al, 0Fh         ; Head # (low nibble)
    or al, 0A0h         ; default high nibble to 'primary' drive (drive 1), 'secondary' drive (drive 2) would be hex B or 1011b
    out dx, al          ; Send head/drive #

    mov dx, 1F2h        ; Sector count port
    mov al, 03h         ; # of sectors to read
    out dx, al

    mov dx, 1F3h        ; Sector # port
    mov al, 2h          ; Sector to start reading at (sectors are 1-based)
    out dx, al

    mov dx, 1F4h        ; Cylinder low port
    xor al, al          ; Cylinder low #
    out dx, al

    mov dx, 1F5h        ; Cylinder high port
    xor al, al          ; Cylinder high #
    out dx, al

    mov dx, 1F7h        ; Command port (writing port 1F7h)
    mov al, 20h         ; Read with retry
    out dx, al

Settings_loop:
    in al, dx           ; Status register (reading port 1F7h)
    test al, 8          ; Sector buffer requires servicing
    je Settings_loop     ; Keep trying until sector buffer is ready

    mov cx, 256         ; # of words to read for 1 sector
    mov dx, 1F0h        ; Data port, reading 
    rep insw            ; Read bytes from DX port # into DI, CX # of times
    
    ;; 400ns delay - Read alternate status register
    mov dx, 3F6h
    in al, dx
    in al, dx
    in al, dx
    in al, dx

    dec bl
    mov dx, 1F7h

 load_VESA:
        mov dl, [Settings]
        jmp 0000h:7E00h

LoadKernelFailed:
    MOV SI, MsgLoadKernelFailed
    CALL Print
    CLI
    HLT

DisableInt:
    MOV AL, 0xff
    OUT 0x21, AL
    NOP
    OUT 0xa1, AL
    RET

EnableA20:
    IN AL, 0x92
    OR AL, 2
    OUT 0x92, AL
    RET

SetupGDT:
    LGDT [gdtr]
    RET

Print:
    MOV AX, 0x03
	INT 0x10

    MOV AH, 0x0e
    MOV BL, 0x07
    MOV BH, 0x00

PrintLoop:
    LODSB
    TEST AL, AL
    JZ PrintEnd
    INT 0x10
    JMP PrintLoop

PrintEnd:
    RET

EnterProtectedMode:
    MOV EAX, CR0
    OR  AL, 1
    MOV CR0, EAX

    JMP 0x8:load_elf

gdtr:
    DW gdt32 - tmp_gdt
    DD tmp_gdt

tmp_gdt:
    DQ 0x0000000000000000
    DQ 0x00CF9A000000FFFF
    DQ 0x00CF92000000FFFF
    DQ 0x0000000000000000
    DQ 0x00A09A0000000000
    DQ 0x00A0920000000000

DAP:
        DB 0x10
        DB 0x00
        DW  (end - executable) / 512 + 1
        DW 0x0000, 0x1000
        DQ 5

gdt32:
[BITS 32]
load_elf:
   MOV	AX, 0x10
   MOV	DS, AX
   MOV	ES, AX
   MOV	FS, AX
   MOV	GS, AX
   MOV	SS, AX
   MOV  ESP, 0xE00000
   MOV  EBX, 0x10000
   MOV  ESI, [EBX + 32] 
   XOR  EDX, EDX

   ADD  ESI, EBX
   ADD  ESI, 40

.one_sec:
    MOV EAX, [ESI + 8]
    TEST EAX, ELF_SHF_ALLOC
    JZ .advance
    MOV EDI, [ESI + 12] ; virtual address
    MOV ECX, [ESI + 20] ; virtual size
    TEST EDI, EDI
    JZ .advance
    CMP WORD [ESI + 4], ELF_SHT_NOBITS
    JE .no_bits
    PUSH ESI
    MOV ESI, [ESI + 16] ; section raw file offset
    ADD ESI, EBX
    CLD
    REP MOVSB
    POP ESI
    JMP .advance

.no_bits:
    XOR EAX, EAX
    REP STOSB

.advance:
    ADD ESI, 40
    INC EDX
    CMP DX, [EBX + 48]
    JNE .one_sec
    CALL DWORD [EBX + 0x18]
    HLT

MsgLoadKernelFailed: DB "Kernel Load Failed.", 0x0D, 0x0A, 0x00
BootDrive: DB 0
Settings: DB 0

TIMES 510-($-$$) DB 0
DW 0xAA55
;512 Byte

;2048 Byte
executable: INCBIN 'obj/settings.bin'

;512*64 Byte 
;executable: 
    INCBIN 'src/kernel.sys'

end:
TIMES (512*128)-($-$$) % (512*128) DB 0
%assign usedMemory ($-$$)
%assign usableMemory (512*128)
%assign payloadMemory (usedMemory/512)
%warning Payload in sector size [payloadMemory / 128]
%warning Total memory used [usedMemory] bytes. BIOS limit: [usableMemory]