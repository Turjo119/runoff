#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++) // check across all candidates
    {
        if (strcmp(name, candidates[i].name) == 0) // check if name equals candiates' names
        {
            preferences[voter][rank] = i; // update prefernece with number representing candidate
            return true;
        }
    }


    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO
    // for every voter and their first ranked preference
    for(int i = 0, j = 0; i < voter_count; i++)
    {
        //if candidate is still in the race
        if (candidates[preferences[i][j]].eliminated == false)
        {
            //increment this candidates votes
            candidates[preferences[i][j]].votes ++;
        }

        else
        {
            // for all other ranked preferences
            for (j = 0; j < candidate_count; j++)
            {
                //if the next candidate is still in the race
                if (candidates[preferences[i][j + 1]].eliminated == false)
                {
                    //increment that candidates votes
                    candidates[preferences[i][j]].votes ++;
                }
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++) // for all candidates
    {
        if ( candidates[i].votes > (voter_count / 2)) // check if this candidate has more than half the total votes
        {
            printf("%s\n", candidates[i].name); // print this candidates name
            return true;
        }

        else // if previous loop condition not met, do nothing and iterate over the next candidates(s)
        {

        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int min_vote = voter_count; // delcare variable for minimum votes and set it to total number of voters
    for (int i = 0; i < candidate_count ; i++)
    {
        if (candidates[i].eliminated == false) // check if candidate is still in the race
        {
            if (min_vote > candidates[i].votes) // check if candidates' votes is smaller than current minimum votes
            {
                min_vote = candidates[i].votes; // update min_votes
            }
        }


    }
    return min_vote;
    return 0;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    // Initiate variable to check for tie
    int check_tie = 0;

    for (int i = 0; i < candidate_count; i++)
    {
    // For candidates still in race and not having min votes
    if ((candidates[i].eliminated == false) && (candidates[i].votes == !min))
    {
        // keep updating check_tie's value
        check_tie = candidates[i].votes;
    }

    for (int j =0; j < candidate_count; j++)
    {
        // Check for all candidates that are valid have equal number of votes
        if ((candidates[i].eliminated == false) && (candidates[i].votes == !min) && (candidates[i].votes == check_tie))
        {

        }
        return true;

    }

    }

    return false;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if(candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}
