; real mode
[BITS 16]
[ORG 0x7c00]

; entry point
start:
    ; initializing segment addresses
    mov ax, cs ; code segment
    mov ds, ax ; data segment
    mov ss, ax ; stack segment
    mov sp, start ; stack pointer

; clear screen
mov ax, 0x0003
int 0x10

; input processing

xor di, di
zero_letters: ; create an array of zeros
    mov al, 0
    mov byte [0x9000 + di], al ; array of allowed letters will be at this address 0x9000
    inc di
    cmp di, 26
    jl zero_letters

; show info for user
xor di, di
mov bx, waiting
call puts
call print_letters

choice_loop: ; user add/del letters or click to enter to begin
    xor cx, cx
    mov ah, 0x00
    int 0x16 ; read keyboard
    cmp al, 0 ; if not ascii
    je choice_loop
    cmp al, 13 ; if enter
    je begin
    cmp al, 'a' ; if not letter
    jl choice_loop
    cmp al, 'z' ; same
    jg choice_loop
    sub al, 'a'
    xor ah, ah
    mov di, ax

    ;change flag
    xor byte [0x9000 + di], 1

    mov ax, 0x0003
    int 0x10
    mov bx, waiting
    call puts
    xor cx, cx
    call print_letters
    jmp choice_loop

print_letters: ; print letter or "_", check flag to choose
    mov di, cx
    mov al, byte [0x9000 + di]
    cmp al, 0
    je skip_letter
    mov ax, di
    call puts_l
    inc cx
    cmp cx, 26
    jl print_letters
end_print:
    ret

skip_letter:
    mov bx, skip_string
    call puts
    inc cx
    cmp cx, 26
    jl print_letters
    jmp end_print

puts:
    mov al, [bx]
    cmp al, 0
    je end_puts
    mov ah, 0x0e
    int 0x10
    add bx, 1
    jmp puts

puts_l:
    add al, 'a'
    mov ah, 0x0e
    int 0x10

end_puts:
    ret

; some useful data
waiting:
    db "Press ENTER to start. Active letters:", 0

skip_string:
    db "_", 0

; global descriptor table
gdt:
    ; zero descriptor
    db 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    ; code segment
    ; base=0, size=4Gb, P=1, DPL=0, S=1, Type=1, Access=00A, G=1, B=32bit
    db 0xff, 0xff, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00 
    ; data segment
    ; base=0, size=4Gb, P=1, DPL=0, S=1, Type=0, Access=0W0, G=1, B=32bit
    db 0xff, 0xff, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00

gdt_info:
    dw gdt_info - gdt ; size
    dw gdt, 0 ; address

begin:
    mov ax, 0x1000 ; kernel address
    mov es, ax 
    mov bx, 0x0000 ; initial loading position
    mov dl, 1      ; disk number 
    mov dh, 0x00   ; head number
    mov ch, 0x00   ; cylinder number
    mov cl, 0x01   ; sector number
    mov al, 59     ; number of sectors 
    mov ah, 0x02   ; reading a set number of sectors from disk to memory
    int 0x13
    
    mov ax, 0x0003
    int 0x10
    cli ; disabling interrupts
    lgdt [gdt_info] ; load gdt_info

    ; enabling the A20 address line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; switching to protected mode
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp 0x8:protected_mode

; protected mode
[BITS 32]
protected_mode:
    mov ax, 0x10
    mov es, ax
    mov ds, ax
    mov ss, ax
    call 0x10000;


times (512-($-start) - 2) db 0
db 0x55, 0xAA ; adding a boot sector signature
