// INCLUDES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



// CONSTANTS
#define MAX_SIZE 50



// STRUCT
struct teamStats
{
    int wins;
    int losses;
    int ties;
};



// FUNCTION PROTOTYPES
int processGames(char* teamFile);  
void parseLine(char* record, char* oppTeam, int* primeScore, int* oppScore);
void displayResults(char* primeTeam, char* oppTeam, int primeScore, int oppScore, struct teamStats* ptr);



int main(void)
{
    FILE* teamsFP = NULL;
    char team[MAX_SIZE] = "";
    int status = 0; 

    // open file for reading
	teamsFP = fopen("teams.txt", "r"); 

    // error: file won't open
    if (teamsFP == NULL)
    {
        printf("Can't open file for writing\n");
		return -1;
    }

    // TRAVERSE THOUGH TEAM NAMES
	while(fgets(team, sizeof team, teamsFP) > 0)
	{
		// is it at the end of the file?
		if(feof(teamsFP))
		{
			// we've reached the end of the file
			break;
		}
		// do we have an error?
		if(ferror(teamsFP))
		{
			// there's an error
			printf("Error reading line from file\n");
			// try to close the file, since we're done with it
			if(fclose(teamsFP) != 0)
			{
				// we can't even close the file
				printf("Can't close file opened for reading\n");
			}
			return -2;
		}

        size_t len = strlen(team);

        if (len > 0 && team[len - 1] == '\n') 
        {
            team[len - 1] = '\0';  // Replace the newline character with a null terminator
        }

        printf("TEAM BEING PROCESSED: %s\n", team);

        // analyze individual team's games
        status = processGames(team);

        if (status == -1)
        {
            printf("Can't open file for reading\n");
        }
        else if (status == -2)
        {
            printf("Error reading line from file\n");
        }
        else if (status == -3)
        {
            printf("Can't close file opened for reading\n");
        }
	}

    // close teams file
    fclose(teamsFP);

    // END OF PROGRAM
    return 0;
}



/*
    Function:       processGames()
    Parameters:     char* teamFile: filename of a team's game results
    Return Value:   int status
    Description:    This function retrieves data on a teams performance
                    from their respective file.
*/
int processGames(char* teamFile)
{
    int status = 0;
    FILE* teamFP = NULL;
    char record[MAX_SIZE] = "";
    char oppTeam[MAX_SIZE] = "";
    int primeScore = 0;
    int oppScore = 0;

    // initialize team stats
    teamStats team = {0, 0, 0};

    // open team file for analysis
    teamFP = fopen(teamFile, "r"); 

    // error: can't open file
    if (teamFP == NULL)
    {
		return -1;
    }

    // get each game result from the team-specific file
	while(fgets(record, sizeof record, teamFP) > 0)
	{
        // is it at the end of the file?
		if(feof(teamFP))
		{
			// we've reached the end of the file
			break;
		}

        // do we have an error
		if(ferror(teamFP))
		{
			// there's an error
            status = -2;
			
			// try to close the file, since we're done with it
			if(fclose(teamFP) != 0)
			{
				// we can't even close the file
                status = -3;
			}
			return status;
		}

        size_t len = strlen(record);

        if (len > 0 && record[len - 1] == '\n') 
        {
            record[len - 1] = '\0';  // Replace the newline character with a null terminator
        }

        // parse line contents
        parseLine(record, oppTeam, &primeScore, &oppScore);

        // display game results
        displayResults(teamFile, oppTeam, primeScore, oppScore, &team);
    }

    // calculate winning percentage
    double winningPercentage = (2.0 * team.wins + team.ties) / (2.0 * (team.wins + team.losses + team.ties));

    // display season performance
    printf("Season result for %s: %.3f (%d-%d-%d)\n\n", teamFile, winningPercentage, team.wins, team.losses, team.ties);

    // close file
    fclose(teamFP);

    return status;
}



/*
    Function:       displayResults()
    Parameters:     char* oppTeam
                    int* primeScore
                    int* oppScore
    Return Value:   None
    Description:    This function outputs a line of a teams's perfromance,
                    indicating whether they lost, tied, or won a game.
*/
void displayResults(char* primeTeam, char* oppTeam, int primeScore, int oppScore, struct teamStats* ptr)
{
    // team lost
    if (primeScore < oppScore) 
    {
        printf("\t%s lost to %s %d-%d\n", primeTeam, oppTeam, oppScore, primeScore);
        ptr->losses++;
    } 
    
    else if (primeScore == oppScore) 
    {
        printf("\t%s and %s tied at %d\n", primeTeam, oppTeam, primeScore);
        ptr->ties++;
    } 
    
    else if (primeScore > oppScore) 
    {
        printf("\t%s beat %s %d-%d\n", primeTeam, oppTeam, primeScore, oppScore);
        ptr->wins++;
    } 
}



/*
    Function:       parseLine()
    Parameters:     char record[]
                    char oppTeam[]
                    int* primeScore
                    int* oppScore      
    Return Value:   int status
    Description:    This function retrieves data on a line containing the scores
                    of the primary and opposing team.
*/
void parseLine(char* record, char* oppTeam, int* primeScore, int* oppScore)
{
    int i = 0, j = 0;

    // extract opposing team name
    while (record[i] != ',' && record[i] != '\0')
    {
        oppTeam[j++] = record[i++];
    }
   
    // null terminate string
    oppTeam[j] = '\0';

    // skip over comma 
    i++;

    // extract scores
    sscanf(record + i, "%d-%d", primeScore, oppScore);
}


