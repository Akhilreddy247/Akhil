#include <stdio.h>

int main() {
    int reg[8] = {0}; // 8 registers
    int mem[32] = {0}; // 32 bytes of memory
    int pc = 0; // program counter
    int inst = 0; // current instruction

    // sample program with data hazards
    int program[6] = {
        0x20020001, // add r2, r0, r1
        0x20030001, // add r3, r0, r1
        0x00221820, // add r3, r1, r2
        0x20040001, // add r4, r0, r1
        0x00242820, // add r5, r1, r4
        0x0c000000  // halt
    };

    
//Explanation:
//please refer this step.

//Step 2/3
    while (inst != 0x0c000000) { // loop until halt instruction
        inst = program[pc++]; // fetch current instruction
        int opcode = inst >> 26; // extract opcode
        int rs = (inst >> 21) & 0x7; // extract first register
        int rt = (inst >> 16) & 0x7; // extract second register
        int rd = (inst >> 11) & 0x7; // extract destination register
        int imm = inst & 0xffff; // extract immediate value

        // execute instruction based on opcode
        switch (opcode) {
            case 0: // R-format
                switch (inst & 0x3f) {
                    case 0x20: // add
                        reg[rd] = reg[rs] + reg[rt];
                        break;
                    case 0x24: // and
                        reg[rd] = reg[rs] & reg[rt];
                        break;
                    case 0x25: // or
                        reg[rd] = reg[rs] | reg[rt];
                        break;
                    case 0x27: // nor
                        reg[rd] = ~(reg[rs] | reg[rt]);
                        break;
                    case 0x2a: // slt
                        reg[rd] = (reg[rs] < reg[rt]) ? 1 : 0;
                        break;
                }
                break;
            case 0x23: // lw
                reg[rt] = mem[reg[rs] + imm];
                break;
            case 0x2b: // sw
                mem[reg[rs] + imm] = reg[rt];
                break;
            case 0x04: // beq
                if (reg[rs] == reg[rt]) {
                    pc += imm;
                }
                break;
        }

        
//Explanation:
//please refer this step.


//Step 3/3

        // check for data hazards
        if (opcode == 0 && (inst & 0x3f) == 0x20) { // add instruction
            if (rd == rs) { // data hazard
                reg[rd] = reg[rt]; // forward value from rt to rd
            } else if (rd == rt) { // data hazard
                reg[rd] = reg[rs]; // forward value from rs to rd
            }
        }
    }

    // print final register values
    for (int i = 0; i < 8; i++) {
        printf("r%d = %d\n", i, reg[i]);
    }

    return 0;
}