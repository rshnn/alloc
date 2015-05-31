// ROSHAN PATEL
// CS415: COMPILERS PROJECT 1
// 	 LOCAL REGISTER ALLOCATION
// Output ILOC instructions for report1.i
// Total physical register = 20
// Allocator type: t

	loadI	1020	=> r0
	loadI	4000	=> r3
	loadI	4	=> r4
	loadI	1	=> r5
	load	r3	=> r6
	add	r3 ,r4	=> r3
	load	r3	=> r7
	add	r3 ,r4	=> r3
	load	r3	=> r8
	add	r3 ,r4	=> r3
	load	r3	=> r9
	add	r3 ,r4	=> r3
	load	r3	=> r10
	add	r3 ,r4	=> r3
	load	r3	=> r11
	add	r3 ,r4	=> r3
	load	r3	=> r12
	add	r3 ,r4	=> r3
	load	r3	=> r13
	add	r3 ,r4	=> r3
	load	r3	=> r14
	add	r3 ,r4	=> r3
	load	r3	=> r15
	add	r3 ,r4	=> r3
	loadI	0	=> r4
	add	r6 ,r7	=> r3
	mult	r5 ,r7	=> r4
	sub	r15 ,r6	=> r5
	add	r3 ,r8	=> r3
	mult	r4 ,r8	=> r4
	add	r5 ,r14	=> r5
	add	r3 ,r9	=> r3
	mult	r4 ,r9	=> r4
	sub	r5 ,r13	=> r5
	add	r3 ,r10	=> r3
	mult	r4 ,r10	=> r4
	add	r5 ,r12	=> r5
	add	r3 ,r11	=> r3
	mult	r4 ,r11	=> r4
	sub	r5 ,r11	=> r6
	add	r3 ,r12	=> r3
	mult	r4 ,r12	=> r4
	add	r5 ,r10	=> r5
	loadI	16	=> r6
	rshift	r4 ,r6	=> r4
	add	r3 ,r13	=> r3
	mult	r4 ,r13	=> r4
	sub	r5 ,r9	=> r5
	add	r3 ,r14	=> r3
	mult	r4 ,r14	=> r4
	add	r5 ,r8	=> r5
	add	r3 ,r15	=> r3
	mult	r4 ,r15	=> r4
	sub	r5 ,r7	=> r5
	loadI	1024	=> r6
	store	r3	=> r6
	loadI	1028	=> r3
	store	r4	=> r3
	loadI	1032	=> r3
	store	r5	=> r3
	output	1024
	output	1028
	output	1032
