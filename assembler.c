#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	// Simulate to registers: AX, BX, CX, DX, init values: 0
	int registers[4] = { 0, 0, 0, 0 };
	
	// Simulate to RAM
	int ram[256] = {0};

	char content[50];
	char content2[50];
	char fileContent[100][50];
	int row = 0, column = 0;

	char *fileName = malloc(sizeof(char) * 20);
	printf( "Dosya ismini giriniz: ");
	scanf("%s", fileName);
  // FILE *file;
   	 FILE *dosya = fopen(fileName, "r");

  //file=fopen(fileName, "r");
	char *letter;
	
	
	

	if(dosya == NULL) {
		printf("Dosya açýlamadý.");
		
	} else {
		printf("FILE ROWS: \n");
		
		while(! feof(dosya) ){
			char ch = fgetc(dosya);
    		
			if(ch == 10) {
				fileContent[row][column] = '\0';
    			printf("ROW-%d: %s\n", row, fileContent[row]);
    			row++;
    			column = 0;
			} else {
				fileContent[row][column] = ch;
				column++;
			}
    	}
    	
    	// For the last row
    	fileContent[row][column] = '\0';
		printf("ROW-%d: %s\n", row, fileContent[row]);
    	fileContent[row + 1][0] = NULL;
	}

	// The file is closing
    fclose(dosya);
    
    printf("\nTotal row counts on file: %d\n", row);
    
    char block[10];
    char *command = "\0";
    char *op1 = "\0";
    char *op2 = "\0";
    
    int addr1 = -1;
    int addr2 = -1;
    int fixed = 0;
    
	int i = 0, j = 0, k = 0;
    for(j=0; j<40; j++){    	
    	// Control of space
    	if(fileContent[i][j] == 32) {
    		block[k] = '\0';
			
			command = malloc(strlen(block) + 1);
			strcpy(command, block);
			
			k=0;
			block[0] = '\0';
    		
    	// Control of enter button
		} else if(fileContent[i][j] == '\0') {
			block[k] = '\0';
			
			// Command that work with only one operand
			if(command == "DEG" || command == "SS" || command == "SSD" || command == "SN" || command == "SP"){
				
			
			// Command that work with two operands
			} else {
				if(block[0] == '['){
	    			if(strlen(block) == 5){
	    				int digit1 = block[1] - 48;
						int digit2 = block[2] - 48;
						int digit3 = block[3] - 48;
						
						addr2 = (digit1 * 100) + (digit2 * 10) + digit3;
					} else if(strlen(block) == 4) {
						int digit1 = block[1] - 48;
						int digit2 = block[2] - 48;
						
						addr2 = (digit1 * 10) + digit2;
					} else {
						addr2 = block[1] - 48;
					}
					// Control to whether operand-1 fixed number or not		
				} else if(block[0] > 47 && block[0] < 58){
					fixed = atoi(block);
					
				// The operand-1 is register
				} else {
					op2 = malloc(strlen(block) + 1);
					strcpy(op2, block);
				}
			}
			
			k = 0; i++; j=-1;
			//printf("\nROW: %d, COMMAND: %s, OPERAND-1: %s, OPERAND-2: %s, ADDRESS-BLOCK-1: %d, ADDRESS-BLOCK-2: %d, FIXED: %d", i, command, op1, op2, addr1, addr2, fixed);
			calculate(registers, ram, command, op1, op2, addr1, addr2, fixed);

			command = "\0"; op1 = "\0"; op2 = "\0";
			addr1 = -1; addr2 = -1; fixed = 0;
			block[0] = '\0';
			
			if(fileContent[i][j+1] == '\0') break;
			    		
    	// Control of comma ','	
		} else if(fileContent[i][j] == 44) {
			block[k] = '\0';
			
			// Control to whether operand-1 address or not 
    		if(block[0] == '['){
    			if(strlen(block) == 5){
    				int digit1 = block[1] - 48;
					int digit2 = block[2] - 48;
					int digit3 = block[3] - 48;
					
					addr1 = (digit1 * 100) + (digit2 * 10) + digit3;
				} else if(strlen(block) == 4) {
					int digit1 = block[1] - 48;
					int digit2 = block[2] - 48;
					
					addr1 = (digit1 * 10) + digit2;
				} else {
					addr1 = block[1] - 48;
				}
				
    		// Control to whether operand-1 fixed number or not		
			} else if(block[0] > 47 && block[0] < 58){
				printf("First operand can not the fixed number !");
			
			// The operand-1 is register
			} else {
				op1 = malloc(strlen(block) + 1);
				strcpy(op1, block);
			}
			
			k = 0;
			block[0] = '\0';
		} else {
			if(fileContent[i][j] == -1) continue;
			
    		block[k] = fileContent[i][j];
    		k++;
		}
	}
 
 	printf("\n\nRAM: ");
 
 	int ind = 0;
	for(ind=0; ind < 256; ind++) {
		if(ram[ind] == 0) continue;
		
		printf("\nRAM[%d]: %d", ind, ram[ind]);
	}
	
	printf("\n\nREGISTERS:");
	
	printf("\nAX: %d", registers[0]);
    printf("\nBX: %d", registers[1]);
    printf("\nCX: %d", registers[2]);
    printf("\nDX: %d", registers[3]);
    
	return 0;
}

