//VM souce Code
//



int pMachine(int* cIc){
	//Contains the main logic for the VM Stack P-Machine
	//

	int OP = cIc[0];
	int L = cIc[1];
	int M = cIc[2];

	//1-11 Basic Instructions
	//12-24 Arithmetic & Operations
	switch(OP){
		case(1):
			//LIT 0 load literal
			
			break;
		case(2):
			//RTN - return

			break;
		case(3):
			//LOD - load from stack to register
			
			break;
		case(4):
			//STO - store

			break;
		case(5):
			//CAL - call procedure

			break;
		case(6):
			//INC - increment SP
			
			break;
		case(7):
			//JMP - jump
			
			break;
		case(8):
			//JPC - jump conditional

			break;
		case(9):
			//SIO1 - Output

			break;
		case(10):
			//SIO2 - Input

			break;
		case(11):
			//SIO3 - Halt

			break;
		case(12):
			//Neg - Negate
			
			break;
		case(13):
			//ADD
		
			break;
		case(14):
			//SUB

			break;
		case(15):
			//MUL

			break;
		case(16):
			//ODD

			break;
		case(17):
			//ODD - Check odd

			break;
		case(18):
			//MOD - Modulo

			break;
		case(19):
			//EQL

			break;
		case(20):
			//NEQ

			break;
		case(21):
			//LSS

			break;
		case(22):
			//LEQ

			break;
		case(23):
			//GTR

			break;
		case(24):
			//GEQ

			break;
		default:
			printf("Improper Instruction Given.");
			return 0;


}


struct stackFrame createStack(){
	stackFrame stack;


}




//Convert from Text -> Ints
int* convertInstructionCode(char* textCode){

	/*Function that receives the raw information from the text document
	 *in the format of a char array and then converts it to an int array
	 *by only extracting the integers for instructions. 
	 *
	 *Parameters:
	 *	char* textCode -> char array from the text document containing
	 *	instruction code for VM.
	 *
	 *Returns:
	 *	int* convertedInstructionCode (cIc) -> extracted ints from the
	 *	textCode array
	 */
	int convertedInstructionCode[3];
	int inc = 0;
	
	for(int i=0;i<5;i++){
		if(i%2==0){
			convertedInstructionCode[inc] = textCode[i] - '0';
			inc++;
		}
	}
	return convertedInstructionCode;
}









