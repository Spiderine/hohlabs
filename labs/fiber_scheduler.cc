#include "labs/fiber_scheduler.h"

//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//

// Function to get the nth Fibnacci
void fiberFib(addr_t* pmain_stack, addr_t* pf_stack,shellstate_t* shellstate){

	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;

	int num;
	int index = shellstate->index;
	
	num = shellstate->tasks[index].arg;

	if ((num==0) || (num==1)){
		// Asks for the 0th or 1st Fibnacci. We give 1
		shellstate->tasks[index].ret = 1;
	}
	else{
		int a =1;
		int b=1;
		int c=0;
		for (int i=1; i<num; i++){
			c = a+b;
			a=b;
			b=c;
			if(i%shellstate->SPEED==0)
				stack_saverestore(f_stack,main_stack);			

		}
		
		shellstate->tasks[index].ret = c;
	}
	//END FIBER

	//To print immediately
	shellstate->intResult[0]=shellstate->tasks[index].ret;
	shellstate->intResult_Length=1;
	
//check probelem not going scheduling old task
	hoh_debug("ret:"<<shellstate->tasks[index].ret);
	// ### Decrementing the total Functions and the Fib functions used. Make changes in the Tasks arrays
	shellstate->tasksRunning--;
	shellstate->fibRunning--;
	hoh_debug("runnin:"<<shellstate->tasksRunning);
	hoh_debug("runnin:"<<shellstate->fibRunning);
	
	//Move struct to history list
//		memcpy( (void*)&RTCclk, (void*)&RTCclkBuffert, sizeof(RTCclk) );
//		shellstate->structCopy(shellstate->prevTasks[tasksCompleted],shellstate->tasksRunning[index])
	shellstate->prevTasks[shellstate->tasksCompleted]=shellstate->tasks[index];
	shellstate->tasksCompleted++;
	hoh_debug("Comp:"<<shellstate->tasksCompleted);
	//END FIBER
	for(;;){//##What is this infinite loop for?
//		hoh_debug("in end loop: "<<shellsate->tasks[index].done);
		shellstate->tasks[index].done=true;
//		hoh_debug("main:"<<main_stack);
//        hoh_debug("f:"<<f_stack);
		// Going back to main
		stack_saverestore(f_stack,main_stack);
	}	
	
	
}

// Function to get the nth prime
void fiberPrime(addr_t* pmain_stack, addr_t* pf_stack,shellstate_t* shellstate){
	
	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;

	
	//Index will be trahsed later
	int index = shellstate->index;
	hoh_debug("index:"<<unsigned(index));
	int num = shellstate->tasks[index].arg;
	hoh_debug("IN PRIME of "<<num);
	
	if (num==0){
		// Zeroth prime asked. We will output "1"
		shellstate->tasks[index].ret = 1;		
	}
	
	else{
		
		// Getting the count of the numbers left
		int counts = num;
		
		// Getting the output value
		int outVal = 0;
		
		int i;
		for(i=2; i<2147483647; i++){
			int n_fact =0;
			for (int j=2; j<i; j++){
				if (i%j ==0){
					n_fact=1;
					break;
				}
			}
			//Give back control to kernel every 1000 iterations ## Make thousand later
			if(i%shellstate->SPEED==0){
				shellstate->tasks[index].done=false;
				stack_saverestore(f_stack,main_stack);
				hoh_debug("PRIME nth= "<<num);
				hoh_debug("Iter: "<<i);
			}			
			
			if (n_fact==0){
				// Means that the given number is prime
				counts--;
				if (counts==0){
					// Means that the value of i is the one that we wanted!
					outVal=i;
					break;
				}
			}
			
		}
		
		// Checking if that given nth prime comes under the limit of INT_MAX
		if ((i==2147483647) || (counts !=0)){
			// Gives an error that the output is too large!
			shellstate->tasks[index].ret = -1;
		}
		
		else{
			shellstate->tasks[index].ret = outVal;
		}
	}
	//To print immediately
	shellstate->intResult[0]=shellstate->tasks[index].ret;
	shellstate->intResult_Length=1;
//check probelem not going scheduling old task
//	hoh_debug("ret:"<<shellstate->tasks[index].ret);
	// ### Decrementing the total Functions and the Fib functions used. Make changes in the Tasks arrays
	shellstate->tasksRunning--;
	shellstate->primeRunning--;
	hoh_debug("runnin:"<<shellstate->tasksRunning);
	hoh_debug("runnin:"<<shellstate->primeRunning);
	//Move struct to history list
//		memcpy( (void*)&RTCclk, (void*)&RTCclkBuffert, sizeof(RTCclk) );
//		shellstate->structCopy(shellstate->prevTasks[tasksCompleted],shellstate->tasksRunning[index])
	shellstate->prevTasks[shellstate->tasksCompleted]=shellstate->tasks[index];
	shellstate->tasksCompleted++;
	hoh_debug("Completed: "<<shellstate->tasksCompleted);
	//END FIBER
	for(;;){//##What is this infinite loop for?

		shellstate->tasks[index].done=true;
		// Going back to main
		hoh_debug("Prime DONE: "<<num);
		stack_saverestore(f_stack,main_stack);
	}	
	

}

