.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

main:
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
addi $fp, $sp, 8
addi $sp, $sp, -4
lw $t0, -12($fp)
li $t0, 0
sw $t0, -12($fp)
addi $sp, $sp, -4
lw $t0, -16($fp)
addi $sp, $sp, -4
lw $t1, -20($fp)
move $t0, $t1
sw $t0, -16($fp)
sw $t1, -20($fp)
addi $sp, $sp, -4
lw $t0, -24($fp)
li $t0, 1
sw $t0, -24($fp)
addi $sp, $sp, -4
lw $t0, -28($fp)
addi $sp, $sp, -4
lw $t1, -32($fp)
move $t0, $t1
sw $t0, -28($fp)
sw $t1, -32($fp)
addi $sp, $sp, -4
lw $t0, -36($fp)
li $t0, 0
sw $t0, -36($fp)
addi $sp, $sp, -4
lw $t0, -40($fp)
addi $sp, $sp, -4
lw $t1, -44($fp)
move $t0, $t1
sw $t0, -40($fp)
sw $t1, -44($fp)
jal read
addi $sp, $sp, -4
lw $t0, -48($fp)
move $t0, $v0
sw $t0, -48($fp)
addi $sp, $sp, -4
lw $t0, -52($fp)
addi $sp, $sp, -4
lw $t1, -56($fp)
move $t0, $t1
sw $t0, -52($fp)
sw $t1, -56($fp)
addi $sp, $sp, -4
lw $t0, -60($fp)
addi $sp, $sp, -4
lw $t1, -64($fp)
move $t0, $t1
sw $t0, -60($fp)
sw $t1, -64($fp)
t4 :
addi $sp, $sp, -4
lw $t0, -68($fp)
addi $sp, $sp, -4
lw $t1, -72($fp)
move $t0, $t1
sw $t0, -68($fp)
sw $t1, -72($fp)
addi $sp, $sp, -4
lw $t0, -76($fp)
addi $sp, $sp, -4
lw $t1, -80($fp)
move $t0, $t1
sw $t0, -76($fp)
sw $t1, -80($fp)
addi $sp, $sp, -4
lw $t0, -84($fp)
addi $sp, $sp, -4
lw $t1, -88($fp)
blt $t0, $t1, t5
sw $t0, -84($fp)
sw $t1, -88($fp)
j t6
t5 :
addi $sp, $sp, -4
lw $t0, -92($fp)
addi $sp, $sp, -4
lw $t1, -96($fp)
move $t0, $t1
sw $t0, -92($fp)
sw $t1, -96($fp)
addi $sp, $sp, -4
lw $t0, -100($fp)
addi $sp, $sp, -4
lw $t1, -104($fp)
move $t0, $t1
sw $t0, -100($fp)
sw $t1, -104($fp)
addi $sp, $sp, -4
lw $t0, -108($fp)
addi $sp, $sp, -4
lw $t1, -112($fp)
addi $sp, $sp, -4
lw $t2, -116($fp)
add $t0, $t1, $t2
sw $t0, -108($fp)
sw $t1, -112($fp)
sw $t2, -116($fp)
addi $sp, $sp, -4
lw $t0, -120($fp)
addi $sp, $sp, -4
lw $t1, -124($fp)
move $t0, $t1
sw $t0, -120($fp)
sw $t1, -124($fp)
addi $sp, $sp, -4
lw $t0, -128($fp)
addi $sp, $sp, -4
lw $t1, -132($fp)
move $t0, $t1
sw $t0, -128($fp)
sw $t1, -132($fp)
lw $a0, 0($sp)
jal write
addi $sp, $sp, -4
lw $t0, -136($fp)
addi $sp, $sp, -4
lw $t1, -140($fp)
move $t0, $t1
sw $t0, -136($fp)
sw $t1, -140($fp)
addi $sp, $sp, -4
lw $t0, -144($fp)
addi $sp, $sp, -4
lw $t1, -148($fp)
move $t0, $t1
sw $t0, -144($fp)
sw $t1, -148($fp)
addi $sp, $sp, -4
lw $t0, -152($fp)
addi $sp, $sp, -4
lw $t1, -156($fp)
move $t0, $t1
sw $t0, -152($fp)
sw $t1, -156($fp)
addi $sp, $sp, -4
lw $t0, -160($fp)
addi $sp, $sp, -4
lw $t1, -164($fp)
move $t0, $t1
sw $t0, -160($fp)
sw $t1, -164($fp)
addi $sp, $sp, -4
lw $t0, -168($fp)
addi $sp, $sp, -4
lw $t1, -172($fp)
move $t0, $t1
sw $t0, -168($fp)
sw $t1, -172($fp)
addi $sp, $sp, -4
lw $t0, -176($fp)
addi $sp, $sp, -4
lw $t1, -180($fp)
move $t0, $t1
sw $t0, -176($fp)
sw $t1, -180($fp)
addi $sp, $sp, -4
lw $t0, -184($fp)
addi $sp, $sp, -4
lw $t1, -188($fp)
move $t0, $t1
sw $t0, -184($fp)
sw $t1, -188($fp)
addi $sp, $sp, -4
lw $t0, -192($fp)
li $t0, 1
sw $t0, -192($fp)
addi $sp, $sp, -4
lw $t0, -196($fp)
addi $sp, $sp, -4
lw $t1, -200($fp)
addi $sp, $sp, -4
lw $t2, -204($fp)
add $t0, $t1, $t2
sw $t0, -196($fp)
sw $t1, -200($fp)
sw $t2, -204($fp)
addi $sp, $sp, -4
lw $t0, -208($fp)
addi $sp, $sp, -4
lw $t1, -212($fp)
move $t0, $t1
sw $t0, -208($fp)
sw $t1, -212($fp)
addi $sp, $sp, -4
lw $t0, -216($fp)
addi $sp, $sp, -4
lw $t1, -220($fp)
move $t0, $t1
sw $t0, -216($fp)
sw $t1, -220($fp)
j t4
t6 :
addi $sp, $sp, -4
lw $t0, -224($fp)
li $t0, 0
sw $t0, -224($fp)
addi $sp, $fp, -8
lw $ra, 4($sp)
lw $fp, 0($sp)
addi $sp, $sp, 8
lw $t0, -224($fp)
move $v0, $t0
jr $ra
sw $t0, -224($fp)
