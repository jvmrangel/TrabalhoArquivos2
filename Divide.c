#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

int compara(const void *e1, const void *e2)
{
    return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main (int argc, char**argv)
{
    FILE *f, *a, *b, *saida;
    Endereco *e, *ea, *eb;
    long posicao, qtd, tamanhoBloco;
    char buffer[30];

    f= fopen("cep.dat","r");
    fseek(f,0,SEEK_END);
    posicao= ftell(f);
    qtd= posicao/sizeof(Endereco);
    tamanhoBloco= qtd/16;

    printf("Quantidade de bytes registro: %ld\n\n", sizeof(Endereco));
    printf("Quantidade Registros: %ld\n\n", qtd);
    printf("Tamanho de cada Bloco: %ld\n\n", tamanhoBloco);
    printf("Tamanho do ultimo Bloco: %ld\n\n",(tamanhoBloco+(qtd%16)));

    rewind(f);

    // Divide o cep.dat em 16 blocos //

    for (int i=0; i<16; i++)   
    {   
        if(i==15)
            tamanhoBloco=tamanhoBloco+(qtd%16);

        e= (Endereco*) malloc(tamanhoBloco*sizeof(Endereco));
        
        if(fread(e,sizeof(Endereco),tamanhoBloco,f) == tamanhoBloco)
        {
            printf("Bloco %d Lido = OK\n", i);
        }
        qsort(e,tamanhoBloco,sizeof(Endereco),compara);
        printf("Bloco %d Ordenado = OK\n", i);
        sprintf(buffer, "cep_%d.dat", i);
        saida = fopen(buffer,"w");
        fwrite(e,sizeof(Endereco),tamanhoBloco,saida);
        fclose(saida);
        printf("Bloco %d Escrito = OK\n\n", i);
        free(e);
        
    }

    // Intercala //

    

    fclose(f);    
}