// Function to get the factors of a given number    
// ### Factor function with the tast part implemented
void fiberFactor(addr_t* pmain_stack, addr_t* pf_stack,shellstate_t* shellstate){
	
	addr_t& main_stack = *pmain_stack; // boilerplate: to ease the transition from existing code
	addr_t& f_stack    = *pf_stack;
	int index = shellstate->index;
	
	int num = shellstate->tasks[index].arg;
	

	int iter,outSize;
	hoh_debug("num:"<<num);
	hoh_debug(shellstate->intLength);
	
	if (num==0){
		shellstate->tasks[index].retN=1;
		shellstate->tasks[index].rets[0]=0;
	}
	
	else{
	
		outSize=0;
		for (iter = 1; iter <= num; iter++){

			if(num%iter==0){
				shellstate->tasks[index].rets[outSize]=iter;
				outSize++;
			}
//			shellstate.tasks[index].done=false;	## Do we need to do false everytime?
			if(iter%shellstate->SPEED==0)	//Give back control to main after every 1000 iterations
				hoh_debug(iter);
				stack_saverestore(f_stack,main_stack);	//Pass control to Kernel
		}

		hoh_debug("Outsize is " << outSize);
		if(outSize<=10){	
			shellstate->tasks[index].retN=outSize;
		}
			
		else{
			shellstate->tasks[index].retN=0;	//0 for too many outputs
			shellstate->tasks[index].error=true;
			shellstate->error = true;
			shellstate->copy(shellstate->errorMessage,shellstate->error_outText,shellstate->error_outTextL);
			hoh_debug(shellstate->error_outText);						
			shellstate->errorMessageL = shellstate->error_outTextL;
		}
	}
	//END factor

	//To print immediately
	shellstate->intResult_Length=shellstate->tasks[index].retN;
		for(int i=0;i<shellstate->tasks[index].retN;i++){
			shellstate->intResult[i]=shellstate->tasks[index].rets[i];
			hoh_debug("act:"<<shellstate->intResult[i]);
		}
		hoh_debug("size;"<<shellstate->intResult_Length);

//check probelem not going scheduling old task
	// ### Decrementing the total Functions and the Fib functions used. Make changes in the Tasks arrays
	shellstate->tasksRunning--;
	shellstate->factorRunning--;
	hoh_debug("runnin:"<<shellstate->tasksRunning);
	hoh_debug("factrunnin:"<<shellstate->factorRunning);
	//Move struct to history list
//		memcpy( (void*)&RTCclk, (void*)&RTCclkBuffert, sizeof(RTCclk) );
//		shellstate->structCopy(shellstate->prevTasks[tasksCompleted],shellstate->tasksRunning[index])
	shellstate->prevTasks[shellstate->tasksCompleted]=shellstate->tasks[index];
	shellstate->tasksCompleted++;
	hoh_debug("Comp:"<<shellstate->tasksCompleted);
	//END FIBER
	for(;;){//##What is this infinite loop for?
//		hoh_debug("in end loop: "<<shellsate->tasks[index].done);
		shellstate->tasks[index].done=true;
//		hoh_debug("main:"<<main_stack);
//        hoh_debug("f:"<<f_stack);
		// Going back to main
		stack_saverestore(f_stack,main_stack);
	}	
}