void calculate(int registers[], int ram[], char *command, char *op1, char *op2, int addr1, int addr2, int fixed){
	int registerInd1 = -1;
	int registerInd2 = -1;
	
	if(strcmp(op1, "X1") == 0) {
		registerInd1 = 0;	
	} else if(strcmp(op1, "X2") == 0) {
		registerInd1 = 1;
	} else if(strcmp(op1, "X3") == 0) {
		registerInd1 = 2;
	} else if(strcmp(op1, "X4") == 0) {
		registerInd1 = 3;
	}
	
	if(strcmp(op2, "X1") == 0) {
		registerInd2 = 0;	
	} else if(strcmp(op2, "X2") == 0) {
		registerInd2 = 1;
	} else if(strcmp(op2, "X3") == 0) {
		registerInd2 = 2;
	} else if(strcmp(op2, "X4") == 0) {
		registerInd2 = 3;
	}
	
	// HRK COMMAND IS PROCESSING
	if(strcmp(command, "HRK") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					ram[addr1] = fixed % 256;
				} else {
					ram[addr1] = ram[addr2] % 256;
				}
			} else {
				ram[addr1] = registers[registerInd2] % 256;
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					registers[registerInd1] = fixed % 256;
				} else {
					registers[registerInd1] = ram[addr2] % 256;
				}
			} else {
				registers[registerInd1] = registers[registerInd2] % 256;
			}
		}
	
	// TOP COMMAND IS PROCESSING
	} else if(strcmp(command, "TOP") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					int sum = ram[addr1] + fixed;
					ram[addr1] = sum % 256;
				} else {
					int sum = ram[addr1] + ram[addr2];
					ram[addr1] = sum % 256;
				}
			} else {
				int sum = ram[addr1] + registers[registerInd2];
				ram[addr1] = sum % 256;
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					int sum = registers[registerInd1] + fixed;
					registers[registerInd1] = sum % 256;
				} else {
					int sum = registers[registerInd1] + ram[addr2];
					registers[registerInd1] = sum % 256;
				}
			} else {
				int sum = registers[registerInd1] + registers[registerInd2];
				registers[registerInd1] = sum % 256;
			}
		}
		
	} else if(strcmp(command, "CRP") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					ram[addr1] *= fixed;	
				} else {
					ram[addr1] *= ram[addr2];
				}
			} else {
				ram[addr1] *= registers[registerInd2];
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					registers[registerInd1] *= fixed;	
				} else {
					registers[registerInd1] *= ram[addr2];
				}
			} else {
				registers[registerInd1] *= registers[registerInd2];
			}
		}
		
	} else if(strcmp(command, "CIK") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					ram[addr1] -= fixed;	
				} else {
					ram[addr1] -= ram[addr2];
				}
			} else {
				ram[addr1] -= registers[registerInd2];
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					registers[registerInd1] -= fixed;	
				} else {
					registers[registerInd1] -= ram[addr2];
				}
			} else {
				registers[registerInd1] -= registers[registerInd2];
			}
		}
		
	} else if(strcmp(command, "BOL") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					ram[addr1] /= fixed;
					registers[3] = ram[addr1] % fixed;
				} else {
					ram[addr1] /= ram[addr2];
					registers[3] = ram[addr1] % ram[addr2];
				}
			} else {
				ram[addr1] /= registers[registerInd2];
				registers[3] = ram[addr1] % registers[registerInd2];
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					registers[registerInd1] /= fixed;
					registers[3] = registers[registerInd1] % fixed;
				} else {
					registers[registerInd1] /= ram[addr2];
					registers[3] = registers[registerInd1] % ram[addr2];
				}
			} else {
				registers[registerInd1] /= registers[registerInd2];
				registers[3] = registers[registerInd1] % registers[registerInd2];
			}
		}
		
	} else if(strcmp(command, "VE") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					ram[addr1] = (ram[addr1] & fixed);
				} else {
					ram[addr1] = (ram[addr1] & ram[addr2]);
				}
			} else {
				ram[addr1] = (ram[addr1] & registers[registerInd2]);
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					registers[registerInd1] = (registers[registerInd1] & fixed);
				} else {
					registers[registerInd1] = (registers[registerInd1] & ram[addr2]);
				}
			} else {
				registers[registerInd1] = (registers[registerInd1] & registers[registerInd2]);
			}
		}
		
	} else if(strcmp(command, "VEYA") == 0) {
		if(op1 == "\0") {
			if(op2 == "\0") {
				if(addr2 == -1) {
					ram[addr1] = (ram[addr1] | fixed);
				} else {
					ram[addr1] = (ram[addr1] | ram[addr2]);
				}
			} else {
				ram[addr1] = (ram[addr1] | registers[registerInd2]);
			}
		} else {
			if(op2 == "\0") {
				if(addr2 == -1) {
					registers[registerInd1] = (registers[registerInd1] | fixed);
				} else {
					registers[registerInd1] = (registers[registerInd1] | ram[addr2]);
				}
			} else {
				registers[registerInd1] = (registers[registerInd1] | registers[registerInd2]);
			}
		}
		
	} else if(strcmp(command, "DEG") == 0) {
		if(op1 == "\0") {
			ram[addr1] = ~ram[addr1];
		} else {
			registers[registerInd1] = ~registers[registerInd1];
		}
	}
}

