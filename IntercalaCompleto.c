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
    Endereco *e, ea, eb;
    long posicao, qtd, tamanhoBloco;
    char buffer[30];
    char novoNome[]= "cep_ordenado_final.dat";
    int inicio= 0, final=16;

    f= fopen("cep.dat","r");
    fseek(f,0,SEEK_END);
    posicao= ftell(f);
    qtd= posicao/sizeof(Endereco);
    tamanhoBloco= qtd/final;

    printf("Quantidade de bytes registro: %ld\n\n", sizeof(Endereco));
    printf("Quantidade Registros: %ld\n\n", qtd);
    printf("Tamanho de cada Bloco: %ld\n\n", tamanhoBloco);
    printf("Tamanho do ultimo Bloco: %ld\n\n",(tamanhoBloco+(qtd%final)));

    rewind(f);

    // Divide o cep.dat em 16 blocos //

    for (int i=0; i<final; i++)   
    {   
        if(i==final-1)
            tamanhoBloco=tamanhoBloco+(qtd%final);

        e= (Endereco*) malloc(tamanhoBloco*sizeof(Endereco));
        
        if(fread(e,sizeof(Endereco),tamanhoBloco,f) == tamanhoBloco)
        {
            printf("Bloco %d Lido = OK\n", i);
        }
        qsort(e,tamanhoBloco,sizeof(Endereco),compara);
        printf("Bloco %d Ordenado = OK\n", i);
        sprintf(buffer, "cep_%d.dat", i); //atribui o nome do arquivo referindo a posição do bloco
        saida = fopen(buffer,"w");
        fwrite(e,sizeof(Endereco),tamanhoBloco,saida);
        fclose(saida);
        printf("Bloco %d Escrito = OK\n\n", i); 
        free(e);
        
    }
    fclose(f);  

    // Intercala //
    
    while( inicio<final-1)
    {
        sprintf(buffer, "cep_%d.dat", inicio);
        a = fopen(buffer,"r");

        sprintf(buffer, "cep_%d.dat", inicio+1);
        b = fopen(buffer,"r");

        sprintf(buffer, "cep_%d.dat", final);
        saida = fopen(buffer,"w");

        fread(&ea,sizeof(Endereco),1,a);
        fread(&eb,sizeof(Endereco),1,b);

        while(!feof(a) && !feof(b))
        {
            if(compara(&ea,&eb)<0) // ea.cep < eb.cep
            {
                fwrite(&ea,sizeof(Endereco),1,saida);
                fread(&ea,sizeof(Endereco),1,a);
            }
            else
            {
                fwrite(&eb,sizeof(Endereco),1,saida);
                fread(&eb,sizeof(Endereco),1,b);
            }
        }

        while(!feof(a))
        {
            fwrite(&ea,sizeof(Endereco),1,saida);
            fread(&ea,sizeof(Endereco),1,a);		
        }

        while(!feof(b))
        {
            fwrite(&eb,sizeof(Endereco),1,saida);
            fread(&eb,sizeof(Endereco),1,b);		
        }

        printf("bloco %d  + bloco %d  = bloco %d intercalado com sucesso\n", inicio, inicio+1, final);

        fclose(a);
        fclose(b);
        fclose(saida);
        final=final+1;
        inicio=inicio+2;
                    
    }

    sprintf(buffer, "cep_%d.dat", final-1);

    if(rename(buffer, novoNome) == 0)   // renomeia ultimo cep_*.dat renomeado 
    {
        printf("Arquivo renomeado com sucesso");
    } 
    else 
    {
        printf("Erro: falha ao renomear o arquivo");
    }
    
  return 0;
}
