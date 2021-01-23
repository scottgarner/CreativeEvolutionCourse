//////////////////////////////////////////////////
/*         Evolution as a Creative Tool         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2013                   */
//////////////////////////////////////////////////

#pragma once

#include "Constants.h"
#include <numeric>
#include <bitset>

#include <boost/swap.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_01.hpp>

/*

 THE SWORD OF GOD

 My population is subject to the whims of a jealous and vengeful god
 
 Super fast convergence makes spinning out at a local maximum a common occurrence.
 In other words, the population reaches a point at which it thinks it is perfect
 even though it isn't. This hubris angers the god and the population is punished
 with increasing severity.
 
 I'm pretty clumsy with C++, so my optimizations may all be insane.
 
 
 Population.h
 
 1) I got rid of all sanity checks
 2) I modified the mutation function to take the generation count
 3) I replaced helper functions with "rawer" code
 4) I swap between two population buffers instead of allocating/deleting every time
 
 Solver.h
 
 1) Try to loop as rarely as possible
 2) Blunt and fast is better than elegant and slow
 3) Mutate often, but subtlety
 
 fitnessFunc()
 
 1) Use a set of maps to nagivate the three operon types (row,column,box)
 1) Loop through all three operon types simultaneously
 2) Score all 27 \s into a single bitset
 3) Accumulate the contents of that bitset into a single score
 
 crossoverFunc()
 
 1) std:copy the first board directly to the output board
 2) Loop through the row count (9)
 3) Half the time, std:copy the current row from the second board
 
 mutateFunc()
 
 1) Randomly chose an operon
 2) Swap two values within it
 3) Every epoc, nuclear fallout increases the mutation rate to 1.0
 4) Every three epocs, gamma rays scramble an operon in half the population
 5) Every six epocs, 80% of the population has 12.5% of their genes scrambled
 6) Every eon, a meteorite kills virtually all life on earth and evolution begins anew
 
*/

#pragma mark -
#pragma mark - TEAM_PARAMS

static const std::string	kAuthorTeam		= "The Sword of God";
static const float			kMutationRate	= .618;

#pragma mark -
#pragma mark - TEAM_FUNCTIONS

static const unsigned char column_map[] = {
    0, 9,18,27,36,45,54,63,72,
    1,10,19,28,37,46,55,64,73,
    2,11,20,29,38,47,56,65,74,
    3,12,21,30,39,48,57,66,75,
    4,13,22,31,40,49,58,67,76,
    5,14,23,32,41,50,59,68,77,
    6,15,24,33,42,51,60,69,78,
    7,16,25,34,43,52,61,70,79,
    8,17,26,35,44,53,62,71,80
};
static const unsigned char box_map[] = {
    0, 1, 2, 9,10,11,18,19,20,
    3, 4, 5,12,13,14,21,22,23,
    6, 7, 8,15,16,17,24,25,26,
    27,28,29,36,37,38,45,46,47,
    30,31,32,39,40,41,48,49,50,
    33,34,35,42,43,44,51,52,53,
    54,55,56,63,64,65,72,73,74,
    57,58,59,66,67,68,75,76,77,
    60,61,62,69,70,71,78,79,80
};

static const unsigned short int epoch = 250;
static const unsigned short int eon = epoch * 10;

std::bitset<27> gridBitSet;

// Faster Randoms

boost::random::mt19937 rng;
boost::random::uniform_smallint<> randnine(0,8);
boost::random::uniform_smallint<> randtwo(0,1);
boost::random::uniform_smallint<> randthree(0,2);
boost::random::uniform_smallint<> randtile(0,80);
boost::random::uniform_smallint<> randgene(1,9);

double random01(boost::random::mt19937 generator)
{
    static boost::random::uniform_01<boost::random::mt19937> dist(generator);
    return dist();
}

unsigned char score = 0;
unsigned char mapIndex;

static float fitnessFunc(const int* iBoard, const size_t& iTileCount)
{
	// EXERCISE: Please implement a function that evaluates the fitness of a given sudoku board...
	
    score = 0;
    
    // Operons
    for (char i = 0; i < 9; i++) {
        
        gridBitSet.reset();
        
        // Genes
        for (char j = 0; j < 9; j++) {
            
            mapIndex = j + (i * 9);
            
            gridBitSet.set(iBoard[mapIndex]-1, 1);
            gridBitSet.set(iBoard[column_map[mapIndex]]-1 + 9, 1);
            gridBitSet.set(iBoard[box_map[mapIndex]]-1 + 18, 1);
            
        }
        
        score += gridBitSet.count();
    }
    
    return score;
}

