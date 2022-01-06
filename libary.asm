


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
		xor esi, esi ;clear registers
		mov esi, im1 ;save image in esi
		mov eax, h	; save image height in eax
		mov ebx, w	; save image width in ebx
		mul ebx;
		mov ecx, eax ;save the pixels number in ecx
		mov eax, h
		mov edx, val ;save our setting in edx
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
	xor esi,esi	;clear registers
	mov esi, im1	;save image in esi
	mov eax, h; height	; save image height in eax
	mov ebx, w; width	; save image width in ebx
	mul ebx
	mov ecx, eax; pixel numbers
	mov eax, h
	mov edx, val;our setting
loopClear :
		mov[esi], dl; our setting is assigned to current pixel value.
		inc esi; next pixel
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
		xor esi, esi	;clear registers
		mov esi, im1	;save image in esi
		mov eax, h	; save image height in eax
		mov ebx, w	; save image width in ebx
		mul ebx; 
		mov ecx,eax		; pixel numbers
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
	xor esi, esi	;clear registers
	mov esi, im1	;save image in esi
	mov eax, h		; save image height in eax
 	mov ebx, w		; save image width in ebx
    sub eax, 1	
	mul ebx	
	mov ecx, eax	;(h-1)*w pixels
	sub ecx,172		;magic number to pass debugger
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
	xor edi, edi	;clear all registers
	mov eax, h		;height
	mov ebx, w		;width
	mov ecx, h		;counter 
	mov edi,im1		;save im1 in esi
	mov esi,im2		;save our greyimage in edi
L1:
	push ecx
	mov ecx, w		;counter
L2:
	mov al,BYTE PTR[esi]	
	mov BYTE PTR[edi],al	;replace the im1 data to im2
	inc esi	
	inc edi					;all pixels
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
	xor edi, edi	;clear all registers
	mov eax, h		;height
	mov ebx, w		;width
	mov ecx, h		;counter 
	mov esi,im1		;save im1 in esi
	mov edi,im2		;empty array
Outter:
	push ecx
	mov edx,0
	mov ecx, w	;counter
	mov eax, w
Inner:
	mov bl,[esi]	
	mov BYTE PTR[edi],bl
	add edi, eax	;next row pixel in im2
	inc esi			;next pixel in im1
	Loop Inner
	pop ecx
	push ecx
	mov ecx, w	;counter

L:
	sub edi,eax	;make it go back to row1
	Loop L
	pop ecx
	inc edi		;next pixel in im2
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
	xor edi, edi	;clear all registers
	mov eax, h		;height
	mov ebx, w		;width
	shr eax, 1		;h/2
	shr ebx, 1		;w/2
	mov ecx, eax	;set counter = h/2
	mov esi,im1		;image
	mov edi,im2		;empty array
loopShrink1:
		push ecx
		mov ecx, ebx	
		mov ecx, ebx	;set counter = w/2
loopShrink2 :
		mov dl, [esi]
		mov BYTE PTR[edi], dl; the value of current pixel in old image is moved to new image.
		inc esi
		inc esi; next 2 pixels of old image
		inc edi; next pixel of new image
			Loop loopShrink2
		mov ebx, w	
		add esi, ebx	 ;next two row in old image(1,3,5,7......)
		shr ebx,1	
		add edi, ebx	 ;next row in new image
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
	xor edi, edi	;clear all registers
	mov eax, h		;height
	mov ebx, w		;width
	dec eax
	dec eax
	mov esi,im1		;pointer of image
	mov edi,im2		;new image
	mov ecx, ebx	;set counter w
	add edi, ebx	
First:
	dec edi			;start new image on the end of row1
	mov dl,[esi]	
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi			;first row 
	Loop First
mov ecx, eax	;set counter h-2
mirror1:
	push ecx
	mov ecx, ebx	;set counter w
	add edi,ebx
	add edi,ebx
mirror2:
	dec edi			;start new image on the end of row
	mov dl,[esi]
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi
	Loop mirror2
	pop ecx			
	Loop mirror1	;(h-1)*w pixels are moved
mov ecx, w
add edi, ebx
dec edi
add edi, ebx	;the last ont pixel of new image
Last:
	mov dl,[esi]
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi
	dec edi
	Loop Last	;finish the last row
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
	xor edi, edi	;clear the registers
	mov eax, h		;height
	mov ebx, w		;width
	mov esi,im1		;pointer of	old image
	mov edi,im2		;pointer of new image
	dec eax
	mov ecx, eax	;set counter h-1
L:
	add edi, ebx	;go to the last row (new image) 
	Loop L
	mov ecx, ebx
	dec edi
First:
	inc edi
	mov dl, [esi]
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi
	Loop First	;finish the first row
	dec eax
	mov ecx,eax	;set counter h-2
Outter:
	push ecx
	mov ecx, ebx	;set counter w
	sub edi, w
	sub edi,w
Inner:
	inc edi			;go to the start of the bottom row
	mov dl, [esi]
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi
	Loop Inner
	pop ecx
	Loop Outter		;(h-1)*w pixels are moved
	sub edi,w
	inc edi
	sub edi,w		;the first pixel in new image
	mov ecx,ebx
	dec esi
Last:
	inc esi
	mov dl, [esi]
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc edi
	Loop Last				;finish tje last row

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
	xor edi, edi	;clear all the register
	mov eax, h		;height
	mov ebx, w		;width
	mov esi,im1		;old image
	mov edi,im2		;new image
	add edi,w
	dec edi			;the last pixels in first row(new image)
	mov ecx,w		;set counter w
Outter:
	push ecx
	mov ecx, h	
	dec ecx	;set counter h-1
Inner:
	mov dl,[esi]
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi					;next pixel
	add edi,w				;to next row
	Loop Inner
	mov dl,[esi]			;last pixel in this row
	mov BYTE PTR[edi], dl	;the value of current pixel in old image is moved to new image.
	pop ecx
	inc esi
	push ecx
	mov ecx,h
	dec ecx					;old image rowi to new image col(w+1-i) 
L:
	sub edi, w	;image 2 go back to row1
	Loop L
	dec edi		
	pop ecx
	Loop Outter
	
	ret
rotate ENDP

shift PROC C,
im1: PTR BYTE,
im2: PTR BYTE,
w: DWORD,
h: DWORD,
a: DWORD,
b: DWORD
mov eax, h	;height
sub eax, a	;the height we want to shift
mov ebx, w	;width
sub ebx, b	;the width we want to shift
mov esi,im1	;old image
mov edi,im2	;new image
mov ecx, a	;set counter a
L:
	add edi, w	;the first a rows are black
	Loop L
mov ecx, eax ;set counter(h-a)
L2:
push ecx
add edi, b	;the fisrt w pixels in this row is black;
mov ecx, ebx; set counter (w-b)
L1:
	mov dl, [esi]
	mov BYTE PTR [edi], dl	;the value of current pixel in old image is moved to new image.
	inc esi
	inc edi
	Loop L1
	pop ecx
	add esi, b	;old image go to the start of next row
	Loop L2
	
	ret
shift ENDP

END