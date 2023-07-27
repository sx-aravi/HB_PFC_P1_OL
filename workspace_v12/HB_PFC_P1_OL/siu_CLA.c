/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file siu_CLA.c
* @brief perform initialization of CLA for the F2837xD
*
*
* @author Sarinx LLC
*******************************************************************************/

#include <siu_CLA.h>
#include <cam_Regulators.h>


/**********************************************************************
* Function: siu_InitCla()
*
* Description: Initializes CLA for the F2837xD
**********************************************************************/
void siu_InitCla(void)
{
#ifdef _FLASH
//--- Copy the CLA program code from its load address to the CLA program
//--- memory (using memcpy() from RTS library).
    memcpy(&Cla1ProgRunStart, &Cla1ProgLoadStart, (uint32_t)&Cla1ProgLoadSize);
#endif

//--- Memory Configuration - Master CPU and CLA Select
    MemCfg_setLSRAMMasterSel(MEMCFG_SECT_LS0, MEMCFG_LSRAMMASTER_CPU_CLA1);     // Shared between the CPU and the CLA
    MemCfg_setLSRAMMasterSel(MEMCFG_SECT_LS1, MEMCFG_LSRAMMASTER_CPU_CLA1);     // Shared between the CPU and the CLA
    MemCfg_setLSRAMMasterSel(MEMCFG_SECT_LS2, MEMCFG_LSRAMMASTER_CPU_CLA1);     // Shared between the CPU and the CLA
    MemCfg_setLSRAMMasterSel(MEMCFG_SECT_LS3, MEMCFG_LSRAMMASTER_CPU_ONLY);     // Dedicated to the CPU
    MemCfg_setLSRAMMasterSel(MEMCFG_SECT_LS4, MEMCFG_LSRAMMASTER_CPU_CLA1);     // Shared between the CPU and the CLA
    MemCfg_setLSRAMMasterSel(MEMCFG_SECT_LS5, MEMCFG_LSRAMMASTER_CPU_ONLY);     // Dedicated to the CPU

//--- Memory Configuration - CLA Data Memory and CLA Program Memory Select
    MemCfg_setCLAMemType(MEMCFG_SECT_LS0, MEMCFG_CLA_MEM_DATA);     // Configured as CLA data memory
    MemCfg_setCLAMemType(MEMCFG_SECT_LS1, MEMCFG_CLA_MEM_DATA);     // Configured as CLA data memory
    MemCfg_setCLAMemType(MEMCFG_SECT_LS2, MEMCFG_CLA_MEM_DATA);     // Configured as CLA data memory
    MemCfg_setCLAMemType(MEMCFG_SECT_LS3, MEMCFG_CLA_MEM_DATA);     // Configured as CLA data memory
    MemCfg_setCLAMemType(MEMCFG_SECT_LS4, MEMCFG_CLA_MEM_PROGRAM);  // Configured as CLA program memory
    MemCfg_setCLAMemType(MEMCFG_SECT_LS5, MEMCFG_CLA_MEM_DATA);     // Configured as CLA data memory

//--- Initialize CLA task interrupt vectors
//*******************************************************************************************************//
//*** Note: suppressing compiler #770-D "conversion from pointer to smaller integer" warning.         ***//
//*** The CLA address range is 16 bits and the addresses passed to the MVECT registers will be in the ***//
//*** lower 64Kw address space. The warning is turned back on after the MVECTs are assigned addresses ***//
//*******************************************************************************************************//
#pragma diag_suppress=770
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_1, (uint16_t)&Cla1Task1);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_2, (uint16_t)&Cla1Task2);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_3, (uint16_t)&Cla1Task3);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_4, (uint16_t)&Cla1Task4);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_5, (uint16_t)&Cla1Task5);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_6, (uint16_t)&Cla1Task6);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_7, (uint16_t)&Cla1Task7);
    CLA_mapTaskVector(CLA1_BASE, CLA_MVECT_8, (uint16_t)&Cla1Task8);