void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size){
       //The index 0 is the Main stack
    	addr_t& main_stack = stackptrs[0];
    	//get stacksize
    	size_t STACK_SIZE = arrays_size/stackptrs_size; 
// ### You need to properly initialize the different things in order to run this code

/*
    //The index 0 is the Main stack
    addr_t& main_stack = stackptrs[0];
	
	// Getting the currently scheduled function
    int index = shellstate.schedulerIndex;
    
    addr_t& f_stack = stackptrs[index + 1];
        
    if (shellstate.tasks[index].startScheduler){
    	
    	shellstate.tasks[index].startScheduler=false;
    }
    
    else{
		
		// ### Execute the function corresponding to the given Scheduler Index
		// ### The logic seems a bit confusing till you implement the state machine. So technically, you should be able to have an integer of schedulerIndex....
		// ### And you should be able to run the function corresponding to this schedulerIndex from the tasks list. Also, I used an integer of index in the shell.h which would be used....
		// ## to point to some index in the tasks list which should be executed correspondingly
		    	
        stack_saverestore(main_stack,f_stack);
            	
    	// Rotating the Scheduler Index
    	
    		shellstate.schedulerIndex=(shellstate.schedulerIndex+1)%shellstate.MAX_TASKS;
    		
    	// ## Check for the done conditions here as well :3
    }
	    
 */
//###PUT HERE FROM FIBER we don't have to touch fiber.cc of 2.2

 	//For the Factor code
	if(shellstate.fiber_factor){	//Called First Time
	
		addr_t& f_stack = stackptrs[shellstate.index+1];	//as 0 is main
		addr_t f_array = addr_t(arrays+(shellstate.index+1)*STACK_SIZE);
        stack_init3(f_stack, f_array, STACK_SIZE, &fiberFactor, &main_stack, &f_stack, &shellstate);
        
        stack_saverestore(main_stack,f_stack); //Call here once so that index can be stored
        hoh_debug("EXIT FACTOR");
		shellstate.tasks[shellstate.index].done = false;	//Make it not done
		shellstate.fiber_factor=false;
    }
       
	//For the Fibonacci code
	if(shellstate.fiber_fib){	//Called First Time
	    addr_t& f_stack = stackptrs[shellstate.index+1];	//as 0 is main
	 	addr_t f_array = arrays+(shellstate.index+1)*STACK_SIZE;
        stack_init3(f_stack, f_array, STACK_SIZE, &fiberFib, &main_stack, &f_stack,&shellstate);
        
        stack_saverestore(main_stack,f_stack); //Call here once so that index can be stored
        hoh_debug("EXIT FIB");
		shellstate.tasks[shellstate.index].done = false;
		shellstate.fiber_fib=false;
    }
    

	//For the Prime code
	if(shellstate.fiber_prime){	//Called First Time
//    	hoh_debug("In shellstate");
  		addr_t& f_stack = stackptrs[shellstate.index+1];	//as 0 is main
	 	addr_t f_array = arrays+(shellstate.index+1)*STACK_SIZE;
        stack_init3(f_stack, f_array, STACK_SIZE, &fiberFib, &main_stack, &f_stack,&shellstate);
        hoh_debug("f:"<<f_stack);
        stack_saverestore(main_stack,f_stack); //Call here once so that index can be stored
		hoh_debug("EXIT PRIME");
		shellstate.tasks[shellstate.index].done = false;
		shellstate.fiber_prime=false;
    }
    
    //Loop to pick unfinished execution
    if(shellstate.tasksRunning>0){
    	//Pick a non done index and execute that
//    	int n;
//    	int *p =&n;
    	//Pick up a job
    	//Increment to give others a chance
    	shellstate.schedulerIndex = (shellstate.schedulerIndex+1)%shellstate.MAX_TASKS;	
    	int count=0;	//to prevent going in infinine loop
		while(shellstate.tasks[shellstate.schedulerIndex].done && count <=/*<##?*/ shellstate.MAX_TASKS){
			shellstate.schedulerIndex = (shellstate.schedulerIndex+1)%shellstate.MAX_TASKS;
			count++;
		}
//		hoh_debug("Running:"<<shellstate.tasksRunning);
//		hoh_debug("Scheduling:"<<shellstate.schedulerIndex);
		addr_t& f_stack = stackptrs[shellstate.schedulerIndex+1];	//1 Indexed as 0 is main
		//Restore the corrspoding stack
        stack_saverestore(main_stack,f_stack); 
//    	hoh_debug("ptr: "<<unsigned(p));
    }
//    0011d7e8

}
