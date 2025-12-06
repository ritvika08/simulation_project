#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int weight;
    int value;
} Item;

void generateItems(Item *items, int n, int maxW, int maxV) {
    int i;
    for (i = 0; i < n; i++) {
        items[i].weight = (rand() % maxW) + 1;
        items[i].value = (rand() % maxV) + 1;
    }
}

int dpKnapsack(Item *items, int n, int capacity) {
    int i, c;

    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (i = 0; i <= n; i++)
        dp[i] = (int *)calloc(capacity + 1, sizeof(int));

    for (i = 1; i <= n; i++) {
        int w = items[i - 1].weight;
        int v = items[i - 1].value;

        for (c = 0; c <= capacity; c++) {
            if (w <= c)
                dp[i][c] = (dp[i - 1][c] > dp[i - 1][c - w] + v)
                           ? dp[i - 1][c] : dp[i - 1][c - w] + v;
            else
                dp[i][c] = dp[i - 1][c];
        }
    }

    int result = dp[n][capacity];

    for (i = 0; i <= n; i++)
        free(dp[i]);
    free(dp);

    return result;
}

int compareItems(const void *a, const void *b) {
    Item *i1 = (Item *)a;
    Item *i2 = (Item *)b;

    double r1 = (double)i1->value / i1->weight;
    double r2 = (double)i2->value / i2->weight;

    if (r2 > r1) return 1;
    if (r2 < r1) return -1;
    return 0;
}

int greedyKnapsack(Item *items, int n, int capacity) {
    qsort(items, n, sizeof(Item), compareItems);

    int i, totalWeight = 0, totalValue = 0;

    for (i = 0; i < n; i++) {
        if (totalWeight + items[i].weight <= capacity) {
            totalWeight += items[i].weight;
            totalValue += items[i].value;
        }
    }
    return totalValue;
}

void runSimulation() {
    int testSizes[] = {10, 20, 50, 100, 150, 200};
    int numTests = 6, t, i;
    int capacity = 200;

    printf("\n=============================================\n");
    printf(" KNAPSACK SIMULATION (C IMPLEMENTATION)\n");
    printf("=============================================\n\n");
    printf("%5s | %12s | %12s | %12s | %12s\n",
           "N", "DP Value", "Greedy Value",
           "DP Time(ms)", "Greedy Time(ms)");
    printf("-------------------------------------------------------------\n");

    for (t = 0; t < numTests; t++) {
        int n = testSizes[t];

        Item *items1 = (Item *)malloc(n * sizeof(Item));
        Item *items2 = (Item *)malloc(n * sizeof(Item));

        generateItems(items1, n, 50, 100);

        for (i = 0; i < n; i++)
            items2[i] = items1[i];

        clock_t start, end;

        start = clock();
        int dpValue = dpKnapsack(items1, n, capacity);
        end = clock();
        double dpTime = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

        start = clock();
        int greedyValue = greedyKnapsack(items2, n, capacity);
        end = clock();
        double greedyTime = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

        printf("%5d | %12d | %12d | %12.3f | %12.3f\n",
               n, dpValue, greedyValue, dpTime, greedyTime);

        free(items1);
        free(items2);
    }

    printf("\nSimulation complete.\n");
}

int main() {
    srand(time(NULL));
    runSimulation();
    return 0;
}

