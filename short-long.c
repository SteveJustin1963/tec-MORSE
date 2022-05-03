
#include <stdio.h>

int main()
{
    int n, i, sum = 0;
    float x[n];

    printf("Enter the number of samples: ");
    scanf("%d", &n);

    printf("Enter the samples: ");
    for(i = 0; i < n; i++)
    {
        scanf("%f", &x[i]);
    }

    for(i = 0; i < n; i++)
    {
        sum = sum + x[i];
    }

    float mean = (float)sum/n;

    printf("Mean of the samples = %f\n", mean);

    if(mean < 0.01)
        printf("The tone is short.\n");
    else
        printf("The tone is long.\n");

    return 0;
}
