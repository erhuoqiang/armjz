.global asm_schedule
asm_schedule:
	stmfd r13!,{r0-r12}
	mrs r1, cpsr
	stmfd r13!,{r1}
	
	mov r1,sp
	bic r1,r1, #0xff0
	bic r1,r1, #0xf
	mov r0,sp
	str r0,[r1]
	
	bl __common_schedule
	ldr sp,[r0]
	ldmfd r13!,{r1}
	msr cpsr,r1
	ldmfd r13!,{r0-r12,r14,pc}
