


; Calls external C++ functions.

INCLUDE Irvine32.inc

.XMM



askForInteger PROTO C
loadImage PROTO C
showInt PROTO C, value:SDWORD, outWidth:DWORD

OUT_WIDTH = 8
ENDING_POWER = 10

.data
intVal DWORD ?

.code
;---------------------------------------------
SetTextOutColor PROC C, 
	color:DWORD
;
; Sets the text colors and clears the console
; window. Calls Irvine32 library functions.
;---------------------------------------------
	mov	eax,color
	call	SetTextColor
	call	Clrscr
	ret
SetTextOutColor ENDP

;---------------------------------------------
DisplayTable PROC C
;
; Inputs an integer n and displays a
; multiplication table ranging from n * 2^1
; to n * 2^10.
;----------------------------------------------
	INVOKE askForInteger	; call C++ function
	mov	intVal,eax            	; save the integer
	mov	ecx,ENDING_POWER       	; loop counter

L1:	push ecx                    	; save loop counter
	shl  intVal,1               	; multiply by 2
	INVOKE showInt,intVal,OUT_WIDTH
	call	Crlf
	pop	ecx	; restore loop counter
    loop	L1

	ret
DisplayTable ENDP




Brightness PROC C,  
	im1: PTR BYTE,
	w: DWORD,
	h: DWORD,
	val: DWORD
		mov esi,im1		; Pointer of image.
		mov ecx,w		; Width of image.
		mov eax,h		; Height of image.
		mul ecx					; size = width * height
		mov edx,val	; Value of brightness.
		
	loopBright:
		mov cl,[esi]
		add ecx,edx
		cmp ecx,255
		jg editPos
		cmp ecx,0
		jl editNeg
		mov [esi],cl
		jmp done
	editPos:

		mov BYTE PTR[esi],255
		jmp done
	editNeg:
		mov BYTE PTR[esi],0
	done:
		inc esi					; address of image is increased.
		xor ecx,ecx
		dec eax					; size is increased.
		cmp eax,0				; if size == 0 ?
		jne loopBright

		ret


Brightness ENDP


Blur PROC C,
	im1: PTR,
	w: DWORD,
	h: DWORD,
	
	mov edx,im1		; Pointer of image.
	mov ecx,h		; height of image for outer loop.
		

	outerLoop:
		push ecx
		mov ecx, w
		shr ecx,2
		innerLoop:
			xor eax,eax
			xor ebx,ebx
			mov bl,[edx]
			add eax,ebx
			mov bl,[edx + 1]
			add eax,ebx

			add edx,w

			mov bl,[edx]

			sub edx,w
			add eax,ebx

			add edx,w
			inc edx
			mov bl,[edx]
			dec edx
			sub edx,w


			add eax,ebx
			shr eax,2

			mov [edx],al
			mov [edx + 1],al

			add edx,w

			mov [edx],al

			mov [edx + 1],al

			sub edx,w
			add edx,2
		Loop innerLoop
		pop ecx
		sub ecx,1
		add edx,w
	Loop outerLoop
	ret

Blur ENDP

Foo PROC C
	vpxor   ymm0, ymm0, ymm0
	ret
Foo ENDP









END