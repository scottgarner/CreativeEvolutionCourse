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

/**
 * THE CONTENTS OF THIS FILE SHOULD BE EDITED TO PRODUCE A WINNING SUDOKU SOLVER...
 */

#pragma mark -
#pragma mark - TEAM_PARAMS

static const std::string	kAuthorTeam		= "The Sword of God";
static const float			kMutationRate	= .618;

#pragma mark -
#pragma mark - TEAM_FUNCTIONS


static const int row_map[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8,
    9,10,11,12,13,14,15,16,17,
    18,19,20,21,22,23,24,25,26,
    27,28,29,30,31,32,33,34,35,
    36,37,38,39,40,41,42,43,44,
    45,46,47,48,49,50,51,52,53,
    54,55,56,57,58,59,60,61,62,
    63,64,65,66,67,68,69,70,71,
    72,73,74,75,76,77,78,79,80
};
static const int column_map[] = {
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
static const int box_map[] = {
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

static const int epoch = 256;
static const int eon = epoch * 10;
std::vector<int> gridTracker(27,0);

static float fitnessFunc(const int* iBoard, const size_t& iTileCount)
{
	// EXERCISE: Please implement a function that evaluates the fitness of a given sudoku board...
	
    size_t score = 0;
    
    // Operons
    for (int i = 0; i < getTileAxis(); i++) {
        
        memset(&gridTracker[0], 0, gridTracker.size() * sizeof gridTracker[0]);
        
        // Genes
        for (int j = 0; j < getTileAxis(); j++) {
            
            size_t mapIndex = j + (i * getTileAxis());
            
            gridTracker[iBoard[row_map[mapIndex]]-1] =
            gridTracker[iBoard[column_map[mapIndex]]-1 + 9] =
            gridTracker[iBoard[box_map[mapIndex]]-1 + 18] = 1;
            
        }
        
        score += std::accumulate(gridTracker.rbegin(), gridTracker.rend(), 0);
        
    }
    
    return score;
    
}

static void crossoverFunc(const int* iBoardA, const int* iBoardB, int* oBoard, const size_t& iTileCount)
{
    
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
    
    std::copy(&iBoardA[0], &iBoardA[(int)iTileCount], oBoard);
    for (int i = 0; i < getTileAxis(); i++) {
        
        if (rand()%2) {
            std::copy(&iBoardB[i * 9], &iBoardB[ (i *9) + 9], &oBoard[i * 9]);
        }
    }
    
}

static void mutateFunc(int* ioBoard, const size_t& iTileCount, const float& iMutationRate, const float& iGenerationIter)
{
	// EXERCISE: Please feel free to replace the contents of this function to improve upon your algorithm's performance...
    
    bool xmen = false;
    bool virus = false;
    bool meteor = false;
    
    float mutationRate = iMutationRate;
    
    if ((int)iGenerationIter % eon == (epoch)) {
        
        mutationRate = 1.0;
        
        //        printf( "***************\n");
        //        printf( "***************\n");
        //        printf( "NUCLEAR FALLOUT\n");
        //        printf( "***************\n");
        //        printf( "***************\n");
        
    } else if ((int)iGenerationIter % eon == (epoch * 3)) {
        
        xmen = true;
        
        //        printf( "*********************\n");
        //        printf( "*********************\n");
        //        printf( "GAMMA RAYS FROM SPACE\n");
        //        printf( "*********************\n");
        //        printf( "*********************\n");
        
    } else if ((int)iGenerationIter % eon == (epoch * 6)) {
        
        virus = true;
        
        //        printf( "***************\n");
        //        printf( "***************\n");
        //        printf( "PLAGUE OUTBREAK\n");
        //        printf( "***************\n");
        //        printf( "***************\n");
        
    } else if ((int)iGenerationIter % eon == (eon - 1)) {
        
        meteor = true;
        
//        printf( "*******************\n");
//        printf( "*******************\n");
//        printf( "CATASTROPHIC IMPACT\n");
//        printf( "*******************\n");
//        printf( "*******************\n");
        
    }
    
    
    if (!virus && !xmen && !meteor) {
        
        if( ( (float)rand() / (float)RAND_MAX ) < mutationRate ) {
            
            size_t mutationType = rand()%3;
            int i = rand()%9;
            
            int swapIndex1 = (rand()%9) + (i * getTileAxis());
            int swapIndex2 = (rand()%9) + (i * getTileAxis());
            
            switch(mutationType) {
                case 0:
                    swapIndex1 = row_map[swapIndex1];
                    swapIndex2 = row_map[swapIndex2];
                    break;
                case 1:
                    swapIndex1 = column_map[swapIndex1];
                    swapIndex2 = column_map[swapIndex2];
                    break;
                case 2:
                    swapIndex1 = box_map[swapIndex1];
                    swapIndex2 = box_map[swapIndex2];
                    break;
            }
            
            int holder = ioBoard[swapIndex1];
            ioBoard[swapIndex1] = ioBoard[swapIndex2];
            ioBoard[swapIndex2] = holder;
            
        }
    }
    
    if (xmen && rand()%2) {
        
        int i = rand()%9;
        
        int mutateType = rand()%3;
        
        for (int j = 0; j < getTileAxis(); j++) {
            
            int mutateIndex;
            int mapIndex = j + (i * getTileAxis());
            
            switch(mutateType) {
                case 0:
                    mutateIndex = row_map[mapIndex];
                    break;
                case 1:
                    mutateIndex = column_map[mapIndex];
                    break;
                case 2:
                    mutateIndex = box_map[mapIndex];
                    break;
            }
            
            ioBoard[mutateIndex] = (rand()%9) +1;
            
        }
    }
    
    if (virus && rand()%5 != 0) {
        for(int i = 0; i < iTileCount; i++) {
            if( rand()%8 == 0 ) {
                ioBoard[i] = (rand()%9) +1;
                
            }
        }
    }
    
    if (meteor && rand()%1000000 != 0)
    {
        for(int i = 0; i < iTileCount; i++) {
            ioBoard[i] = (rand()%9) +1;
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