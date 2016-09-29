#include <cstdio>
#include <cstring>
#include <ctime>

const int MaxN = 1000000;
const int MaxM = 10000000 * 2;

int Pts;
int S, T;
int E[MaxM], Flow[MaxM], Cap[MaxM], Next[MaxM], Node[MaxN], nE;
double Cost[MaxM];

void InsertEdge(int A, int B, int C, double V)
{
	E[nE] = B;
	Flow[nE] = 0;
	Cap[nE] = C;
	Cost[nE] = V;
	Next[nE] = Node[A];
	Node[A] = nE ++;
}

double TotCost;
double Dist[MaxN];
int Prev[MaxN], Q[MaxN], InQ[MaxN];

int SPFA() // Maximum cost flow
{
	for (int i = 0; i < Pts; i ++)
		Dist[i] = -1e100;
	memset(InQ, 0, sizeof(int) * Pts);
	Dist[S] = 0;
	Q[0] = S;
	InQ[S] = 0;
	int Tail = 1;
	for (int Cur = 0; Cur != Tail; )
	{
		int i = Q[Cur];
		for (int j = Node[i]; j != -1; j = Next[j])
			if (Cap[j] > Flow[j] && Dist[i] + Cost[j] > Dist[E[j]]) // Just change it
			{
				Dist[E[j]] = Dist[i] + Cost[j];
				Prev[E[j]] = j;
				if (! InQ[E[j]])
				{
					Q[Tail] = E[j];
					Tail ++;
					if (Tail == MaxN)
						Tail = 0;
					InQ[E[j]] = 1;
				}
			}
		InQ[i] = 0;
		Cur ++;
		if (Cur == MaxN)
			Cur = 0;
	}
	return (Dist[T] >= 0);
}

int Min(int i, int j)
{
	return (i < j) ? i : j;
}

int AugPath()
{
	int F = 100000000;
	for (int i = T; i != S; i = E[Prev[i] ^ 1])
		F = Min(F, Cap[Prev[i]] - Flow[Prev[i]]);
	for (int i = T; i != S; i = E[Prev[i] ^ 1])
	{
		Flow[Prev[i]] += F;
		Flow[Prev[i] ^ 1] -= F;
		TotCost += Cost[Prev[i]] * F;
	}
	return F;
}

void Work()
{
	TotCost = 0;
	memset(Flow, 0, nE * sizeof(int));
	int F = 0;
	while (SPFA())
		F += AugPath();
	
	printf("MaxFlow = %d\n", F);
	printf("MaxCost(MaxFlow) = %lf\n", TotCost);
}

int main()
{
	freopen("MaxCostMaxFlow.in", "r", stdin);
	freopen("MaxCostMaxFlow.out", "w", stdout);
	
	int n, e;
	scanf("%d%d", &n, &e);
	
	Pts = n;
	nE = 0;
	memset(Node, -1, sizeof(Node));
	TotCost = 0;
	S = 0;
	T = 1;	
	while (e --)
	{
		int a, b, Cap;
		double Cost;
		scanf("%d%d%d%lf", &a, &b, &Cap, &Cost);
		a --;
		b --;
		InsertEdge(a, b, Cap, Cost);
		InsertEdge(b, a, 0, -Cost);
	}
	
	int start = clock(); 
	Work();
	int finish = clock(); 
	double duration = (double) (finish - start) / CLOCKS_PER_SEC; 
	
	printf("Duration = %lf seconds\n", duration);
	return 0;
}
