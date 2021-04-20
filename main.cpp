#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <iomanip>
#include <ctime>
#include "gerador.hpp"

#define tamanhoRAM 64
#define tcache1 8
#define tcache2 16
#define tcache3 32
#define custoRAM 10000
#define custoCache1 10
#define custoCache2 100
#define custoCache3 1000
#define totalInstrucoes 1000
#define tamanhoHD 10000

using namespace std;

typedef struct endereco
{
	int endBloco;
	int endPalavra;
} Endereco;

typedef struct instrucao
{
	Endereco add1;
	Endereco add2;
	Endereco add3;
	int opcode;
} Instrucao;

typedef struct bloco
{
	long int* palavras;
	int addBloco;
	int atualizado;
	int LFU;
} blocoMemoria;

int montaHD ();

void buscarHD (int endBuscar, blocoMemoria* retornar);

void salvarNoHD (blocoMemoria* salvar);

blocoMemoria* alocaBlocosMemoria (int tamanho);

void desalocaBlocosMemoria (blocoMemoria* bloco, int tamanho);

Instrucao* montaMemoriaInstrucoes (int total);

void desalocaMemoriaIntrucoes (Instrucao* memoria);

void maquina (
	Instrucao* memoriaInstrucoes, 
	blocoMemoria* RAM, blocoMemoria* cache1, 
	blocoMemoria* cache2, blocoMemoria* cache3, 
	int* hit1, 
	int* hit2, 
	int* hit3, 
	int* miss1, 
	int* miss2, 
	int* miss3, 
	int* ram, 
	long int* custo, 
	int t1, 
	int t2, 
	int t3, 
	int tr
);

void interrupcaoMaquina (
	Instrucao* interrupcao, 
	blocoMemoria* RAM, 
	blocoMemoria* cache1, 
	blocoMemoria* cache2, 
	blocoMemoria* cache3, 
	int* hit1, 
	int* hit2, 
	int* hit3, 
	int* miss1, 
	int* miss2, 
	int* miss3, 
	int* ram, 
	long int* custo, 
	int t1, 
	int t2, 
	int t3, 
	int tr
);

int verificaVazio (blocoMemoria* bloco, int tamanho);

int verificaLFU (blocoMemoria* bloco, int tamanho);

blocoMemoria* varrerCache (int endEncontrar, blocoMemoria* cache, int tamanhoCache);

void limpaPosicaoCache (blocoMemoria* posicao);

int verificaRAM (blocoMemoria* RAM, int tRAM);

int verificaCache3 (
	blocoMemoria* c3, 
	blocoMemoria* RAM, 
	int tamanho3, 
	int tRAM
);

int verificaCache2 (
	blocoMemoria* RAM, 
	blocoMemoria* c3, 
	blocoMemoria* c2, 
	int tamanho3, 
	int tamanho2, 
	int tRAM
);

int verificaCache1 (
	blocoMemoria* RAM, 
	blocoMemoria* c3, 
	blocoMemoria* c2, 
	blocoMemoria* c1, 
	int tamanho3, 
	int tamanho2, 
	int tamanho1, 
	int tRAM
);

blocoMemoria* buscarNasMemorias (
	int endBuscar, 
	blocoMemoria* RAM, 
	blocoMemoria* cache1, 
	blocoMemoria* cache2, 
	blocoMemoria* cache3, 
	int* hit1, 
	int* hit2, 
	int* hit3, 
	int* miss1, 
	int* miss2, 
	int* miss3, 
	int* ram, 
	long int* custo, 
	int t1, 
	int t2, 
	int t3, 
	int tRAM
);

