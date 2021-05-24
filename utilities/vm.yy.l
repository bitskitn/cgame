%{
%}
%%
.align   ;
.ascii   ;
.asciiz  ;
.bss     ; // begin bss segment
.byte    ; // store bytes
.data    ; // begin data segment
.half    ; // store 16-bit values
.rodata  ; // begin rodata segment
.text    ; // begin text segment
.word    ; // store 32-bit values
add      ; // add int reg+reg
addi     ; // add int reg+imm
addu     ; // add unsigned int reg+reg
addiu    ; // add unsigned int reg+imm
and      ; // bitwise and reg
andi     ; // bitwise and imm
beq      ; // branch if equal
bgt      ; // branch if greater than
bge      ; // branch if greater than or equal
blt      ; // branch if less than
ble      ; // branch if less than or equal
bne      ; // branch if not equal
lw       ; // load word (32-bit) from address
or       ; // bitwise or reg
ori      ; // bitwise or imm
ret      ; // return
sll      ; // shift left (logical)
srl      ; // shift right (logical)
sub      ; // subtract int
sw       ; // store word (32-bit) at address
xor      ; // bitwise xor reg
xori     ; // bitwise xor imm

li       ; // load halfword imm
lui      ; // load upper halfword
sb       ; // store byte

%%

