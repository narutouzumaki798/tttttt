#include <bits/stdc++.h>
using namespace std;


int main()
{
    int coins[] = {3, 5, 7};
    int n = (sizeof(coins)/sizeof(coins[0]));
    int target = 25;

    int dp[n][target+1];

    // reset
    for(int i=0; i<n; i++)
	for(int j=0; j<=target; j++)
	    dp[i][j] = 0;

    // dp
    for(int i=0; i<n; i++)
    {
	dp[i][0] = 1;
	for(int j=0; j<=target; j++)
	{
	    if(i > 0)
	    dp[i][j] = dp[i-1][j];

	    if(j-coins[i] >= 0 && dp[i][j-coins[i]])
		dp[i][j] = coins[i];
	}
    }

    // display table
    if(0)
    {
	for(int i=0; i<n; i++)
	{
	    printf("coin %d: ", coins[i]);
	    for(int j=0; j<=target; j++)
	    {
		printf("t[%d]=%d ", j, dp[i][j]);
	    }
	    printf("\n");
	}
    }

    for(int i=0; i<=target; i++)
    {
	printf("%d: ", i);
	if(dp[n-1][i])
	{
	    printf("hobe: ");
	    int t = i;
	    while(t) { printf("%d, ", dp[n-1][t]); t = t - dp[n-1][t]; }
	    printf("\n");
	}
	else printf("na");
	printf("\n");
    }
    printf("\n\n");
    return 0;
}