int main ()
{
	// armazena a quantidade de hits nas caches
	int* hit1 = new int;
	int* hit2 = new int;
	int* hit3 = new int;

	// armazena a quantidade de misses nas caches
	int* miss1 = new int;
	int* miss2 = new int;
	int* miss3 = new int;

	// armazena quantas vezes a mmu acessou a RAM
	int* idasRAM = new int;

	// armazena o custo total do programa
	long int* custo = new long int;

	*hit1 = 0;
	*hit2 = 0;
	*hit3 = 0;

	*miss1 = 0;
	*miss2 = 0;
	*miss3 = 0;

	*idasRAM = 0;
	*custo = 0;
	
	// funcao que simula a montagem do hd da maquina
	if (montaHD() == 0)
		cout<<"Inicio Programa."<<endl;
		
	// alocacao das memorias da maquina
	blocoMemoria* RAM = alocaBlocosMemoria (tamanhoRAM);
	blocoMemoria* cache1 = alocaBlocosMemoria (tcache1);
	blocoMemoria* cache2 = alocaBlocosMemoria (tcache2);
	blocoMemoria* cache3 = alocaBlocosMemoria (tcache3); 
	
	// funcao geradora de instrucoes
	gerador();
	
	// cria uma memoria de instrucoes geradas
	Instrucao* memoriaInstrucoes = montaMemoriaInstrucoes (totalInstrucoes);

	// simula a execucao da maquina
	maquina (
		memoriaInstrucoes, 
		RAM, 
		cache1, 
		cache2, 
		cache3, 
		hit1, 
		hit2, 
		hit3, 
		miss1, 
		miss2, 
		miss3, 
		idasRAM, 
		custo, 
		tcache1, 
		tcache2, 
		tcache3, 
		tamanhoRAM
	);
	
	// relatorio final da execuca da maquina
	cout<<"\n** FINAL DO PROGRAMA ** "<<endl;

	cout<<"Cache1: "<<endl;
	cout<<"- HITS: "<<*hit1<<endl;
	cout<<"- MISS: "<<*miss1<<endl;

	cout<<"\nCache2: "<<endl;
	cout<<"- HITS: "<<*hit2<<endl;
	cout<<"- MISS: "<<*miss2<<endl;

	cout<<"\nCache3: "<<endl;
	cout<<"- HITS: "<<*hit3<<endl;
	cout<<"- MISS: "<<*miss3<<endl;

	cout<<"\nIdas a RAM: "<<*idasRAM<<endl;

	cout<<"\nCUSTO DO PROGRAMA: "<<*custo<<endl;
	
	// desalocacao das memorias da maquina
	delete hit1;
	delete hit2;
	delete hit3;
	delete miss1;
	delete miss2;
	delete miss3;
	delete idasRAM;
	delete custo;
	
	desalocaBlocosMemoria (cache1, tcache1);
	desalocaBlocosMemoria (cache2, tcache2);
	desalocaBlocosMemoria (cache3, tcache3);
	desalocaBlocosMemoria (RAM, tamanhoRAM);
	
	desalocaMemoriaIntrucoes (memoriaInstrucoes);
	
	return 0;
}

// funcao que simula a abertura do hd atraves de um arquivo binario
int montaHD () 
{
	FILE* HD = fopen ("harddrive.bin", "w+b");
	
	int hdWords[4];
	
	for (int i=0; i<tamanhoHD; i++)
	{
		srand(time(NULL));
		
		hdWords[0] = rand()%100;
		hdWords[1] = rand()%100;
		hdWords[2] = rand()%100;
		hdWords[3] = rand()%100;
		
		fwrite (hdWords, sizeof (int), 4, HD);
	}
	
	fclose (HD);
	
	return 0;
}

// funcao que busca um bloco de instrucoes no hd
void buscarHD (int endBuscar, blocoMemoria* retornar) 
{
	int hdPalavras[4];
	
	FILE* HD = fopen ("harddrive.bin", "r+b");
	
	if (HD == NULL)
		cout<<"Arquivo de busca nao foi aberto."<<endl;
	
	fseek (HD, 0, SEEK_SET);
	
	fseek (HD, sizeof(hdPalavras)*(endBuscar-1), SEEK_SET);
	
	fread (hdPalavras, sizeof(hdPalavras), 1, HD);
	
	fclose (HD);
	
	retornar->palavras[0] = hdPalavras[0];
	retornar->palavras[1] = hdPalavras[1];
	retornar->palavras[2] = hdPalavras[2];
	retornar->palavras[3] = hdPalavras[3];
}

// funcao que salva um bloco de intrucoes no hd
void salvarNoHD (blocoMemoria* salvar)
{
	//cout<<"salvou no hd"<<endl;
	FILE* HD = fopen ("harddrive.bin", "w+b");
	
	fseek (HD, 0, SEEK_SET);
	fseek (HD, salvar->addBloco-1, 0);
	fwrite (salvar->palavras, 4*(sizeof(int)), 1, HD);
	
	fclose (HD);
}

