[BITS 16]
[ORG 0x7E00]
[SECTION .TEXT]

main:
    PUSHA
    XOR AX, AX
    MOV ES, AX
    mov ah, 4Fh
    mov di, vbe_info_block
    int 10h

    cmp ax, 4Fh
    jne error

    mov ax, word [vbe_info_block.video_mode_pointer]
    mov [offset], ax
    mov ax, word [vbe_info_block.video_mode_pointer+2]
    mov [t_segment], ax
        
    mov fs, ax
    mov si, [offset]

    .find_mode:
        mov dx, [fs:si]
        inc si
        inc si
        mov [offset], si
        mov [mode], dx

        cmp dx, word 0FFFFh
        je end_of_modes

        mov ax, 4F01h
        mov cx, [mode]
        mov di, mode_info_block
        int 10h

        cmp ax, 4Fh
        jne error

        mov dx, [mode_info_block.x_resolution]	
        call print_hex
        mov ax, 0E20h
        int 10h

        mov dx, [mode_info_block.y_resolution]
        call print_hex
        mov ax, 0E20h
        int 10h

        xor dh, dh
        mov dl, [mode_info_block.bits_per_pixel]
        call print_hex
        mov ax, 0E0Ah
        int 10h
        mov al, 0Dh
        int 10h

        mov ax, [width]
        cmp ax, [mode_info_block.x_resolution]
        jne .next_mode

        mov ax, [height]					
        cmp ax, [mode_info_block.y_resolution]
        jne .next_mode

        mov ax, [bpp]
        cmp al, [mode_info_block.bits_per_pixel]
        jne .next_mode

        mov ax, 4F02h
        mov bx, [mode]
        or bx, 4000h
        xor di, di
        int 10h

        cmp ax, 4Fh
        jne error

        popa

        ret

    .next_mode:
        mov ax, [t_segment]
        mov fs, ax
        mov si, [offset]
        jmp .find_mode

    error:
        mov ax, 0E46h
        int 10h
        cli
        hlt

    end_of_modes:
        mov ax, 0E4Eh
        int 10h
        cli
        hlt

    print_hex:
        mov cx, 4
        .hex_loop:
            mov ax, dx
            and al, 0Fh
            mov bx, hex_to_ascii
            xlatb

            mov bx, cx
            mov [hexString+bx+1], al
            ror dx, 4
        loop .hex_loop 

        mov si, hexString
        mov ah, 0Eh
        mov cx, 6
        .loop:
            lodsb
            int 10h
        loop .loop
        ret

width: dw 800
height: dw 600
bpp: db 32
offset: dw 0
t_segment: dw 0
mode: dw 0

hexString: db '0x0000'
hex_to_ascii: db '0123456789ABCDEF'

times 1024-($-$$) db 0

vbe_info_block:
	.vbe_signature: db 'VBE2'
	.vbe_version: dw 0
	.oem_string_pointer: dd 0 
	.capabilities: dd 0
	.video_mode_pointer: dd 0
	.total_memory: dw 0
	.oem_software_rev: dw 0
	.oem_vendor_name_pointer: dd 0
	.oem_product_name_pointer: dd 0
	.oem_product_revision_pointer: dd 0
	.reserved: times 222 db 0
	.oem_data: times 256 db 0

times 1536 - ($ - $$) db 0

mode_info_block:
	.mode_attributes: dw 0
	.window_a_attributes: db 0
	.window_b_attributes: db 0
	.window_granularity: dw 0
	.window_size: dw 0
	.window_a_segment: dw 0
	.window_b_segment: dw 0
	.window_function_pointer: dd 0
	.bytes_per_scanline: dw 0

	.x_resolution: dw 0
	.y_resolution: dw 0
	.x_charsize: db 0
	.y_charsize: db 0
	.number_of_planes: db 0
	.bits_per_pixel: db 0
	.number_of_banks: db 0
	.memory_model: db 0
	.bank_size: db 0
	.number_of_image_pages: db 0
	.reserved1: db 1

	.red_mask_size: db 0
	.red_field_position: db 0
	.green_mask_size: db 0
	.green_field_position: db 0
	.blue_mask_size: db 0
	.blue_field_position: db 0
	.reserved_mask_size: db 0
	.reserved_field_position: db 0
	.direct_color_mode_info: db 0

	.physical_base_pointer: dd 0
	.reserved2: dd 0
	.reserved3: dw 0
	.linear_bytes_per_scan_line: dw 0
    .bank_number_of_image_pages: db 0
    .linear_number_of_image_pages: db 0
    .linear_red_mask_size: db 0
    .linear_red_field_position: db 0
    .linear_green_mask_size: db 0
    .linear_green_field_position: db 0
    .linear_blue_mask_size: db 0
    .linear_blue_field_position: db 0
    .linear_reserved_mask_size: db 0
    .linear_reserved_field_position: db 0
    .max_pixel_clock: dd 0

    .reserved4: times 190 db 0

TIMES 2048-($-$$) DB 0