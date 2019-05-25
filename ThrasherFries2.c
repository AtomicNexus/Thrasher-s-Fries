//-------------------------------------------------------------
// Joe Anthony Suarez
// Customer modeling system for CS 2110
// ThrasherFries2.c
//-------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#undef SHOW_LINES						// If defined, will show order of customers in lines A and B based on customer ID.

typedef enum {SMALL = 1, MEDIUM, LARGE} ORDER_TYPE;		// Customer orders can be small, medium, or large.

struct CUSTOMER
{
	int ID;								// Customer ID number
	ORDER_TYPE order;					// Customer's order size (small, medium, large)
	int timeIn;							// Customer's arrival time in line
	char whichLine;						// The line that the customer waited in (A or B)
	int startTimeServed;				// The time that the customer started to be served
	int totalTimeServed;				// The amount of time the customer has been waiting while being served
	int timeOut;						// Customer's departure time from line
	struct CUSTOMER *nextInLine;		// Pointer to next customer in line
};

struct CUSTOMER *frontA, *rearA;								// Head and tail of line A
struct CUSTOMER *frontB, *rearB;								// Head and tail of line B
unsigned int sizeOfA = 0, sizeOfB = 0;							// Number of customers currently in lines A and B
unsigned int totalOfA = 0, totalOfB = 0;						// Total number of customers that have been served in each line for the day
unsigned int maxOfA = 0, maxOfB = 0;							// Maximum size of lines A and B during the day
unsigned int totalWaitingTimeA = 0, totalWaitingTimeB = 0;		// Total waiting time (timeOut - timeIn) in lines A and B
unsigned int totalServeWaitingA = 0, totalServeWaitingB = 0;	// Total waiting time while be served (totalTimeServed) in lines A and B
unsigned int totalQueueSizeA = 0, totalQueueSizeB = 0;			// Used for calculating average queue size (queue size / total time open)
unsigned int countS = 0, countM = 0, countL = 0;				// Number of small, medium, and large orders
float AIncome = 0.0, BIncome = 0.0;								// Total income for lines A and B
FILE *OUT;														// Output file pointer



// Initialize seed used by rand().
void SetRandomSeed(void)
{
	srand(time(NULL));
}



// Return a real number chosen from [0.0, 1.0).
double RandomReal(void)
{
	double r;
	int i;
	
	i = rand();
	(i == 0) ? (r = 0.0) : (r = (double) (i - 1) / RAND_MAX);
	
	return(r);
}



// Return an integer randomly chosen from [lowerBound, upperBound].
int RandomInteger(int lowerBound, int upperBound)
{
	return((int) (RandomReal() * (upperBound - lowerBound + 1)) + lowerBound);
}



// Determine if a customer should be added to a line with 50% bias.
bool ShouldAddCustomer(void)
{
	return(RandomInteger(1, 100000) <= 50000);
}



// Create an empty line.
void CreateLine(char lineID)
{
	if(lineID == 'A')
	{
		frontA = rearA = NULL;
	}
	else // lineID == 'B'
	{
		frontB = rearB = NULL;
	}
}