// funcao que cria um novo bloco de memoria na maquina
blocoMemoria* alocaBlocosMemoria (int tamanho) 
{
	blocoMemoria* aux = new blocoMemoria [tamanho];
	
	for (int i=0; i<tamanho; i++)
	{
		aux[i].addBloco = -1;
		aux[i].atualizado = 0;
		aux[i].LFU = 0;
		aux[i].palavras = new long int [4];
	}
	
	return aux;
}

// funcao que desaloca blocos de memoria
void desalocaBlocosMemoria (blocoMemoria* bloco, int tamanho)
{
	delete[] bloco;
}

// funcao que le um arquivo de instrucoes e cria um conjunto de instrucoes a serem lidas pela maquina
Instrucao* montaMemoriaInstrucoes (int total)
{
	Instrucao* aux = new Instrucao [total];
	ifstream fileentrada;
		
	fileentrada.open ("instrucoes.txt",ios::in);
	
	 if (fileentrada.is_open())
   	{
   		//cout<<"Arquivo de instrucoes foi aberto."<<endl<<endl;
   		
		for (int i=0; i<total; i++)
		{
			fileentrada>>aux[i].opcode;
		 	
		 	fileentrada>>aux[i].add1.endBloco;
		 	fileentrada>>aux[i].add1.endPalavra;
			
			fileentrada>>aux[i].add2.endBloco;
		 	fileentrada>>aux[i].add2.endPalavra;
		 	
			fileentrada>>aux[i].add3.endBloco;
		 	fileentrada>>aux[i].add3.endPalavra;
		}
		
		fileentrada.close();
	}
	else
		cout<<"Memoria de instrucoes nao foi criada."<<endl<<endl;

	return aux;
}

// funcao que desaloca a memoria de instrucoes
void desalocaMemoriaIntrucoes (Instrucao* memoria)
{
	delete[] memoria;
}

// funcao que verifica se um bloco de instrucoes esta vazio
int verificaVazio (blocoMemoria* bloco, int tamanho)
{
	int indiceVazio = -1;
	
	for (int i=0; i<tamanho; i++)
	{
		if (bloco[i].addBloco == -1)
			return i; 
	}
	
	return indiceVazio;
}

// funcao que verifica qual bloco de instrucao foi menos utilizado
int verificaLFU (blocoMemoria* bloco, int tamanho)
{
	int menorLFU = 0;
	
	for (int k=0; k<tamanho; k++)
	{
		if (bloco[k].LFU<bloco[menorLFU].LFU)				
			menorLFU = k;
	}
					
	return menorLFU;
}

// funcao que varre uma memoria cache e busca uma instrucao por meio de seu endereco
blocoMemoria* varrerCache (int endEncontrar, blocoMemoria* cache, int tamanhoCache)
{
	for (int i=0; i<tamanhoCache; i++)
		if (cache[i].addBloco == endEncontrar)
			return &(cache[i]);
	
	return NULL;
}

// funcao que atualiza os dados de um bloco de memoria ao ser trocado de uma memoria a outra
void limpaPosicaoCache (blocoMemoria* posicao)
{
	posicao->addBloco = -1;
	posicao->atualizado = 0;
	posicao->LFU = 0;
}

// funcao que faz uma varredura na memoria ram e sobreescreve a funcao menos utilizada
int verificaRAM (blocoMemoria* RAM, int tRAM)
{
	int vazioRAM = verificaVazio (RAM, tRAM);
	int lfuRAM;
	
	if (vazioRAM != -1)
		return vazioRAM;
	else
	{
		lfuRAM = verificaLFU (RAM, tRAM);
		
		if (RAM[lfuRAM].atualizado == 1)
		{
			salvarNoHD(&(RAM[lfuRAM]));
			limpaPosicaoCache(&(RAM[lfuRAM]));
		}
	}
	
	return lfuRAM;
}

