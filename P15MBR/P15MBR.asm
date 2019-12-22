BITS 16h ;blah blah blah
ORG 7c00h 	
jmp Main

msg db "Hello, and welcome to Phosphorous !",0xd,0xa,"System destruction brought to you by,",0xd,0xa,"NullException & a ton of help from Chris",0xd,0xa,"Greets to The Group",0xd,0xa,0xa,"Living monster of the night",0xd,0xa,"You made me what I am today",0xa,0xd,"You don't have to scream and shout",0xa,0xd,"In this silent place",0xa,0xd,0xa,176,177,178,"Press any key",178,177,176
msglen dw 277

;wait function used by Print
PWait:
    pusha
    mov cx, 1
	mov dx, 10
	mov ah, 86h
	int 15h
	popa
	ret 

Print: ;print length defined string (not length prefixed fuck standards)
mov bl, 50d
mov dx, [msglen]
;i forgot to comment this when i made it and it's such a mess i can't actully be bothered
    .PrintFunc:
    lodsb 
    dec dx
    jz .PrintDone 
	cmp bl, 60 ;93 = Max
	jg .RevBl
	cmp cl, 1
	je .DecBl
	inc bl
	call Beep
	
	.PrP2:
    mov ah, 0eh 
    int 10h 
	call PWait
    jmp .PrintFunc 
	
	.RevBl:
	not cl
	sub cl, 254d
	dec bl
	jmp .PrP2
	
	.DecBl:
	cmp bl, 50d
	jle .RevBl
	dec bl
	jmp .PrP2
	
    .PrintDone:
    ret ;Exit this function	


Beep:
    pusha
	;prepare PC speaker
	mov al, 182 ;Init the speaker and set the first OUT
    out 43h, al
	mov ax, 4060
	;start note
	.BeepLoop:
    out 42h, al
	mov al, ah
	out 42h, al
	in al, 61h
	or al, 3
	out 61h, al
	mov dx, 0x0060

	;Wait, don't ask me why i'm not using int 15h
	.pauseTimeB:
	mov cx, 0xFFFF
	.pauseTimeA:
	dec cx
	jnz .pauseTimeA
	dec dx
	jnz .pauseTimeB

	;Code reaches here but DOESN'T stop beeper speaker note !
	;Update: This is because of QEMU
	;Update: ok apparently they fixed it
	;Stop note and return
	in al, 61h
	and al, 11111100b
	out 61h, al
	popa
	ret
	
Main:
;Set graphics mode
	mov ax, 13h
	int 10h
;Set text color
  	mov ah, 0Bh
	mov bx, 0009h
;Print it
	mov si, msg
	call Print
;Press any key...
	xor ax, ax
	WaitLoop:
	int 16h
	or ah, ah
	jz WaitLoop
;Prep graphics
	mov ax, 0A000h
	mov es, ax ;set up a pointer to screen memory
	xor ax, ax
	mov dl, 1
;2nd payload (the epilepsy one)
	Payload:
	mov [es:di], dl
	inc dl 
	cmp dl, 64h ;Adjust color range here, h end, Interesting numbers: 64, 50, 64h 
	jg .MaxCol
	inc di
	jmp Payload
		.MaxCol:
		mov dl, 2 ;Adjust color range here, low end, Default = 2, 16h is interesting too
		jmp .PP2
 

times 510 - ($ - $$) nop
dw 0aa55h