#pragma diag_warning=770

//--- Select Task interrupt source
    CLA_setTriggerSource(CLA_TASK_1, CLA_TRIGGER_ADCB1);        // ADCAINT1 is trigger source for Task1
    CLA_setTriggerSource(CLA_TASK_8, CLA_TRIGGER_SOFTWARE);     // Software (none) is trigger source for Task8

//--- Enable use of software to start a task (IACK)
    CLA_enableIACK(CLA1_BASE);                  // Enable IACKE to start task using software

//--- Force one-time initialization Task 8 - zero delay buffer
    CLA_enableTasks(CLA1_BASE, CLA_TASKFLAG_8);                 // Enable CLA task interrupt 8
    asm("  IACK  #0x0080");                                     // IACK - CLA task force instruction
    asm("  RPT #3 || NOP");                                     // Wait at least 4 cycles
    while(CLA_getTaskRunStatus(CLA1_BASE, CLA_TASK_8) == 1);    // Loop until task completes
    CLA_disableTasks(CLA1_BASE, CLA_TASKFLAG_8);                // Disable CLA task interrupt 8

//--- Enable CLA task interrupt 1
    CLA_enableTasks(CLA1_BASE, CLA_TASKFLAG_1);                 // Enable CLA interrupt 1

//--- Enable CLA1_1 interrupt
    Interrupt_register(INT_CLA1_1, &cla1Isr1);    // Re-map CLA1 Task1 interrupt signal to call the ISR function
    Interrupt_enable(INT_CLA1_1);                 // Enable CLA1 Task1 in PIE group 11 and enable INT11 in IER to enable PIE group 11

} // end of siu_InitCla()



//
// CLA_initCpu1Cla1 - Setup task vectors and end of task interrupt handlers
//
void CLA_initCpu1Cla1(void)
{
    //
    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //
    EALLOW;
#pragma diag_suppress=770
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
    Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
    Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
    Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
    Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);
#pragma diag_warning=770
    //
    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    //
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all     = 0x00FF;

    //
    // Configure the vectors for the end-of-task interrupt for all
    // 8 tasks
    //
    PieVectTable.CLA1_1_INT   = &cla1Isr1;
    PieVectTable.CLA1_2_INT   = &cla1Isr2;
    PieVectTable.CLA1_3_INT   = &cla1Isr3;
    PieVectTable.CLA1_4_INT   = &cla1Isr4;
    PieVectTable.CLA1_5_INT   = &cla1Isr5;
    PieVectTable.CLA1_6_INT   = &cla1Isr6;
    PieVectTable.CLA1_7_INT   = &cla1Isr7;
    PieVectTable.CLA1_8_INT   = &cla1Isr8;

    //
    // Enable CLA interrupts at the group and subgroup levels
    //
    PieCtrlRegs.PIEIER11.all  = 0xFFFF;
    IER |= (M_INT11 );
}



//
// cla1Isr1 - CLA1 ISR 1
//

__interrupt void cla1Isr1 ()
{
    //
    // Acknowledge the end-of-task interrupt for task 1
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP11);


    //
    // Uncomment to halt debugger and stop here
    //
    // asm(" ESTOP0");
}

//
// cla1Isr2 - CLA1 ISR 2
//
__interrupt void cla1Isr2 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr3 - CLA1 ISR 3
//
__interrupt void cla1Isr3 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr4 - CLA1 ISR 4
//
__interrupt void cla1Isr4 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr5 - CLA1 ISR 5
//
__interrupt void cla1Isr5 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr6 - CLA1 ISR 6
//
__interrupt void cla1Isr6 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr7 - CLA1 ISR 7
//
__interrupt void cla1Isr7 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr8 - CLA1 ISR 8
//
__interrupt void cla1Isr8 ()
{
    //
    // Acknowledge the end-of-task interrupt for task 8
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP11);
    //
    // Uncomment to halt debugger and stop here
    //
//    asm(" ESTOP0");
}


// End of file