// funcao que faz uma varredura na cache 3 buscando um bloco de memoria
// caso a cache 3 esteja cheia, manda um bloco de memoria para a RAM selecionando aquele de menor LFU
int verificaCache3 (blocoMemoria* c3, blocoMemoria* RAM, int tamanho3, int tRAM)
{
	int vazio3 = verificaVazio (c3, tamanho3);
	int lfu3;
	
	if (vazio3 != -1)
		return vazio3;
	else
	{
		lfu3 = verificaLFU (c3, tamanho3);
		int inserirRAM = verificaRAM (RAM, tRAM);
		
		RAM[inserirRAM] = c3[lfu3];	
	
		limpaPosicaoCache(&(c3[lfu3]));	
	}
	
	return lfu3;
}

// funcao que faz uma varredura na cache 2 buscando um bloco de memoria
// caso a cache 2 esteja cheia, manda um bloco de memoria para a cache 3 selecionando aquele de menor LFU
int verificaCache2 (blocoMemoria* RAM, blocoMemoria* c3, blocoMemoria* c2, int tamanho3, int tamanho2, int tRAM)
{
	int vazio2 = verificaVazio (c2, tamanho2);
	int lfu2;
	
	if (vazio2 != -1)
		return vazio2;
	else
	{
		lfu2 = verificaLFU (c2, tamanho2);
		int inserir3 = verificaCache3 (c3, RAM, tamanho3, tRAM);
		
		c3[inserir3] = c2[lfu2];
		
		limpaPosicaoCache(&(c2[lfu2]));
	}
	
	return lfu2;
}

// funcao que faz uma varredura na cache 1 buscando um bloco de memoria
// caso a cache 1 esteja cheia, manda um bloco de memoria para a cache 2 selecionando aquele de menor LFU
int verificaCache1 (blocoMemoria* RAM, blocoMemoria* c3, blocoMemoria* c2, blocoMemoria* c1, int tamanho3, int tamanho2, int tamanho1, int tRAM)
{
	int vazio1 = verificaVazio (c1, tamanho1);
	int lfu1;
	
	if (vazio1 != -1)
		return vazio1;
	else
	{
		lfu1 = verificaLFU (c1, tamanho1);
		int inserir2 = verificaCache2 (RAM, c3, c2, tamanho3, tamanho2, tRAM);
		
		c2[inserir2] = c1[lfu1];
		
		limpaPosicaoCache(&(c1[lfu1]));
	}
	
	return lfu1;
}

