


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
;SetTextOutColor PROC C, 
;	color:DWORD
;
; Sets the text colors and clears the console
; window. Calls Irvine32 library functions.
;---------------------------------------------
;	mov	eax,color
;	call	SetTextColor
;	call	Clrscr
;	ret
;SetTextOutColor ENDP

;---------------------------------------------
;DisplayTable PROC C
;
; Inputs an integer n and displays a
; multiplication table ranging from n * 2^1
; to n * 2^10.
;----------------------------------------------
;	INVOKE askForInteger	; call C++ function
;	mov	intVal,eax            	; save the integer
;	mov	ecx,ENDING_POWER       	; loop counter
;
;L1:	push ecx                    	; save loop counter
;	shl  intVal,1               	; multiply by 2
;	INVOKE showInt,intVal,OUT_WIDTH
;	call	Crlf
;	pop	ecx	; restore loop counter
;    loop	L1
;
;	ret
;DisplayTable ENDP

Blur PROC C,
	im1: PTR BYTE,
	w: DWORD,
	h: DWORD,

	
	mov edx,im1		; Pointer of image.
	mov ecx,h		; height of image for outer loop.
		

	outerLoop:
		push ecx
		xor ecx,ecx
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
	
	xor eax,eax
	
	ret

Blur ENDP

Exposure PROC C,  
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
		xor eax,eax
		ret

Exposure ENDP

Contrast PROC C,  
im1: PTR BYTE,
w: DWORD,
h: DWORD,
val: DWORD
		mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0
		xor esi, esi
		mov esi, im1
		mov eax, h
		mov ebx, w
		mul ebx;
		mov ecx, eax
		mov eax, h
		mov edx, val
loopContr:
		mov eax,0
		mov ebx,0
		mov eax, edx; value of contrast assigned to eax.
		mov bl, [esi]
		mul bl; current pixel value multiplied by eax.
		shr eax, 6; after multiplying result is divided by 64.
		cmp eax, 255; if eax value not greater than 255
		jna regularContr; jump regularContr otherwise value is
		mov BYTE PTR[esi], 255; set up to maximum value which means 255.
		jmp doneContr
regularContr :
	mov BYTE PTR[esi], al; result is assigned to current pixel value.
doneContr:
		inc esi; address is increased.
		Loop loopContr
		xor eax,eax
		ret
		
Contrast ENDP

Clear PROC C,
	im1: PTR BYTE,
	w: DWORD,
	h: DWORD,
	val: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi,esi
	mov esi, im1
	mov eax, h; height
	mov ebx, w; width
	mul ebx
	mov ecx, eax; pixel numbers
	mov eax, h
	mov edx, val;our setting
loopClear :
		mov[esi], dl; 
		inc esi; 
		Loop loopClear
		ret
Clear ENDP

Invert PROC C,
im1: PTR BYTE,
w: DWORD,
h: DWORD
	    mov eax, 0
		mov ebx, 0
		mov ecx, 0
		mov edx, 0
		xor esi, esi
		mov esi, im1
		mov eax, h
		mov ebx, w
		mul ebx; 
		mov ecx,eax
		mov eax,h
loopInvert :
	    neg BYTE PTR[esi]; 
		inc esi; to 255 - pixel.
		Loop loopInvert
		ret
Invert ENDP

Edge PROC C,
im1: PTR BYTE,
w: DWORD,
h: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	mov esi, im1
	mov eax, h
 	mov ebx, w
    sub eax, 1 
	mul ebx
	mov ecx, eax
	sub ecx,172
loopEdge :
		mov eax, 0
		mov al,[esi]
		add esi,ebx
		mov dl,[esi]
		sub esi,ebx
		cmp al,dl
		jnae pixelWhite; image[i] is not greater or equal to image[i + width]
		mov BYTE PTR[esi], 0; set them black
		inc esi; do it for every pixel
		Loop loopEdge
pixelWhite :
		mov BYTE PTR[esi], 255; assing 255 white
		inc esi; address increments
			Loop loopEdge 
		mov ecx,w
		add ecx,172
pixelTop:
		mov BYTE PTR[esi], 0; if current pixel is reached to top
		inc esi; set to 255. And do it every pixel
		Loop pixelTop
	ret
Edge ENDP

isEmpty PROC C,
im1: PTR BYTE,
	w: DWORD,
	h: DWORD
	mov eax, h
	mov ebx, w
	mul ebx
	mov ecx, eax
	dec ecx
	mov esi, im1
	add esi,ecx
	mov eax, [esi]
	ret 
	isEmpty ENDP
Grey PROC C,
	im1: PTR BYTE,
	im2: PTR BYTE,
	w: DWORD,
	h: DWORD,
	
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	xor edi, edi
	mov eax, h
	mov ebx, w
	mov ecx, h
	mov edi,im1
	mov esi,im2