static void crossoverFunc(const int* iBoardA, const int* iBoardB, int* oBoard, const size_t& iTileCount)
{
    
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
    
    std::copy(&iBoardA[0], &iBoardA[(int)iTileCount], oBoard);
    for (char i = 0; i < 9; i++) {
        
        if (randtwo(rng)) {
            std::copy(&iBoardB[i * 9], &iBoardB[ (i *9) + 9], &oBoard[i * 9]);
        }
    }
    
}

static void mutateFunc(int* ioBoard, const size_t& iTileCount, const float& iMutationRate, const size_t& iGenerationIter)
{
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
        
    bool xmen = false;
    bool virus = false;
    bool meteor = false;
    
    float mutationRate = iMutationRate;
    
    if (iGenerationIter == 0) {
        
        rng.seed(static_cast<unsigned int>(std::time(0)));
        
    } else if (iGenerationIter % eon == (epoch)) {
        
        mutationRate = 1.0;
        
        //        printf( "***************\n");
        //        printf( "***************\n");
        //        printf( "NUCLEAR FALLOUT\n");
        //        printf( "***************\n");
        //        printf( "***************\n");
        
    } else if (iGenerationIter % eon == (epoch * 3)) {
        
        xmen = true;
        
        //        printf( "*********************\n");
        //        printf( "*********************\n");
        //        printf( "GAMMA RAYS FROM SPACE\n");
        //        printf( "*********************\n");
        //        printf( "*********************\n");
        
    } else if (iGenerationIter % eon == (epoch * 6)) {
        
        virus = true;
        
        //        printf( "***************\n");
        //        printf( "***************\n");
        //        printf( "PLAGUE OUTBREAK\n");
        //        printf( "***************\n");
        //        printf( "***************\n");
        
    } else if (iGenerationIter % eon == (eon - 1)) {
        
        meteor = true;
        
    //        printf( "*******************\n");
    //        printf( "*******************\n");
    //        printf( "CATASTROPHIC IMPACT\n");
    //        printf( "*******************\n");
    //        printf( "*******************\n");
        
    }
    
    
    if (!virus && !xmen && !meteor) {
        
        if( random01(rng) < mutationRate ) {
            
            char mutationType = randthree(rng);
            char i = randnine(rng);
            
            char swapIndex1 = (randnine(rng)) + (i * 9);
            char swapIndex2 = (randnine(rng)) + (i * 9);
            
            if(mutationType == 1) {
                    swapIndex1 = column_map[swapIndex1];
                    swapIndex2 = column_map[swapIndex2];
            } else if (mutationType == 2) {
                    swapIndex1 = box_map[swapIndex1];
                    swapIndex2 = box_map[swapIndex2];
            }
            
            boost::swap(ioBoard[swapIndex1], ioBoard[swapIndex2]);
            
        }
    }
    
    if (xmen && randtwo(rng)) {
        
        char i = randnine(rng);
        char mutateType = randthree(rng);
        
        for (char j = 0; j < 9; j++) {
            
            char mutateIndex = j + (i * 9);
            
            if(mutateType == 1) {
                    mutateIndex = column_map[mutateIndex];
            } else if (mutateType == 2) {
                    mutateIndex = box_map[mutateIndex];
            }
            
            ioBoard[mutateIndex] = randgene(rng);
            
        }
    }
    
    if (virus && rand()%5 != 0) {
        for(char i = 0; i < iTileCount; i++) {
            if( rand()%8 == 0 ) {
                ioBoard[i] = randgene(rng);
                
            }
        }
    }
    
    if (meteor && rand()%1000 != 0)
    {
        for(char i = 0; i < iTileCount; i++) {
            ioBoard[i] = randgene(rng);
        }
    }
    
}


static void randomBoard(int* ioBoard, const size_t& iTileCount)
{
	// EXERCISE: The contents of this function can be edited for testing, but will be reverted for league play...
	
	for(int i = 0; i < iTileCount; i++) {
		ioBoard[i] = randomInt( getTileValueMin(), getTileValueMax() + 1 );
	}
}

static void printBoard(int* iBoard, const size_t& iTileCount)
{
	// EXERCISE: The contents of this function can be edited for testing, but will be reverted for league play...
	
	int tAxisLen = sqrt( iTileCount );
	printf( "%ix%i BOARD:\n", tAxisLen, tAxisLen );
	for(int i = 0; i < tAxisLen; i++) {
		for(int j = 0; j < tAxisLen; j++) {
			printf( "%i ", iBoard[ i * tAxisLen + j ] );
		}
		printf( "\n" );
	}
	printf( "\n" );
}