// busca uma instrucao nas memorias da maquina de
// caso nao encontre na cache 3, verifica na cache 2. Caso nao encontre na cache 2, verifica na cache 1
// caso nao esteja em nenhuma das memorias caches verifica a memoria ram
blocoMemoria* buscarNasMemorias (
	int endBuscar, 
	blocoMemoria* RAM, 
	blocoMemoria* cache1, 
	blocoMemoria* cache2, 
	blocoMemoria* cache3, 
	int* hit1, 
	int* hit2, 
	int* hit3, 
	int* miss1, 
	int* miss2, 
	int* miss3, 
	int* ram, 
	long int* custo, 
	int t1, 
	int t2, 
	int t3, 
	int tRAM
	)
{
	
	//cout<<"buscar nas memorias abriu"<<endl;
	
	blocoMemoria* buscar;
	buscar = varrerCache (endBuscar, cache1, t1);
	
	//cout<<"foi na cache1"<<endl;
	if (buscar != NULL)
	{
		*hit1 += 1;
		*custo += custoCache1;
		
		return buscar;
	}
	else
	{
		*miss1 += 1;
		//cout<<"foi na cache2"<<endl;
		buscar = varrerCache (endBuscar, cache2, t2);
		
		*custo += custoCache1;
		*custo += custoCache2;
		
		if (buscar != NULL)
		{
			*hit2 += 1;	
			
			int inserir1 = verificaCache1 (RAM, cache3, cache2, cache1, t3, t2, t1, tRAM);
			cache1[inserir1] = *buscar;
			
			return &(cache1[inserir1]);	
		}
		else
		{
			*miss2 += 1;
			//cout<<"foi na cache3"<<endl;
			buscar = varrerCache (endBuscar, cache3, t3);
		
			*custo += custoCache3;
			
			if (buscar != NULL)
			{
				*hit3 += 1;
				
				int inserir2 = verificaCache2 (RAM, cache3, cache2, t3, t2, tRAM);
				cache2[inserir2] = *buscar;
				
				int inserir1 = verificaCache1 (RAM, cache3, cache2, cache1, t3, t2, t1, tRAM);
				cache1[inserir1] = cache2[inserir2];
				
				return &(cache1[inserir1]);
			}
			else
			{
				*miss3 += 1;
				*ram += 1 ;
				
				*custo += custoRAM; 
				//cout<<"foi na ram"<<endl;
				buscar = varrerCache (endBuscar, RAM, tRAM);
				
				if (buscar != NULL)
				{		
					int inserir3 = verificaCache3 (cache3, RAM, t3, tRAM);
					cache3[inserir3] = *buscar;
					
					int inserir2 = verificaCache2 (RAM, cache3, cache2, t3, t2, tRAM);
					cache2[inserir2] = cache3[inserir3];
					
					int inserir1 = verificaCache1 (RAM, cache3, cache2, cache1, t3, t2, t1, tRAM);
					cache1[inserir1] = cache2[inserir2];
					
					return &(cache1[inserir1]);
				}
				else
				{
					//cout<<"foi no hd!!"<<endl;
					blocoMemoria* buscar;
					buscar->addBloco = endBuscar;
					
					buscarHD(endBuscar, buscar);
					
					int inserirRAM = verificaRAM (RAM, tRAM);
					RAM[inserirRAM] = *buscar;
					
					int inserir3 = verificaCache3 (cache3, RAM, t3, tRAM);
					cache3[inserir3] = RAM[inserirRAM];
					
					int inserir2 = verificaCache2 (RAM, cache3, cache2, t3, t2, tRAM);
					cache2[inserir2] = cache3[inserir3];
					
					int inserir1 = verificaCache1 (RAM, cache3, cache2, cache1, t3, t2, t1, tRAM);
					cache1[inserir1] = cache2[inserir2];
					
					return &(cache1[inserir1]);
				}
				
			}	
		} 	
	}
}

