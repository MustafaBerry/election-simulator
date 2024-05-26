#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

typedef struct
{
    char name[50];
    int votes;
    bool eliminated;
} candidate;

int voter_count;
int candidate_count;
int preferences[MAX_VOTERS][MAX_CANDIDATES];  // Added definition for preferences array
candidate candidates[MAX_CANDIDATES];

// Function declarations
bool vote(int voter, int rank, char name[]);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(void)
{
    // Populate array of candidates
    printf("Enter the number of candidates (up to %d): ", MAX_CANDIDATES);
    scanf("%d", &candidate_count);

    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %d\n", MAX_CANDIDATES);
        return 1;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        printf("Enter name for candidate %d: ", i + 1);
        scanf("%s", candidates[i].name);
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    printf("Number of voters: ");
    scanf("%d", &voter_count);

    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %d\n", MAX_VOTERS);
        return 2;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            char name[50]; // Adjust the size according to your needs
            printf("Rank %d for voter %d: ", j + 1, i + 1);
            scanf("%s", name);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        printf("\n");
    }

    // Keep holding runoffs until a winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if the election has been won
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

        // Eliminate anyone with the minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }

    return 0;
}

// Record preference if the vote is valid
bool vote(int voter, int rank, char name[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (!candidates[preferences[i][j]].eliminated)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }
}

// Print the winner of the election if there is one
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > voter_count / 2)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = voter_count;
    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min)
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int count = 0;
    int remove = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            remove++;
        }
        if (candidates[i].votes == min)
        {
            count++;
        }
    }
    return count == remove;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
}