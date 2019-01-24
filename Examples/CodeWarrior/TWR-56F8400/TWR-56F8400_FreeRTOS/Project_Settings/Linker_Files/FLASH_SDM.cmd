## ###################################################################
##
##     This component module is generated by Processor Expert. Do not modify it.
##
##     Filename  : FLASH_SDM.cmd
##
##     Project   : ProcessorExpert
##
##     Processor : MC56F84789VLL
##
##     Compiler  : CodeWarrior DSP C Compiler
##
##     Date/Time : 2019-01-24, 21:16, # CodeGen: 10
##
##     Abstract  :
##
##     This file is used by the linker. It describes files to be linked,
##     memory ranges, stack size, etc. For detailed description about linker
##     command files see CodeWarrior documentation. This file is generated by default.
##     You can switch off generation by setting the property "Generate linker file = no"
##     in the "Build options" tab of the CPU bean and then modify this file as needed.
##
##
## ###################################################################

MEMORY {
        # I/O registers area for on-chip peripherals
        .x_Peripherals (RW)   : ORIGIN = 0xC000, LENGTH = 0

        # List of all sections specified in the "Build options" tab
        .p_Interrupts  (RWX) : ORIGIN = 0x00000000, LENGTH = 0x000000DE
        .p_Code  (RWX) : ORIGIN = 0x00000208, LENGTH = 0x0001FDF8
        .x_Data  (RW) : ORIGIN = 0x00000000, LENGTH = 0x00004000
        .p_reserved_FCF  (RWX) : ORIGIN = 0x00000200, LENGTH = 0x00000008


        # p_flash_ROM_data mirrors x_Data, mapping to origin and length
        # the "X" flag in "RX" tells the debugger flash p-memory.
        # the p-memory flash is directed to the address determined by AT
        # in the data_in_p_flash_ROM section definition
        .p_flash_ROM_data  (RX) : ORIGIN = 0x00000000, LENGTH = 0x00004000

        #Other memory segments
        .x_internal_ROM  (RW)  : ORIGIN = 0x00008000, LENGTH = 0x00004000
        .p_internal_RAM  (RWX) : ORIGIN = 0x0060000, LENGTH = 0x4000

}

KEEP_SECTION { interrupt_vectors.text }
KEEP_SECTION { reserved_FCF.text }

SECTIONS {

        .interrupt_vectors :
        {
          F_vector_addr = .;
          # interrupt vectors
          * (interrupt_vectors.text)
        } > .p_Interrupts

        .reserved_FCF :
        {
          F_FCF_addr = .;
          # reserved FCF - Flash Configuration Field
          * (reserved_FCF.text)
        } > .p_reserved_FCF

        .ApplicationCode :
        {

              F_Pcode_start_addr = .;

              # .text sections
              * (.text)
              * (rtlib.text)
              * (startup.text)
              * (fp_engine.text)
              * (ll_engine.text)
              * (user.text)
              * (.data.pmem)

              F_Pcode_end_addr = .;

              # save address where for the data start in pROM
              . = ALIGN(2);
              __pROM_data_start = .;

        } > .p_Code

        # AT sets the download address
        # the download stashes the data just after the program code
        # as determined by our saved pROM data start variable

        .data_in_p_flash_ROM : AT(__pROM_data_start)
        {
              # the data sections flashed to pROM
              # save data start so we can copy data later to xRAM

              __xRAM_data_start = .;

              # .data sections
              * (.const.data.char)     # used if "Emit Separate Char Data Section" enabled
              * (.const.data)

              * (fp_state.data)
              * (rtlib.data)
              * (.data.char)        # used if "Emit Separate Char Data Section" enabled
              * (.data)

              # save data end and calculate data block size
              . = ALIGN(2);
              __xRAM_data_end = .;
              __data_size = __xRAM_data_end - __xRAM_data_start;

        } > .p_flash_ROM_data          # this section is designated as p-memory
                                       # with X flag in the memory map
                                       # the start address and length map to
                                       # actual internal xRAM

        .ApplicationData :
        {
              # save space for the pROM data copy
              . = __xRAM_data_start + __data_size;

              # .bss sections
              * (rtlib.bss.lo)
              * (rtlib.bss)
              . = ALIGN(4);
              F_Xbss_start_addr = .;
              _START_BSS = .;
              * (.bss.char)         # used if "Emit Separate Char Data Section" enabled
              * (.bss)
              . = ALIGN(2);         # used to ensure proper functionality of the zeroBSS hardware loop utility
              _END_BSS   = .;
              F_Xbss_length = _END_BSS - _START_BSS;

              /* Setup the HEAP address */
              . = ALIGN(4);
              _HEAP_ADDR = .;
              _HEAP_SIZE = 0x00000100;
              _HEAP_END = _HEAP_ADDR + _HEAP_SIZE;
              . = _HEAP_END;

              /* SETUP the STACK address */
              _min_stack_size = 0x00000200;
              _stack_addr = _HEAP_END;
              _stack_end  = _stack_addr + _min_stack_size;
              . = _stack_end;

              /* EXPORT HEAP and STACK runtime to libraries */
              F_heap_addr   = _HEAP_ADDR;
              F_heap_end    = _HEAP_END;
              F_Lstack_addr = _HEAP_END;
              F_StackAddr = _HEAP_END;
              F_StackEndAddr = _stack_end - 1;

              # runtime code __init_sections uses these globals:

              F_Ldata_size     = __data_size;
              F_Ldata_RAM_addr = __xRAM_data_start;
              F_Ldata_ROM_addr = __pROM_data_start;

              F_Livt_size     = 0x0000;
              F_Livt_RAM_addr = 0x0000;
              F_Livt_ROM_addr = 0x0000;

              F_xROM_to_xRAM   = 0x0000;
              F_pROM_to_xRAM   = 0x0001;
              F_pROM_to_pRAM   = 0x0000;

              F_start_bss   = _START_BSS;
              F_end_bss     = _END_BSS;

              __DATA_END=.;

        } > .x_Data

}