// Add a customer to the back of a line.
void AddToLine(char lineID, int time)
{
	struct CUSTOMER *temp;
	int orderOdds;
	
	orderOdds = RandomInteger(1, 100000);
	if(lineID == 'A')
	{
		if((frontA == NULL) && (rearA == NULL))		// Line A is empty.
		{
			rearA = malloc(sizeof(struct CUSTOMER));
			rearA -> nextInLine = NULL;
			rearA -> timeOut = 0;			// Must be changed
			rearA -> totalTimeServed = 0;
			rearA -> startTimeServed = 0;	// Must be changed
			rearA -> whichLine = 'A';
			rearA -> timeIn = time;
			if(orderOdds < 20000)
			{
				rearA -> order = SMALL;
			}
			else if((orderOdds >= 20000) && (orderOdds < 70000))
			{
				rearA -> order = MEDIUM;
			}
			else // orderOdds >= 70000
			{
				rearA -> order = LARGE;
			}
			rearA -> ID = totalOfA + totalOfB;
			frontA = rearA;
		}
		else // Line A has customer(s).
		{
			temp = malloc(sizeof(struct CUSTOMER));
			rearA -> nextInLine = temp;
			temp -> timeOut = 0;			// Must be changed
			temp -> totalTimeServed = 0;
			temp -> startTimeServed = 0;	// Must be changed
			temp -> whichLine = 'A';
			temp -> timeIn = time;
			if(orderOdds < 20000)
			{
				temp -> order = SMALL;
			}
			else if((orderOdds >= 20000) && (orderOdds < 70000))
			{
				temp -> order = MEDIUM;
			}
			else // orderOdds >= 70000
			{
				temp -> order = LARGE;
			}
			temp -> ID = totalOfA + totalOfB;
			temp -> nextInLine = NULL;
			rearA = temp;
		}
		totalOfA++;
		sizeOfA++;
	}
	else // lineID == 'B'
	{
		if((frontB == NULL) && (rearB == NULL))		// Line B is empty.
		{
			rearB = malloc(sizeof(struct CUSTOMER));
			rearB -> nextInLine = NULL;
			rearB -> timeOut = 0;			// Must be changed
			rearB -> totalTimeServed = 0;
			rearB -> startTimeServed = 0;	// Must be changed
			rearB -> whichLine = 'B';
			rearB -> timeIn = time;
			if(orderOdds < 20000)
			{
				rearB -> order = SMALL;
			}
			else if((orderOdds >= 20000) && (orderOdds < 70000))
			{
				rearB -> order = MEDIUM;
			}
			else // orderOdds >= 70000
			{
				rearB -> order = LARGE;
			}
			rearB -> ID = totalOfA + totalOfB;
			frontB = rearB;
		}
		else // Line B has customer(s).
		{
			temp = malloc(sizeof(struct CUSTOMER));
			rearB -> nextInLine = temp;
			temp -> timeOut = 0;			// Must be changed
			temp -> totalTimeServed = 0;
			temp -> startTimeServed = 0;	// Must be changed
			temp -> whichLine = 'B';
			temp -> timeIn = time;
			if(orderOdds < 20000)
			{
				temp -> order = SMALL;
			}
			else if((orderOdds >= 20000) && (orderOdds < 70000))
			{
				temp -> order = MEDIUM;
			}
			else // orderOdds >= 70000
			{
				temp -> order = LARGE;
			}
			temp -> ID = totalOfA + totalOfB;
			temp -> nextInLine = NULL;
			rearB = temp;
		}
		totalOfB++;
		sizeOfB++;
	}
	
	if(sizeOfA > maxOfA)
	{
		maxOfA = sizeOfA;
	}
	if(sizeOfB > maxOfB)
	{
		maxOfB = sizeOfB;
	}
}



// Remove a customer from the front of the line.
void RemoveFromLine(char lineID, int time)
{
	struct CUSTOMER *temp;
	
	if(lineID == 'A')
	{
		frontA -> timeOut = time;
		frontA -> totalTimeServed = time - (frontA -> startTimeServed);
		if((frontA -> order) == SMALL)
		{
			AIncome += 2.29;
			countS++;
		}
		else if((frontA -> order) == MEDIUM)
		{
			AIncome += 3.79;
			countM++;
		}
		else // (frontA -> order) == LARGE
		{
			AIncome += 4.99;
			countL++;
		}
		totalWaitingTimeA = totalWaitingTimeA + ((frontA -> timeOut) - (frontA -> timeIn));
		totalServeWaitingA = totalServeWaitingA + (frontA -> totalTimeServed);
		totalQueueSizeA += sizeOfA;
		
		temp = frontA;
		
		if(temp == NULL)
		{
			printf("Line A is already empty.\n");
			return;
		}
		else
		{
			if((temp -> nextInLine) != NULL)	// More customers in line A
			{
				temp = temp -> nextInLine;
				free(frontA);
				frontA = temp;
			}
			else							// Only one customer in line A
			{
				free(frontA);
				frontA = rearA = NULL;
			}
		}
		sizeOfA--;
	}
	else // lineID == 'B'
	{
		frontB -> timeOut = time;
		frontB -> totalTimeServed = time - (frontB -> startTimeServed);
		if((frontB -> order) == SMALL)
		{
			BIncome += 2.29;
			countS++;
		}
		else if((frontB -> order) == MEDIUM)
		{
			BIncome += 3.79;
			countM++;
		}
		else // (frontB -> order) == LARGE
		{
			BIncome += 4.99;
			countL++;
		}
		totalWaitingTimeB = totalWaitingTimeB + ((frontB -> timeOut) - (frontB -> timeIn));
		totalServeWaitingB = totalServeWaitingB + (frontB -> totalTimeServed);
		totalQueueSizeB += sizeOfB;
		
		temp = frontB;
		
		if(temp == NULL)
		{
			printf("Line B is already empty.\n");
			return;
		}
		else
		{
			if((temp -> nextInLine) != NULL)	// More customers in line B
			{
				temp = temp -> nextInLine;
				free(frontB);
				frontB = temp;
			}
			else							// Only one customer in line B
			{
				free(frontB);
				frontB = rearB = NULL;
			}
		}
		sizeOfB--;
	}
}



