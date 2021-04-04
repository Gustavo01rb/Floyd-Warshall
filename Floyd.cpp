#include <iostream>
#include <fstream>
#include <cfloat>
#include <list>

using namespace std;

/*Estruturas Básicas de um grafo*/
    
    typedef struct Aresta{
        int viz;    //Vizinho
        float peso; //Peso da aresta
    }Aresta;

    typedef struct Vertice{
        int grau;
        list<Aresta> adj; //Lista de adjacencia
    }Vertice;

    typedef struct Grafo{
        int NV; //Número de Vértices
        int NA; //Número de Arestas
        Vertice* V;
    }Grafo;

/*Fim Estruturas Básicas do Grafo*/

/*Funções*/

    int CreateGrafo(Grafo& G);                                                      //Criação do Grafo
    void Floyd(Grafo G, float** dist, int** pi);                                    //Algoritmo de Floyd
    void MenorCaminho(float** dist, int** pi, int i, int j) ;                       //Menor caminho
     void MenorCaminhoR(float** dist, float *custo,int** pi, int i, int j);         //Complemento de menor caminho


    void printMatriz(float** matriz, int m, int n);

/*Fim Funções*/

/*Main*/
    
    int main(){
        
        Grafo G;
        if(!CreateGrafo(G)){cout<< "\n\nFalha na leitura do aruqivo\n\n"; return -1; } 

        //Criação das matrizes
        int N = G.NV; // N = Número de Vértices do Grafo G.
        float** dist = new float*[N];
        int** pi = new int*[N];
        for(int i = 0; i < N; i++){
            dist[i] = new float[N];
            pi[i] = new int[N];
        }

        Floyd(G,dist,pi);

        cout<<"\n\n-----------------------------Matriz de distância -----------------------------"<<endl;
        printMatriz(dist,N,N);

        cout<<"\n\nDistância entre todos os elementos interligados: " << endl;
        for(int i = 0; i < N; i++){
            cout<<"\n";        
            for(int j = 0; j < N ; j++){
                if(i==j) continue;
                MenorCaminho(dist,pi,i,j);
            }
        }

        cout<<"\n\n";        
        return 0;
    }

/*Fim Main*/

/*Implementação das funções*/

    int CreateGrafo(Grafo& G){
        Aresta A;
        ifstream arq;
        
        arq.open("Entradas.txt");               //Abrindo o arquivo de entrada
        
        if(arq.fail()) return 0;                //Verificando erros na abertura

        arq >> G.NV;                            //Atribui o Número de vértices do grafo
        G.NA = 0;                               //Iguala o número de arestas igual a 0, pois depois todas as arestas serão tratadas separadamente
        G.V = NULL;       
        G.V = new Vertice[G.NV];                 // Atribuindo os vétices do grafo

        for(int i = 0; i < G.NV; i++){          //Esse for controla o ínicio de cada linha da entrada
            int grau;
            arq >> grau;
            G.V[i].grau = grau;                 //Atribuindo o valor lido no arquivo para a estrutura do Grafo
            G.NA += grau;                       //O núemro de arestas de um grafo é igual a soma de todas as arestas incidentes dos vétices desse grafo
            
            for(int j = 0; j < grau; j++){      //Esse For é responsável por caminhas nas Colunas do arquivo
                int viz;
                float peso;                     //A ideia nessa parte é ler cada aresta e seu respetivo peso.
                                            
                arq >> viz;                     
                arq >> peso;

                A.viz = viz;
                A.peso = peso;
                G.V[i].adj.push_back(A);        //Aqui se cria a lista de ajacencia de cada vétice, o comando push_back adiciona itens na lista
            }

        }

        arq.close();
        return 1;
    }

    void Floyd(Grafo G, float** dist, int** pi){
        list<Aresta>::iterator it;              //Ciração de uma lista auxiliar para acessar a lista de adj
        int j = 0;

        int N = G.NV;
        for(int i = 0; i < N; i++){             //Essa parte serve para setar valores default nas matrizes de distância e de menor caminho
            for(int j = 0; j < N; j++){
                dist[i][j] = DBL_MAX;           //DBL_MAX significa infinito, é o maior valor que um tipo float pode assumir
                pi[i][j] = -1;
            }
            dist[i][i] = 0;                      //A distância de um vértice até ele mesmo é 0
        }

        for(int i = 0; i < N; i++)              //Nessa parte retiramos os valores da lista de adj para a matriz de distâncias. Cada posição representa o peso.
            for(it = G.V[i].adj.begin(); it != G.V[i].adj.end(); ++it){
                j = (*it).viz;                  
                dist[i][j] = (*it).peso;
            }
        
        for(int k = 0; k < N; k++){             //Nessa parte analisamos a ditância entre um ponto intermediario entre i e j 
            for(int i = 0; i < N; i++)
                for(int j = 0; j < N; j++)
                    if(dist[i][j] > dist[i][k]+dist[k][j]){
                        dist[i][j] = dist[i][k] + dist[k][j];
                        pi[i][j] = k;
                    }
        }

    }


    void MenorCaminho(float** dist, int** pi, int i, int j){
        float x = 0;
        float *custo = &x;

        cout<< "\nMenor caminho entre [" << i << "] e [" << j << "]: " <<i << " ";
        MenorCaminhoR(dist,custo,pi,i,j);
        cout<< "        Custo: " << *custo ;
    }
    void MenorCaminhoR(float** dist, float *custo,int** pi, int i, int j) {
        if (pi[i][j] == -1) {
            *custo += dist[i][j];
            cout<<j;
            return;
        }
        MenorCaminhoR(dist,custo,pi,i,pi[i][j]);
        cout<<" ";
        MenorCaminhoR(dist,custo,pi,pi[i][j],j);
    }






    void printMatriz(float** matriz, int m, int n){
        cout<<"\n"<<endl;
        /*cout<<fixed;
        cout.precision(2);*/
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++){
                if(matriz[i][j] < DBL_MAX) cout<<matriz[i][j]<<"    ";
                else cout<<"if  ";
            }
            cout<<"\n";
        }
    }
    