// funcao que simula o disparo de uma interrupcao na execucao da maquina
void interrupcaoMaquina (
	Instrucao* interrupcao, 
	blocoMemoria* RAM, 
	blocoMemoria* cache1, 
	blocoMemoria* cache2, 
	blocoMemoria* cache3, 
	int* hit1, 
	int* hit2, 
	int* hit3, 
	int* miss1, 
	int* miss2, 
	int* miss3, 
	int* ram, 
	long int* custo, 
	int t1, 
	int t2, 
	int t3, 
	int tr
	)
{
	cout<<"Uma interrupcao foi realizada."<<endl;
	cout<<"."<<endl;
	cout<<"."<<endl;
	cout<<"."<<endl;
	cout<<"."<<endl;
	cout<<"."<<endl;
	cout<<"."<<endl;
	
	int PC2 = 0;
	int opcode = 100;
	
	while (opcode != -1 && PC2<100)
	{
		Instrucao interrupt = interrupcao[PC2];
		opcode = interrupt.opcode;
		//cout<<"OPCODE: "<<opcode<<endl;
		
		if (opcode != -1)
		{
			
			blocoMemoria* dadosAdd1 = buscarNasMemorias (interrupt.add1.endBloco, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
			blocoMemoria* dadosAdd2 = buscarNasMemorias (interrupt.add2.endBloco, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
			blocoMemoria* dadosAdd3 = buscarNasMemorias (interrupt.add3.endBloco, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
			
			dadosAdd1->LFU = dadosAdd1->LFU+1;
			dadosAdd2->LFU = dadosAdd2->LFU+1;
			dadosAdd3->LFU = dadosAdd3->LFU+1;
			
			switch (opcode)
			{
				case 1:
				{
					long int conteudo1 = dadosAdd1->palavras[interrupt.add1.endPalavra];
					long int conteudo2 = dadosAdd2->palavras[interrupt.add2.endPalavra];
					
					long int modulo = conteudo1 - conteudo2+10;
					
					//cout<<setw(3)<<conteudo1<<" / "<<setw(3)<<conteudo2<<" = "<<setw(3)<<subtracao<<endl;
					
					dadosAdd3->palavras[interrupt.add3.endPalavra] = modulo;
					dadosAdd3->atualizado = 1;				
				}
				break;
				
				case 2:
				{
					long int conteudo1 = dadosAdd1->palavras[interrupt.add1.endPalavra];
					long int conteudo2 = dadosAdd2->palavras[interrupt.add2.endPalavra];
					
					long int conta = conteudo1+conteudo2-10;
					
					//cout<<setw(3)<<conteudo1<<" * "<<setw(3)<<conteudo2<<" = "<<setw(3)<<mult<<endl;
					
					dadosAdd3->palavras[interrupt.add3.endPalavra] = conta;	
					dadosAdd3->atualizado = 1;			
				}
				break;
			}
		}
			
		PC2++;	
	}
	
	cout<<"A interrupcao foi tratada."<<endl;
}

// funcao que simula a execucao da maquina
void maquina (
	Instrucao* memoriaInstrucoes, 
	blocoMemoria* RAM, 
	blocoMemoria* cache1, 
	blocoMemoria* cache2, 
	blocoMemoria* cache3, 

	int* hit1, 
	int* hit2, 
	int* hit3, 

	int* miss1, 
	int* miss2, 
	int* miss3, 

	int* ram, 

	long int* custo, 

	int t1, 
	int t2, 
	int t3, 
	int tr
	)
{
	int PC = 0;
	int opcode = 100;
	int stop = 3;

	// a maquina executa enquanto houver um opcode valido e o program counter seja do tamanho da quantidade de instrucoes
	while (opcode != -1 && PC<totalInstrucoes)
	{
		int i = rand()%200;
		
		if (i == stop)
		{
			Instrucao* interrupcao = montaMemoriaInstrucoes(5);
			interrupcao[4].opcode = -1;
			
			interrupcaoMaquina (interrupcao, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
		}
			
		Instrucao umaInstrucao = memoriaInstrucoes [PC];
		opcode = umaInstrucao.opcode;
		
		if (opcode != -1)
		{
			//cout<<"foi buscar o add1"<<endl;
			blocoMemoria* dadosAdd1 = buscarNasMemorias (umaInstrucao.add1.endBloco, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
			//cout<<"foi buscar o add2"<<endl;
			blocoMemoria* dadosAdd2 = buscarNasMemorias (umaInstrucao.add2.endBloco, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
			//cout<<"foi buscar o add3"<<endl;
			blocoMemoria* dadosAdd3 = buscarNasMemorias (umaInstrucao.add3.endBloco, RAM, cache1, cache2, cache3, hit1, hit2, hit3, miss1, miss2, miss3, ram, custo, t1, t2, t3, tr);
			
			dadosAdd1->LFU = dadosAdd1->LFU+1;
			dadosAdd2->LFU = dadosAdd2->LFU+1;
			dadosAdd3->LFU = dadosAdd3->LFU+1;
			
			switch (opcode)
			{
				case 1:
				{
					long int conteudo1 = dadosAdd1->palavras[umaInstrucao.add1.endPalavra];
					long int conteudo2 = dadosAdd2->palavras[umaInstrucao.add2.endPalavra];
					
					long int soma = conteudo1 + conteudo2;
					
					//cout<<setw(3)<<conteudo1<<" + "<<setw(3)<<conteudo2<<" = "<<setw(3)<<soma<<endl;
					
					dadosAdd3->palavras[umaInstrucao.add3.endPalavra] = soma;
					dadosAdd3->atualizado = 1;				
				}
				break;
				
				case 2:
				{
					long int conteudo1 = dadosAdd1->palavras[umaInstrucao.add1.endPalavra];
					long int conteudo2 = dadosAdd2->palavras[umaInstrucao.add2.endPalavra];
					
					long int subtracao = conteudo1 - conteudo2;
					
					//cout<<setw(3)<<conteudo1<<" - "<<setw(3)<<conteudo2<<" = "<<setw(3)<<subtracao<<endl;
					
					dadosAdd3->palavras[umaInstrucao.add3.endPalavra] = subtracao;	
					dadosAdd3->atualizado = 1;			
				}
				break;
			}
		}
			
		PC++;
	}
}