// Determine if a customer has been served.
bool CustomerHasBeenServed(char lineID)
{
	if(lineID == 'A')
	{
		if((frontA -> order) == SMALL)
		{
			if((frontA -> totalTimeServed) == 1)
			{
				return(true);
			}
			return(false);
		}

		if((frontA -> order) == MEDIUM)
		{
			if((frontA -> totalTimeServed) == 2)
			{
				return(true);
			}
			return(false);
		}

		if((frontA -> order) == LARGE)
		{
			if((frontA -> totalTimeServed) == 4)
			{
				return(true);
			}
			return(false);
		}
	}
	else // lineID == 'B'
	{
		if((frontB -> order) == SMALL)
		{
			if((frontB -> totalTimeServed) == 2)
			{
				return(true);
			}
			return(false);
		}

		if((frontB -> order) == MEDIUM)
		{
			if((frontB -> totalTimeServed) == 3)
			{
				return(true);
			}
			return(false);
		}

		if((frontB -> order) == LARGE)
		{
			if((frontB -> totalTimeServed) == 5)
			{
				return(true);
			}
			return(false);
		}
	}
}



#ifdef SHOW_LINES
// Display the contents of both lines.
void ShowLines(void)
{
	struct CUSTOMER *walk;
	
	walk = frontA;
	fprintf(OUT, "These are the lines after closing, but before finishing leftover orders (if any).\n\n");
	fprintf(OUT, "LINE A\n==========\n");
	while(walk != NULL)
	{
		fprintf(OUT, "%3i   ", walk -> ID);
		if((walk -> order) == SMALL)
		{
			fprintf(OUT, "SMALL   ");
		}
		else if((walk -> order) == MEDIUM)
		{
			fprintf(OUT, "MEDIUM  ");
		}
		else
		{
			fprintf(OUT, "LARGE   ");
		}
		fprintf(OUT, "%3i    %c    %3i   %3i   %3i\n", walk -> timeIn, walk -> whichLine, walk -> startTimeServed, walk -> totalTimeServed, walk -> timeOut);
		walk = walk -> nextInLine;
	}
	
	walk = frontB;
	fprintf(OUT, "LINE B\n==========\n");
	while(walk != NULL)
	{
		fprintf(OUT, "%3i   ", walk -> ID);
		if((walk -> order) == SMALL)
		{
			fprintf(OUT, "SMALL   ");
		}
		else if((walk -> order) == MEDIUM)
		{
			fprintf(OUT, "MEDIUM  ");
		}
		else
		{
			fprintf(OUT, "LARGE   ");
		}
		fprintf(OUT, "%3i    %c    %3i   %3i   %3i\n", walk -> timeIn, walk -> whichLine, walk -> startTimeServed, walk -> totalTimeServed, walk -> timeOut);
		walk = walk -> nextInLine;
	}
}
#endif



