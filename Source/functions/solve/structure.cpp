
bool next()
{

    //calculates next move according to log

		//case puzzle solved
			//return 0;

        //case last log empty
            //backtrack

        //case last log exactly one solution
			//put all remaining puzzle pieces into new log entry
            //try next piece/place with solve abstraction level 0

        //case last log multiple entries
            //advance abstraction layer of last log by one and solve()
            //or pick first if highest level reached

		//return 1;
}

void solve()
{
    //case
        //abstraction layer = 0
            //go to abstraction layer 0 solver
    
    //case
        //abstraction layer = 1
            //go to abstraction layer 1 solver
    //default
            //random one of the parts 
}

void abstractionlayer0solver()
{
    //throw all remaining puzzle pieces into log
        //remove all impossible
}

void abstractionlayer1solver()
{
    //remove all impossible according to abstraction layer one
}

void setsolution()
{
    //put 
    //set pointer to log into matrix 
}  

void backtrack()
{
    //following possibilities:
        //last log entry empty
            //delete last log + backtrack
        //last log entry only one solution
            //delete last log + backtrack
        //last log entry multiple solutions (and current one was randomed)
            //delete randomed piece from puzzleCollector and go to next (which might random again depending on function)
}