L1:
	push ecx
	mov ecx, w
L2:
	mov al,BYTE PTR[esi]
	mov BYTE PTR[edi],al
	inc esi
	inc edi
	Loop L2
	pop ecx
	Loop L1
	ret 
Grey ENDP

Transpose PROC C,
im1: PTR BYTE,
im2: PTR BYTE,
	w: DWORD,
	h: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	xor edi, edi
	mov eax, h
	mov ebx, w
	mov ecx, h
	mov esi,im1
	mov edi,im2
Outter:
	push ecx
	mov edx,0
	mov ecx, w
	mov eax, w
Inner:
	mov bl,[esi]
	mov BYTE PTR[edi], 0
	mov BYTE PTR[edi],bl
	add edi, eax
	inc esi
	Loop Inner
	pop ecx
	push ecx
	mov ecx, w

L:
	sub edi,eax
	Loop L
	pop ecx
	inc edi
	Loop Outter

	ret
Transpose ENDP

shrink PROC C,
im1: PTR BYTE,
im2: PTR BYTE,
	w: DWORD,
	h: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	xor edi, edi
	mov eax, h
	mov ebx, w
	shr eax, 1
	shr ebx, 1
	mov ecx, eax
	mov esi,im1
	mov edi,im2
loopShrink1:
		push ecx
		mov ecx, ebx
loopShrink2 :
		mov dl, [esi]
		mov BYTE PTR[edi], dl; the value of current pixel is moved to ecx.
		inc esi
		inc esi; next 2 pixels of old image
		inc edi; next pixel of new image
			Loop loopShrink2
		mov ebx, w
		add esi, ebx; 
		shr ebx,1
		add edi, ebx
		pop ecx
			Loop loopShrink1

		ret
shrink ENDP

mirrorHori PROC C,
im1: PTR BYTE,
im2: PTR BYTE,
	w: DWORD,
	h: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	xor edi, edi
	mov eax, h
	mov ebx, w
	dec eax
	dec eax
	mov esi,im1
	mov edi,im2
	mov ecx, ebx
	add edi, ebx
First:
	dec edi
	mov dl,[esi]
	mov BYTE PTR[edi], dl
	inc esi
	Loop First
mov ecx, eax
mirror1:
	push ecx
	mov ecx, ebx
	add edi,ebx
	add edi,ebx
mirror2:
	dec edi
	mov dl,[esi]
	mov BYTE PTR[edi], dl
	inc esi
	Loop mirror2
	pop ecx
	Loop mirror1
mov ecx, w
add edi, ebx
dec edi
add edi, ebx
Last:
	mov dl,[esi]
	mov BYTE PTR[edi], dl
	inc esi
	dec edi
	Loop Last
	ret

mirrorHori ENDP

mirrorVert PROC C,
im1: PTR BYTE,
im2: PTR BYTE,
w: DWORD,
h: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	xor edi, edi
	mov eax, h
	mov ebx, w
	mov esi,im1
	mov edi,im2
	dec eax
	mov ecx, eax
L:
	add edi, ebx
	Loop L
	mov ecx, ebx
	dec edi
First:
	inc edi
	mov dl, [esi]
	mov BYTE PTR[edi], dl
	inc esi
	Loop First
	dec eax
	mov ecx,eax
Outter:
	push ecx
	mov ecx, ebx
	sub edi, w
	sub edi,w
Inner:
	inc edi
	mov dl, [esi]
	mov BYTE PTR[edi], dl
	inc esi
	Loop Inner
	pop ecx
	Loop Outter
	sub edi,w
	inc edi
	sub edi,w
	mov ecx,ebx
	dec esi
Last:
	inc esi
	mov dl, [esi]
	mov BYTE PTR[edi], dl
	inc edi
	Loop Last

	ret 
mirrorVert ENDP


ColorTemperature PROC C,  
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
		sub ecx,edx
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
		xor eax,eax
		ret

ColorTemperature ENDP

rotate PROC C,
im1: PTR BYTE,
im2: PTR BYTE,
w: DWORD,
h: DWORD
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	xor esi, esi
	xor edi, edi
	mov eax, h
	mov ebx, w
	mov esi,im1
	mov edi,im2
	add edi,w
	dec edi
	mov ecx,w
Outter:
	push ecx
	mov ecx, h
	dec ecx
Inner:
	mov dl,[esi]
	mov BYTE PTR[edi], dl
	inc esi
	add edi,w
	Loop Inner
	mov dl,[esi]
	mov BYTE PTR[edi], dl
	pop ecx
	inc esi
	push ecx
	mov ecx,h
	dec ecx
L:
	sub edi, w
	Loop L
	dec edi
	pop ecx
	Loop Outter
	
	ret
rotate ENDP








END