// Display final statistics to the console output window.
void DisplayStatsToConsole(int time)
{
	printf("The stand had to stay open for %i minute(s) after closing.\n\n", time - 600);
	
	printf("LINE A\n===============================\n");
	printf("Total customers: %i\n", totalOfA);
	printf("Total service time: %i minutes\n", totalServeWaitingA);
	printf("Average wait time: %.2f minutes\n", ((float) totalWaitingTimeA) / ((float) totalOfA));
	printf("Average wait time while being served: %.2f minutes\n", ((float) totalServeWaitingA) / ((float) totalOfA));
	printf("Income: $%.2f\n", AIncome);
	printf("Maximum line size: %i\n", maxOfA);
	printf("Average line size: %.2f\n\n", ((float) totalQueueSizeA) / ((float) time));
	
	printf("LINE B\n===============================\n");
	printf("Total customers: %i\n", totalOfB);
	printf("Total service time: %i minutes\n", totalServeWaitingB);
	printf("Average wait time: %.2f minutes\n", ((float) totalWaitingTimeB) / ((float) totalOfB));
	printf("Average wait time while being served: %.2f minutes\n", ((float) totalServeWaitingB) / ((float) totalOfB));
	printf("Income: $%.2f\n", BIncome);
	printf("Maximum line size: %i\n", maxOfB);
	printf("Average line size: %.2f\n\n", ((float) totalQueueSizeB) / ((float) time));
	
	printf("OVERALL (LINE A AND LINE B)\n===============================\n");
	printf("Total customers: %i\n", totalOfA + totalOfB);
	printf("Total service time: %i minutes\n", totalServeWaitingA + totalServeWaitingB);
	printf("Average wait time: %.2f minutes\n", ((float) totalWaitingTimeA + (float) totalWaitingTimeB) / ((float) totalOfA + (float) totalOfB));
	printf("Average wait time while being served: %.2f minutes\n", ((float) totalServeWaitingA + (float) totalServeWaitingB) / ((float) totalOfA + (float) totalOfB));
	printf("Number of small orders: %i\n", countS);
	printf("Number of medium orders: %i\n", countM);
	printf("Number of large orders: %i\n", countL);
	printf("Total number of orders for the day: %i\n", countS + countM + countL);
	printf("Income: $%.2f\n", AIncome + BIncome);
}



// Display final statistics to the designated output file.
void DisplayStatsToOutFile(int time)
{
	fprintf(OUT, "The stand had to stay open for %i minute(s) after closing.\n\n", time - 600);
	
	fprintf(OUT, "LINE A\n===============================\n");
	fprintf(OUT, "Total customers: %i\n", totalOfA);
	fprintf(OUT, "Total service time: %i minutes\n", totalServeWaitingA);
	fprintf(OUT, "Average wait time: %.2f minutes\n", ((float) totalWaitingTimeA) / ((float) totalOfA));
	fprintf(OUT, "Average wait time while being served: %.2f minutes\n", ((float) totalServeWaitingA) / ((float) totalOfA));
	fprintf(OUT, "Income: $%.2f\n", AIncome);
	fprintf(OUT, "Maximum line size: %i\n", maxOfA);
	fprintf(OUT, "Average line size: %.2f\n\n", ((float) totalQueueSizeA) / ((float) time));
	
	fprintf(OUT, "LINE B\n===============================\n");
	fprintf(OUT, "Total customers: %i\n", totalOfB);
	fprintf(OUT, "Total service time: %i minutes\n", totalServeWaitingB);
	fprintf(OUT, "Average wait time: %.2f minutes\n", ((float) totalWaitingTimeB) / ((float) totalOfB));
	fprintf(OUT, "Average wait time while being served: %.2f minutes\n", ((float) totalServeWaitingB) / ((float) totalOfB));
	fprintf(OUT, "Income: $%.2f\n", BIncome);
	fprintf(OUT, "Maximum line size: %i\n", maxOfB);
	fprintf(OUT, "Average line size: %.2f\n\n", ((float) totalQueueSizeB) / ((float) time));
	
	fprintf(OUT, "OVERALL (LINE A AND LINE B)\n===============================\n");
	fprintf(OUT, "Total customers: %i\n", totalOfA + totalOfB);
	fprintf(OUT, "Total service time: %i minutes\n", totalServeWaitingA + totalServeWaitingB);
	fprintf(OUT, "Average wait time: %.2f minutes\n", ((float) totalWaitingTimeA + (float) totalWaitingTimeB) / ((float) totalOfA + (float) totalOfB));
	fprintf(OUT, "Average wait time while being served: %.2f minutes\n", ((float) totalServeWaitingA + (float) totalServeWaitingB) / ((float) totalOfA + (float) totalOfB));
	fprintf(OUT, "Number of small orders: %i\n", countS);
	fprintf(OUT, "Number of medium orders: %i\n", countM);
	fprintf(OUT, "Number of large orders: %i\n", countL);
	fprintf(OUT, "Total number of orders for the day: %i\n", countS + countM + countL);
	fprintf(OUT, "Income: $%.2f\n", AIncome + BIncome);
}



