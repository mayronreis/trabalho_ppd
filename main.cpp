#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <sys/types.h>
#include <values.h>
#include <time.h>     // for seconds()
//#include <random>
#define MaxDim  100
#define Rand_Max  100000

using namespace std;

int MAdj[MaxDim][MaxDim];
int Dist[MaxDim][MaxDim];

int LisAdjP[MaxDim][MaxDim];
int LisAdjN[MaxDim][MaxDim];
int CardP[MaxDim];
int CardN[MaxDim];

int M_arcos; // Numero de Arcos no Grafo
int I_arco[MaxDim*MaxDim]; //  I_arco[k] Inicio(Origem) do Arco k
int J_arco[MaxDim*MaxDim]; //  J_arco[k] Fim(Destino) do Arco k

int Hq, Tq;
int Dim;

int Q[MaxDim];
int mark[MaxDim];

int DistInfinita;

int AGM_LisAdjP[MaxDim][MaxDim];
int AGM_CardP[MaxDim];

void Prim(int s)
{
	int v,w,k,l, vmin, dmin;
	int d_temp[MaxDim];
	int pred[MaxDim];
	int pos_prox[MaxDim];


    // Inicializacao: d_temp contem um estimativa pessimista
	// da distancia de s a v

	for (v=1; v<=Dim; v++)
	{
		d_temp[v] = DistInfinita;
		pos_prox[v] = 0;
		pred[v] = -1;
	}


	d_temp[s] = 0;
	pred[s] = s;

	k = 1;
	pos_prox[s] = k;


	// Agora pode-se atualizar as estimativas pessimistas
	// das distancias.

	for(l=1; l<=CardP[s]; l++)
	{
		v = LisAdjP[s][l];
		if (d_temp[v] > Dist[s][v]) // d_temp[s]+Dist[s][v])
		{
           d_temp[v] = Dist[s][v];//d_temp[s]+Dist[s][v];
		   pred[v] = s;
		}
	}


    for (k=2; k<=Dim; k++)
	{

	   vmin = 0;
	   dmin = DistInfinita;
       for (v=1; v<=Dim; v++)
	   {
		   if ((d_temp[v] < dmin) && (pos_prox[v] == 0))
		   {
               vmin = v;
               dmin = d_temp[v];
		   }
	   }


       printf(" Vmin: %d  DistMin: %d\n", vmin, dmin);

	   pos_prox[vmin] = k;


	  // Atualiza-se as estimativas pessimistas
	  // das distancias.

   	  for(l=1; l<=CardP[vmin]; l++)
	  {
	 	 v = LisAdjP[vmin][l];
		 if ( (pos_prox[v] == 0) && (d_temp[v] > Dist[vmin][v])) //d_temp[vmin]+Dist[s][v])
		 {
            d_temp[v] = Dist[vmin][v]; //d_temp[vmin]+Dist[s][v];
		    pred[v] = vmin;
		 }
	  }

	}

	// Distancias e caminhos mais curtos

	printf(" PRIM - vértice inicial: %d\n",s);
	for (v=1; v<=Dim; v++)
	{
		printf(" %d D=%d %d-esimo mais proximo Caminho: ",
			v, d_temp[v], pos_prox[v]);
		w = v;
		while (pred[w] > 0)
		{
			printf(" %d ",pred[w]);
			w = pred[w];
		}
		printf("\n");
	}

	for (v=1; v<=Dim; v++)
	{
			printf("Aresta: %d %d %d\n",v ,pred[v], Dist[v][pred[v]]);
    }

}

//  Gera Grafo

void gera_grafo(int n, double dens)
{
	int i,j;

	M_arcos = 0;
	DistInfinita = 1;
	printf("\n\n Gerando Grafo Aleatório de Densidade: %f #Vertices: %d\n\n",dens,n);
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
		  if(i != j)
		  {
            if(rand() < dens)
			{
				M_arcos++;
				I_arco[M_arcos] = i;
				J_arco[M_arcos] = j;
				MAdj[i][j]=1;
				Dist[i][j]=  600 - (int)(rand() * 1000.);
		        DistInfinita += Dist[i][j];
			}
			else
			{
				MAdj[i][j]=0;
			}
          }
          else
          {
			MAdj[i][j]=0;
          }
		}
	}

	/*for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			if(MAdj[i][j]==0)
			{
				Dist[i][j]=DistInfinita;
			}
		}
	}*/

}

//  Print Grafo

void print_grafo(int n)
{
	int i,j;
	printf("\n GRAFO: Matriz de Adjacencia\n");
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			printf("%d:%d %d ",i,j,MAdj[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			printf("%d:%d %d ",i,j,Dist[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Constroi Lista de Adjacência

void  constroi_lista(int n)
{
	int i,j,p;

	for (i=1; i<=n; i++)
	{
		CardP[i] = 0;
		CardN[i] = 0;
	}

	for(i=1; i<=n; i++)
	{
		for(j=1; j<=n; j++)
		{
			if (MAdj[i][j] == 1)
			{
				p = CardP[i] + 1;
				LisAdjP[i][p] = j;
				CardP[i] = p;

				p = CardN[j] + 1;
				LisAdjN[j][p] = i;
				CardN[j] = p;
			}
		}
	}

}

// Print Listas

void print_listas(int n)
{
	int i,j, p, sum;
	sum = 0;
	printf("\n GRAFO: Listas de Adjacência\n");
	printf("\n  Listas de Saida de Arcos \n");
	for(i=1; i<=n; i++)
	{
		printf("%d | %d Arcos Saindo : ", i, CardP[i]);
		p = CardP[i];
		for(j=1; j<=p; j++)
		{
			printf(" %d ",LisAdjP[i][j]);
		}
		printf("\n");
	}
	printf("\n  Listas de Entrada de Arcos \n");
	for(i=1; i<=n; i++)
	{
		printf("%d | %d Arcos Chegando : ", i, CardN[i]);
		p = CardN[i];
		for(j=1; j<=p; j++)
		{
			printf(" %d ",LisAdjN[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
int main()
{
int v,a;
	float  faux;
	double dens;

	printf(" Digite o numero de vertices no grafo: ");
	scanf("%d", &Dim);

	printf(" Digite a densidade o grafo: ");
	scanf("%f", &faux);

	dens = faux;


	// Geração do Grafo

	gera_grafo(Dim, dens);

	constroi_lista (Dim);

	// Imprime Grafo

	print_grafo(Dim);


	printf("Tecle <enter>");
	getch();

	print_listas(Dim);


//	printf("Digite o numero do vertice para saber seu alcance: ");
//	scanf("%d",&v);
//	printf("Digite o numero maximo de arestas: ");
//	scanf("%d",&a);

//	printf(" Alcance (Busca em Largura) \n");
//    alcance(v, Dim);

//	printf("Tecle <enter>");
//	getch();

//	printf(" Boruvka \n");
//  Boruvka( Dim );

//	printf("Tecle <enter>");
//	getch();


//	printf(" CMC de %d aos demais vertice via ALg. Ford & Belman  \n", v);
//    FordBellman(v);
//	printf("Tecle <enter>");
//	getch();

//    Kruskal();
//    Dijkstra(v);
//	printf("Tecle <enter>");
//	getch();
//    Prim(v);

//	printf("Tecle <enter>");
//	getch();

//    init_buscaP ();
//    buscaP ( v );

//	printf("Tecle <enter>");
//	getch();




    v = 1;
//	printf(" Kruskal   \n");

//	Kruskal();
//	printf(" Prim   \n");

	Prim(v);
	getch();
    return 0;
}