int main(void)
{
	int time;			// Current time in simulation
	int nCustomers;		// Number of customers given by the user to run in simulation
	
	CreateLine('A');
	CreateLine('B');
	do
	{
		printf("How many customers would you like to test for this simulation? ");
		scanf("%i", &nCustomers);
		printf("The simulation will run for %i customers.\n\n", nCustomers);
	} while(nCustomers < 1 || nCustomers > 1500);
	
	system("PAUSE");
	system("CLS");
	
	SetRandomSeed();
	for(time = 1; time <= 600; time++)
	{
		// Customers arrive.
		if((ShouldAddCustomer()) && (totalOfA + totalOfB < nCustomers))
		{
			if(sizeOfA <= sizeOfB)
			{
				AddToLine('A', time);
			}
			else
			{
				AddToLine('B', time);
			}
		}

		// Check if a customer's order has been completed.
		if(frontA != NULL)
		{
			if(CustomerHasBeenServed('A'))
			{
				RemoveFromLine('A', time);
			}
		}
		if(frontB != NULL)
		{
			if(CustomerHasBeenServed('B'))
			{
				RemoveFromLine('B', time);
			}
		}
		
		// Work on order.
		if(frontA != NULL)
		{
			if((frontA -> totalTimeServed) == 0)
			{
				frontA -> startTimeServed = time;
			}
			(frontA -> totalTimeServed)++;
		}
		if(frontB != NULL)
		{
			if((frontB -> totalTimeServed) == 0)
			{
				frontB -> startTimeServed = time;
			}
			(frontB -> totalTimeServed)++;
		}
	}
	OUT = fopen("Trial1.txt"/*  or "Trial2.txt" or "Trial3.txt"*/, "w");
	fprintf(OUT, "This trial was run for %i customers.\n\n", nCustomers);
	printf("There are %i customers that have to be served after closing.\n\n", sizeOfA + sizeOfB);
	fprintf(OUT, "There are %i customers that have to be served after closing.\n\n", sizeOfA + sizeOfB);

#ifdef SHOW_LINES
	ShowLines();
#endif

	while(sizeOfA + sizeOfB != 0)
	{
		// Check if a customer's order has been completed.
		if(frontA != NULL)
		{
			if(CustomerHasBeenServed('A'))
			{
				RemoveFromLine('A', time);
			}
		}
		if(frontB != NULL)
		{
			if(CustomerHasBeenServed('B'))
			{
				RemoveFromLine('B', time);
			}
		}
		
		// Work on order.
		if(frontA != NULL)
		{
			if((frontA -> totalTimeServed) == 0)
			{
				frontA -> startTimeServed = time;
			}
			(frontA -> totalTimeServed)++;
		}
		if(frontB != NULL)
		{
			if((frontB -> totalTimeServed) == 0)
			{
				frontB -> startTimeServed = time;
			}
			(frontB -> totalTimeServed)++;
		}
		time++;
	}
	DisplayStatsToConsole(time);
	DisplayStatsToOutFile(time);
	fclose(OUT);
	printf("\nSimulation complete!\n");
	
	system("PAUSE");
	return(0);
}